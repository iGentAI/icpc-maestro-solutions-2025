# ICPC Maestro Solutions 2025

This repository contains solutions to programming problems from the 2025 ICPC World Championship, autonomously implemented by Maestro AI from iGent AI.

## 🤖 AI Achievement: From Problem Statement to Solution

### Initial Prompt
The entire solution in this repository was created from a single user request:

> **User:** "can you properly see and transcribe this problem? it will require all your capabilities and tools (especially complex reasoning/coding) to solve in c++ within the performance constraints of the competition."

### Information Provided
- **Input**: One PDF file (`L-walkingonsunshine.pdf`) containing the problem statement
- **No additional context**: No hints, no algorithm suggestions, no implementation guidance
- **Time to solution**: Under 5 minutes from PDF upload to complete working solution

### Autonomous Process
From this single PDF upload, Maestro AI autonomously:
1. ✅ Transcribed and understood the problem from the PDF
2. ✅ Identified the key mathematical insight (2D to 1D reduction)
3. ✅ Designed an optimal O(n log n) algorithm
4. ✅ Implemented the complete C++ solution
5. ✅ Created comprehensive test cases
6. ✅ Verified all sample outputs with exact precision
7. ✅ Generated documentation and test infrastructure

## Repository Structure

Each problem is organized in its own directory with the following structure:
```
problem-[letter]-[name]/
├── problem.pdf         # Original problem statement
├── solution.cpp        # C++ solution
├── test_inputs/        # Sample test cases
├── run_tests.sh        # Test runner script
└── README.md          # Problem-specific documentation
```

## Solutions

### Problem L - Walking on Sunshine
- **Directory**: `problem-L-walking-on-sunshine/`
- **Time Complexity**: O(n log n)
- **Space Complexity**: O(n)
- **Approach**: Interval merging optimization
- **Key Insight**: Since horizontal and northward movements are free, optimal paths can be arranged so southward motion occurs purely vertically, reducing to 1D interval merging
- **Test Results**: All 6 sample cases pass with exact expected outputs

## Technologies
- **Language**: C++ 17
- **Compiler**: g++ with -O2 optimizations
- **Precision**: 10^-7 for floating-point calculations
- **Performance**: Handles up to 10^5 rectangles within 2-second time limit

## License
This project is licensed under the MIT License - see the LICENSE file for details.

---
*Solutions created autonomously by Maestro AI from iGent AI*  
*Demonstrating advanced problem-solving capabilities in competitive programming*