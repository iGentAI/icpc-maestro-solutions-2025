import sys
sys.setrecursionlimit(1_000_000)

from typing import Optional, Tuple, Iterator, Dict, List, Union

# We adopt the consistent min-heap skew heap insertion model:
# - If inserting value y into heap with root x:
#   - If y < x: new root y with old tree as its left child.
#   - Else: swap children of current root, and insert y into left subtree.
#
# Working backwards (undo one insertion on a node T with root r, left L, right R):
# - Case 1 (root was last inserted): if R is None and (L is None or r < L.val),
#   then s = r, and Prev = L.
# - Case 2 (inserted into left after a swap): for each one-step undo (s, B) of L
#   (where B is the previous tree for L), if s >= r, then Prev = Node(r, left=R, right=B),
#   provided Prev satisfies local structural and heap constraints at the root:
#   - Not (Prev.left is None and Prev.right is not None)  [structural]
#   - Prev.val <= Prev.left.val (if left exists) and Prev.val <= Prev.right.val (if right exists)  [heap]
#
# The removal sequence is the sequence of s values produced by repeatedly undoing.
# Among all possible removal sequences, lexicographically max and min can be obtained greedily by
# trying candidates (s, Prev) in descending or ascending s order respectively, always verifying
# that the remaining tree can still be undone. We memoize the results by a structural digest.

class Node:
    __slots__ = ('val', 'left', 'right', 'digest')
    def __init__(self, val: int, left: Optional['Node'] = None, right: Optional['Node'] = None):
        self.val = val
        self.left = left
        self.right = right
        self.digest: Optional[Tuple[int, Optional[Tuple], Optional[Tuple]]] = None


def compute_digest(node: Optional[Node]) -> Optional[Tuple[int, Optional[Tuple], Optional[Tuple]]]:
    if node is None:
        return None
    if node.digest is not None:
        return node.digest
    ld = compute_digest(node.left)
    rd = compute_digest(node.right)
    node.digest = (node.val, ld, rd)
    return node.digest


def parse_input() -> Tuple[int, List[int], List[int]]:
    data = sys.stdin.read().strip().split()
    if not data:
        return 0, [], []
    it = iter(data)
    try:
        n = int(next(it))
    except StopIteration:
        return 0, [], []
    L = [0] * (n + 1)
    R = [0] * (n + 1)
    for i in range(1, n + 1):
        try:
            l = int(next(it)); r = int(next(it))
        except StopIteration:
            # Incomplete input
            raise ValueError("Invalid input: insufficient child pairs")
        L[i] = l
        R[i] = r
    return n, L, R


def find_root_and_validate(n: int, L: List[int], R: List[int]) -> Optional[int]:
    """Return root index if valid tree (single root, connected), else None."""
    if n == 0:
        return None
    indeg = [0] * (n + 1)
    for i in range(1, n + 1):
        l, r = L[i], R[i]
        if l != 0:
            if l < 1 or l > n:
                return None
            indeg[l] += 1
            if indeg[l] > 1:
                return None
        if r != 0:
            if r < 1 or r > n:
                return None
            indeg[r] += 1
            if indeg[r] > 1:
                return None
    roots = [i for i in range(1, n + 1) if indeg[i] == 0]
    if len(roots) != 1:
        return None
    root = roots[0]
    # Check connectedness via DFS
    seen = [False] * (n + 1)
    stack = [root]
    while stack:
        u = stack.pop()
        if seen[u]:
            continue
        seen[u] = True
        if L[u] != 0:
            stack.append(L[u])
        if R[u] != 0:
            stack.append(R[u])
    if not all(seen[1:]):
        return None
    return root


def initial_properties_valid(n: int, L: List[int], R: List[int]) -> bool:
    """Validate heap property (parent <= children) and structural property (not right without left)."""
    for i in range(1, n + 1):
        l, r = L[i], R[i]
        # Structural: no right child while left is empty
        if r != 0 and l == 0:
            return False
        # Heap property: parent <= children (values == indices)
        if l != 0 and not (i <= l):
            return False
        if r != 0 and not (i <= r):
            return False
    return True


def build_nodes(n: int, L: List[int], R: List[int], root_idx: int) -> Node:
    nodes = [None] * (n + 1)
    for i in range(1, n + 1):
        nodes[i] = Node(i)
    for i in range(1, n + 1):
        li, ri = L[i], R[i]
        nodes[i].left = nodes[li] if li != 0 else None
        nodes[i].right = nodes[ri] if ri != 0 else None
    root = nodes[root_idx]
    compute_digest(root)
    return root


def structural_ok_at_root(node: Node) -> bool:
    # No case where right exists but left is None
    return not (node.left is None and node.right is not None)


def heap_ok_at_root(node: Node) -> bool:
    if node.left is not None and node.val > node.left.val:
        return False
    if node.right is not None and node.val > node.right.val:
        return False
    return True


def make_prev_node(root_val: int, left: Optional[Node], right: Optional[Node]) -> Optional[Node]:
    prev = Node(root_val, left=left, right=right)
    # Structural and heap checks at root (quick prune)
    if not structural_ok_at_root(prev):
        return None
    # The left child is 'left', right is 'right' as per construction
    if left is not None and prev.val > left.val:
        return None
    if right is not None and prev.val > right.val:
        return None
    # Compute digest
    compute_digest(prev)
    return prev


def iter_one_step_undos(node: Node, order: str) -> Iterator[Tuple[int, Optional[Node]]]:
    """
    Yield (s, Prev) pairs for undoing one insertion from 'node'.
    Order: 'desc' yields candidates in descending s; 'asc' yields in ascending s.
    """
    # Helper to generate left-derived (case 2) candidates in a given order
    def gen_left_first_desc(n: Node) -> Iterator[Tuple[int, Optional[Node]]]:
        # Left-derived candidates first (descending by s)
        if n.left is not None:
            for s_left, B in iter_one_step_undos(n.left, 'desc'):
                # Case 2 condition: s_left >= n.val
                if s_left < n.val:
                    # Since descending, all further s will be <= current, so can break early
                    break
                # Build Prev for this node: Prev.left = n.right, Prev.right = B
                prev_node = make_prev_node(n.val, left=n.right, right=B)
                if prev_node is None:
                    continue
                yield (s_left, prev_node)
        # Root-derived candidate (case 1) last
        if n.right is None and (n.left is None or n.val < n.left.val):
            # Prev is the left subtree itself
            yield (n.val, n.left)

    def gen_root_first_asc(n: Node) -> Iterator[Tuple[int, Optional[Node]]]:
        # Root-derived candidate first (ascending by s)
        if n.right is None and (n.left is None or n.val < n.left.val):
            yield (n.val, n.left)
        # Then left-derived candidates (ascending by s)
        if n.left is not None:
            for s_left, B in iter_one_step_undos(n.left, 'asc'):
                if s_left >= n.val:
                    prev_node = make_prev_node(n.val, left=n.right, right=B)
                    if prev_node is None:
                        continue
                    yield (s_left, prev_node)
                # If s_left < n.val, skip; later s_left may be >= n.val since ascending

    if order == 'desc':
        yield from gen_left_first_desc(node)
    else:
        yield from gen_root_first_asc(node)


# Caches for lexicographic removal sequences
LexSeq = Optional[Tuple[int, ...]]
lex_cache_desc: Dict[Optional[Tuple], LexSeq] = {}
lex_cache_asc: Dict[Optional[Tuple], LexSeq] = {}


def lex_removal(node: Optional[Node], order: str) -> LexSeq:
    """
    Return the lexicographically extreme removal sequence for the given node:
    - order == 'desc': lexicographically maximal removal sequence
    - order == 'asc': lexicographically minimal removal sequence
    Returns None if the tree is not producible.
    """
    if node is None:
        return tuple()
    digest = node.digest
    cache = lex_cache_desc if order == 'desc' else lex_cache_asc
    if digest in cache:
        return cache[digest]

    # Quick local invalidity checks (shouldn't occur for valid states, but prune aggressively)
    if not structural_ok_at_root(node) or not heap_ok_at_root(node):
        cache[digest] = None
        return None

    for s, prev in iter_one_step_undos(node, order):
        seq_rest = lex_removal(prev, order)
        if seq_rest is not None:
            res = (s,) + seq_rest
            cache[digest] = res
            return res

    cache[digest] = None
    return None


def main():
    try:
        n, L, R = parse_input()
    except ValueError:
        print("impossible")
        return

    if n == 0:
        # No nodes; specification implies n >= 1 normally, but handle gracefully.
        print("impossible")
        return

    root_idx = find_root_and_validate(n, L, R)
    if root_idx is None:
        print("impossible")
        return

    if not initial_properties_valid(n, L, R):
        print("impossible")
        return

    root = build_nodes(n, L, R, root_idx)

    # Compute lexicographically extreme removal sequences
    desc_rem = lex_removal(root, 'desc')
    if desc_rem is None:
        print("impossible")
        return
    asc_rem = lex_removal(root, 'asc')
    if asc_rem is None:
        print("impossible")
        return

    # Convert to insertion sequences by reversing the removal sequences.
    # Following the provided guideline: min insertion = reverse(max removal), max insertion = reverse(min removal).
    ins_min = list(reversed(desc_rem))
    ins_max = list(reversed(asc_rem))

    # Output
    print(" ".join(map(str, ins_min)))
    print(" ".join(map(str, ins_max)))


if __name__ == "__main__":
    main()