macro(patch_prepare)
    file(COPY "${ALTSERVER_DIR}/upstream_repo/" DESTINATION "${ALTSERVER_DIR}/upstream_patched/")
endmacro()

macro(patch)
    set(argn_list "${ARGN}")
    list(POP_FRONT argn_list PATCH)

    cmake_parse_arguments("ARG" "" "PATCH;PATTERN" "EXCLUDE_PATTERN" ${ARGN})

    file(GLOB to_patch "${ALTSERVER_DIR}/upstream_repo/${ARG_PATTERN}")

    foreach(file ${to_patch})
        execute_process(
                COMMAND python3 "${ALTSERVER_DIR}/patches/${PATCH}.py" "${file}" 
                WORKING_DIRECTORY "${ALTSERVER_DIR}"
                OUTPUT_VARIABLE patched
            )

        string(REPLACE "upstream_repo" "upstream_patched" patched_file "${file}")
        file(WRITE "${patched_file}" "${patched}")

        message(STATUS "[altserver] Patched: ${file}")
    endforeach()

    file(GLOB to_remove "${ALTSERVER_DIR}/upstream_patched/${ARG_EXCLUDE_PATTERN}")

    foreach(pattern ${ARG_EXCLUDE_PATTERN})
        file(GLOB to_remove "${ALTSERVER_DIR}/upstream_patched/${pattern}")
        foreach(file ${to_remove})
            file(REMOVE "${file}")
            message(STATUS "[altserver] Removed: ${file}")
        endforeach()
    endforeach()
endmacro()