#Description: middleware.edgefast_bluetooth.shell_ble; user_visible: False
include_guard(GLOBAL)
message("middleware_edgefast_bluetooth_shell_ble component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/source/shell/shell_bt.c
    ${CMAKE_CURRENT_LIST_DIR}/source/shell/shell_gatt.c
    ${CMAKE_CURRENT_LIST_DIR}/source/shell/shell_hci.c
    ${CMAKE_CURRENT_LIST_DIR}/source/shell/shell_l2cap.c
    ${CMAKE_CURRENT_LIST_DIR}/source/shell/fsl_shell.c
    ${CMAKE_CURRENT_LIST_DIR}/source/shell/shell_test_mode.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include
    ${CMAKE_CURRENT_LIST_DIR}/source/shell
)


include(component_common_task)
