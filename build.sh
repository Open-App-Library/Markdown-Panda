#!/bin/bash
set -e

# Will install to build/ if INSTALL_PREFIX variable is not set.
INSTALL_DIR=${INSTALL_PREFIX:-build/} 

MYHTML_VERSION=4.0.5
CMARK_VERSION=0.28.3.gfm.19

# MyHTML
echo MyHTML
cd external-libraries/myhtml-$MYHTML_VERSION
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
cmake -DCMAKE_INSTALL_PREFIX=../$INSTALL_DIR ..
make
make install

echo
echo
echo ====================================================
echo SUCCESS! Installed MarkdownPanda to \'$INSTALL_DIR\'
echo ----------------------------------------------------
echo If you would like to customize where MarkdownPanda
echo is installed, set the env. variable INSTALL_PREFIX
echo before running build.sh
echo example: INSTALL_PREFIX=myapp sh build.sh
echo ====================================================
echo
