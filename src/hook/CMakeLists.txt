include(AmxConfig)

if (MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif ()

set(HOOK_SOURCES SimpleAsm.hpp SimpleInlineHook.cpp SimpleInlineHook.hpp)

add_library(hook STATIC ${HOOK_SOURCES})

