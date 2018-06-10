cmake_minimum_required(VERSION 3.5.0)
project(GC-pokemon-RNG-manip-assistant)

include(FindOpenMP)
if(OPENMP_FOUND)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

set(SRCS PokemonRNGSystem/BaseRNGSystem.cpp
         PokemonRNGSystem/Colosseum/ColosseumRNGSystem.cpp
         PokemonRNGSystem/XD/GaleDarknessRNGSystem.cpp
         GUI/SPokemonRNG.cpp
         GUI/GUICommon.cpp
         GUI/Settings/CommonPredictorFiltersWidget.cpp
         GUI/Settings/SeedFinderTab.cpp
         GUI/Settings/CommonPredictorFiltersWidget.cpp
         GUI/Settings/ColosseumTab.cpp
         GUI/Settings/GaleDarknessTab.cpp
         GUI/Settings/DlgSettings.cpp
         GUI/SeedFinder/SeedFinderPassPage.cpp
         GUI/SeedFinder/SeedFinderWizard.cpp
         GUI/Predictor/PredictorWidget.cpp
         GUI/MainWindow.cpp
         main.cpp)

set(CMAKE_INCLUE_CURRENT_DIR ON)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Concurrent REQUIRED)
         
set(CMAKE_AUTOMOC ON)
         
IF(WIN32)
  SET(GUI_TYPE WIN32)
ENDIF(WIN32)
         
add_executable(GC-pokemon-RNG-manip-assistant ${GUI_TYPE} ${SRCS})
         
target_link_libraries(GC-pokemon-RNG-manip-assistant Qt5::Widgets)
target_link_libraries(GC-pokemon-RNG-manip-assistant Qt5::Gui)
target_link_libraries(GC-pokemon-RNG-manip-assistant Qt5::Core)
target_link_libraries(GC-pokemon-RNG-manip-assistant Qt5::Concurrent)
         
if(WIN32)
  if($<CONFIG:Debug>)
    get_target_property(WIDGETDLL Qt5::Widgets IMPORTED_LOCATION_DEBUG)
    get_target_property(COREDLL Qt5::Widgets IMPORTED_LOCATION_DEBUG)
    get_target_property(GUIDLL Qt5::Widgets IMPORTED_LOCATION_DEBUG)
  else($<CONFIG:Debug>)
    get_target_property(WIDGETDLL Qt5::Widgets IMPORTED_LOCATION_RELEASE)
    get_target_property(COREDLL Qt5::Widgets IMPORTED_LOCATION_RELEASE)
    get_target_property(GUIDLL Qt5::Widgets IMPORTED_LOCATION_RELEASE)
  endif($<CONFIG:Debug>)
  add_custom_command(
    TARGET GC-pokemon-RNG-manip-assistant POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      $<TARGET_FILE:Qt5::Widgets>
      <TARGET_FILE_DIR:GC-pokemon-RNG-manip-assistant>
    TARGET GC-pokemon-RNG-manip-assistant POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      $<TARGET_FILE:Qt5::Core>
      $<TARGET_FILE_DIR:GC-pokemon-RNG-manip-assistant>
    TARGET GC-pokemon-RNG-manip-assistant POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      $<TARGET_FILE:Qt5::Gui>
      $<TARGET_FILE_DIR:GC-pokemon-RNG-manip-assistant>
    TARGET GC-pokemon-RNG-manip-assistant POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      $<TARGET_FILE:Qt5::Concurrent>
      $<TARGET_FILE_DIR:GC-pokemon-RNG-manip-assistant>
    )
endif(WIN32)
