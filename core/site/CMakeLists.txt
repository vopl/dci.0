set(PROJECT dci-site)

project(${PROJECT})

set(INC
    include/dci/site/error.hpp
    include/dci/site/manager.hpp
    include/dci/site/moduleId.hpp
    include/dci/site/modulePlace.hpp
    include/dci/site/moduleEntry.hpp
    include/dci/site/moduleInfo.hpp
    include/dci/site/layoutProvider.hpp
)

set(SRC
    src/main.cpp
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


add_executable(${PROJECT}
    ${INC}  ${SRC}
    DciDefineModule.cmake
)
set_property(TARGET ${PROJECT} PROPERTY CXX_STANDARD 14)
set_property(TARGET ${PROJECT} PROPERTY CXX_STANDARD_REQUIRED ON)

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


target_link_libraries(${PROJECT} ${Boost_PROGRAM_OPTIONS_LIBRARY} ${Boost_FILESYSTEM_LIBRARY})

target_link_libraries(${PROJECT}
    dci-mm
    dci-async
    dci-io
    dci-couple-runtime
    dl
)

if(COMMAND localinstall)
    localinstall(${PROJECT} include DROP_PREFIX include/ ${INC})
    localinstall(${PROJECT} share/cmake/Modules DciDefineModule.cmake)
endif()
