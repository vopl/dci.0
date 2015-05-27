set(PROJECT dci-poll)
project(${PROJECT})




############################################################
set(INC
    include/dci/poll.hpp
    include/dci/poll/api.hpp
    include/dci/poll/layoutProvider.hpp
    include/dci/poll/error.hpp
    include/dci/poll/descriptor.hpp
    include/dci/poll/timer.hpp
    include/dci/poll/functions.hpp
)

############################################################
set(SRC
    src/error.cpp

    src/descriptor.cpp
    src/timer.cpp
    src/functions.cpp

    src/impl/descriptor.hpp
    src/impl/descriptor.cpp

    src/impl/timer.hpp
    src/impl/timer.cpp

    src/impl/timerEngine.hpp
    src/impl/timerEngine.cpp
    src/impl/timerEngine/gridPoint.hpp
    src/impl/timerEngine/gridPoint.cpp
    src/impl/timerEngine/grid.hpp

    src/impl/engine.hpp
    src/impl/engine.cpp

    src/impl/epoll.hpp
    src/impl/epoll.cpp
)




############################################################
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)





############################################################
add_library(${PROJECT} SHARED
    ${INC}                  ${SRC}
)
set_property(TARGET ${PROJECT} PROPERTY CXX_STANDARD 14)
set_property(TARGET ${PROJECT} PROPERTY CXX_STANDARD_REQUIRED ON)

include(dciApi)
dciApiExport(${PROJECT})

target_link_libraries(${PROJECT}
    dci-logger
    dci-himpl
    dci-async)

include(dciHimpl)
dciMakeHimplLayouts(${PROJECT}
    OUTPUT
        ${CMAKE_CURRENT_SOURCE_DIR}/include/dci/poll/layoutProvider.hpp

    HEADERS
        ${CMAKE_CURRENT_SOURCE_DIR}/src/impl/descriptor.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/impl/timer.hpp

    CLASSES
        ::dci::poll::impl::Descriptor
        ::dci::poll::impl::Timer
)



if(COMMAND localinstall)
    localinstall(${PROJECT} include DROP_PREFIX include/ ${INC})
endif()
