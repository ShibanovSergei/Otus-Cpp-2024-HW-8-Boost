cmake_minimum_required (VERSION 3.2)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

project(bayan VERSION ${PROJECT_VERSION})

#configure_file(version.h.in version.h)

find_package(Boost 1.74.0 REQUIRED COMPONENTS system filesystem program_options)

add_executable (bayan 
       "main.cpp" "main.h" 
       "CmdLineOptions.cpp" "CmdLineOptions.h" 
       "BlockHashGetter.cpp" 
       "FileReader.h" "FileReader.cpp" 
       "FilesDuplicatesSeacher.h" "FilesDuplicatesSeacher.cpp")

set_target_properties(bayan PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

if (Boost_FOUND)
    target_link_libraries(bayan PRIVATE 
        Boost::system 
        Boost::filesystem 
        Boost::program_options
    )
endif()

if (MSVC)
    target_compile_options(bayan PRIVATE
        /W4
    )
else ()
    target_compile_options(bayan PRIVATE
        -Wall -Wextra -pedantic
    )
endif()

install(TARGETS bayan RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)