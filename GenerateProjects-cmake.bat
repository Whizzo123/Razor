@echo off
cmake -B build\cmake ^
      -DCMAKE_BUILD_TYPE=Debug ^
      -G Ninja ^
      -DCMAKE_C_COMPILER=cl ^
      -DCMAKE_CXX_COMPILER=cl

echo.
echo CMake project generated at build\cmake\
echo Build with: cmake --build build\cmake
