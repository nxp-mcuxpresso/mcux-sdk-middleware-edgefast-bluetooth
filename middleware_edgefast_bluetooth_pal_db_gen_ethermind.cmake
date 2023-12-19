#Description: middleware.edgefast_bluetooth.pal.db_gen.ethermind; user_visible: False
include_guard(GLOBAL)
message("middleware_edgefast_bluetooth_pal_db_gen_ethermind component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/bt_db_gen.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind
)


include(middleware_edgefast_bluetooth_pal)
