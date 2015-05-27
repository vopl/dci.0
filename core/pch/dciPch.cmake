
#######################################################################################
MACRO(TO_NATIVE_PATH PATH OUT)
	FILE(TO_NATIVE_PATH "${PATH}" "${OUT}")
	IF(MINGW)
		STRING(REPLACE "/" "\\" "${OUT}" "${${OUT}}")
	ENDIF(MINGW)
ENDMACRO(TO_NATIVE_PATH)

#######################################################################################
IF(MSVC)
	MESSAGE(STATUS "pch: MSVC")
	
	MACRO(PCH_KEY2FILENAME pchfile key header)
		GET_FILENAME_COMPONENT(filename ${header} NAME_WE)
		SET(pchfile "$(IntDir)/${filename}.pch")
		GET_FILENAME_COMPONENT(key ${header} NAME)
	ENDMACRO(PCH_KEY2FILENAME)


	MACRO(CREATE_PCH target header srcfile)
		PCH_KEY2FILENAME(pchfile key ${header})

		SET_SOURCE_FILES_PROPERTIES(${srcfile} PROPERTIES COMPILE_FLAGS "/Yc${key} /Fp${pchfile}")
#		MESSAGE("precompiled ${key} created by ${srcfile}")
	ENDMACRO(CREATE_PCH)

	MACRO(USE_PCH target header)
		PCH_KEY2FILENAME(pchfile key ${header})
	
		FOREACH(i ${ARGN})
			SET_SOURCE_FILES_PROPERTIES(${i} PROPERTIES COMPILE_FLAGS "/Yu${key} /Fp${pchfile}")
#			MESSAGE("precompiled ${key} used in ${i}")
		ENDFOREACH(i)
	ENDMACRO(USE_PCH)
ELSEIF(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANG)
	MESSAGE(STATUS "pch: GCC")

        MACRO(PCH_KEY2FILENAME pchfile key header)
		GET_FILENAME_COMPONENT(filename ${header} NAME)

		#GET_FILENAME_COMPONENT(path ${header} PATH)
		SET(path ${CMAKE_CURRENT_BINARY_DIR})
		
		SET(pchfile "${path}/${filename}.gch")
                GET_FILENAME_COMPONENT(key ${header} NAME)
        ENDMACRO(PCH_KEY2FILENAME)


        MACRO(USE_PCH target header)
                PCH_KEY2FILENAME(pchfile key ${header})

                #generate stub cpp
                set(cpp ${CMAKE_CURRENT_BINARY_DIR}/pch.cpp)
                add_custom_command(OUTPUT ${cpp}
                    COMMAND ${CMAKE_COMMAND} -E echo \\\#include \\"${header}\\" > ${cpp})

                set_source_files_properties(${cpp} PROPERTIES COMPILE_FLAGS "-x c++-header")

                #compile header as object
                add_library(${target}-pch OBJECT ${cpp})
                get_target_property(CXX_STANDARD ${target} CXX_STANDARD)
                set_target_properties(${target}-pch PROPERTIES CXX_STANDARD ${CXX_STANDARD})

                get_target_property(TYPE ${target} TYPE)
                if("SHARED_LIBRARY" STREQUAL TYPE OR "MODULE_LIBRARY" STREQUAL TYPE)
                    string(REPLACE "-" "_" export "${target}-EXPORTS")
                    set_target_properties(${target}-pch PROPERTIES COMPILE_FLAGS "-fPIC -D${export}")
                endif()

                add_dependencies(${target} ${target}-pch)

                set(objfile ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${target}-pch.dir/pch.cpp.o)

                #copy object to header
                add_custom_command(OUTPUT ${pchfile}
                    COMMAND ${CMAKE_COMMAND} -E copy ${objfile} ${pchfile}
                    DEPENDS ${objfile}
                )

                target_sources(${target} PRIVATE ${pchfile})

                #use header
                foreach(i ${ARGN})
                    set_source_files_properties(${i} PROPERTIES INCLUDE_DIRECTORIES ${CMAKE_CURRENT_BINARY_DIR})
                    set_source_files_properties(${i} PROPERTIES COMPILE_FLAGS "-Winvalid-pch -include ${key}" )
                    set_source_files_properties(${i} PROPERTIES OBJECT_DEPENDS ${pchfile} )
                endforeach(i)

        ENDMACRO(USE_PCH)
	
ELSE()
	MESSAGE(STATUS "pch: NO USE")
	MACRO(CREATE_PCH target header srcfile)
	ENDMACRO(CREATE_PCH)

	MACRO(USE_PCH target header)
	ENDMACRO(USE_PCH)
ENDIF()
