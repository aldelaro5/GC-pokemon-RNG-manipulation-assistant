cmake_minimum_required(VERSION 3.5.0)
project(GC-pokemon-RNG-manip-assistant)

# This won't work for Mac OS because they disabled the builtin OpenMP
if(NOT APPLE)
  include(FindOpenMP)
  if(OPENMP_FOUND)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
  	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
  	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
	endif()
endif()

set(SRCS PokemonRNGSystem/BaseRNGSystem.cpp
         PokemonRNGSystem/Colosseum/ColosseumRNGSystem.cpp
         PokemonRNGSystem/XD/GaleDarknessRNGSystem.cpp
         GUI/GUICommon.cpp
         GUI/SPokemonRNG.cpp
         GUI/Settings/SConfig.cpp
         GUI/Settings/CommonPredictorFiltersWidget.cpp
         GUI/Settings/GeneralTab.cpp
         GUI/Settings/CommonPredictorFiltersWidget.cpp
         GUI/Settings/ColosseumTab.cpp
         GUI/Settings/GaleDarknessTab.cpp
         GUI/Settings/DlgSettings.cpp
         GUI/SeedFinder/SeedFinderPassPage.cpp
         GUI/SeedFinder/SeedFinderWizard.cpp
         GUI/Predictor/PredictorWidget.cpp
         GUI/StatsReporter/PokemonPropertiesFrame.cpp
         GUI/StatsReporter/StatsReporterWidget.cpp
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

if(APPLE)
  set(MACOSX_BUNDLE_BUNDLE_NAME GC-pokemon-RNG-manip-assistant)
  add_executable(GC-pokemon-RNG-manip-assistant MACOSX_BUNDLE ${SRCS})
  # The reason OpenMP is complicated to setup is because altough recent versions of CMake supports
  # a more elegant solution, there's an issue with Mojave that prevents it from working and forces
  # us to manually add the coresponding flags. This requires OpenMP to be installed with homebrew.
  # The details of the issue is available here: https://gitlab.kitware.com/cmake/cmake/issues/18520  
  set(OpenMP_C_FLAGS "-Xpreprocessor -fopenmp -I/usr/local/opt/libomp/include")
  set(OpenMP_C_LIB_NAMES omp)
  set(OpenMP_CXX_FLAGS "-Xpreprocessor -fopenmp -I/usr/local/opt/libomp/include")
  set(OpenMP_CXX_LIB_NAMES omp)
  set(OpenMP_omp_LIBRARY /usr/local/opt/libomp/lib/libomp.dylib)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
  target_link_libraries(GC-pokemon-RNG-manip-assistant ${OpenMP_omp_LIBRARY})
else()         
add_executable(GC-pokemon-RNG-manip-assistant ${GUI_TYPE} ${SRCS})
endif()
         
target_link_libraries(GC-pokemon-RNG-manip-assistant Qt5::Widgets)
target_link_libraries(GC-pokemon-RNG-manip-assistant Qt5::Gui)
target_link_libraries(GC-pokemon-RNG-manip-assistant Qt5::Core)
target_link_libraries(GC-pokemon-RNG-manip-assistant Qt5::Concurrent)

If(APPLE)   
  add_custom_command(TARGET GC-pokemon-RNG-manip-assistant POST_BUILD 
                     COMMAND ${CMAKE_COMMAND} -E copy 
                     $<TARGET_FILE:GC-pokemon-RNG-manip-assistant> 
                     GC-pokemon-RNG-manip-assistant.app/Contents/MacOS/)
  # On Mac OS, macdeployqt is a tool offered by the Qt framework to automatically setup a
  # standalone app from an app bundle. It handles everything Qt would need.
  add_custom_command(TARGET GC-pokemon-RNG-manip-assistant POST_BUILD COMMAND macdeployqt 
                     GC-pokemon-RNG-manip-assistant.app)
endif()

add_custom_command(TARGET GC-pokemon-RNG-manip-assistant POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory 
                   ${CMAKE_CURRENT_BINARY_DIR}/../Resources ${CMAKE_CURRENT_BINARY_DIR}/Resources)
      
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
