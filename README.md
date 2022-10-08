# simple-wallpaper
 Dynamic wallpaper  linux /windwos 

## What is this ?
This is a dynamic wallpaper scheme for Linux, using QT and libmpv

using mpv >0.27
using qt5

## ubuntu 18.04

sudo apt install qtcreator libx11-xcb-dev libxcb-ewmh-dev libmpv-dev mpv ffmpeg dpkg-dev fakeroot libmtdev-dev libqt5opengl5-dev qttools5-dev-tools qtbase5-dev qt5-qmake qtwebengine5-dev libxext-dev libxrender-dev libqt5x11extras5-dev libglib2.0-dev qt5-default build-essential g++ libxcb-shape0-dev build-essential debhelper

dpkg-buildpackage -b -us -uc

## windows

编译后将mpv的库移动到程序执行目录下，msvc编译才可以使用，mingw不行，因为有windows的api

