if(NOT COMMAND TP_EnumMapGen)
    function(TP_EnumMapGen Source Target)
        file(READ "${Source}" FILE_CONTENTS)
        string(REPLACE "\n" ";" FILE_LINES "${FILE_CONTENTS}")
        list(GET FILE_LINES 0 FIRST_LINE)#
        string(REPLACE "Destination " "" DEST_FILE "${FIRST_LINE}")
        
        
        add_custom_command(
                OUTPUT "${CMAKE_CURRENT_SOURCE_DIR}/include/${DEST_FILE}"
                DEPENDS "${Source}"
                WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
                COMMENT "Generating ${DEST_FILE} from ${Source}"
                COMMAND python "${TEAPACKET_ROOT}/Generators/EnumMapGen/main.py" "${Source}"
                VERBATIM
        )
        target_sources(${Target} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/${DEST_FILE}")
    endfunction()
endif()