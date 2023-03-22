#! /usr/bin/env python3
import os
import subprocess
import sys
from executeCommand import execCmd

PICON = False

for i, arg in enumerate(sys.argv):
    if i == 0:
        continue
    if arg == 'PIC=ON':
        PICON = True

PICSTRING = 'CMAKE_POSITION_INDEPENDENT_CODE=OFF'
        
if PICON == True:
    PICSTRING = 'CMAKE_POSITION_INDEPENDENT_CODE=ON'

output, error = execCmd('git submodule update --init --recursive')
output, error = execCmd('mkdir -p lib')
output, error = execCmd('cmake -B build -DCMAKE_OSX_ARCHITECTURES=arm64 -DCMAKE_BUILD_TYPE=Release -D FT_DISABLE_HARFBUZZ=TRUE -D{}'.format(PICSTRING), './freetype')
output, error = execCmd('make -j8', './freetype/build')
output, error = execCmd('mv ./libfreetype.a ../../lib/libfreetype.a', './freetype/build')

output, error = execCmd('cmake . -DCMAKE_OSX_ARCHITECTURES=arm64 -D{}'.format(PICSTRING), './libpng')
output, error = execCmd('make -j8', './libpng')
output, error = execCmd('mv ./libpng16.a ../lib/libpng16.a', './libpng')

output, error = execCmd('cmake . -DCMAKE_OSX_ARCHITECTURES=arm64 -D{}'.format(PICSTRING), './zlib')
output, error = execCmd('make -j8', './zlib')
output, error = execCmd('mv ./libz.a ../lib/libz.a', './zlib')

output, error = execCmd('mkdir -p build', 'bzip2-new')
output, error = execCmd('cmake ../ -DCMAKE_OSX_ARCHITECTURES=arm64 -DENABLE_STATIC_LIB=true -D{} ..'.format(PICSTRING), 'bzip2-new/build')
output, error = execCmd('cmake --build . --config Release', 'bzip2-new/build')
output, error = execCmd('mv libbz2_static.a ../../lib/libbz2.a', 'libbz2-new/build')

output, error = execCmd('cmake . -DCMAKE_OSX_ARCHITECTURES=arm64 -D{}'.format(PICSTRING), './brotli')
output, error = execCmd('make -j8', './brotli')
output, error = execCmd('mv libbrotlicommon-static.a ../lib', './brotli')
output, error = execCmd('mv libbrotlidec-static.a ../lib', './brotli')
output, error = execCmd('mv libbrotlienc-static.a ../lib', './brotli')

