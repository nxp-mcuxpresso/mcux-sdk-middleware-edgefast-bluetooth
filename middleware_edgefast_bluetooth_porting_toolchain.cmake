#Description: middleware.edgefast_bluetooth.porting.toolchain; user_visible: False
include_guard(GLOBAL)
message("middleware_edgefast_bluetooth_porting_toolchain component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include
)


include(middleware_edgefast_bluetooth_pal)
