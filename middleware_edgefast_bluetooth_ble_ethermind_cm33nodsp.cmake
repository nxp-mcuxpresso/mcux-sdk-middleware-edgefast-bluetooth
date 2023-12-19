#Description: middleware.edgefast_bluetooth.ble.ethermind.cm33nodsp; user_visible: False
include_guard(GLOBAL)
message("middleware_edgefast_bluetooth_ble_ethermind_cm33nodsp component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/host/bt_pal_att.c
    ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/host/bt_pal_conn.c
    ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/host/bt_pal_crypto.c
    ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/host/bt_pal_gatt.c
    ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/host/bt_pal_hci_core.c
    ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/host/bt_pal_adv.c
    ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/host/bt_pal_br.c
    ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/host/bt_pal_id.c
    ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/host/bt_pal_iso.c
    ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/host/bt_pal_scan.c
    ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/host/bt_pal_ecc.c
    ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/host/bt_pal_addr.c
    ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/host/bt_pal_buf.c
    ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/host/bt_pal_direction.c
    ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/host/bt_pal_keys.c
    ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/host/bt_pal_l2cap.c
    ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/host/bt_pal_monitor.c
    ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/host/bt_pal_rpa.c
    ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/host/bt_pal_settings.c
    ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/host/bt_pal_smp.c
    ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/host/bt_pal_smp_null.c
    ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/host/bt_pal_uuid.c
    ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/common/addr.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include
    ${CMAKE_CURRENT_LIST_DIR}/source
)


include(middleware_edgefast_bluetooth_pal)
include(middleware_edgefast_bluetooth_ble_ethermind_lib_cm33nodsp)
