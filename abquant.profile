[settings]
os=Linux
os_build=Linux
arch=x86_64
arch_build=x86_64
compiler=clang
compiler.version=11
compiler.libcxx=libstdc++11
build_type=Release
# build_type=Debug
[options]
qt:qtdeclarative=True
qt:qttools=True
qt:qtwebsockets=True
[build_requires]
[env]
# CC=$PROFILE_DIR/ccache-clang
# CXX=$PROFILE_DIR/ccache-clang++
CC="ccache clang"
CXX="ccache clang++"
CPP="ccache cpp"

# CC="clang"
# CXX="clang++"
# CPP="cpp"

CONAN_CMAKE_GENERATOR=Ninja
