set(PROJECT dci-couple-serialize)
project(${PROJECT})


############################################################
set(INC
    include/dci/couple/serialize.hpp
    include/dci/couple/serialize/details/processor.hpp
)

add_library(${PROJECT} STATIC
    ${INC}
)
set_target_properties(${PROJECT} PROPERTIES LINKER_LANGUAGE CXX)


if(COMMAND localinstall)
    localinstall(${PROJECT} include DROP_PREFIX include/ ${INC})
endif()


