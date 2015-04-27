set(PROJECT dci-couple-translator)
project(${PROJECT})




############################################################
set(SRC
    src/main.cpp
    DciUseIdl.cmake
)


set(Boost_USE_STATIC_LIBS OFF)
find_package(Boost COMPONENTS program_options filesystem)
if(NOT Boost_FOUND)
    message(FATAL_ERROR "boost program_options or filesystem are not found")
endif()

include_directories(${Boost_INCLUDE_DIRS})



############################################################
add_executable(${PROJECT}
    ${INC}                  ${SRC}
)
set_property(TARGET ${PROJECT} PROPERTY CXX_STANDARD 14)
set_property(TARGET ${PROJECT} PROPERTY CXX_STANDARD_REQUIRED ON)

target_link_libraries(${PROJECT} ${Boost_PROGRAM_OPTIONS_LIBRARY} ${Boost_FILESYSTEM_LIBRARY})

target_link_libraries(${PROJECT}
    dci-couple-parser
    dci-couple-meta
    dci-couple-generator
)

if(COMMAND localinstall)
    localinstall(${PROJECT} share/cmake/Modules DciUseIdl.cmake)
endif()
