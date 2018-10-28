MarkdownPanda
-------------------------------

<!-- markdown-toc start - Don't edit this section. Run M-x markdown-toc-refresh-toc -->
**Table of Contents**

- [Usage](#usage)
    - [C](#c)
    - [C++](#c)
    - [C++ / Qt5](#c--qt5)
    - [Manual Installation (Linux / Mac OS X)](#manual-installation-linux-mac-os-x)
        - [Installing dependencies](#installing-dependencies)
            - [MyHTML](#myhtml)
            - [CMark-GFM](#cmark-gfm)
            - [MarkdownPanda](#markdownpanda)
    - [How to compile your program](#how-to-compile-your-program)
    - [Compile Without Markdown->HTML support](#compile-without-markdown-html-support)

<!-- markdown-toc end -->

## State of MarkdownPanda

Everything is functional. I must implement unit testing to ensure accurate and consistant bi-directional markdown/html conversion.

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
  // HTML to Markdown
  HtmlObject obj = load_html_from_string("<h1>Test</h1>");
  // HtmlObject obj = load_html_from_file("myfile.html");
  char *md = mdpanda_to_markdown(obj);
  printf("%s", md);
  destroy_html_object(obj);

  // Markdown to HTML
  char *html = mdpanda_to_html("# Test");
  printf("%s", html);
  free(html); // HTML is on the heap so we must free it.
}
```

### C++

```c++
#include <iostream>
#include <markdownpanda/cpp.hpp>

int main()
{
  // HTML to Markdown
  MarkdownPanda panda;
  panda.loadHtmlFile("index.html");
  // panda.loadHtmlString("<p>You could also use an std::string like this</p>");
  std::cout << panda.markdown();

  // Markdown to HTML
  MarkdownPanda panda;
  panda.loadMarkdownFile("index.md");
  // panda.loadMarkdownString("You could also load an **std::string** like this.")
  std::cout << panda.html();
}

```

### C++ / Qt5

```c++
#include <iostream>
#include <markdownpanda/qt.hpp>

int main()
{
  // HTML to Markdown
  MarkdownPandaQt panda;
  panda.loadHtmlFile(":/index.html"); // Loading resource file or regular file
  // panda.loadHtmlString("<p>You could also use a QString like this</p>");
  std::cout << panda.markdown().toStdString();


  // Markdown to HTML
  MarkdownPandaQt panda;
  panda.loadMarkdownFile(":/index.md"); // Loading resource file or regular file
  // panda.loadMarkdownString("You could also use a **QString** like this");
  std::cout << panda.html().toStdString();
}

```

## Manual Installation (Linux / Mac OS X)

Installing is pretty simple if you've got a little bit of experience with C/C++ libraries.

### Installing dependencies

You will need the following C libraries:

- [myhtml](https://github.com/lexborisov/myhtml)
- [CMark GFM](https://github.com/github/cmark-gfm) (Only needed if you need Markdown->HTML conversion)

If you don't want to install these on your own, you could install them locally in the following way.

In the following commands, you will clone the MarkdownPanda repository, change directory into it, pull down the MyHTML submodule (a dependency), compile MyHTML, and install it to the project root.

1. `git clone https://gitlab.com/Open-App-Library/markdownpanda.git`
2. `cd markdownpanda`
3. `git submodule update --init`

#### MyHTML

1. `cd external-libraries/myhtml`
2. `make`
3. `make install prefix=../..`

#### CMark-GFM

1. `cd external-libraries/cmark-gfm`
2. `mkdir build && cd build`
3. `cmake .. -DCMAKE_INSTALL_PREFIX=../../..`
4. `make`
5. `make install`

#### MarkdownPanda

With the following commands you will compile the project to a folder named "cmake" and then install the files into a folder named "dist".

- `mkdir cmake && cd cmake`
- `cmake -DCMAKE_INSTALL_PREFIX=../dist ..`
- `make install`

## How to compile your program

On Linux, you can compile with GCC.

```bash
gcc your-file.c -I /path-to/markdownpanda/dist/include/ -L /path-to/markdownpanda/dist/lib/ -lmarkdownpanda_static -lmyhtml_static -lcmark-gfm-extensions -lcmark-gfm -pthread
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

target_link_libraries(mdpanda ${PROJECT_SOURCE_DIR}/path-to/markdownpanda/dist/lib/libmarkdownpanda_static.a)
target_link_libraries(mdpanda ${PROJECT_SOURCE_DIR}/path-to/markdownpanda/dist/lib/libmyhtml_static.a)
target_link_libraries(mdpanda ${PROJECT_SOURCE_DIR}/path-to/markdownpanda/dist/lib/libcmark-gfm-extensions.a)
target_link_libraries(mdpanda ${PROJECT_SOURCE_DIR}/path-to/markdownpanda/dist/lib/libcmark-gfm.a)
target_link_libraries (mdpanda ${CMAKE_THREAD_LIBS_INIT})
```

## Compile Without Markdown->HTML support

Pass `EXCLUDE_MD2HTML=yes` to cmake. Ex. `cmake .. -DEXCLUDE_MD2HTML=yes`
