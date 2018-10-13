cmake_minimum_required (VERSION 2.6)
project (HTMLtoMD)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories("${PROJECT_SOURCE_DIR}/myhtml-dist/include")
 
find_package (Threads)
add_executable(htmltomd src/main.c)
target_link_libraries(htmltomd ${PROJECT_SOURCE_DIR}/myhtml-dist/lib/libmyhtml_static.a)
target_link_libraries (htmltomd ${CMAKE_THREAD_LIBS_INIT})
