if(NOT COMMAND TP_AM_BuildAssets)
    function(TP_AM_BuildAssets)
        message("Building assets...")
        set(oneValueArgs SOURCE DESTINATION TARGET ASSET_PLATFORM)
        set(multiValueArgs PARSER_DIRS)
        cmake_parse_arguments(PARSE_ARGV 0 TP_AM_BuildAssets
                "${options}" "${oneValueArgs}" "${multiValueArgs}"
        )
        message(${TEAPACKET_ROOT})
        add_custom_command(
                TARGET ${TP_AM_BuildAssets_TARGET}
                PRE_BUILD
                COMMAND python
                    "${TEAPACKET_ROOT}/Subtools/AssetMixer/asset_mixer.py"
                    "${TP_AM_BuildAssets_SOURCE}"
                    "${TP_AM_BuildAssets_DESTINATION}"
                    ${TP_AM_BuildAssets_ASSET_PLATFORM}
                    ${TP_AM_BuildAssets_PARSER_DIRS}
        )
    endfunction()
endif()