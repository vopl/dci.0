set(PROJECT dci-api)
project(${PROJECT})


############################################################
set(INC
    include/dci/api.hpp
)

add_library(${PROJECT} STATIC
    ${INC}
    dciApi.cmake
)
set_target_properties(${PROJECT} PROPERTIES LINKER_LANGUAGE CXX)


if(COMMAND localinstall)
    localinstall(${PROJECT} include DROP_PREFIX include/ ${INC})
    localinstall(${PROJECT} share/cmake/Modules dciApi.cmake)
endif()


