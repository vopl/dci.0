set(PROJECT dci-couple-generator)
project(${PROJECT})




############################################################
set(INC
    include/executor.hpp
)


############################################################
set(SRC
    src/executor.cpp

    src/impl/executorCpp.hpp
    src/impl/executorCpp.cpp
)






############################################################
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)






############################################################
add_library(${PROJECT} SHARED
    ${INC}                  ${SRC}
)
set_property(TARGET ${PROJECT} PROPERTY CXX_STANDARD 14)
set_property(TARGET ${PROJECT} PROPERTY CXX_STANDARD_REQUIRED ON)



if(COMMAND localinstall)
    localinstall(${PROJECT} include/dci/couple/generator DROP_PREFIX include/ ${INC})
endif()
