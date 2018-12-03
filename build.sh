#!/bin/bash
set -e

MYHTML_VERSION=4.0.5
CMARK_VERSION=0.28.3.gfm.19

# MyHTML
echo MyHTML
cd external-libraries/myhtml-$MYHTML_VERSION
ls
make
make install prefix=../..
cd ../..

#CMark-GFM
echo CMark-GFM
cd external-libraries/cmark-gfm-$CMARK_VERSION
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
