set(PROJECT dci-himpl)
project(${PROJECT})


############################################################
set(INC
    include/faceLayout.hpp
    include/layoutProvider.hpp
    include/layoutProviderDefault.hpp
    include/layoutFetcher.hpp
    include/face2Impl.hpp
    include/impl2Face.hpp
)

add_library(${PROJECT} STATIC
    ${INC}
)
set_target_properties(${PROJECT} PROPERTIES LINKER_LANGUAGE CXX)




if(COMMAND localinstall)
    localinstall(${PROJECT} include/dci/himpl DROP_PREFIX include/ ${INC})
    localinstall(${PROJECT} share/cmake/Modules DciHimpl.cmake)
endif()


