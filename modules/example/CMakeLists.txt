set(PROJECT dci-module-example)
project(${PROJECT} CXX)
cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)



set(SRC
    src/entry.cpp
)



############################################################
add_library(${PROJECT} MODULE
    ${SRC}
    idl/talk.idl
    generated/talk.idl.hpp
    generated/talk.idl.cpp

    generated/talk/Peer.fwd.hpp
    generated/talk/Peer.hpp
    generated/talk/details/PeerScope.hpp
    generated/talk/details/PeerState.hpp

    generated/talk/Chat.fwd.hpp

)

target_link_libraries(${PROJECT}
    dci-himpl
    dci-mm
    dci-async
    dci-couple-runtime
    dci-logger)

set_property(TARGET ${PROJECT} PROPERTY CXX_STANDARD 14)
set_property(TARGET ${PROJECT} PROPERTY CXX_STANDARD_REQUIRED ON)




include_directories(generated)





function(dciDefineModule target)
    message("dciDefineModule ${target}")

    include(CMakeParseArguments)
    cmake_parse_arguments(OPTS "" "PROVIDER;ID;REVISION;NAME;DESCRIPTION;MAINBINARY" "SERVICEIDS;TAGS;REQUIREDSERVICEIDS;REQUIREDMODULEIDS" ${ARGN})


    set(manifest "${CMAKE_CURRENT_BINARY_DIR}/index.json")

    file(WRITE ${manifest} "{\n")

    file(APPEND ${manifest} "  \"provider\": \"${OPTS_PROVIDER}\",\n")
    file(APPEND ${manifest} "  \"id\": \"${OPTS_ID}\",\n")

    file(APPEND ${manifest} "  \"serviceIds\": [")
    set(f TRUE)
    foreach(i ${OPTS_SERVICEIDS})
        if(NOT f)
            file(APPEND ${manifest} ",")
        endif()
        set(f)
        file(APPEND ${manifest} "\"${i}\"")
    endforeach()
    file(APPEND ${manifest} "],\n")

    file(APPEND ${manifest} "  \"revision\": \"${OPTS_REVISION}\",\n")
    file(APPEND ${manifest} "  \"name\": \"${OPTS_NAME}\",\n")
    file(APPEND ${manifest} "  \"description\": \"${OPTS_DESCRIPTION}\",\n")

    file(APPEND ${manifest} "  \"tags\": [")
    set(f TRUE)
    foreach(i ${OPTS_TAGS})
        if(NOT f)
            file(APPEND ${manifest} ",")
        endif()
        set(f)
        file(APPEND ${manifest} "\"${i}\"")
    endforeach()
    file(APPEND ${manifest} "],\n")

    file(APPEND ${manifest} "  \"requiredServiceIds\": [")
    set(f TRUE)
    foreach(i ${OPTS_REQUIREDSERVICEIDS})
        if(NOT f)
            file(APPEND ${manifest} ",")
        endif()
        set(f)
        file(APPEND ${manifest} "\"${i}\"")
    endforeach()
    file(APPEND ${manifest} "],\n")

    file(APPEND ${manifest} "  \"requiredModuleIds\": [")
    set(f TRUE)
    foreach(i ${OPTS_REQUIREDMODULEIDS})
        if(NOT f)
            file(APPEND ${manifest} ",")
        endif()
        set(f)
        file(APPEND ${manifest} "\"${i}\"")
    endforeach()
    file(APPEND ${manifest} "],\n")

    file(APPEND ${manifest} "  \"mainBinary\": \"${OPTS_MAINBINARY}\"\n")
    file(APPEND ${manifest} "}\n")

    ######################
    add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${OPTS_MAINBINARY}
        COMMAND ${CMAKE_COMMAND} -E create_symlink $<TARGET_FILE:${target}> ${OPTS_MAINBINARY}
        DEPENDS ${target})


    if(COMMAND localinstall)
        localinstall(${target} share/dci-modules/${OPTS_NAME} DROP_PREFIX ${CMAKE_CURRENT_BINARY_DIR}/ ${manifest})
        localinstall(${target} share/dci-modules/${OPTS_NAME} DROP_PREFIX ${CMAKE_CURRENT_BINARY_DIR}/ ${CMAKE_CURRENT_BINARY_DIR}/${OPTS_MAINBINARY})
    endif()

endfunction()




if(COMMAND dciDefineModule)
    dciDefineModule(${PROJECT}

        PROVIDER            "dci internal"
        ID                  ae014f8e04b222312e3e4750597d9d1b5f9a8f53
        SERVICEIDS
        REVISION            0
        NAME                "example"
        DESCRIPTION         "example module"
        TAGS
        REQUIREDSERVICEIDS
        REQUIREDMODULEIDS
        MAINBINARY          example.so
    )
endif()
