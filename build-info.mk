# Desktop
_DESKTOP_LIBS = -lSDL2 -lGLEW -lGL
_DESKTOP_CFLAGS =
_DESKTOP_CXXFLAGS = 
_DESKTOP_SOURCES =

# Android
_ANDROID_LIBS = -llog -landroid  -lEGL -lGLESv2
_ANDROID_CFLAGS =
_ANDROID_CXXFLAGS = 
_ANDROID_SOURCES =

# Common
_CFLAGS = 
_CXXFLAGS = -std=c++11
_SOURCES = source/media/android.c source/media/desktop.c source/main.cpp source/renderer.cpp
_HEADERS = source/media/media.h source/renderer.hpp source/complex.hpp
