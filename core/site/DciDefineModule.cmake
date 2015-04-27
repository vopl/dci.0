
function(dciDefineModule target)

    include(CMakeParseArguments)
    cmake_parse_arguments(OPTS "" "PROVIDER;ID;REVISION;NAME;DESCRIPTION;MAINBINARY" "SERVICEIDS;TAGS;REQUIREDSERVICEIDS;REQUIREDMODULEIDS;IDL" ${ARGN})


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

    add_dependencies(${target} dci-site)

    if(COMMAND localinstall)
        localinstall(${target} share/dci-modules/${OPTS_NAME} DROP_PREFIX ${CMAKE_CURRENT_BINARY_DIR}/ ${manifest})
        localinstall(${target} share/dci-modules/${OPTS_NAME} DROP_PREFIX ${CMAKE_CURRENT_BINARY_DIR}/ ${CMAKE_CURRENT_BINARY_DIR}/${OPTS_MAINBINARY})
        if(OPTS_IDL)
            localinstall(${target} idl DROP_PREFIX idl/ ${OPTS_IDL})
        endif()
    endif()

endfunction()
