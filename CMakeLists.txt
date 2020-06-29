#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(main)

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable

option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
# Setup include directory for this project

include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/include/NR_Physics)


#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)
file(GLOB nr_sources ${PROJECT_SOURCE_DIR}/src/NR_Physics/*.cc)
file(GLOB nr_headers ${PROJECT_SOURCE_DIR}/include/NR_Physics/*.hh)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries

add_executable(main main.cc ${sources} ${headers} ${nr_sources} ${nr_headers})
target_link_libraries(main ${Geant4_LIBRARIES})

#----------------------------------------------------------------------------
# Copy .mac scripts

file(GLOB PTFE_SCRIPTS ${PROJECT_SOURCE_DIR}/mac/*)
file(COPY ${PTFE_SCRIPTS} DESTINATION ${PROJECT_BINARY_DIR}/mac)
file(COPY initialize.mac DESTINATION ${PROJECT_BINARY_DIR})
file(COPY initialize_vis.mac DESTINATION ${PROJECT_BINARY_DIR})


#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX

install(TARGETS main DESTINATION bin)


