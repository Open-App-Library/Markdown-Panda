cmake_minimum_required (VERSION 2.6)
project (MarkdownPanda)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories("${PROJECT_SOURCE_DIR}/include")
 
find_package (Threads)
add_executable(mdpanda src/main.c)
target_link_libraries (mdpanda ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(mdpanda ${PROJECT_SOURCE_DIR}/lib/libmyhtml_static.a)
