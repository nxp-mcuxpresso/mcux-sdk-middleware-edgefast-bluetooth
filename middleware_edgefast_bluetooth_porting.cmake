#Description: middleware.edgefast_bluetooth.porting; user_visible: False
include_guard(GLOBAL)
message("middleware_edgefast_bluetooth_porting component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/source/porting/porting.c
    ${CMAKE_CURRENT_LIST_DIR}/source/porting/utf8.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include
    ${CMAKE_CURRENT_LIST_DIR}/source/porting
)


include(middleware_edgefast_bluetooth_porting_atomic)
include(middleware_edgefast_bluetooth_porting_toolchain)
include(component_log_backend_debugconsole_RW612)
include(middleware_edgefast_bluetooth_porting_work_queue)
include(middleware_edgefast_bluetooth_porting_list)
include(middleware_edgefast_bluetooth_porting_net)
