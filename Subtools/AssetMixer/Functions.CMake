if(NOT COMMAND TP_AM_BuildAssets)
    function(TP_AM_BuildAssets)
        #message("Building assets...")
        set(oneValueArgs SOURCE DESTINATION TARGET)
        set(multiValueArgs PASSES)
        cmake_parse_arguments(PARSE_ARGV 0 TP_AM_BuildAssets
                "${options}" "${oneValueArgs}" "${multiValueArgs}"
        )
        #message(${TEAPACKET_ROOT})
        #message("${TEAPACKET_ROOT}/Subtools/AssetMixer/asset_mixer.py")
        list(REMOVE_DUPLICATES TP_AM_BuildAssets_PASSES)
        message("PASS IS ${TP_AM_BuildAssets_PASSES}")
        add_custom_command(
                TARGET ${TP_AM_BuildAssets_TARGET}
                PRE_BUILD
                COMMENT "Building assets..."
                COMMAND python3
                    "${TEAPACKET_ROOT}/Subtools/AssetMixer/main.py"
                    "${TP_AM_BuildAssets_SOURCE}"
                    "${TP_AM_BuildAssets_DESTINATION}"
                    "--passes"
                    ${TP_AM_BuildAssets_PASSES}
        )
    endfunction()
endif()