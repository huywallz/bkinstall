{
    depfiles_format = "gcc",
    files = {
        [[src\bkres.cpp]]
    },
    values = {
        [[C:\msys64\mingw64\bin\x86_64-w64-mingw32-g++]],
        {
            "-m64",
            "-g",
            "-O0",
            "-std=c++17",
            "-Iinclude",
            [[-Iexternal\fltk]],
            [[-Iexternal\fltk\build]]
        }
    },
    depfiles = "bkres.o: src\\bkres.cpp include/bkres.hpp\
"
}