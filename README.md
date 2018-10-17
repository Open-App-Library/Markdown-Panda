MarkdownPanda: WORK IN PROGRESS
-------------------------------

**Table of Contents**

- [Usage](#usage)
  - [C](#c)
  - [C++](#c)
  - [C++ / Qt5](#c--qt5)
- [Manual Installation (Linux / Mac OS X)](#manual-installation-linux--mac-os-x)
  - [Installing dependencies](#installing-dependencies)
  - [Compiling the library](#compiling-the-library)
- [How to compile your program](#how-to-compile-your-program)

## Usage

This library provides C, C++, and Qt5 enhanced bindings in order to make your life easier. This library can be installed by following through any of the installation sections of this readme.

To compile any of these examples, please follow the [How to compile your program](#how-to-compile-your-program) section.

### C

**Converting HTML to markdown:**

```c
#include <stdio.h>
#include <markdownpanda.h>

int main()
{
  HtmlObject obj = load_html_from_string("<h1>Test</h1>");
  // HtmlObject obj = load_html_from_file("myfile.html");
  char *md = mdpanda_to_markdown(obj);
  printf("%s", md);
  destroy_html_object(obj);
}
```

### C++

### C++ / Qt5

## Manual Installation (Linux / Mac OS X)

Installing is pretty simple if you've got a little bit of experience with C/C++ libraries.

### Installing dependencies

In the following commands, you will clone the MarkdownPanda reposotory, change directory into it, pull down the MyHTML submodule (a dependency), compile MyHTML, and install it to the project root.

1. `git clone https://gitlab.com/Open-App-Library/markdownpanda.git`
2. `cd markdownpanda`
3. `git submodule update --init`
4. `cd external-libraries/myhtml`
5. `make`
6. `make install prefix="../.."`

### Compiling the library

With the following commands you will compile the project to a folder named "cmake" and then install the files into a folder named "dist".

- `mkdir cmake && cd cmake`
- `cmake -DCMAKE_INSTALL_PREFIX=../dist ..`
- `make install`


## How to compile your program

On Linux, you can compile with GCC.

```bash
gcc your-file.c -I /path-to/markdownpanda/dist/include/ -L /path-to/markdownpanda/dist/lib/ -l markdownpanda_static -l myhtml_static -pthread
```

Note that you are specifying the path where the markdownpanda header files are, where the static libraries are, what static libraries to use, and you are finally specifying that you would like to use pthread.

Here is a CMake template as well.

**CMakeLists.txt**

```cmake
cmake_minimum_required (VERSION 2.6)
project (MyApp)

include_directories("${PROJECT_SOURCE_DIR}/path-to/markdownpanda/dist/include")

find_package (Threads)

add_executable(myapp src/main.c)

target_link_libraries(mdpanda ${PROJECT_SOURCE_DIR}/path-to/markdownpanda/dist/lib/markdownpanda_static.a)
target_link_libraries(mdpanda ${PROJECT_SOURCE_DIR}/path-to/markdownpanda/dist/lib/myhtml_static.a)
target_link_libraries (mdpanda ${CMAKE_THREAD_LIBS_INIT})
```
