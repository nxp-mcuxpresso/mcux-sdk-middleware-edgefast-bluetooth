#Description: middleware.edgefast_bluetooth.profile.pxr; user_visible: False
include_guard(GLOBAL)
message("middleware_edgefast_bluetooth_profile_pxr component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/source/services/pxr.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include
    ${CMAKE_CURRENT_LIST_DIR}/source/services
)


include(middleware_edgefast_bluetooth_pal)
