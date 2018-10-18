cmake_minimum_required (VERSION 2.6)
project (MarkdownPanda)

include_directories("${PROJECT_SOURCE_DIR}/include")

find_package (Threads)

add_executable(mdpanda src/main.c src/markdownpanda.c)

target_link_libraries(mdpanda ${PROJECT_SOURCE_DIR}/lib/libmyhtml_static.a)
target_link_libraries (mdpanda ${CMAKE_THREAD_LIBS_INIT})

add_library( markdownpanda_static STATIC
             src/markdownpanda.c)

install(TARGETS mdpanda markdownpanda_static
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib)

install(FILES src/markdownpanda.h DESTINATION include/markdownpanda)
install(FILES src/cpp.hpp DESTINATION include/markdownpanda)      
install(FILES src/qt.hpp DESTINATION include/markdownpanda)      
install(FILES lib/libmyhtml_static.a DESTINATION lib)

