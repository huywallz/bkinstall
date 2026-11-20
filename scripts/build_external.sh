BUILD_MODE="${1:-debug}"
BUILD_MODE_LOWER="${BUILD_MODE,,}"

case "${BUILD_MODE_LOWER}" in
    debug)
        CMAKE_CONFIG="Debug"
        ;;
    release)
        CMAKE_CONFIG="Release"
        ;;
    *)
        echo "error: invalid build mode '$1'. Use 'debug' or 'release'." >&2
        exit 1
        ;;
esac

cd external/fltk
cmake -S . -B build -G Ninja \
    -DCMAKE_BUILD_TYPE=Release \
    -DFLTK_BUILD_SHARED_LIBS=OFF \
    -DFLTK_BUILD_EXAMPLES=OFF \
    -DFLTK_BUILD_TEST=OFF \
    -DCMAKE_INSTALL_PREFIX=/mingw64/local-static

cmake --build build --config ${CMAKE_CONFIG} --parallel

cd ../..