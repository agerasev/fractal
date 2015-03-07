# Desktop
_DESKTOP_LIBS = -g -lSDL2 -lGLEW -lGL
_DESKTOP_CFLAGS = -g
_DESKTOP_CXXFLAGS = -g
_DESKTOP_SOURCES =

# Android
_ANDROID_LIBS = -llog -landroid  -lEGL -lGLESv2
_ANDROID_CFLAGS =
_ANDROID_CXXFLAGS = 
_ANDROID_SOURCES =

# Common
_CFLAGS = -Wall
_CXXFLAGS = -Wall -std=c++11
_SOURCES = source/media/android.c source/media/desktop.c source/main.cpp source/renderer.cpp source/mandelbrot/mandelbrot128.cpp source/mandelbrot/mandelbrot32.cpp source/mandelbrot/mandelbrot64.cpp source/framebuffer.cpp source/director.cpp source/videodirector.cpp source/mandelbrot/mandelbrot.cpp
_HEADERS = source/media/media.h source/renderer.hpp source/complex.hpp source/shader/mandel32.h source/shader/mandel64.h source/shader/mandel128.h source/mandelbrot/mandelbrot128.hpp source/mandelbrot/mandelbrot32.hpp source/mandelbrot/mandelbrot64.hpp source/framebuffer.hpp source/shader/texture.h source/director.hpp source/videodirector.hpp source/mandelbrot/mandelbrot.hpp
