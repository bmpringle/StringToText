default:
	clang++ -g -O0 -I freetype/include/ -std=c++20 main.cpp lib/* -framework CoreGraphics -framework CoreFoundation -framework CoreText -o test 