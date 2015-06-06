
function(dciTest target stage)

    include(CMakeParseArguments)
    cmake_parse_arguments(OPTS "" "" "SRC;LINK" ${ARGN})

    set(availableStages noenv min mload mstart)
    list(FIND availableStages ${stage} stageIdx)
    if(${stageIdx} EQUAL -1)
        message(FATAL_ERROR "stage '${stage}' unrecognized. Available stages are: ${availableStages}.")
    endif()

    set(testTarget ${target}-test-${stage})
    add_library(${testTarget} SHARED ${OPTS_SRC})
    set_property(TARGET ${testTarget} PROPERTY CXX_STANDARD 14)

    target_link_libraries(${testTarget} dci-gtest ${OPTS_LINK})


    get_property(libsInHub TARGET dci-gtest-hub-${stage} PROPERTY LINK_LIBRARIES)
    set(libsInHub ${libsInHub} ${testTarget})
    set_property(TARGET dci-gtest-hub-${stage} PROPERTY LINK_LIBRARIES ${libsInHub})

endfunction()
