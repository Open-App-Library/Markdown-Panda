cmake_minimum_required (VERSION 2.6)
project (MarkdownPanda)

include_directories("${PROJECT_SOURCE_DIR}/include")

find_package (Threads)

if (EXCLUDE_MD2HTML MATCHES "yes")
  message(STATUS "Markdown->HTML conversion is DISABLED.")
  add_executable(mdpanda src/main.c src/markdownpanda.c)
  add_library( markdownpanda_static STATIC
               src/markdownpanda.c)
else()
  message(STATUS "Markdown->HTML conversion is ENABLED. If needed, you can disable it by passing the '-DEXCLUDE_MD2HTML=yes' option to cmake.")
  add_executable(mdpanda src/main.c src/markdownpanda.c src/markdownpanda_md2html.c)
  add_library( markdownpanda_static STATIC
               src/markdownpanda.c src/markdownpanda_md2html.c)
endif()

target_link_libraries(mdpanda ${PROJECT_SOURCE_DIR}/lib/libmyhtml_static.a)
target_link_libraries (mdpanda ${CMAKE_THREAD_LIBS_INIT})

install(TARGETS mdpanda markdownpanda_static
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib)

install(FILES src/markdownpanda.h DESTINATION include/markdownpanda)
install(FILES src/cpp.hpp DESTINATION include/markdownpanda)      
install(FILES src/qt.hpp DESTINATION include/markdownpanda)      
install(FILES lib/libmyhtml_static.a DESTINATION lib)

