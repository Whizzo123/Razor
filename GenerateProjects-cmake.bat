@echo off

call "C:\Program Files\Microsoft Visual Studio\18\Community\Common7\Tools\VsDevCmd.bat" -arch=x64 -host_arch=x64

set LLVM_BIN=C:\\Program Files\\Microsoft Visual Studio\\18\\Community\\VC\\Tools\\Llvm\\x64\\bin

where rc
where clang-cl
where lld-link
where link
where mt

cmake -B build\cmake ^
      -DCMAKE_BUILD_TYPE=Debug ^
      -G Ninja ^
      -DCMAKE_C_COMPILER="%LLVM_BIN%\clang-cl.exe" ^
      -DCMAKE_CXX_COMPILER="%LLVM_BIN%\clang-cl.exe"

echo.
echo CMake project generated at build\cmake\
echo Build with: cmake --build build\cmake
