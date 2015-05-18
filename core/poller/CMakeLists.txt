set(PROJECT dci-poller)
project(${PROJECT})




############################################################
set(INC
    include/dci/poller.hpp
    include/dci/poller/layoutProvider.hpp
    include/dci/poller/descriptor.hpp
)

############################################################
set(SRC
    src/descriptor.cpp

    src/impl/descriptor.hpp
    src/impl/descriptor.cpp

    src/impl/engine.hpp
    src/impl/engine.cpp

    src/impl/engineEpoll.hpp
    src/impl/engineEpoll.cpp
)




############################################################
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)





############################################################
add_library(${PROJECT} SHARED
    ${INC}                  ${SRC}
)
set_property(TARGET ${PROJECT} PROPERTY CXX_STANDARD 14)
set_property(TARGET ${PROJECT} PROPERTY CXX_STANDARD_REQUIRED ON)

target_link_libraries(${PROJECT}
    dci-logger
    dci-himpl
    dci-async)

include(DciHimpl)
mkHimplLayouts(${PROJECT}
    OUTPUT
        ${CMAKE_CURRENT_SOURCE_DIR}/include/dci/poller/layoutProvider.hpp

    HEADERS
        ${CMAKE_CURRENT_SOURCE_DIR}/src/impl/descriptor.hpp

    CLASSES
        ::dci::poller::impl::Descriptor
)



if(COMMAND localinstall)
    localinstall(${PROJECT} include DROP_PREFIX include/ ${INC})
endif()
