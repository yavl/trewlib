cmake -G "Visual Studio 16 2019" -DCMAKE_CXX_FLAGS="/MP" -A Win32 .
xcopy /s "..\extlibs\libs-msvc\x86" ".\"
xcopy /s "..\assets" ".\assets\"