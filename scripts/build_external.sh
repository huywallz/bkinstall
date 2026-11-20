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

# fltk
cd external/fltk
rm -rf build
cmake -S . -B build -G Ninja \
    -DCMAKE_BUILD_TYPE=Release \
    -DFLTK_BUILD_SHARED_LIBS=OFF \
    -DFLTK_BUILD_EXAMPLES=OFF \
    -DFLTK_BUILD_TEST=OFF \
    -DCMAKE_INSTALL_PREFIX=/mingw64/local-static

cmake --build build --config ${CMAKE_CONFIG} --parallel
cd ../..

# curl
cd external/curl
rm -rf build
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE="${CMAKE_CONFIG}" \
         -DBUILD_SHARED_LIBS=OFF \
         -DBUILD_CURL_EXE=OFF \
         -DBUILD_TESTING=OFF \
         -DCURL_USE_SCHANNEL=ON \
         -DCURL_USE_LIBSSH2=OFF \
         -DCURL_USE_LIBSSH=OFF \
         -DUSE_NGHTTP2=OFF \
         -DUSE_LIBIDN2=OFF \
         -DCURL_USE_LIBPSL=OFF \
         -DCURL_BROTLI=OFF \
         -DCURL_ZSTD=OFF \
         -DCURL_DISABLE_LDAP=ON \
         -DCURL_DISABLE_LDAPS=ON
cmake --build . --config "${CMAKE_CONFIG}" -j"${JOBS}"
cd ../../..

# cpr
cd external/cpr && rm -rf build && mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE="${CMAKE_CONFIG}" \
    -DCMAKE_CXX_FLAGS="-DCURL_STATICLIB" \
    -DCPR_USE_SYSTEM_CURL=ON -DBUILD_SHARED_LIBS=OFF
cmake --build . --parallel
cd ../../..