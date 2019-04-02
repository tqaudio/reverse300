if(SMTG_AAX_SDK_PATH AND SMTG_ADD_VSTGUI)
  include(AddAAXLibrary)
  set(
    base_sources
    ${SDK_ROOT}/public.sdk/source/vst/basewrapper/basewrapper.sdk.cpp
  )
  set(
    Reverse300_aax_sources
    source/aax.cpp
    ../Reverse300/include/buffer.h
    ../Reverse300/include/constants.h
    ../Reverse300/include/controller.h
    ../Reverse300/include/id.h
    ../Reverse300/include/parameter.h
    ../Reverse300/include/processor.h
    ../Reverse300/include/version.h
    ../Reverse300/source/buffer.cpp
    ../Reverse300/source/controller.cpp
    ../Reverse300/source/factory.cpp
    ../Reverse300/source/parameter.cpp
    ../Reverse300/source/processor.cpp
  )
  set(Reverse300_sources ${Reverse300_aax_sources} ${base_sources})
  set(target Reverse300aax)
  smtg_add_aaxplugin(${target} ${Reverse300_sources})
  target_include_directories(${target} PUBLIC ../Reverse300/include)
  target_include_directories(${target} PUBLIC ../Reverse300/source)
  target_include_directories(${target} PUBLIC ${VSTGUI_ROOT}/vstgui4)
  target_link_libraries(${target} PRIVATE base sdk vstgui_support aaxwrapper)

  smtg_add_vst3_resource(${target} "../Reverse300/resource/main.uidesc")

  smtg_add_vst3_resource(${target} "../Reverse300/image/knob_1x.png")
  smtg_add_vst3_resource(${target} "../Reverse300/image/knob_2x.png")
  smtg_add_vst3_resource(${target} "../Reverse300/image/knob_3x.png")

  if(SMTG_MAC)
    smtg_set_bundle(${target} INFOPLIST "${CMAKE_CURRENT_LIST_DIR}/../Reverse300/resource/Info.plist" PREPROCESS)
  elseif(SMTG_WIN)
    target_sources(${target} PRIVATE ../Reverse300/resource/Plugin.rc)
  endif()
endif()
