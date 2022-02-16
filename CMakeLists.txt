cmake_minimum_required(VERSION 2.8)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  if(DEFINED ENV{VITASDK})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
  else()
    message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
  endif()
endif()

project(evovita)
include("${VITASDK}/share/vita.cmake" REQUIRED)

set(VITA_APP_NAME "Evovita")
set(VITA_TITLEID  "EVOVITA00")
set(VITA_VERSION  "00.01")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SDL2_INCLUDE_DIR "$ENV{VITASDK}/arm-vita-eabi/include/SDL2")

include_directories(${SDL2_INCLUDE_DIR})

add_executable(${PROJECT_NAME}
  src/main.cpp

  src/board.cpp
  src/individual.cpp
  src/vector.cpp
  src/utils.cpp
)

target_link_libraries(${PROJECT_NAME}
  FLAC
  SDL2
  SDL2_image
  SDL2_mixer
  SDL2_ttf
  SDL2_gfx
  SceAppMgr_stub
  SceAppUtil_stub
  SceAudio_stub
  SceCommonDialog_stub
  SceCtrl_stub
  SceDisplay_stub
  SceGxm_stub
  SceHid_stub
  SceMotion_stub
  SceSysmodule_stub
  SceTouch_stub
  freetype
  jpeg
  m
  mikmod
  mpg123
  ogg
  png16
  vorbis
  vorbisfile
  z
)

vita_create_self(eboot.bin ${PROJECT_NAME})
vita_create_vpk(${PROJECT_NAME}.vpk ${VITA_TITLEID} eboot.bin
  VERSION ${VITA_VERSION}
  NAME ${VITA_APP_NAME}
  FILE sce_sys/icon0.png sce_sys/icon0.png
  FILE sce_sys/livearea/contents/bg.png sce_sys/livearea/contents/bg.png
  FILE sce_sys/livearea/contents/startup.png sce_sys/livearea/contents/startup.png
  FILE sce_sys/livearea/contents/template.xml sce_sys/livearea/contents/template.xml
  FILE resources resources
)

add_custom_target(send
  COMMAND curl -T eboot.bin ftp://"$ENV{PSVITAIP}":1337/ux0:/app/${VITA_TITLEID}/
  DEPENDS eboot.bin
)

add_custom_target(copy
  COMMAND cp eboot.bin ${PSVITADRIVE}/app/${VITA_TITLEID}/eboot.bin
  DEPENDS eboot.bin
)
