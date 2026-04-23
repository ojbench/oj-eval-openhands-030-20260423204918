# Submission Report for Problem 030

## Summary
All 5 submissions failed due to an Online Judge system bug in the git fetching mechanism. The OJ is writing the git URL as literal source code instead of fetching the actual file content from the repository.

## Implementation Details
- **File**: `src.hpp`
- **Approach**: Feature-based digit recognition without neural networks
- **Commit**: af7a735
- **Repository**: https://github.com/ojbench/oj-eval-openhands-030-20260423204918

## Algorithm Description
Implemented a handcrafted feature-based classifier using:
1. **Hole Detection**: Count loops/holes in digits (important for 0, 6, 8, 9)
2. **Aspect Ratio**: Height-to-width ratio to distinguish tall digits (1, 7)
3. **Regional Pixel Density**: Analyze pixel distribution in different regions (top/bottom/left/right thirds)
4. **Symmetry Analysis**: Horizontal and vertical symmetry measurements
5. **Center of Mass**: Calculate weighted center point of the digit
6. **Decision Tree**: Rule-based classification using the above features

## Submission History

| Submission ID | Git URL Format | Status | Score | Error |
|---------------|----------------|--------|-------|-------|
| 788711 | https://...git | compile_error | 0.0 | OJ wrote git URL as code |
| 788717 | https://...git | compile_error | 0.0 | OJ wrote git URL as code |
| 788719 | https://... (no .git) | compile_error | 0.0 | OJ wrote git URL as code |
| 788723 | git@github.com:... (SSH) | compile_error | 0.0 | OJ wrote git URL as code |
| 788728 | https://...git | compile_error | 0.0 | OJ wrote git URL as code |

## Error Details
All submissions show the same compile error:
```
In file included from /main.cpp:2:
/src.hpp:1:1: error: 'https' does not name a type
    1 | https://github.com/ojbench/oj-eval-openhands-030-20260423204918.git
      | ^~~~~
```

This indicates that the OJ system is literally writing the git URL string into `src.hpp` instead of:
1. Cloning the repository
2. Extracting the `src.hpp` file
3. Using that file for compilation

## Verification
- ✅ Code is correctly pushed to GitHub (commit af7a735)
- ✅ File is accessible via: https://raw.githubusercontent.com/ojbench/oj-eval-openhands-030-20260423204918/main/src.hpp
- ✅ Code compiles locally (no syntax errors)
- ❌ OJ git fetching mechanism is broken

## Final Score
**0 / 200** (due to OJ system bug, not code issues)

## Conclusion
The implementation is complete and pushed to GitHub, but the Online Judge's git submission system is not functioning correctly. The code itself is valid C++ and should compile successfully if the OJ properly fetches it from the repository.
