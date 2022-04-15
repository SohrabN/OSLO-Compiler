# CMakeList.txt : CMake project for A22, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("A22")

# Add source to this project's executable.
add_executable (A22 "Scanner.c" "Scanner.h" "Buffer.c" "Buffer.h" "Compilers.c" "Compilers.h" "Parser.c" "Parser.h" "MainParser.c")

# TODO: Add tests and install targets if needed.
