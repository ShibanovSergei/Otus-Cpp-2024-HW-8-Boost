cmake_minimum_required (VERSION 3.2)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

project(HW_08_Boost VERSION ${PROJECT_VERSION})

#configure_file(version.h.in version.h)

add_executable (HW_08_Boost "main.cpp" "main.h")

set_target_properties(HW_08_Boost PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)
target_include_directories(HW_08_Boost PRIVATE "${CMAKE_BINARY_DIR}")

if (MSVC)
    target_compile_options(HW_08_Boost PRIVATE
        /W4
    )
else ()
    target_compile_options(HW_08_Boost PRIVATE
        -Wall -Wextra -pedantic
    )
endif()

install(TARGETS HW_08_Boost RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)