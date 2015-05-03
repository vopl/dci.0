set(PROJECT dci-himpl)
project(${PROJECT})


############################################################
set(INC
    include/dci/himpl/faceLayout.hpp
    include/dci/himpl/layoutProvider.hpp
    include/dci/himpl/layoutProviderDefault.hpp
    include/dci/himpl/layoutFetcher.hpp
    include/dci/himpl/face2Impl.hpp
    include/dci/himpl/impl2Face.hpp
)

add_library(${PROJECT} STATIC
    ${INC}
    DciHimpl.cmake
)
set_target_properties(${PROJECT} PROPERTIES LINKER_LANGUAGE CXX)




if(COMMAND localinstall)
    localinstall(${PROJECT} include DROP_PREFIX include/ ${INC})
    localinstall(${PROJECT} share/cmake/Modules DciHimpl.cmake)
endif()


