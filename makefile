test:
	clang++ -g -O0 -I freetype/include/ -std=c++20 main.cpp lib/* -framework CoreGraphics -framework CoreFoundation -framework CoreText -o test 
library:
	mkdir -p obj/
	mkdir -p lib/
	clang++ -c -O3 -I freetype/include/ -std=c++20 StringToText.cpp -o obj/StringToText.o
	ar rvs ./lib/stringToText.a ./obj/StringToText.o
library-POC:
	mkdir -p obj/
	mkdir -p lib/
	clang++ -c -O3 -fPIC -I freetype/include/ -std=c++20 StringToText.cpp -o obj/StringToText.o
	ar rvs ./lib/stringToText.a ./obj/StringToText.o
clean:
	rm obj/*
	rm test
