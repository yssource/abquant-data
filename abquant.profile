[settings]
os=Linux
os_build=Linux
arch=x86_64
arch_build=x86_64
compiler=clang
compiler.version=10
compiler.libcxx=libstdc++11
# compiler.libcxx=libstdc++
# build_type=Release
build_type=Debug
[options]
qt:qtdeclarative=True
qt:qttools=True
qt:qtwebsockets=True
[build_requires]
[env]
CC=$PROFILE_DIR/ccache-clang
CXX=$PROFILE_DIR/ccache-clang++
# CC=/usr/bin/clang
# CXX=/usr/bin/clang++
CONAN_CMAKE_GENERATOR=Ninja
