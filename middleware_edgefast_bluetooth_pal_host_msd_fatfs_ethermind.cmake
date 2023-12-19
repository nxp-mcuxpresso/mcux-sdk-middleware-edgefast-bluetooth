#Description: middleware.edgefast_bluetooth.pal.host_msd_fatfs.ethermind; user_visible: False
include_guard(GLOBAL)
message("middleware_edgefast_bluetooth_pal_host_msd_fatfs_ethermind component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/platform/host_msd_fatfs.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/platform
)


include(middleware_edgefast_bluetooth_pal)
