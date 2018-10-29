#!/bin/bash
set -e

# MyHTML
echo MyHTML
cd external-libraries/myhtml
ls
make
make install prefix=../..
cd ../..

#CMark-GFM
echo CMark-GFM
cd external-libraries/cmark-gfm
mkdir -p build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=../../..
make
make install
cd ../../..

# MarkdownPanda
echo MarkdownPanda
mkdir -p cmake && cd cmake
cmake -DCMAKE_INSTALL_PREFIX=../build ..
make
make install

echo
echo DONE! Installed to 'build/'
