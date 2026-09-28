cmake_minimum_required(VERSION 3.20)
project(gtk4_prog01)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/exe)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED gtk4)

find_program(BLUEPRINT_COMPILER blueprint-compiler REQUIRED)
find_program(GLIB_COMPILE_RESOURCES glib-compile-resources REQUIRED)

# Compile Blueprint (.blp) -> GtkBuilder XML (.ui)
set(BLUEPRINT_FILE ${CMAKE_CURRENT_SOURCE_DIR}/window.blp)
set(UI_FILE        ${CMAKE_CURRENT_BINARY_DIR}/window.ui)

add_custom_command(
    OUTPUT  ${UI_FILE}
    COMMAND ${BLUEPRINT_COMPILER} compile --output ${UI_FILE} ${BLUEPRINT_FILE}
    DEPENDS ${BLUEPRINT_FILE}
    COMMENT "blueprint-compiler: window.blp -> window.ui"
)

# Bundle the compiled UI into a C source file via GResource
set(GRESOURCE_XML ${CMAKE_CURRENT_SOURCE_DIR}/resources.gresource.xml)
set(GRESOURCE_C   ${CMAKE_CURRENT_BINARY_DIR}/resources.c)

add_custom_command(
    OUTPUT  ${GRESOURCE_C}
    COMMAND ${GLIB_COMPILE_RESOURCES}
                --target=${GRESOURCE_C}
                --sourcedir=${CMAKE_CURRENT_BINARY_DIR}
                --generate-source
                ${GRESOURCE_XML}
    DEPENDS ${GRESOURCE_XML} ${UI_FILE}
    COMMENT "glib-compile-resources: resources.gresource.xml -> resources.c"
)

add_executable(gtk4_prog01
    main.cpp
    ${GRESOURCE_C}
)

target_include_directories(gtk4_prog01 PRIVATE ${GTK4_INCLUDE_DIRS})
target_link_libraries(gtk4_prog01      PRIVATE ${GTK4_LIBRARIES})
target_compile_options(gtk4_prog01     PRIVATE ${GTK4_CFLAGS_OTHER})