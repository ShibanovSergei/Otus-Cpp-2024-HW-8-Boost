cmake_minimum_required (VERSION 3.2)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

project(Bayan VERSION ${PROJECT_VERSION})

#configure_file(version.h.in version.h)

find_package(Boost COMPONENTS program_options filesystem regex REQUIRED)

add_executable (Bayan "main.cpp" "main.h")

set_target_properties(Bayan PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)
target_include_directories(Bayan PRIVATE "${CMAKE_BINARY_DIR}")

if (MSVC)
    target_compile_options(Bayan PRIVATE
        /W4
    )
else ()
    target_compile_options(Bayan PRIVATE
        -Wall -Wextra -pedantic
    )
endif()

install(TARGETS Bayan RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)