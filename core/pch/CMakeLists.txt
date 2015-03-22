set(PROJECT dci-pch)
project(${PROJECT})


############################################################
add_library(${PROJECT} STATIC EXCLUDE_FROM_ALL DciPch.cmake)
set_target_properties(${PROJECT} PROPERTIES LINKER_LANGUAGE CXX)




if(COMMAND localinstall)
    localinstall(${PROJECT} share/cmake/Modules DciPch.cmake)
endif()


