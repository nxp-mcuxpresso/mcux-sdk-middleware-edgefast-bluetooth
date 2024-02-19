include_guard(GLOBAL)


if (CONFIG_USE_middleware_edgefast_bluetooth_le_audio_config_template)
# Add set(CONFIG_USE_middleware_edgefast_bluetooth_le_audio_config_template true) in config.cmake to use this component

message("middleware_edgefast_bluetooth_le_audio_config_template component is included from ${CMAKE_CURRENT_LIST_FILE}.")


endif()


if (CONFIG_USE_middleware_edgefast_bluetooth_le_audio_cap)
# Add set(CONFIG_USE_middleware_edgefast_bluetooth_le_audio_cap true) in config.cmake to use this component

message("middleware_edgefast_bluetooth_le_audio_cap component is included from ${CMAKE_CURRENT_LIST_FILE}.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/audio/cap_acceptor.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/audio/cap_initiator.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/audio/cap_stream.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/./include/bluetooth/audio
  ${CMAKE_CURRENT_LIST_DIR}/./include/bluetooth/audio/config
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/audio
)


endif()


if (CONFIG_USE_middleware_edgefast_bluetooth_le_audio_aics)
# Add set(CONFIG_USE_middleware_edgefast_bluetooth_le_audio_aics true) in config.cmake to use this component

message("middleware_edgefast_bluetooth_le_audio_aics component is included from ${CMAKE_CURRENT_LIST_FILE}.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/audio/aics.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/audio/aics_client.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/./include/bluetooth/audio
  ${CMAKE_CURRENT_LIST_DIR}/./include/bluetooth/audio/config
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/audio
)


endif()


if (CONFIG_USE_middleware_edgefast_bluetooth_le_audio_vocs)
# Add set(CONFIG_USE_middleware_edgefast_bluetooth_le_audio_vocs true) in config.cmake to use this component

message("middleware_edgefast_bluetooth_le_audio_vocs component is included from ${CMAKE_CURRENT_LIST_FILE}.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/audio/vocs.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/audio/vocs_client.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/./include/bluetooth/audio
  ${CMAKE_CURRENT_LIST_DIR}/./include/bluetooth/audio/config
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/audio
)


endif()


if (CONFIG_USE_middleware_edgefast_bluetooth_le_audio_csip)
# Add set(CONFIG_USE_middleware_edgefast_bluetooth_le_audio_csip true) in config.cmake to use this component

message("middleware_edgefast_bluetooth_le_audio_csip component is included from ${CMAKE_CURRENT_LIST_FILE}.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/audio/csip_crypto.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/audio/csip_set_coordinator.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/audio/csip_set_member.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/./include/bluetooth/audio
  ${CMAKE_CURRENT_LIST_DIR}/./include/bluetooth/audio/config
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/audio
)


endif()


if (CONFIG_USE_middleware_edgefast_bluetooth_le_audio_ccid)
# Add set(CONFIG_USE_middleware_edgefast_bluetooth_le_audio_ccid true) in config.cmake to use this component

message("middleware_edgefast_bluetooth_le_audio_ccid component is included from ${CMAKE_CURRENT_LIST_FILE}.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/audio/ccid.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/./include/bluetooth/audio
  ${CMAKE_CURRENT_LIST_DIR}/./include/bluetooth/audio/config
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/audio
)


endif()


if (CONFIG_USE_middleware_edgefast_bluetooth_le_audio_bap)
# Add set(CONFIG_USE_middleware_edgefast_bluetooth_le_audio_bap true) in config.cmake to use this component

message("middleware_edgefast_bluetooth_le_audio_bap component is included from ${CMAKE_CURRENT_LIST_FILE}.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/audio/ascs.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/audio/bap_iso.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/audio/audio.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/audio/bap_broadcast_sink.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/audio/bap_broadcast_source.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/audio/codec.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/audio/pacs.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/audio/bap_stream.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/audio/bap_unicast_client.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/audio/bap_unicast_server.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/./include/bluetooth/audio
  ${CMAKE_CURRENT_LIST_DIR}/./include/bluetooth/audio/config
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/audio
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind
)


endif()


if (CONFIG_USE_middleware_edgefast_bluetooth_le_audio_has)
# Add set(CONFIG_USE_middleware_edgefast_bluetooth_le_audio_has true) in config.cmake to use this component

message("middleware_edgefast_bluetooth_le_audio_has component is included from ${CMAKE_CURRENT_LIST_FILE}.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/audio/has_client.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/audio/has.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/./include/bluetooth/audio
  ${CMAKE_CURRENT_LIST_DIR}/./include/bluetooth/audio/config
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/audio
)


endif()


if (CONFIG_USE_middleware_edgefast_bluetooth_le_audio_tbs)
# Add set(CONFIG_USE_middleware_edgefast_bluetooth_le_audio_tbs true) in config.cmake to use this component

message("middleware_edgefast_bluetooth_le_audio_tbs component is included from ${CMAKE_CURRENT_LIST_FILE}.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/audio/tbs_client.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/audio/tbs.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/./include/bluetooth/audio
  ${CMAKE_CURRENT_LIST_DIR}/./include/bluetooth/audio/config
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/audio
)


endif()


if (CONFIG_USE_middleware_edgefast_bluetooth_le_audio_tmap)
# Add set(CONFIG_USE_middleware_edgefast_bluetooth_le_audio_tmap true) in config.cmake to use this component

message("middleware_edgefast_bluetooth_le_audio_tmap component is included from ${CMAKE_CURRENT_LIST_FILE}.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/audio/tmap.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/./include/bluetooth/audio
  ${CMAKE_CURRENT_LIST_DIR}/./include/bluetooth/audio/config
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/audio
)


endif()


if (CONFIG_USE_middleware_edgefast_bluetooth_config_template)
# Add set(CONFIG_USE_middleware_edgefast_bluetooth_config_template true) in config.cmake to use this component

message("middleware_edgefast_bluetooth_config_template component is included from ${CMAKE_CURRENT_LIST_FILE}.")

add_config_file(${CMAKE_CURRENT_LIST_DIR}/./output/templates/config/edgefast_bluetooth_config.h ${CMAKE_CURRENT_LIST_DIR}/./output/templates/config middleware_edgefast_bluetooth_config_template)
add_config_file(${CMAKE_CURRENT_LIST_DIR}/./output/templates/config/edgefast_bluetooth_debug_config.h "" middleware_edgefast_bluetooth_config_template)
add_config_file(${CMAKE_CURRENT_LIST_DIR}/./output/templates/app_bluetooth_config/app_bluetooth_config.h ${CMAKE_CURRENT_LIST_DIR}/./output/templates/app_bluetooth_config middleware_edgefast_bluetooth_config_template)


endif()


if (CONFIG_USE_middleware_edgefast_bluetooth_liblc3codec)
# Add set(CONFIG_USE_middleware_edgefast_bluetooth_liblc3codec true) in config.cmake to use this component

message("middleware_edgefast_bluetooth_liblc3codec component is included from ${CMAKE_CURRENT_LIST_FILE}.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/lib/liblc3codec/src/attdet.c
  ${CMAKE_CURRENT_LIST_DIR}/lib/liblc3codec/src/bits.c
  ${CMAKE_CURRENT_LIST_DIR}/lib/liblc3codec/src/bwdet.c
  ${CMAKE_CURRENT_LIST_DIR}/lib/liblc3codec/src/energy.c
  ${CMAKE_CURRENT_LIST_DIR}/lib/liblc3codec/src/lc3.c
  ${CMAKE_CURRENT_LIST_DIR}/lib/liblc3codec/src/ltpf.c
  ${CMAKE_CURRENT_LIST_DIR}/lib/liblc3codec/src/mdct.c
  ${CMAKE_CURRENT_LIST_DIR}/lib/liblc3codec/src/plc.c
  ${CMAKE_CURRENT_LIST_DIR}/lib/liblc3codec/src/sns.c
  ${CMAKE_CURRENT_LIST_DIR}/lib/liblc3codec/src/spec.c
  ${CMAKE_CURRENT_LIST_DIR}/lib/liblc3codec/src/tables.c
  ${CMAKE_CURRENT_LIST_DIR}/lib/liblc3codec/src/tns.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/lib/liblc3codec/include
  ${CMAKE_CURRENT_LIST_DIR}/lib/liblc3codec
  ${CMAKE_CURRENT_LIST_DIR}/lib/.
)

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  if(CONFIG_TOOLCHAIN STREQUAL iar)
    target_compile_options(${MCUX_SDK_PROJECT_NAME} PUBLIC
      --vla
    )
  endif()

endif()


endif()


if (CONFIG_USE_middleware_edgefast_bluetooth_asrc_sw)
# Add set(CONFIG_USE_middleware_edgefast_bluetooth_asrc_sw true) in config.cmake to use this component

message("middleware_edgefast_bluetooth_asrc_sw component is included from ${CMAKE_CURRENT_LIST_FILE}.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/lib/asrc_sw/srCvtFrm.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/lib/asrc_sw/include
  ${CMAKE_CURRENT_LIST_DIR}/lib/asrc_sw
  ${CMAKE_CURRENT_LIST_DIR}/lib/.
)


endif()


if (CONFIG_USE_middleware_edgefast_bluetooth_sdio_template_mcxn9xxevk)
# Add set(CONFIG_USE_middleware_edgefast_bluetooth_sdio_template_mcxn9xxevk true) in config.cmake to use this component

message("middleware_edgefast_bluetooth_sdio_template_mcxn9xxevk component is included from ${CMAKE_CURRENT_LIST_FILE}.")

if((CONFIG_DEVICE_ID STREQUAL MCXN947))

else()

message(SEND_ERROR "middleware_edgefast_bluetooth_sdio_template_mcxn9xxevk dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()

endif()


if (CONFIG_USE_middleware_edgefast_bluetooth_mcux_linker_template_mcxn9xxevk)
# Add set(CONFIG_USE_middleware_edgefast_bluetooth_mcux_linker_template_mcxn9xxevk true) in config.cmake to use this component

message("middleware_edgefast_bluetooth_mcux_linker_template_mcxn9xxevk component is included from ${CMAKE_CURRENT_LIST_FILE}.")

if((CONFIG_DEVICE_ID STREQUAL MCXN947))

if(CONFIG_TOOLCHAIN STREQUAL mcux)
  add_config_file(${CMAKE_CURRENT_LIST_DIR}/../../boards/mcxn9xxevk/edgefast_bluetooth_examples/template/end_text.ldt "" middleware_edgefast_bluetooth_mcux_linker_template_mcxn9xxevk)
  add_config_file(${CMAKE_CURRENT_LIST_DIR}/../../boards/mcxn9xxevk/edgefast_bluetooth_examples/template/main_data.ldt "" middleware_edgefast_bluetooth_mcux_linker_template_mcxn9xxevk)
  add_config_file(${CMAKE_CURRENT_LIST_DIR}/../../boards/mcxn9xxevk/edgefast_bluetooth_examples/template/main_text.ldt "" middleware_edgefast_bluetooth_mcux_linker_template_mcxn9xxevk)
  add_config_file(${CMAKE_CURRENT_LIST_DIR}/../../boards/mcxn9xxevk/edgefast_bluetooth_examples/template/symbols.ldt "" middleware_edgefast_bluetooth_mcux_linker_template_mcxn9xxevk)
endif()

else()

message(SEND_ERROR "middleware_edgefast_bluetooth_mcux_linker_template_mcxn9xxevk dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()

endif()


if (CONFIG_USE_middleware_edgefast_bluetooth_pal)
# Add set(CONFIG_USE_middleware_edgefast_bluetooth_pal true) in config.cmake to use this component

message("middleware_edgefast_bluetooth_pal component is included from ${CMAKE_CURRENT_LIST_FILE}.")

if(CONFIG_USE_middleware_freertos-kernel_heap_4 AND CONFIG_USE_middleware_freertos-kernel AND CONFIG_USE_middleware_mbedtls AND CONFIG_USE_middleware_usb_host_stack AND CONFIG_USE_middleware_edgefast_bluetooth_common_ethermind AND CONFIG_USE_middleware_edgefast_bluetooth_porting AND CONFIG_USE_middleware_edgefast_bluetooth_pal_platform_ethermind AND CONFIG_USE_middleware_edgefast_bluetooth_pal_crypto_ethermind AND CONFIG_USE_middleware_edgefast_bluetooth_pal_host_msd_fatfs_ethermind AND CONFIG_USE_middleware_edgefast_bluetooth_pal_db_gen_ethermind AND ((CONFIG_USE_middleware_edgefast_bluetooth_ble_ethermind_cm33 AND (NOT CONFIG_USE_middleware_edgefast_bluetooth_br_ethermind_cm33) AND (NOT CONFIG_USE_middleware_edgefast_bluetooth_btble_ethermind_cm33) AND (CONFIG_CORE STREQUAL cm33) AND (CONFIG_DSP STREQUAL DSP)) OR (CONFIG_USE_middleware_edgefast_bluetooth_br_ethermind_cm33 AND (NOT CONFIG_USE_middleware_edgefast_bluetooth_ble_ethermind_cm33) AND (NOT CONFIG_USE_middleware_edgefast_bluetooth_btble_ethermind_cm33) AND (CONFIG_CORE STREQUAL cm33) AND (CONFIG_DSP STREQUAL DSP)) OR (CONFIG_USE_middleware_edgefast_bluetooth_btble_ethermind_cm33 AND (NOT CONFIG_USE_middleware_edgefast_bluetooth_ble_ethermind_cm33) AND (NOT CONFIG_USE_middleware_edgefast_bluetooth_br_ethermind_cm33) AND (CONFIG_CORE STREQUAL cm33) AND (CONFIG_DSP STREQUAL DSP))))

else()

message(SEND_ERROR "middleware_edgefast_bluetooth_pal dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()

endif()


if (CONFIG_USE_middleware_edgefast_bluetooth_common_ethermind_hci)
# Add set(CONFIG_USE_middleware_edgefast_bluetooth_common_ethermind_hci true) in config.cmake to use this component

message("middleware_edgefast_bluetooth_common_ethermind_hci component is included from ${CMAKE_CURRENT_LIST_FILE}.")

if(CONFIG_USE_middleware_edgefast_bluetooth_pal AND (CONFIG_USE_middleware_edgefast_bluetooth_common_ethermind_hci_uart))

else()

message(SEND_ERROR "middleware_edgefast_bluetooth_common_ethermind_hci dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()

endif()


if (CONFIG_USE_middleware_edgefast_bluetooth_common_ethermind_hci_uart)
# Add set(CONFIG_USE_middleware_edgefast_bluetooth_common_ethermind_hci_uart true) in config.cmake to use this component

message("middleware_edgefast_bluetooth_common_ethermind_hci_uart component is included from ${CMAKE_CURRENT_LIST_FILE}.")

if(CONFIG_USE_middleware_edgefast_bluetooth_common_ethermind AND (CONFIG_DEVICE_ID STREQUAL MCXN547 OR CONFIG_DEVICE_ID STREQUAL MCXN947) AND CONFIG_USE_component_serial_manager AND CONFIG_USE_component_serial_manager_uart AND (CONFIG_USE_middleware_edgefast_bluetooth_wifi_nxp_controller_base))

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/port/pal/mcux/bluetooth/hci_uart.c
)

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
    -DDEBUG_CONSOLE_TRANSFER_NON_BLOCKING=1
    -DSERIAL_PORT_TYPE_UART=1
  )

endif()

else()

message(SEND_ERROR "middleware_edgefast_bluetooth_common_ethermind_hci_uart dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()

endif()


if (CONFIG_USE_middleware_edgefast_bluetooth_common_ethermind)
# Add set(CONFIG_USE_middleware_edgefast_bluetooth_common_ethermind true) in config.cmake to use this component

message("middleware_edgefast_bluetooth_common_ethermind component is included from ${CMAKE_CURRENT_LIST_FILE}.")

if(CONFIG_USE_middleware_edgefast_bluetooth_pal AND CONFIG_USE_middleware_edgefast_bluetooth_extension_common_ethermind AND CONFIG_USE_middleware_edgefast_bluetooth_common_ethermind_hci AND CONFIG_USE_middleware_edgefast_bluetooth_config_ethermind AND CONFIG_USE_middleware_littlefs AND CONFIG_USE_middleware_fatfs AND CONFIG_USE_component_osa)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/export/vendor/vendor_specific_init.c
  ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/port/osal/src/freertos/EM_config.c
  ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/port/osal/src/freertos/EM_debug.c
  ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/port/osal/src/freertos/EM_fops.c
  ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/port/osal/src/freertos/EM_os.c
  ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/port/osal/src/freertos/EM_serial.c
  ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/port/osal/src/freertos/EM_timer.c
  ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/port/pal/mcux/bluetooth/BT_status_pl.c
  ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/port/pal/mcux/bluetooth/BT_storage_pl.c
  ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/port/pal/mcux/bluetooth/btsnoop_pl.c
  ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/port/pal/mcux/bluetooth/sco_audio_pl.c
  ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/port/pal/mcux/bluetooth/ctn_pl.c
  ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/port/pal/mcux/bluetooth/ftp_pl.c
  ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/port/pal/mcux/bluetooth/ht_read_task_pl.c
  ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/port/pal/mcux/bluetooth/map_pl.c
  ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/port/pal/mcux/bluetooth/obex_pl.c
  ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/port/pal/mcux/bluetooth/pbap_pl.c
  ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/port/pal/mcux/bluetooth/write_task_pl.c
  ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/port/pal/mcux/bluetooth/littlefs_pl.c
  ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/port/pal/mcux/bluetooth/fw_loader_uart.c
  ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/port/pal/mcux/ethal/ethal_config.c
  ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/port/pal/mcux/ethal/ethal_debug.c
  ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/port/pal/mcux/ethal/ethal_fops.c
  ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/port/pal/mcux/ethal/ethal_os.c
  ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/port/pal/mcux/ethal/ethal_serial.c
  ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/port/pal/mcux/ethal/ethal_timer.c
  ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/port/pal/mcux/EM_platform.c
)

if(CONFIG_TOOLCHAIN STREQUAL mcux)
  target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/port/pal/mcux/toolspec/mcuxpresso/toolspec.c
  )
endif()

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/export/eOSAL
  ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/export/include
  ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/export/vendor
  ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/protocols/att
  ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/protocols/avctp
  ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/protocols/avdtp
  ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/protocols/bnep
  ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/protocols/dbase
  ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/protocols/hci_1.2
  ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/protocols/mcap
  ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/protocols/obex
  ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/protocols/rfcomm
  ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/protocols/sdp
  ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/protocols/sm
  ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/protocols/smp
  ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/utils/aes_cmac
  ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/utils/at_parser
  ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/utils/object_parser
  ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/utils/racp
  ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/utils/storage
  ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/utils/xml_parser
  ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/port/pal/mcux/bluetooth
  ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/port/pal/mcux/ethal
  ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/port/pal/mcux/sbc
  ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/port/osal/src/freertos
  ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/port/pal/mcux
  ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/utils/sbc
)

if(CONFIG_TOOLCHAIN STREQUAL mcux)
target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/port/pal/mcux/toolspec/mcuxpresso
)
endif()

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
    -DLFS_NO_INTRINSICS=1
    -DLFS_NO_ERROR=1
    -DFSL_OSA_TASK_ENABLE=1
    -DFSL_OSA_MAIN_FUNC_ENABLE=0
  )

endif()

else()

message(SEND_ERROR "middleware_edgefast_bluetooth_common_ethermind dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()

endif()


if (CONFIG_USE_middleware_edgefast_bluetooth_common_ethermind_lc3)
# Add set(CONFIG_USE_middleware_edgefast_bluetooth_common_ethermind_lc3 true) in config.cmake to use this component

message("middleware_edgefast_bluetooth_common_ethermind_lc3 component is included from ${CMAKE_CURRENT_LIST_FILE}.")

if((CONFIG_USE_middleware_edgefast_bluetooth_ble_ethermind_lib_lc3_cm33 AND (NOT CONFIG_USE_middleware_edgefast_bluetooth_br_ethermind_lib_lc3_cm33) AND (NOT CONFIG_USE_middleware_edgefast_bluetooth_btble_ethermind_lib_lc3_cm33) AND (CONFIG_CORE STREQUAL cm33) AND (CONFIG_DSP STREQUAL DSP)) OR (CONFIG_USE_middleware_edgefast_bluetooth_br_ethermind_lib_lc3_cm33 AND (NOT CONFIG_USE_middleware_edgefast_bluetooth_ble_ethermind_lib_lc3_cm33) AND (NOT CONFIG_USE_middleware_edgefast_bluetooth_btble_ethermind_lib_lc3_cm33) AND (CONFIG_CORE STREQUAL cm33) AND (CONFIG_DSP STREQUAL DSP)) OR (CONFIG_USE_middleware_edgefast_bluetooth_btble_ethermind_lib_lc3_cm33 AND (NOT CONFIG_USE_middleware_edgefast_bluetooth_ble_ethermind_lib_lc3_cm33) AND (NOT CONFIG_USE_middleware_edgefast_bluetooth_br_ethermind_lib_lc3_cm33) AND (CONFIG_CORE STREQUAL cm33) AND (CONFIG_DSP STREQUAL DSP)))

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/port/pal/mcux/lc3
  ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/utils/lc3/export/include
  ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/utils/lc3/private/fixed/config
)

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
    -DLC3_DSP=1
  )

endif()

else()

message(SEND_ERROR "middleware_edgefast_bluetooth_common_ethermind_lc3 dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()

endif()


if (CONFIG_USE_middleware_edgefast_bluetooth_wifi_nxp_controller_base)
# Add set(CONFIG_USE_middleware_edgefast_bluetooth_wifi_nxp_controller_base true) in config.cmake to use this component

message("middleware_edgefast_bluetooth_wifi_nxp_controller_base component is included from ${CMAKE_CURRENT_LIST_FILE}.")

if(CONFIG_USE_middleware_edgefast_bluetooth_common_ethermind AND CONFIG_USE_middleware_wifi_fwdnld AND ((CONFIG_USE_middleware_edgefast_bluetooth_sdio_template_mcxn5xxevk AND (CONFIG_BOARD STREQUAL mcxn5xxevk)) OR (CONFIG_USE_middleware_edgefast_bluetooth_sdio_template_mcxn9xxevk AND (CONFIG_BOARD STREQUAL mcxn9xxevk))))

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/port/pal/mcux/bluetooth/controller/controller_wifi_nxp.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/port/pal/mcux/bluetooth/controller
)

else()

message(SEND_ERROR "middleware_edgefast_bluetooth_wifi_nxp_controller_base dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()

endif()


if (CONFIG_USE_middleware_edgefast_bluetooth_config_ethermind)
# Add set(CONFIG_USE_middleware_edgefast_bluetooth_config_ethermind true) in config.cmake to use this component

message("middleware_edgefast_bluetooth_config_ethermind component is included from ${CMAKE_CURRENT_LIST_FILE}.")

if(CONFIG_USE_middleware_edgefast_bluetooth_common_ethermind)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/config
)

else()

message(SEND_ERROR "middleware_edgefast_bluetooth_config_ethermind dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()

endif()


if (CONFIG_USE_middleware_edgefast_bluetooth_extension_common_ethermind)
# Add set(CONFIG_USE_middleware_edgefast_bluetooth_extension_common_ethermind true) in config.cmake to use this component

message("middleware_edgefast_bluetooth_extension_common_ethermind component is included from ${CMAKE_CURRENT_LIST_FILE}.")

if(CONFIG_USE_middleware_edgefast_bluetooth_common_ethermind)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/export/extension/aes_cmac_pl.c
  ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/export/extension/BT_common_pl.c
  ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/export/extension/device_queue_pl.c
  ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/export/extension/sm_pl.c
  ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/export/extension/sm_ssp_pl.c
  ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/export/extension/smp_pl.c
  ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/export/extension/gatt_db_pl.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/export/extension
)

else()

message(SEND_ERROR "middleware_edgefast_bluetooth_extension_common_ethermind dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()

endif()


if (CONFIG_USE_middleware_edgefast_bluetooth_btble_ethermind_cm33)
# Add set(CONFIG_USE_middleware_edgefast_bluetooth_btble_ethermind_cm33 true) in config.cmake to use this component

message("middleware_edgefast_bluetooth_btble_ethermind_cm33 component is included from ${CMAKE_CURRENT_LIST_FILE}.")

if(CONFIG_USE_middleware_edgefast_bluetooth_pal AND CONFIG_USE_middleware_edgefast_bluetooth_template AND CONFIG_USE_middleware_edgefast_bluetooth_btble_ethermind_lib_cm33 AND (NOT CONFIG_USE_middleware_edgefast_bluetooth_ble_ethermind_lib_cm33) AND (NOT CONFIG_USE_middleware_edgefast_bluetooth_br_ethermind_lib_cm33) AND (CONFIG_CORE STREQUAL cm33))

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/host/bt_pal_a2dp.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/host/a2dp_codec/sbc/a2dp_codec_sbc.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/host/bt_pal_avrcp.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/host/bt_pal_avrcp_al_internal.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/host/bt_pal_avrcp_al_api.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/host/bt_pal_att.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/host/bt_pal_conn.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/host/bt_pal_data.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/host/bt_pal_ead.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/host/bt_pal_crypto.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/host/bt_pal_gatt.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/host/bt_pal_hci_core.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/host/bt_pal_adv.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/host/bt_pal_br.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/host/bt_pal_id.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/host/bt_pal_iso.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/host/bt_pal_scan.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/host/bt_pal_ecc.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/host/bt_pal_addr.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/host/bt_pal_buf.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/host/bt_pal_direction.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/host/bt_pal_keys.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/host/bt_pal_keys_br.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/host/bt_pal_l2cap.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/host/bt_pal_l2cap_br.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/host/bt_pal_monitor.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/host/bt_pal_rfcomm.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/host/bt_pal_spp.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/host/bt_pal_rpa.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/host/bt_pal_settings.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/host/bt_pal_sdp.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/host/bt_pal_smp.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/host/bt_pal_ssp.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/host/bt_pal_uuid.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/host/bt_pal_hfp_ag.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/host/bt_pal_hfp_hf.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/common/addr.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/host
  ${CMAKE_CURRENT_LIST_DIR}/./include
)

else()

message(SEND_ERROR "middleware_edgefast_bluetooth_btble_ethermind_cm33 dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()

endif()


if (CONFIG_USE_middleware_edgefast_bluetooth_ble_ethermind_cm33)
# Add set(CONFIG_USE_middleware_edgefast_bluetooth_ble_ethermind_cm33 true) in config.cmake to use this component

message("middleware_edgefast_bluetooth_ble_ethermind_cm33 component is included from ${CMAKE_CURRENT_LIST_FILE}.")

if(CONFIG_USE_middleware_edgefast_bluetooth_pal AND CONFIG_USE_middleware_edgefast_bluetooth_template AND CONFIG_USE_middleware_edgefast_bluetooth_ble_ethermind_lib_cm33 AND (NOT CONFIG_USE_middleware_edgefast_bluetooth_br_ethermind_lib_cm33) AND (NOT CONFIG_USE_middleware_edgefast_bluetooth_btble_ethermind_lib_cm33) AND (CONFIG_CORE STREQUAL cm33))

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/host/bt_pal_att.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/host/bt_pal_conn.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/host/bt_pal_data.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/host/bt_pal_ead.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/host/bt_pal_crypto.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/host/bt_pal_gatt.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/host/bt_pal_hci_core.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/host/bt_pal_adv.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/host/bt_pal_br.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/host/bt_pal_id.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/host/bt_pal_iso.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/host/bt_pal_scan.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/host/bt_pal_ecc.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/host/bt_pal_addr.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/host/bt_pal_buf.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/host/bt_pal_direction.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/host/bt_pal_keys.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/host/bt_pal_l2cap.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/host/bt_pal_monitor.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/host/bt_pal_rpa.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/host/bt_pal_settings.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/host/bt_pal_smp.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/host/bt_pal_smp_null.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/host/bt_pal_uuid.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/common/addr.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/host
  ${CMAKE_CURRENT_LIST_DIR}/./include
)

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
    -DCFG_BLE
  )

endif()

else()

message(SEND_ERROR "middleware_edgefast_bluetooth_ble_ethermind_cm33 dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()

endif()


if (CONFIG_USE_middleware_edgefast_bluetooth_br_ethermind_cm33)
# Add set(CONFIG_USE_middleware_edgefast_bluetooth_br_ethermind_cm33 true) in config.cmake to use this component

message("middleware_edgefast_bluetooth_br_ethermind_cm33 component is included from ${CMAKE_CURRENT_LIST_FILE}.")

if(CONFIG_USE_middleware_edgefast_bluetooth_pal AND CONFIG_USE_middleware_edgefast_bluetooth_template AND CONFIG_USE_middleware_edgefast_bluetooth_br_ethermind_lib_cm33 AND (NOT CONFIG_USE_middleware_edgefast_bluetooth_ble_ethermind_lib_cm33) AND (NOT CONFIG_USE_middleware_edgefast_bluetooth_btble_ethermind_lib_cm33) AND (CONFIG_CORE STREQUAL cm33))

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/host/bt_pal_a2dp.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/host/a2dp_codec/sbc/a2dp_codec_sbc.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/host/bt_pal_avrcp.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/host/bt_pal_avrcp_al_internal.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/host/bt_pal_avrcp_al_api.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/host/bt_pal_conn.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/host/bt_pal_data.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/host/bt_pal_ead.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/host/bt_pal_crypto.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/host/bt_pal_hci_core.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/host/bt_pal_adv.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/host/bt_pal_br.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/host/bt_pal_id.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/host/bt_pal_scan.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/host/bt_pal_ecc.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/host/bt_pal_addr.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/host/bt_pal_buf.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/host/bt_pal_direction.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/host/bt_pal_keys.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/host/bt_pal_keys_br.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/host/bt_pal_l2cap.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/host/bt_pal_l2cap_br.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/host/bt_pal_monitor.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/host/bt_pal_rfcomm.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/host/bt_pal_spp.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/host/bt_pal_rpa.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/host/bt_pal_settings.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/host/bt_pal_sdp.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/host/bt_pal_smp.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/host/bt_pal_ssp.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/host/bt_pal_uuid.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/host/bt_pal_hfp_ag.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/host/bt_pal_hfp_hf.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/common/addr.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/host
  ${CMAKE_CURRENT_LIST_DIR}/./include
)

else()

message(SEND_ERROR "middleware_edgefast_bluetooth_br_ethermind_cm33 dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()

endif()


if (CONFIG_USE_middleware_edgefast_bluetooth_le_audio_vcp)
# Add set(CONFIG_USE_middleware_edgefast_bluetooth_le_audio_vcp true) in config.cmake to use this component

message("middleware_edgefast_bluetooth_le_audio_vcp component is included from ${CMAKE_CURRENT_LIST_FILE}.")

if(CONFIG_USE_middleware_edgefast_bluetooth_le_audio_aics AND CONFIG_USE_middleware_edgefast_bluetooth_le_audio_vocs)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/audio/vcp_vol_ctlr.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/audio/vcp_vol_rend.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/./include/bluetooth/audio
  ${CMAKE_CURRENT_LIST_DIR}/./include/bluetooth/audio/config
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/audio
)

else()

message(SEND_ERROR "middleware_edgefast_bluetooth_le_audio_vcp dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()

endif()


if (CONFIG_USE_middleware_edgefast_bluetooth_le_audio_micp)
# Add set(CONFIG_USE_middleware_edgefast_bluetooth_le_audio_micp true) in config.cmake to use this component

message("middleware_edgefast_bluetooth_le_audio_micp component is included from ${CMAKE_CURRENT_LIST_FILE}.")

if(CONFIG_USE_middleware_edgefast_bluetooth_le_audio_aics)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/audio/micp_mic_ctlr.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/audio/micp_mic_dev.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/./include/bluetooth/audio
  ${CMAKE_CURRENT_LIST_DIR}/./include/bluetooth/audio/config
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/audio
)

else()

message(SEND_ERROR "middleware_edgefast_bluetooth_le_audio_micp dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()

endif()


if (CONFIG_USE_middleware_edgefast_bluetooth_le_audio_mcs)
# Add set(CONFIG_USE_middleware_edgefast_bluetooth_le_audio_mcs true) in config.cmake to use this component

message("middleware_edgefast_bluetooth_le_audio_mcs component is included from ${CMAKE_CURRENT_LIST_FILE}.")

if(CONFIG_USE_middleware_edgefast_bluetooth_le_audio_ccid)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/audio/mcs.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/./include/bluetooth/audio
  ${CMAKE_CURRENT_LIST_DIR}/./include/bluetooth/audio/config
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/audio
)

else()

message(SEND_ERROR "middleware_edgefast_bluetooth_le_audio_mcs dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()

endif()


if (CONFIG_USE_middleware_edgefast_bluetooth_le_audio_mcc)
# Add set(CONFIG_USE_middleware_edgefast_bluetooth_le_audio_mcc true) in config.cmake to use this component

message("middleware_edgefast_bluetooth_le_audio_mcc component is included from ${CMAKE_CURRENT_LIST_FILE}.")

if(CONFIG_USE_middleware_edgefast_bluetooth_le_audio_ccid)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/audio/mcc.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/./include/bluetooth/audio
  ${CMAKE_CURRENT_LIST_DIR}/./include/bluetooth/audio/config
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/audio
)

else()

message(SEND_ERROR "middleware_edgefast_bluetooth_le_audio_mcc dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()

endif()


if (CONFIG_USE_middleware_edgefast_bluetooth_le_audio_mpl)
# Add set(CONFIG_USE_middleware_edgefast_bluetooth_le_audio_mpl true) in config.cmake to use this component

message("middleware_edgefast_bluetooth_le_audio_mpl component is included from ${CMAKE_CURRENT_LIST_FILE}.")

if(CONFIG_USE_middleware_edgefast_bluetooth_le_audio_ccid)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/audio/media_proxy.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/audio/mpl.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/./include/bluetooth/audio
  ${CMAKE_CURRENT_LIST_DIR}/./include/bluetooth/audio/config
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/audio
)

else()

message(SEND_ERROR "middleware_edgefast_bluetooth_le_audio_mpl dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()

endif()


if (CONFIG_USE_middleware_edgefast_bluetooth_le_audio_bass)
# Add set(CONFIG_USE_middleware_edgefast_bluetooth_le_audio_bass true) in config.cmake to use this component

message("middleware_edgefast_bluetooth_le_audio_bass component is included from ${CMAKE_CURRENT_LIST_FILE}.")

if(CONFIG_USE_middleware_edgefast_bluetooth_le_audio_ccid)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/audio/bap_broadcast_assistant.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/audio/bap_scan_delegator.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/./include/bluetooth/audio
  ${CMAKE_CURRENT_LIST_DIR}/./include/bluetooth/audio/config
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/audio
)

else()

message(SEND_ERROR "middleware_edgefast_bluetooth_le_audio_bass dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()

endif()


if (CONFIG_USE_middleware_edgefast_bluetooth_profile_hrs)
# Add set(CONFIG_USE_middleware_edgefast_bluetooth_profile_hrs true) in config.cmake to use this component

message("middleware_edgefast_bluetooth_profile_hrs component is included from ${CMAKE_CURRENT_LIST_FILE}.")

if(CONFIG_USE_middleware_edgefast_bluetooth_pal)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/./source/services/hrs.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/./source/services
  ${CMAKE_CURRENT_LIST_DIR}/./include
)

else()

message(SEND_ERROR "middleware_edgefast_bluetooth_profile_hrs dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()

endif()


if (CONFIG_USE_middleware_edgefast_bluetooth_profile_bas)
# Add set(CONFIG_USE_middleware_edgefast_bluetooth_profile_bas true) in config.cmake to use this component

message("middleware_edgefast_bluetooth_profile_bas component is included from ${CMAKE_CURRENT_LIST_FILE}.")

if(CONFIG_USE_middleware_edgefast_bluetooth_pal)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/./source/services/bas.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/./source/services
  ${CMAKE_CURRENT_LIST_DIR}/./include
)

else()

message(SEND_ERROR "middleware_edgefast_bluetooth_profile_bas dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()

endif()


if (CONFIG_USE_middleware_edgefast_bluetooth_profile_dis)
# Add set(CONFIG_USE_middleware_edgefast_bluetooth_profile_dis true) in config.cmake to use this component

message("middleware_edgefast_bluetooth_profile_dis component is included from ${CMAKE_CURRENT_LIST_FILE}.")

if(CONFIG_USE_middleware_edgefast_bluetooth_pal)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/./source/services/dis.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/./source/services
  ${CMAKE_CURRENT_LIST_DIR}/./include
)

else()

message(SEND_ERROR "middleware_edgefast_bluetooth_profile_dis dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()

endif()


if (CONFIG_USE_middleware_edgefast_bluetooth_profile_hts)
# Add set(CONFIG_USE_middleware_edgefast_bluetooth_profile_hts true) in config.cmake to use this component

message("middleware_edgefast_bluetooth_profile_hts component is included from ${CMAKE_CURRENT_LIST_FILE}.")

if(CONFIG_USE_middleware_edgefast_bluetooth_pal)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/./source/services/hts.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/./source/services
  ${CMAKE_CURRENT_LIST_DIR}/./include
)

else()

message(SEND_ERROR "middleware_edgefast_bluetooth_profile_hts dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()

endif()


if (CONFIG_USE_middleware_edgefast_bluetooth_profile_ias)
# Add set(CONFIG_USE_middleware_edgefast_bluetooth_profile_ias true) in config.cmake to use this component

message("middleware_edgefast_bluetooth_profile_ias component is included from ${CMAKE_CURRENT_LIST_FILE}.")

if(CONFIG_USE_middleware_edgefast_bluetooth_pal)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/./source/services/ias/ias.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/services/ias/ias_client.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/./source/services
  ${CMAKE_CURRENT_LIST_DIR}/./include
)

else()

message(SEND_ERROR "middleware_edgefast_bluetooth_profile_ias dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()

endif()


if (CONFIG_USE_middleware_edgefast_bluetooth_profile_hps)
# Add set(CONFIG_USE_middleware_edgefast_bluetooth_profile_hps true) in config.cmake to use this component

message("middleware_edgefast_bluetooth_profile_hps component is included from ${CMAKE_CURRENT_LIST_FILE}.")

if(CONFIG_USE_middleware_edgefast_bluetooth_pal)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/./source/services/hps.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/./source/services
  ${CMAKE_CURRENT_LIST_DIR}/./include
)

else()

message(SEND_ERROR "middleware_edgefast_bluetooth_profile_hps dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()

endif()


if (CONFIG_USE_middleware_edgefast_bluetooth_profile_pxr)
# Add set(CONFIG_USE_middleware_edgefast_bluetooth_profile_pxr true) in config.cmake to use this component

message("middleware_edgefast_bluetooth_profile_pxr component is included from ${CMAKE_CURRENT_LIST_FILE}.")

if(CONFIG_USE_middleware_edgefast_bluetooth_pal)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/./source/services/pxr.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/./source/services
  ${CMAKE_CURRENT_LIST_DIR}/./include
)

else()

message(SEND_ERROR "middleware_edgefast_bluetooth_profile_pxr dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()

endif()


if (CONFIG_USE_middleware_edgefast_bluetooth_profile_ipsp)
# Add set(CONFIG_USE_middleware_edgefast_bluetooth_profile_ipsp true) in config.cmake to use this component

message("middleware_edgefast_bluetooth_profile_ipsp component is included from ${CMAKE_CURRENT_LIST_FILE}.")

if(CONFIG_USE_middleware_edgefast_bluetooth_pal)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/./source/services/ipsp.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/./source/services
  ${CMAKE_CURRENT_LIST_DIR}/./include
)

else()

message(SEND_ERROR "middleware_edgefast_bluetooth_profile_ipsp dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()

endif()


if (CONFIG_USE_middleware_edgefast_bluetooth_profile_ots)
# Add set(CONFIG_USE_middleware_edgefast_bluetooth_profile_ots true) in config.cmake to use this component

message("middleware_edgefast_bluetooth_profile_ots component is included from ${CMAKE_CURRENT_LIST_FILE}.")

if(CONFIG_USE_middleware_edgefast_bluetooth_pal)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/./source/services/ots/ots.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/services/ots/ots_client.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/services/ots/ots_dir_list.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/services/ots/ots_l2cap.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/services/ots/ots_oacp.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/services/ots/ots_obj_manager.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/services/ots/ots_olcp.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/./source/services
  ${CMAKE_CURRENT_LIST_DIR}/./source/services/ots
  ${CMAKE_CURRENT_LIST_DIR}/./include/bluetooth/services
  ${CMAKE_CURRENT_LIST_DIR}/./include/bluetooth/services/config
)

else()

message(SEND_ERROR "middleware_edgefast_bluetooth_profile_ots dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()

endif()


if (CONFIG_USE_middleware_edgefast_bluetooth_profile_wu)
# Add set(CONFIG_USE_middleware_edgefast_bluetooth_profile_wu true) in config.cmake to use this component

message("middleware_edgefast_bluetooth_profile_wu component is included from ${CMAKE_CURRENT_LIST_FILE}.")

if(CONFIG_USE_middleware_edgefast_bluetooth_pal)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/./source/services/wu.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/./source/services
  ${CMAKE_CURRENT_LIST_DIR}/./include
)

else()

message(SEND_ERROR "middleware_edgefast_bluetooth_profile_wu dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()

endif()


if (CONFIG_USE_middleware_edgefast_bluetooth_profile_fmp)
# Add set(CONFIG_USE_middleware_edgefast_bluetooth_profile_fmp true) in config.cmake to use this component

message("middleware_edgefast_bluetooth_profile_fmp component is included from ${CMAKE_CURRENT_LIST_FILE}.")

if(CONFIG_USE_middleware_edgefast_bluetooth_pal)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/./source/services/fmp.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/./source/services
  ${CMAKE_CURRENT_LIST_DIR}/./include
)

else()

message(SEND_ERROR "middleware_edgefast_bluetooth_profile_fmp dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()

endif()


if (CONFIG_USE_middleware_edgefast_bluetooth_profile_tip)
# Add set(CONFIG_USE_middleware_edgefast_bluetooth_profile_tip true) in config.cmake to use this component

message("middleware_edgefast_bluetooth_profile_tip component is included from ${CMAKE_CURRENT_LIST_FILE}.")

if(CONFIG_USE_middleware_edgefast_bluetooth_pal)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/./source/services/tip.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/./source/services
  ${CMAKE_CURRENT_LIST_DIR}/./include
)

else()

message(SEND_ERROR "middleware_edgefast_bluetooth_profile_tip dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()

endif()


if (CONFIG_USE_middleware_edgefast_bluetooth_pal_crypto_ethermind)
# Add set(CONFIG_USE_middleware_edgefast_bluetooth_pal_crypto_ethermind true) in config.cmake to use this component

message("middleware_edgefast_bluetooth_pal_crypto_ethermind component is included from ${CMAKE_CURRENT_LIST_FILE}.")

if(CONFIG_USE_middleware_edgefast_bluetooth_pal)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/crypto/bt_crypto.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/crypto
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/host
)

else()

message(SEND_ERROR "middleware_edgefast_bluetooth_pal_crypto_ethermind dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()

endif()


if (CONFIG_USE_middleware_edgefast_bluetooth_pal_platform_ethermind)
# Add set(CONFIG_USE_middleware_edgefast_bluetooth_pal_platform_ethermind true) in config.cmake to use this component

message("middleware_edgefast_bluetooth_pal_platform_ethermind component is included from ${CMAKE_CURRENT_LIST_FILE}.")

if(CONFIG_USE_middleware_edgefast_bluetooth_pal)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/platform/bt_ble_platform.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/platform/bt_ble_settings.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/platform
)

else()

message(SEND_ERROR "middleware_edgefast_bluetooth_pal_platform_ethermind dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()

endif()


if (CONFIG_USE_middleware_edgefast_bluetooth_pal_host_msd_fatfs_ethermind)
# Add set(CONFIG_USE_middleware_edgefast_bluetooth_pal_host_msd_fatfs_ethermind true) in config.cmake to use this component

message("middleware_edgefast_bluetooth_pal_host_msd_fatfs_ethermind component is included from ${CMAKE_CURRENT_LIST_FILE}.")

if(CONFIG_USE_middleware_edgefast_bluetooth_pal AND CONFIG_USE_middleware_fatfs_usb)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/platform/host_msd_fatfs.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/platform
)

else()

message(SEND_ERROR "middleware_edgefast_bluetooth_pal_host_msd_fatfs_ethermind dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()

endif()


if (CONFIG_USE_middleware_edgefast_bluetooth_pal_db_gen_ethermind)
# Add set(CONFIG_USE_middleware_edgefast_bluetooth_pal_db_gen_ethermind true) in config.cmake to use this component

message("middleware_edgefast_bluetooth_pal_db_gen_ethermind component is included from ${CMAKE_CURRENT_LIST_FILE}.")

if(CONFIG_USE_middleware_edgefast_bluetooth_pal)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/bt_db_gen.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind
)

else()

message(SEND_ERROR "middleware_edgefast_bluetooth_pal_db_gen_ethermind dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()

endif()


if (CONFIG_USE_middleware_edgefast_bluetooth_porting_atomic)
# Add set(CONFIG_USE_middleware_edgefast_bluetooth_porting_atomic true) in config.cmake to use this component

message("middleware_edgefast_bluetooth_porting_atomic component is included from ${CMAKE_CURRENT_LIST_FILE}.")

if(CONFIG_USE_middleware_edgefast_bluetooth_pal)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/./source/porting/atomic_c.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/./source/porting
  ${CMAKE_CURRENT_LIST_DIR}/./include
)

else()

message(SEND_ERROR "middleware_edgefast_bluetooth_porting_atomic dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()

endif()


if (CONFIG_USE_middleware_edgefast_bluetooth_porting_list)
# Add set(CONFIG_USE_middleware_edgefast_bluetooth_porting_list true) in config.cmake to use this component

message("middleware_edgefast_bluetooth_porting_list component is included from ${CMAKE_CURRENT_LIST_FILE}.")

if(CONFIG_USE_middleware_edgefast_bluetooth_pal)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/./source/porting/slist.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/./source/porting
  ${CMAKE_CURRENT_LIST_DIR}/./include
)

else()

message(SEND_ERROR "middleware_edgefast_bluetooth_porting_list dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()

endif()


if (CONFIG_USE_middleware_edgefast_bluetooth_porting_net)
# Add set(CONFIG_USE_middleware_edgefast_bluetooth_porting_net true) in config.cmake to use this component

message("middleware_edgefast_bluetooth_porting_net component is included from ${CMAKE_CURRENT_LIST_FILE}.")

if(CONFIG_USE_middleware_edgefast_bluetooth_pal)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/./source/porting/buf.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/./source/porting
  ${CMAKE_CURRENT_LIST_DIR}/./include
)

else()

message(SEND_ERROR "middleware_edgefast_bluetooth_porting_net dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()

endif()


if (CONFIG_USE_middleware_edgefast_bluetooth_porting_toolchain)
# Add set(CONFIG_USE_middleware_edgefast_bluetooth_porting_toolchain true) in config.cmake to use this component

message("middleware_edgefast_bluetooth_porting_toolchain component is included from ${CMAKE_CURRENT_LIST_FILE}.")

if(CONFIG_USE_middleware_edgefast_bluetooth_pal)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/./include
)

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
    -DCONFIG_ARM=1
  )

endif()

else()

message(SEND_ERROR "middleware_edgefast_bluetooth_porting_toolchain dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()

endif()


if (CONFIG_USE_middleware_edgefast_bluetooth_porting_work_queue)
# Add set(CONFIG_USE_middleware_edgefast_bluetooth_porting_work_queue true) in config.cmake to use this component

message("middleware_edgefast_bluetooth_porting_work_queue component is included from ${CMAKE_CURRENT_LIST_FILE}.")

if(CONFIG_USE_middleware_edgefast_bluetooth_pal)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/./source/porting/work_queue.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/./source/porting
  ${CMAKE_CURRENT_LIST_DIR}/./include
)

else()

message(SEND_ERROR "middleware_edgefast_bluetooth_porting_work_queue dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()

endif()


if (CONFIG_USE_middleware_edgefast_bluetooth_template)
# Add set(CONFIG_USE_middleware_edgefast_bluetooth_template true) in config.cmake to use this component

message("middleware_edgefast_bluetooth_template component is included from ${CMAKE_CURRENT_LIST_FILE}.")

if(CONFIG_USE_middleware_edgefast_bluetooth_config_template AND ((CONFIG_USE_middleware_edgefast_bluetooth_mcux_linker_template_mcxn5xxevk AND (CONFIG_BOARD STREQUAL mcxn5xxevk)) OR (CONFIG_USE_middleware_edgefast_bluetooth_mcux_linker_template_mcxn9xxevk AND (CONFIG_BOARD STREQUAL mcxn9xxevk))))

add_config_file(${CMAKE_CURRENT_LIST_DIR}/./output/templates/config/mbedtls/mbedtls_config_client.h ${CMAKE_CURRENT_LIST_DIR}/./output/templates/config/mbedtls middleware_edgefast_bluetooth_template)

else()

message(SEND_ERROR "middleware_edgefast_bluetooth_template dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()

endif()


if (CONFIG_USE_middleware_edgefast_bluetooth_porting)
# Add set(CONFIG_USE_middleware_edgefast_bluetooth_porting true) in config.cmake to use this component

message("middleware_edgefast_bluetooth_porting component is included from ${CMAKE_CURRENT_LIST_FILE}.")

if(CONFIG_USE_middleware_edgefast_bluetooth_pal AND CONFIG_USE_middleware_edgefast_bluetooth_porting_work_queue AND CONFIG_USE_middleware_edgefast_bluetooth_porting_toolchain AND CONFIG_USE_middleware_edgefast_bluetooth_porting_net AND CONFIG_USE_middleware_edgefast_bluetooth_porting_list AND CONFIG_USE_middleware_edgefast_bluetooth_porting_atomic AND CONFIG_USE_component_log AND CONFIG_USE_component_log_backend_debugconsole)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/./source/porting/ring_buffer.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/porting/porting.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/porting/utf8.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/porting/fifo/fifo.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/./include
  ${CMAKE_CURRENT_LIST_DIR}/./source/porting
)

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
    -DLOG_ENABLE_ASYNC_MODE=1
    -DLOG_MAX_ARGUMENT_COUNT=10
    -DLOG_ENABLE_OVERWRITE=0
  )

endif()

else()

message(SEND_ERROR "middleware_edgefast_bluetooth_porting dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()

endif()


if (CONFIG_USE_middleware_edgefast_bluetooth_shell)
# Add set(CONFIG_USE_middleware_edgefast_bluetooth_shell true) in config.cmake to use this component

message("middleware_edgefast_bluetooth_shell component is included from ${CMAKE_CURRENT_LIST_FILE}.")

if(CONFIG_USE_middleware_edgefast_bluetooth_pal AND CONFIG_USE_component_common_task AND ((CONFIG_USE_middleware_edgefast_bluetooth_btble_ethermind_cm33 AND (CONFIG_CORE STREQUAL cm33) AND (CONFIG_DSP STREQUAL DSP))))

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/./source/shell/shell_bt.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/shell/shell_bredr.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/shell/shell_gatt.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/shell/shell_hci.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/shell/shell_l2cap.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/shell/shell_rfcomm.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/shell/fsl_shell.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/shell/shell_a2dp.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/shell/shell_avrcp.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/shell/shell_test_mode.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/shell/shell_iso.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/./include
  ${CMAKE_CURRENT_LIST_DIR}/./source/shell
)

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
    -DSHELL_ADVANCE=1
    -DDEBUG_CONSOLE_RX_ENABLE=0
    -DOSA_USED=1
    -DSHELL_USE_COMMON_TASK=0
  )

endif()

else()

message(SEND_ERROR "middleware_edgefast_bluetooth_shell dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()

endif()


if (CONFIG_USE_middleware_edgefast_bluetooth_shell_le_audio)
# Add set(CONFIG_USE_middleware_edgefast_bluetooth_shell_le_audio true) in config.cmake to use this component

message("middleware_edgefast_bluetooth_shell_le_audio component is included from ${CMAKE_CURRENT_LIST_FILE}.")

if(CONFIG_USE_middleware_edgefast_bluetooth_pal AND CONFIG_USE_component_common_task AND ((CONFIG_USE_middleware_edgefast_bluetooth_btble_ethermind_cm33 AND (CONFIG_CORE STREQUAL cm33) AND (CONFIG_DSP STREQUAL DSP))))

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/audio/shell/bap_broadcast_assistant.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/audio/shell/bap_scan_delegator.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/audio/shell/bap.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/audio/shell/cap_acceptor.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/audio/shell/cap_initiator.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/audio/shell/csip_set_coordinator.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/audio/shell/csip_set_member.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/audio/shell/has_client.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/audio/shell/has.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/audio/shell/mcc.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/audio/shell/media_controller.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/audio/shell/micp_mic_ctlr.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/audio/shell/micp_mic_dev.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/audio/shell/mpl.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/audio/shell/tbs_client.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/audio/shell/tbs.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/audio/shell/tmap.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/audio/shell/vcp_vol_ctlr.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/audio/shell/vcp_vol_rend.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/./include
  ${CMAKE_CURRENT_LIST_DIR}/./source/shell
  ${CMAKE_CURRENT_LIST_DIR}/./source/impl/ethermind/audio/shell
)

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
    -DSHELL_ADVANCE=1
    -DDEBUG_CONSOLE_RX_ENABLE=0
    -DOSA_USED=1
    -DSHELL_USE_COMMON_TASK=0
  )

endif()

else()

message(SEND_ERROR "middleware_edgefast_bluetooth_shell_le_audio dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()

endif()


if (CONFIG_USE_middleware_edgefast_bluetooth_shell_ble)
# Add set(CONFIG_USE_middleware_edgefast_bluetooth_shell_ble true) in config.cmake to use this component

message("middleware_edgefast_bluetooth_shell_ble component is included from ${CMAKE_CURRENT_LIST_FILE}.")

if(CONFIG_USE_middleware_edgefast_bluetooth_pal AND CONFIG_USE_component_common_task AND ((CONFIG_USE_middleware_edgefast_bluetooth_ble_ethermind_cm33 AND (CONFIG_CORE STREQUAL cm33) AND (CONFIG_DSP STREQUAL DSP))))

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/./source/shell/shell_bt.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/shell/shell_gatt.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/shell/shell_hci.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/shell/shell_l2cap.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/shell/fsl_shell.c
  ${CMAKE_CURRENT_LIST_DIR}/./source/shell/shell_test_mode.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/./include
  ${CMAKE_CURRENT_LIST_DIR}/./source/shell
)

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
    -DSHELL_ADVANCE=1
    -DDEBUG_CONSOLE_RX_ENABLE=0
    -DOSA_USED=1
    -DSHELL_USE_COMMON_TASK=0
  )

endif()

else()

message(SEND_ERROR "middleware_edgefast_bluetooth_shell_ble dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()

endif()


if (CONFIG_USE_middleware_edgefast_bluetooth_btble_ethermind_lib_cm33)
# Add set(CONFIG_USE_middleware_edgefast_bluetooth_btble_ethermind_lib_cm33 true) in config.cmake to use this component

message("middleware_edgefast_bluetooth_btble_ethermind_lib_cm33 component is included from ${CMAKE_CURRENT_LIST_FILE}.")

if((CONFIG_CORE STREQUAL cm33) AND CONFIG_USE_middleware_edgefast_bluetooth_btble_ethermind_cm33)

if(CONFIG_TOOLCHAIN STREQUAL armgcc AND CONFIG_CORE STREQUAL cm33)
  target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/gcc/libethermind_bt_a2dp.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/gcc/libethermind_bt_avrcp.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/gcc/libethermind_bt_bip.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/gcc/libethermind_bt_bpp.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/gcc/libethermind_bt_core.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/gcc/libethermind_bt_ctn.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/gcc/libethermind_bt_ftp.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/gcc/libethermind_bt_ga.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/gcc/libethermind_bt_gatt.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/gcc/libethermind_bt_hfp.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/gcc/libethermind_bt_hid.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/gcc/libethermind_bt_map.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/gcc/libethermind_bt_opp.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/gcc/libethermind_bt_pan.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/gcc/libethermind_bt_pbap.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/gcc/libethermind_bt_protocol.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/gcc/libethermind_bt_sap.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/gcc/libethermind_bt_spp.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/gcc/libethermind_bt_util.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/gcc/libethermind_sbc_common.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/gcc/libethermind_sbc_decoder.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/gcc/libethermind_sbc_encoder.a
      -Wl,--end-group
  )
endif()

if(CONFIG_TOOLCHAIN STREQUAL mcux AND CONFIG_CORE STREQUAL cm33)
  target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/mcuxpresso/libethermind_bt_a2dp.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/mcuxpresso/libethermind_bt_avrcp.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/mcuxpresso/libethermind_bt_bip.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/mcuxpresso/libethermind_bt_bpp.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/mcuxpresso/libethermind_bt_core.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/mcuxpresso/libethermind_bt_ctn.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/mcuxpresso/libethermind_bt_ftp.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/mcuxpresso/libethermind_bt_ga.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/mcuxpresso/libethermind_bt_gatt.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/mcuxpresso/libethermind_bt_hfp.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/mcuxpresso/libethermind_bt_hid.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/mcuxpresso/libethermind_bt_map.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/mcuxpresso/libethermind_bt_opp.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/mcuxpresso/libethermind_bt_pan.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/mcuxpresso/libethermind_bt_pbap.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/mcuxpresso/libethermind_bt_protocol.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/mcuxpresso/libethermind_bt_sap.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/mcuxpresso/libethermind_bt_spp.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/mcuxpresso/libethermind_bt_util.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/mcuxpresso/libethermind_sbc_common.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/mcuxpresso/libethermind_sbc_decoder.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/mcuxpresso/libethermind_sbc_encoder.a
      -Wl,--end-group
  )
endif()

else()

message(SEND_ERROR "middleware_edgefast_bluetooth_btble_ethermind_lib_cm33 dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()

endif()


if (CONFIG_USE_middleware_edgefast_bluetooth_br_ethermind_lib_cm33)
# Add set(CONFIG_USE_middleware_edgefast_bluetooth_br_ethermind_lib_cm33 true) in config.cmake to use this component

message("middleware_edgefast_bluetooth_br_ethermind_lib_cm33 component is included from ${CMAKE_CURRENT_LIST_FILE}.")

if((CONFIG_CORE STREQUAL cm33) AND CONFIG_USE_middleware_edgefast_bluetooth_br_ethermind_cm33)

if(CONFIG_TOOLCHAIN STREQUAL armgcc AND CONFIG_CORE STREQUAL cm33)
  target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/gcc/libethermind_bt_a2dp.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/gcc/libethermind_bt_avrcp.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/gcc/libethermind_bt_bip.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/gcc/libethermind_bt_bpp.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/gcc/libethermind_bt_core.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/gcc/libethermind_bt_ctn.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/gcc/libethermind_bt_ftp.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/gcc/libethermind_bt_ga.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/gcc/libethermind_bt_gatt.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/gcc/libethermind_bt_hfp.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/gcc/libethermind_bt_hid.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/gcc/libethermind_bt_map.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/gcc/libethermind_bt_opp.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/gcc/libethermind_bt_pan.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/gcc/libethermind_bt_pbap.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/gcc/libethermind_bt_protocol.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/gcc/libethermind_bt_sap.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/gcc/libethermind_bt_spp.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/gcc/libethermind_bt_util.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/gcc/libethermind_sbc_common.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/gcc/libethermind_sbc_decoder.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/gcc/libethermind_sbc_encoder.a
      -Wl,--end-group
  )
endif()

if(CONFIG_TOOLCHAIN STREQUAL mcux AND CONFIG_CORE STREQUAL cm33)
  target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/mcuxpresso/libethermind_bt_a2dp.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/mcuxpresso/libethermind_bt_avrcp.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/mcuxpresso/libethermind_bt_bip.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/mcuxpresso/libethermind_bt_bpp.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/mcuxpresso/libethermind_bt_core.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/mcuxpresso/libethermind_bt_ctn.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/mcuxpresso/libethermind_bt_ftp.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/mcuxpresso/libethermind_bt_ga.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/mcuxpresso/libethermind_bt_gatt.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/mcuxpresso/libethermind_bt_hfp.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/mcuxpresso/libethermind_bt_hid.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/mcuxpresso/libethermind_bt_map.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/mcuxpresso/libethermind_bt_opp.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/mcuxpresso/libethermind_bt_pan.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/mcuxpresso/libethermind_bt_pbap.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/mcuxpresso/libethermind_bt_protocol.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/mcuxpresso/libethermind_bt_sap.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/mcuxpresso/libethermind_bt_spp.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/mcuxpresso/libethermind_bt_util.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/mcuxpresso/libethermind_sbc_common.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/mcuxpresso/libethermind_sbc_decoder.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/mcuxpresso/libethermind_sbc_encoder.a
      -Wl,--end-group
  )
endif()

else()

message(SEND_ERROR "middleware_edgefast_bluetooth_br_ethermind_lib_cm33 dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()

endif()


if (CONFIG_USE_middleware_edgefast_bluetooth_ble_ethermind_lib_cm33)
# Add set(CONFIG_USE_middleware_edgefast_bluetooth_ble_ethermind_lib_cm33 true) in config.cmake to use this component

message("middleware_edgefast_bluetooth_ble_ethermind_lib_cm33 component is included from ${CMAKE_CURRENT_LIST_FILE}.")

if((CONFIG_CORE STREQUAL cm33) AND CONFIG_USE_middleware_edgefast_bluetooth_ble_ethermind_cm33)

if(CONFIG_TOOLCHAIN STREQUAL armgcc AND CONFIG_CORE STREQUAL cm33)
  target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/ble/cm33/gcc/libethermind_ble_core.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/ble/cm33/gcc/libethermind_ble_gatt.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/ble/cm33/gcc/libethermind_ble_protocol.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/ble/cm33/gcc/libethermind_ble_util.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/ble/cm33/gcc/libethermind_ble_ga.a
      -Wl,--end-group
  )
endif()

if(CONFIG_TOOLCHAIN STREQUAL mcux AND CONFIG_CORE STREQUAL cm33)
  target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/ble/cm33/mcuxpresso/libethermind_ble_core.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/ble/cm33/mcuxpresso/libethermind_ble_gatt.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/ble/cm33/mcuxpresso/libethermind_ble_protocol.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/ble/cm33/mcuxpresso/libethermind_ble_util.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/ble/cm33/mcuxpresso/libethermind_ble_ga.a
      -Wl,--end-group
  )
endif()

else()

message(SEND_ERROR "middleware_edgefast_bluetooth_ble_ethermind_lib_cm33 dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()

endif()


if (CONFIG_USE_middleware_edgefast_bluetooth_btble_ethermind_lib_lc3_cm33)
# Add set(CONFIG_USE_middleware_edgefast_bluetooth_btble_ethermind_lib_lc3_cm33 true) in config.cmake to use this component

message("middleware_edgefast_bluetooth_btble_ethermind_lib_lc3_cm33 component is included from ${CMAKE_CURRENT_LIST_FILE}.")

if((CONFIG_CORE STREQUAL cm33) AND CONFIG_USE_middleware_edgefast_bluetooth_btble_ethermind_cm33)

if(CONFIG_TOOLCHAIN STREQUAL armgcc AND CONFIG_CORE STREQUAL cm33)
  target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/gcc/libethermind_lc3_common.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/gcc/libethermind_lc3_decoder.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/gcc/libethermind_lc3_encoder.a
      -Wl,--end-group
  )
endif()

if(CONFIG_TOOLCHAIN STREQUAL mcux AND CONFIG_CORE STREQUAL cm33)
  target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/mcuxpresso/libethermind_lc3_common.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/mcuxpresso/libethermind_lc3_decoder.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/mcuxpresso/libethermind_lc3_encoder.a
      -Wl,--end-group
  )
endif()

else()

message(SEND_ERROR "middleware_edgefast_bluetooth_btble_ethermind_lib_lc3_cm33 dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()

endif()


if (CONFIG_USE_middleware_edgefast_bluetooth_br_ethermind_lib_lc3_cm33)
# Add set(CONFIG_USE_middleware_edgefast_bluetooth_br_ethermind_lib_lc3_cm33 true) in config.cmake to use this component

message("middleware_edgefast_bluetooth_br_ethermind_lib_lc3_cm33 component is included from ${CMAKE_CURRENT_LIST_FILE}.")

if((CONFIG_CORE STREQUAL cm33) AND CONFIG_USE_middleware_edgefast_bluetooth_br_ethermind_cm33)

if(CONFIG_TOOLCHAIN STREQUAL armgcc AND CONFIG_CORE STREQUAL cm33)
  target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/gcc/libethermind_lc3_common.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/gcc/libethermind_lc3_decoder.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/gcc/libethermind_lc3_encoder.a
      -Wl,--end-group
  )
endif()

if(CONFIG_TOOLCHAIN STREQUAL mcux AND CONFIG_CORE STREQUAL cm33)
  target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/mcuxpresso/libethermind_lc3_common.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/mcuxpresso/libethermind_lc3_decoder.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm33/mcuxpresso/libethermind_lc3_encoder.a
      -Wl,--end-group
  )
endif()

else()

message(SEND_ERROR "middleware_edgefast_bluetooth_br_ethermind_lib_lc3_cm33 dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()

endif()


if (CONFIG_USE_middleware_edgefast_bluetooth_ble_ethermind_lib_lc3_cm33)
# Add set(CONFIG_USE_middleware_edgefast_bluetooth_ble_ethermind_lib_lc3_cm33 true) in config.cmake to use this component

message("middleware_edgefast_bluetooth_ble_ethermind_lib_lc3_cm33 component is included from ${CMAKE_CURRENT_LIST_FILE}.")

if((CONFIG_CORE STREQUAL cm33) AND CONFIG_USE_middleware_edgefast_bluetooth_ble_ethermind_cm33)

if(CONFIG_TOOLCHAIN STREQUAL armgcc AND CONFIG_CORE STREQUAL cm33)
  target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/ble/cm33/gcc/libethermind_lc3_common.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/ble/cm33/gcc/libethermind_lc3_decoder.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/ble/cm33/gcc/libethermind_lc3_encoder.a
      -Wl,--end-group
  )
endif()

if(CONFIG_TOOLCHAIN STREQUAL mcux AND CONFIG_CORE STREQUAL cm33)
  target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/ble/cm33/mcuxpresso/libethermind_lc3_decoder.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/ble/cm33/mcuxpresso/libethermind_lc3_encoder.a
      ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/lib/mcux/default/ble/cm33/mcuxpresso/libethermind_lc3_common.a
      -Wl,--end-group
  )
endif()

else()

message(SEND_ERROR "middleware_edgefast_bluetooth_ble_ethermind_lib_lc3_cm33 dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()

endif()


if (CONFIG_USE_middleware_edgefast_bluetooth_sdio_template_mcxn5xxevk)
# Add set(CONFIG_USE_middleware_edgefast_bluetooth_sdio_template_mcxn5xxevk true) in config.cmake to use this component

message("middleware_edgefast_bluetooth_sdio_template_mcxn5xxevk component is included from ${CMAKE_CURRENT_LIST_FILE}.")

if((CONFIG_DEVICE_ID STREQUAL MCXN547))

else()

message(SEND_ERROR "middleware_edgefast_bluetooth_sdio_template_mcxn5xxevk dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()

endif()


if (CONFIG_USE_middleware_edgefast_bluetooth_mcux_linker_template_mcxn5xxevk)
# Add set(CONFIG_USE_middleware_edgefast_bluetooth_mcux_linker_template_mcxn5xxevk true) in config.cmake to use this component

message("middleware_edgefast_bluetooth_mcux_linker_template_mcxn5xxevk component is included from ${CMAKE_CURRENT_LIST_FILE}.")

if((CONFIG_DEVICE_ID STREQUAL MCXN547))

if(CONFIG_TOOLCHAIN STREQUAL mcux)
  add_config_file(${CMAKE_CURRENT_LIST_DIR}/../../boards/mcxn5xxevk/edgefast_bluetooth_examples/template/data.ldt "" middleware_edgefast_bluetooth_mcux_linker_template_mcxn5xxevk)
  add_config_file(${CMAKE_CURRENT_LIST_DIR}/../../boards/mcxn5xxevk/edgefast_bluetooth_examples/template/end_text.ldt "" middleware_edgefast_bluetooth_mcux_linker_template_mcxn5xxevk)
  add_config_file(${CMAKE_CURRENT_LIST_DIR}/../../boards/mcxn5xxevk/edgefast_bluetooth_examples/template/main_data.ldt "" middleware_edgefast_bluetooth_mcux_linker_template_mcxn5xxevk)
  add_config_file(${CMAKE_CURRENT_LIST_DIR}/../../boards/mcxn5xxevk/edgefast_bluetooth_examples/template/main_text.ldt "" middleware_edgefast_bluetooth_mcux_linker_template_mcxn5xxevk)
  add_config_file(${CMAKE_CURRENT_LIST_DIR}/../../boards/mcxn5xxevk/edgefast_bluetooth_examples/template/symbols.ldt "" middleware_edgefast_bluetooth_mcux_linker_template_mcxn5xxevk)
endif()

else()

message(SEND_ERROR "middleware_edgefast_bluetooth_mcux_linker_template_mcxn5xxevk dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()

endif()

