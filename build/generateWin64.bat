cmake -G "Visual Studio 17 2022" -DCMAKE_CXX_FLAGS="/MP /utf-8" -A x64 .
xcopy /s "..\extlibs\libs-msvc\x64" ".\"
xcopy /s "..\assets" ".\assets\"
cd assets/shaders/
./compile-shaders.bat