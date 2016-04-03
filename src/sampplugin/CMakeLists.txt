include(AmxConfig)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set_property(DIRECTORY ${CMAKE_SOURCE_DIR} APPEND PROPERTY INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/..
        )

if (MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif ()

add_library(amx STATIC
        amx/amx.h
        amx/getch.h
        amx/osdefs.h
        amx/sclinux.h
        amxplugin.cpp
        plugin.h
        plugincommon.h
        )