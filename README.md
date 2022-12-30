# YUVPlayer
A cross-platform YUV player based on wxWidgets & SDL2.   
It can run on Windows, macOS or Linux.

# Third-party dependent libraries

Name|Version
---|---
[SDL](http://www.libsdl.org/)|[2.26](https://github.com/libsdl-org/SDL/releases/tag/release-2.26.1)
[wxWidgets](https://www.wxwidgets.org/)|[3.2.1](https://github.com/wxWidgets/wxWidgets/releases/tag/v3.2.1)


# Compilation Introduction

## Windows

Download **wxWidgets-3.2.1.zip** from
 https://github.com/wxWidgets/wxWidgets/releases/tag/v3.2.1.

Decompress the package and open `wxWidgets-3.2.1\build\msw\wx_vc17.sln` with VS2022.

Compile `Debug|x64` & `Release|x64`, then move all generated files in `wxWidgets-3.2.1\lib\vc_x64_lib\` to `YUVPlayer\wxWidgets\lib\vc_x64_lib\`.

## Linux/Ubuntu

### download & compile SDL

```sh
# tarball: https://github.com/libsdl-org/SDL/releases/tag/release-2.26.1

git clone https://github.com/libsdl-org/SDL.git
git checkout release-2.26.1
mkdir build && cd build
cmake ..
make install
```
or
```sh
sudo apt install libsdl2-dev
```

### download & compile wxWidgets

```sh
sudo apt install pkg-config libgtk2.0-dev libwxgtk3.0-gtk3-dev
sudo apt install mesa-common-dev libgl1-mesa-dev libglu1-mesa-dev
```
and
```
# tarball: https://github.com/wxWidgets/wxWidgets/releases/tag/v3.2.1

git clone https://github.com/wxWidgets/wxWidgets.git
git checkout v3.2.1
./configure --disable-shared
make
make install
```