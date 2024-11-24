cmake -G "Visual Studio 17 2022" -DCMAKE_CXX_FLAGS="/MP /utf-8" -A Win32 .
xcopy /s "..\extlibs\libs-msvc\x86" ".\"
xcopy /s "..\assets" ".\assets\"