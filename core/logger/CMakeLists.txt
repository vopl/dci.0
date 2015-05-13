set(PROJECT dci-logger)
project(${PROJECT})


############################################################
set(INC
    include/dci/logger.hpp
    include/dci/logger/logger.hpp
)

set(SRC
    src/logger.cpp
)

add_library(${PROJECT} SHARED
    ${INC} ${SRC}
)


if(COMMAND localinstall)
    localinstall(${PROJECT} include DROP_PREFIX include/ ${INC})
endif()


