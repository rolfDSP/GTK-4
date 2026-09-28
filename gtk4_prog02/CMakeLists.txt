cmake_minimum_required(VERSION 3.20)
project(gtk4_prog02)

set(CMAKE_CXX_STANDARD 20)

# ── Output directory ──────────────────────────────────────────────────────────
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/exe")

# ── Dependencies ──────────────────────────────────────────────────────────────
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED gtk4)

find_program(BLUEPRINT_COMPILER blueprint-compiler REQUIRED
    DOC "Blueprint UI compiler (blueprint-compiler)")

find_program(GLIB_COMPILE_RESOURCES glib-compile-resources REQUIRED
    DOC "GLib resource compiler (glib-compile-resources)")

# ── Compile .blp → .ui ───────────────────────────────────────────────────────
set(BLP_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/ui")
set(BLP_OUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/ui")

set(UI_NAMES
    main_window
    about_window
    settings_window
    info_window
)

set(UI_FILES "")
foreach(NAME ${UI_NAMES})
    set(BLP "${BLP_SRC_DIR}/${NAME}.blp")
    set(UI  "${BLP_OUT_DIR}/${NAME}.ui")
    add_custom_command(
        OUTPUT  "${UI}"
        COMMAND "${CMAKE_COMMAND}" -E make_directory "${BLP_OUT_DIR}"
        COMMAND "${BLUEPRINT_COMPILER}" compile --output "${UI}" "${BLP}"
        DEPENDS "${BLP}"
        COMMENT "Blueprint: ${NAME}.blp → ${NAME}.ui"
        VERBATIM
    )
    list(APPEND UI_FILES "${UI}")
endforeach()

# ── Compile GResource bundle ──────────────────────────────────────────────────
set(GRESOURCE_XML "${CMAKE_CURRENT_SOURCE_DIR}/resources/resources.gresource.xml")
set(GRESOURCE_C   "${CMAKE_CURRENT_BINARY_DIR}/resources.c")

add_custom_command(
    OUTPUT  "${GRESOURCE_C}"
    COMMAND "${GLIB_COMPILE_RESOURCES}"
                "--target=${GRESOURCE_C}"
                "--sourcedir=${CMAKE_CURRENT_BINARY_DIR}"
                "--generate-source"
                "${GRESOURCE_XML}"
    DEPENDS "${GRESOURCE_XML}" ${UI_FILES}
    COMMENT "Compiling GResources → resources.c"
    VERBATIM
)

# ── Executable ────────────────────────────────────────────────────────────────
add_executable(gtk4_prog02
    main.cpp
    src/main_window.cpp
    src/settings_window.cpp
    "${GRESOURCE_C}"
)

target_include_directories(gtk4_prog02 PRIVATE
    ${GTK4_INCLUDE_DIRS}
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

target_link_libraries(gtk4_prog02 PRIVATE
    ${GTK4_LIBRARIES}
)

target_compile_options(gtk4_prog02 PRIVATE
    ${GTK4_CFLAGS_OTHER}
)
