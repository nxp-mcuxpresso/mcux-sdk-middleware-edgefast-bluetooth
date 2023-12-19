#Description: middleware.edgefast_bluetooth.porting.atomic; user_visible: False
include_guard(GLOBAL)
message("middleware_edgefast_bluetooth_porting_atomic component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/source/porting/atomic_c.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include
    ${CMAKE_CURRENT_LIST_DIR}/source/porting
)


include(middleware_edgefast_bluetooth_pal)
