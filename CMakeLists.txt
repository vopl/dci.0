project(dci)
cmake_minimum_required(VERSION 3.0)

set(CMAKE_BUILD_TYPE Debug)


###########################################
set(LOCALINSTALL_DIR ${CMAKE_BINARY_DIR})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${LOCALINSTALL_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${LOCALINSTALL_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${LOCALINSTALL_DIR}/lib)

function(localinstall_copy target src dst)

    get_filename_component(dstDir ${dst} DIRECTORY)
    file(MAKE_DIRECTORY ${dstDir})

    if(NOT IS_ABSOLUTE ${src})
        set(src "${CMAKE_CURRENT_SOURCE_DIR}/${src}")
    endif()
    get_filename_component(src ${src} ABSOLUTE)

    add_custom_command(
        OUTPUT ${dst}
        DEPENDS ${src}
        COMMAND ${CMAKE_COMMAND} -E create_symlink ${src} ${dst}
    )
    if(EXISTS ${src})
        execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink ${src} ${dst})
    endif()
    target_sources(${target} PRIVATE ${dst})
endfunction()

macro(localinstall target dir)
    if("DROP_PREFIX" STREQUAL ${ARGV2})
        set(argn ${ARGN})
        math(EXPR up ${ARGC}-3)
        foreach(i RANGE 2 ${up})
            list(GET argn ${i} f)
            string(REGEX REPLACE "^${ARGV3}" "" fnp ${f})
            localinstall_copy(${target} ${f} ${LOCALINSTALL_DIR}/${dir}/${fnp})
        endforeach()
    else()
        foreach(f ${ARGN})
            localinstall_copy(${target} ${f} ${LOCALINSTALL_DIR}/${dir}/${f})
        endforeach()
    endif()
endmacro()

set(CMAKE_MODULE_PATH ${LOCALINSTALL_DIR}/share/cmake/Modules ${CMAKE_MODULE_PATH})
include_directories(${LOCALINSTALL_DIR}/include)

set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--no-undefined")
set(CMAKE_MODULE_LINKER_FLAGS "-Wl,--no-undefined")


###########################################
file(GLOB subs */CMakeLists.txt)
foreach(sub ${subs})
    get_filename_component(sub ${sub} DIRECTORY)
    get_filename_component(sub ${sub} NAME)
    add_subdirectory(${sub})
endforeach()
