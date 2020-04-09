
set(core_sources
  CoreProcessor.h
  CoreProcessor.cpp
  CoreController.h
  CoreController.cpp
  coreid.h
  factory.cpp
  version.h
)

set(target core)
smtg_add_vst3plugin(${target} ${core_sources})

# set_target_properties(${target} PROPERTIES ${SDK_IDE_PLUGIN_EXAMPLES_FOLDER})
target_link_libraries(${target} PRIVATE base sdk)

if(SMTG_MAC)
    smtg_set_bundle(${target} INFOPLIST "${CMAKE_CURRENT_LIST_DIR}/resource/Info.plist" PREPROCESS)
elseif(SMTG_WIN)
    # target_sources(${target} PRIVATE resource/adelay.rc)
endif()
