include(AmxConfig)
include(Plugin)

find_package(JNI REQUIRED)
add_definitions(-DSAMPGDK_AMALGAMATION)

set(CMAKE_CXX_FLAGS_DEBUG "-DSHOEBILL_DEBUG")

if (UNIX)
    find_package(Iconv REQUIRED)
endif ()

if (JNI_FOUND)
    include_directories(${JNI_INCLUDE_DIRS})
    message(STATUS "JNI_INCLUDE_DIRS=${JNI_INCLUDE_DIRS}")
    message(STATUS "JNI_LIBRARIES=${JNI_LIBRARIES}")
else ()
    message(ERROR "JNI not found!")
endif ()

if (UNIX)
    if (ICONV_FOUND)
        include_directories(${ICONV_INCLUDE_DIR})
        message(STATUS "LIBICONV was found at ${ICONV_LIBRARIES}")
        message(STATUS "LIBICONV includes at ${ICONV_INCLUDE_DIR}")
    else ()
        message(ERROR "LIBICONV was NOT found.")
    endif ()
endif ()

include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}
)

if (UNIX)
    add_definitions(-DAMXPROF_RELOCATE_OPCODES)
endif ()

include_directories(shoebill wrapper sampplugin hook)

set(SHOEBILL_SOURCES shoebill/EncodingUtils.cpp shoebill/EncodingUtils.h shoebill/JniUtils.cpp shoebill/JniUtils.h
        shoebill/JniFunctions.h shoebill/JniFunctions.cpp wrapper/AmxHelper.h wrapper/AmxInstanceManager.hpp
        wrapper/Callbacks.cpp wrapper/Callbacks.h wrapper/exports.def
        wrapper/PluginMain.cpp sampgdk.c sampgdk.h shoebill/Shoebill.cpp shoebill/Shoebill.h)

add_subdirectory(sampplugin)
add_subdirectory(hook)
add_plugin(Shoebill ${SHOEBILL_SOURCES})
if (UNIX)
    set_target_properties(Shoebill PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")
endif ()
target_link_libraries(Shoebill amx hook ${JNI_LIBRARIES})
if (UNIX)
    target_link_libraries(Shoebill ${ICONV_LIBRARIES})
endif ()
install(TARGETS Shoebill LIBRARY DESTINATION ".")
