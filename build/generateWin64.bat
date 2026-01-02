cmake -G "Visual Studio 18 2026" -DCMAKE_CXX_FLAGS="/MP /utf-8" -A x64 .
xcopy /s "..\extlibs\libs-msvc\x64" ".\"
xcopy /s "..\assets" ".\assets\"
cd assets/shaders/
./compile-shaders.bat