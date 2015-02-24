include(AmxConfig)

if(MSVC)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

set(SHOEBILL_FOLDER_SRC EncodingUtils.cpp EncodingUtils.h JniFunctions.cpp JniFunctions.h JniUtils.cpp JniUtils.h
                     linux.cpp linux.h ShoebillMain.cpp ShoebillMain.h)

add_library(shoebill_folder STATIC ${SHOEBILL_FOLDER_SRC})
target_link_libraries(shoebill_folder amx)