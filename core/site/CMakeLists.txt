set(PROJECT dci-site)

project(${PROJECT})

set(INC
    include/dci/site/error.hpp
    include/dci/site/manager.hpp
    include/dci/site/moduleId.hpp
    include/dci/site/modulePlace.hpp
    include/dci/site/moduleEntry.hpp
    include/dci/site/moduleInfo.hpp
    include/dci/site/serviceFactory.hpp
    include/dci/site/layoutProvider.hpp
)

set(SRC
    src/error.cpp
    src/manager.cpp
    src/modulePlace.cpp

    src/impl/manager.hpp
    src/impl/module.hpp
    src/impl/modulePlace.hpp
    src/impl/moduleState.hpp

    src/impl/manager.cpp
    src/impl/module.cpp
    src/impl/modulePlace.cpp
)

############################################################
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)





find_package(Boost COMPONENTS program_options filesystem)
if(NOT Boost_FOUND)
    message(FATAL_ERROR "boost program_options not found")
endif()

include_directories(${Boost_INCLUDE_DIRS})


add_library(${PROJECT} SHARED
    ${INC}  ${SRC}
    DciDefineModule.cmake
)
set_property(TARGET ${PROJECT} PROPERTY CXX_STANDARD 14)
set_property(TARGET ${PROJECT} PROPERTY CXX_STANDARD_REQUIRED ON)

target_link_libraries(${PROJECT}
    dci-mm
    dci-async
    dci-poller
    dci-couple-runtime
    dl
)
target_link_libraries(${PROJECT} ${Boost_FILESYSTEM_LIBRARY})


add_executable(${PROJECT}-executable
    src/main.cpp
)
set_property(TARGET ${PROJECT}-executable PROPERTY OUTPUT_NAME ${PROJECT})
set_property(TARGET ${PROJECT}-executable PROPERTY CXX_STANDARD 14)
set_property(TARGET ${PROJECT}-executable PROPERTY CXX_STANDARD_REQUIRED ON)
target_link_libraries(${PROJECT}-executable ${PROJECT})
target_link_libraries(${PROJECT}-executable ${Boost_PROGRAM_OPTIONS_LIBRARY})


include(DciHimpl)
mkHimplLayouts(${PROJECT}
    OUTPUT
        ${CMAKE_CURRENT_SOURCE_DIR}/include/dci/site/layoutProvider.hpp

    HEADERS
        ${CMAKE_CURRENT_SOURCE_DIR}/src/impl/modulePlace.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/impl/manager.hpp

    CLASSES
        ::dci::site::impl::ModulePlace
        ::dci::site::impl::Manager
)


if(COMMAND localinstall)
    localinstall(${PROJECT} include DROP_PREFIX include/ ${INC})
    localinstall(${PROJECT} share/cmake/Modules DciDefineModule.cmake)
endif()
