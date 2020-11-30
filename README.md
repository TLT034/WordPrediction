# WordPrediction
Simple word prediction command line app built with C++.
Each time the user types a letter, a new list of predictions will be calculated and displayed.
Utilizes N-ary tree structure for efficent predicting.

## Build
- CMake version 3.10+ required
- Clang-Format optional

1. Run `cmake -s . -B ./build` while inside this directory.
2. Copy `dictionary.txt` into the build folder, or use your own text file full of words.
3. Run `make` inside build directory.
4. Run the `TypeAhead` executable to run the program, or the `UnitTestRunner` executable to run the unit tests.
