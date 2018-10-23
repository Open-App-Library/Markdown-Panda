cmake_minimum_required (VERSION 2.6)
project (MarkdownPanda)

set(CMAKE_EXPORT_COMPILE_COMMANDS yes)

include_directories("${PROJECT_SOURCE_DIR}/include")
include_directories("${PROJECT_SOURCE_DIR}/src")

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
target_link_libraries(mdpanda ${PROJECT_SOURCE_DIR}/lib/libhoedown.a)
target_link_libraries(mdpanda ${CMAKE_THREAD_LIBS_INIT})

# Unit testing
find_library(CRITERION criterion)
if (CRITERION)
  add_executable(RunUnitTests test/src/main.c)
  add_executable(TestHelpersTest test/src/test-helpers-test.c)
  target_link_libraries(RunUnitTests criterion)
  target_link_libraries(RunUnitTests markdownpanda_static)
  target_link_libraries(RunUnitTests ${PROJECT_SOURCE_DIR}/lib/libmyhtml_static.a)
  target_link_libraries(RunUnitTests ${PROJECT_SOURCE_DIR}/lib/libhoedown.a)
  target_link_libraries(RunUnitTests ${CMAKE_THREAD_LIBS_INIT})
  target_link_libraries(TestHelpersTest markdownpanda_static)
  target_link_libraries(TestHelpersTest ${PROJECT_SOURCE_DIR}/lib/libmyhtml_static.a)
  target_link_libraries(TestHelpersTest ${PROJECT_SOURCE_DIR}/lib/libhoedown.a)
  target_link_libraries(TestHelpersTest ${CMAKE_THREAD_LIBS_INIT})
else()
  message(WARNING "Criterion unit testing framework is NOT installed. You may proceed but no unit tests will be generated.")
endif()

# Install
install(TARGETS mdpanda markdownpanda_static
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib)
install(FILES src/markdownpanda.h DESTINATION include/markdownpanda)
install(FILES src/cpp.hpp DESTINATION include/markdownpanda)      
install(FILES src/qt.hpp DESTINATION include/markdownpanda)      
install(FILES lib/libmyhtml_static.a DESTINATION lib)
install(FILES lib/libhoedown.a DESTINATION lib)

