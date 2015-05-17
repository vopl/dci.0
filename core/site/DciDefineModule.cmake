
function(dciDefineModule name)

    include(CMakeParseArguments)
    cmake_parse_arguments(OPTS "" "MAINTARGET" "IMPLTARGETS;IDL" ${ARGN})



    ######################
    add_dependencies(${OPTS_MAINTARGET} dci-site-executable)

    set(manifest ${CMAKE_CURRENT_BINARY_DIR}/manifest.json)
    add_custom_command(OUTPUT ${manifest}
        COMMAND dci-site-executable --genmanifest $<TARGET_FILE:${OPTS_MAINTARGET}> --outfile ${manifest}
        DEPENDS ${OPTS_MAINTARGET} dci-site-executable)

    target_sources(${OPTS_MAINTARGET} PUBLIC ${manifest})

    set_target_properties(${OPTS_MAINTARGET} PROPERTIES LIBRARY_OUTPUT_NAME ${name})
    set_target_properties(${OPTS_MAINTARGET} PROPERTIES PREFIX "")
    set_target_properties(${OPTS_MAINTARGET} PROPERTIES SUFFIX ".main")

    foreach(impltarget ${OPTS_IMPLTARGETS})
        set_target_properties(${impltarget} PROPERTIES PREFIX "")
        set_target_properties(${impltarget} PROPERTIES SUFFIX ".impl")
    endforeach()


    if(COMMAND localinstall)
        set_target_properties(${OPTS_MAINTARGET} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${LOCALINSTALL_DIR}/share/dci-modules/${name})

        foreach(impltarget ${OPTS_IMPLTARGETS})
            set_target_properties(${impltarget} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${LOCALINSTALL_DIR}/share/dci-modules/${name})
        endforeach()

        localinstall(${OPTS_MAINTARGET} share/dci-modules/${name} DROP_PREFIX ${CMAKE_CURRENT_BINARY_DIR}/ ${manifest})
        if(OPTS_IDL)
            localinstall(${OPTS_MAINTARGET} idl DROP_PREFIX idl/ ${OPTS_IDL})
        endif()
    endif()

endfunction()
