cmake_minimum_required (VERSION 2.6)
project (MarkdownPanda)

include_directories("${PROJECT_SOURCE_DIR}/include")
include_directories("${PROJECT_SOURCE_DIR}/src")
include_directories("${PROJECT_SOURCE_DIR}/external-libraries/sds/")

# Settings CMAKE variables
set(CMAKE_EXPORT_COMPILE_COMMANDS yes)

# Settings variables
set(SOURCE_FILES_MDPANDA src/markdownpanda.c src/plugins.c)
set(SOURCE_FILES ${SOURCE_FILES_MDPANDA} src/markdownpanda_md2html.c external-libraries/sds/sds.c)

# Platform Specific
if(APPLE)
  include_directories(/usr/local/include)
  link_directories(/usr/local/lib)
endif()


# Check if user wants to disable markdown->HTML conversion
if (EXCLUDE_MD2HTML MATCHES "yes")
  message(STATUS "Markdown->HTML conversion is DISABLED.")
else()
  message(STATUS "Markdown->HTML conversion is ENABLED. If needed, you can disable it by passing the '-DEXCLUDE_MD2HTML=yes' option to cmake.")
	set(SOURCE_FILES ${SOURCE_FILES} src/markdownpanda_md2html.c)
endif()

add_executable(mdpanda src/main.c ${SOURCE_FILES})
add_library(markdownpanda_static STATIC ${SOURCE_FILES})

find_package (Threads)
target_link_libraries(mdpanda ${PROJECT_SOURCE_DIR}/lib/libmyhtml_static.a)
target_link_libraries(mdpanda ${PROJECT_SOURCE_DIR}/lib/libcmark-gfm-extensions.a)
target_link_libraries(mdpanda ${PROJECT_SOURCE_DIR}/lib/libcmark-gfm.a)
target_link_libraries(mdpanda ${CMAKE_THREAD_LIBS_INIT})

# Unit testing
find_library(CRITERION criterion)
if (CRITERION)
  add_executable(RunUnitTests test/src/main-test.c)
  target_link_libraries(RunUnitTests criterion)
  target_link_libraries(RunUnitTests markdownpanda_static)
  target_link_libraries(RunUnitTests ${PROJECT_SOURCE_DIR}/lib/libmyhtml_static.a)
  target_link_libraries(RunUnitTests ${PROJECT_SOURCE_DIR}/lib/libcmark-gfm-extensions.a)
  target_link_libraries(RunUnitTests ${PROJECT_SOURCE_DIR}/lib/libcmark-gfm.a)
  target_link_libraries(RunUnitTests ${CMAKE_THREAD_LIBS_INIT})
else()
  message(WARNING "Criterion unit testing framework is NOT installed. You may proceed but no unit tests will be generated.")
endif()

add_executable(NotAUnitTest test/src/not-a-unit-test.c)
target_link_libraries(NotAUnitTest markdownpanda_static)
target_link_libraries(NotAUnitTest ${PROJECT_SOURCE_DIR}/lib/libmyhtml_static.a)
target_link_libraries(NotAUnitTest ${PROJECT_SOURCE_DIR}/lib/libcmark-gfm-extensions.a)
target_link_libraries(NotAUnitTest ${PROJECT_SOURCE_DIR}/lib/libcmark-gfm.a)
target_link_libraries(NotAUnitTest ${CMAKE_THREAD_LIBS_INIT})


# Install
install(TARGETS mdpanda markdownpanda_static
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib)
install(FILES src/markdownpanda.h DESTINATION include/markdownpanda)
install(FILES src/cpp.hpp DESTINATION include/markdownpanda)
install(FILES src/qt.hpp DESTINATION include/markdownpanda)
install(FILES lib/libmyhtml_static.a DESTINATION lib)
install(FILES lib/libcmark-gfm-extensions.a DESTINATION lib)
install(FILES lib/libcmark-gfm.a DESTINATION lib)
