#! /usr/bin/env python3
import os
import subprocess
from executeCommand import execCmd

output, error = execCmd('git submodule update --init --recursive')
output, error = execCmd('mkdir -p lib')
output, error = execCmd('cmake -B build -DCMAKE_BUILD_TYPE=Release -D FT_DISABLE_HARFBUZZ=TRUE', './freetype')
output, error = execCmd('make -j8', './freetype/build')
output, error = execCmd('mv ./libfreetype.a ../../lib/libfreetype.a', './freetype/build')

output, error = execCmd('cmake . -DPNG_ARM_NEON=off', './libpng')
output, error = execCmd('make -j8', './libpng')
output, error = execCmd('mv ./libpng16.a ../lib/libpng16.a', './libpng')

output, error = execCmd('cmake .', './zlib')
output, error = execCmd('make -j8', './zlib')
output, error = execCmd('mv ./libz.a ../lib/libz.a', './zlib')

output, error = execCmd('make -j8', './bzip2')
output, error = execCmd('mv ./libbz2.a ../lib/libbz2.a', './bzip2')

output, error = execCmd('cmake .', './brotli')
output, error = execCmd('make -j8', './brotli')
output, error = execCmd('mv libbrotlicommon-static.a ../lib', './brotli')
output, error = execCmd('mv libbrotlidec-static.a ../lib', './brotli')
output, error = execCmd('mv libbrotlienc-static.a ../lib', './brotli')

