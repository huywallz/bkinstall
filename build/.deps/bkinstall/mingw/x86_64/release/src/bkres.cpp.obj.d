{
    files = {
        [[src\bkres.cpp]]
    },
    depfiles = "bkres.o: src\\bkres.cpp include/bkres.hpp\
",
    depfiles_format = "gcc",
    values = {
        [[C:\msys64\mingw64\bin\x86_64-w64-mingw32-g++]],
        {
            "-m64",
            "-fvisibility=hidden",
            "-fvisibility-inlines-hidden",
            "-O3",
            "-std=c++17",
            "-Iinclude",
            [[-Iexternal\fltk]],
            [[-Iexternal\fltk\build]],
            "-DNDEBUG"
        }
    }
}