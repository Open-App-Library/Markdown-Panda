cmake_minimum_required (VERSION 2.6)
project (MarkdownPanda)

include_directories("${PROJECT_SOURCE_DIR}/include")
 
find_package (Threads)

add_executable(mdpanda src/main.c src/markdownpanda.c)

target_link_libraries(mdpanda ${PROJECT_SOURCE_DIR}/lib/libmyhtml_static.a)
target_link_libraries (mdpanda ${CMAKE_THREAD_LIBS_INIT})

ADD_LIBRARY( MarkdownPanda STATIC
             src/markdownpanda.c )
