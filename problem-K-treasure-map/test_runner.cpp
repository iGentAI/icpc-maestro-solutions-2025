#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>

using namespace std;

struct TestCase {
    string input;
    string expected_output;
    string description;
};

// Trim whitespace from string
string trim(const string& str) {
    size_t first = str.find_first_not_of(" \n\r\t");
    if (first == string::npos) return "";
    size_t last = str.find_last_not_of(" \n\r\t");
    return str.substr(first, (last - first + 1));
}

// Check if file exists and is executable
bool isExecutable(const string& path) {
    struct stat st;
    if (stat(path.c_str(), &st) != 0) {
        return false;
    }
    return (st.st_mode & S_IXUSR) != 0;
}

// Execute binary with input and capture output using fork/exec
string executeWithInput(const string& binaryPath, const string& input, int& exitCode) {
    exitCode = -1;
    
    // Create pipes for stdin and stdout
    int stdin_pipe[2];
    int stdout_pipe[2];
    
    if (pipe(stdin_pipe) == -1 || pipe(stdout_pipe) == -1) {
        cerr << "Failed to create pipes: " << strerror(errno) << "\n";
        return "";
    }
    
    pid_t pid = fork();
    
    if (pid == -1) {
        cerr << "Failed to fork: " << strerror(errno) << "\n";
        close(stdin_pipe[0]); close(stdin_pipe[1]);
        close(stdout_pipe[0]); close(stdout_pipe[1]);
        return "";
    }
    
    if (pid == 0) {
        // Child process
        
        // Set up stdin from pipe
        close(stdin_pipe[1]); // Close write end
        if (dup2(stdin_pipe[0], STDIN_FILENO) == -1) {
            _exit(1);
        }
        close(stdin_pipe[0]);
        
        // Set up stdout to pipe
        close(stdout_pipe[0]); // Close read end
        if (dup2(stdout_pipe[1], STDOUT_FILENO) == -1) {
            _exit(1);
        }
        close(stdout_pipe[1]);
        
        // Execute the binary
        char* const argv[] = {const_cast<char*>(binaryPath.c_str()), nullptr};
        execv(binaryPath.c_str(), argv);
        
        // If we get here, exec failed
        cerr << "Failed to execute " << binaryPath << ": " << strerror(errno) << "\n";
        _exit(127);
    }
    
    // Parent process
    close(stdin_pipe[0]); // Close read end
    close(stdout_pipe[1]); // Close write end
    
    // Write input to child's stdin
    size_t totalWritten = 0;
    while (totalWritten < input.size()) {
        ssize_t written = write(stdin_pipe[1], 
                               input.c_str() + totalWritten, 
                               input.size() - totalWritten);
        if (written == -1) {
            if (errno == EINTR) continue;
            cerr << "Failed to write to child stdin: " << strerror(errno) << "\n";
            break;
        }
        totalWritten += written;
    }
    close(stdin_pipe[1]); // Close to signal EOF to child
    
    // Read output from child's stdout
    string output;
    char buffer[4096];
    ssize_t bytesRead;
    
    while ((bytesRead = read(stdout_pipe[0], buffer, sizeof(buffer))) > 0) {
        output.append(buffer, bytesRead);
    }
    
    if (bytesRead == -1 && errno != 0) {
        cerr << "Failed to read from child stdout: " << strerror(errno) << "\n";
    }
    
    close(stdout_pipe[0]);
    
    // Wait for child to complete
    int status;
    pid_t result = waitpid(pid, &status, 0);
    
    if (result == -1) {
        cerr << "Failed to wait for child: " << strerror(errno) << "\n";
        return output;
    }
    
    if (WIFEXITED(status)) {
        exitCode = WEXITSTATUS(status);
    } else if (WIFSIGNALED(status)) {
        exitCode = -WTERMSIG(status);
        cerr << "Child terminated by signal " << WTERMSIG(status) << "\n";
    }
    
    return output;
}

vector<TestCase> getTestCases() {
    vector<TestCase> cases;
    
    // Sample Input 1
    cases.push_back({
        "3 3 5 1 1\n"
        "1 3 1\n"
        "3 3 2\n"
        "2 3 3\n"
        "2 2 4\n"
        "2 1 5\n",
        "3",
        "Sample 1"
    });
    
    // Sample Input 2
    cases.push_back({
        "3 5 4 3 4\n"
        "2 4 1\n"
        "2 2 2\n"
        "1 1 4\n"
        "3 1 5\n",
        "1",
        "Sample 2"
    });
    
    // Sample Input 3
    cases.push_back({
        "3 3 3 3 3\n"
        "2 3 1\n"
        "2 1 2\n"
        "1 2 4\n",
        "0",
        "Sample 3"
    });
    
    // Sample Input 4
    cases.push_back({
        "3 3 4 3 2\n"
        "2 1 2\n"
        "2 3 3\n"
        "1 3 4\n"
        "1 1 5\n",
        "impossible",
        "Sample 4"
    });
    
    // Sample Input 5
    cases.push_back({
        "3 3 3 2 2\n"
        "3 2 0\n"
        "2 2 1\n"
        "2 3 0\n",
        "impossible",
        "Sample 5 - Data cannot be extended to valid map"
    });
    
    return cases;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <solution_binary>\n";
        cerr << "Example: " << argv[0] << " ./treasure_map_fixed\n";
        return 1;
    }
    
    string solutionBinary = argv[1];
    
    // Check if binary exists and is executable
    if (!isExecutable(solutionBinary)) {
        cerr << "Error: " << solutionBinary << " does not exist or is not executable\n";
        return 1;
    }
    
    cout << "Testing Treasure Map Solution: " << solutionBinary << "\n";
    cout << "===============================================\n\n";
    
    vector<TestCase> cases = getTestCases();
    int passed = 0;
    int failed = 0;
    
    for (size_t i = 0; i < cases.size(); i++) {
        cout << "Test Case " << (i + 1) << ": " << cases[i].description << "\n";
        
        int exitCode;
        string actualOutput = executeWithInput(solutionBinary, cases[i].input, exitCode);
        actualOutput = trim(actualOutput);
        string expectedOutput = trim(cases[i].expected_output);
        
        cout << "Expected: " << expectedOutput << "\n";
        cout << "Actual:   " << actualOutput << "\n";
        
        if (exitCode != 0) {
            cout << "Exit Code: " << exitCode << " (non-zero)\n";
        }
        
        if (actualOutput == expectedOutput && exitCode == 0) {
            cout << "Result:   [PASS]\n";
            passed++;
        } else {
            cout << "Result:   [FAIL]\n";
            failed++;
        }
        cout << "---\n";
    }
    
    cout << "\n===============================================\n";
    cout << "Summary: " << passed << " passed, " << failed << " failed\n";
    
    return (failed > 0) ? 1 : 0;
}