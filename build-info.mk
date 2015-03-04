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
_CFLAGS = -Wall
_CXXFLAGS = -Wall -std=c++11
_SOURCES = source/media/android.c source/media/desktop.c source/main.cpp source/renderer.cpp source/mandelbrot32.cpp source/mandelbrot64.cpp source/mandelbrot128.cpp
_HEADERS = source/media/media.h source/renderer.hpp source/complex.hpp source/mandelbrot32.hpp source/mandelbrot64.hpp source/shader/mandel32.h source/shader/mandel64.h source/shader/mandel128.h source/mandelbrot128.hpp
