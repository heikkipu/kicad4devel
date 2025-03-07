# Install script for directory: /opt/kicad/program/master/Tears_Master/demos

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "0")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set path to fallback-tool for dependency-resolution.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "resources" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/local/share/kicad/demos/complex_hierarchy;/usr/local/share/kicad/demos/custom_pads_test;/usr/local/share/kicad/demos/ecc83;/usr/local/share/kicad/demos/flat_hierarchy;/usr/local/share/kicad/demos/interf_u;/usr/local/share/kicad/demos/kit-dev-coldfire-xilinx_5213;/usr/local/share/kicad/demos/microwave;/usr/local/share/kicad/demos/multichannel;/usr/local/share/kicad/demos/pic_programmer;/usr/local/share/kicad/demos/python_scripts_examples;/usr/local/share/kicad/demos/royalblue54L_feather;/usr/local/share/kicad/demos/simulation;/usr/local/share/kicad/demos/sonde xilinx;/usr/local/share/kicad/demos/stickhub;/usr/local/share/kicad/demos/test_pads_inside_pads;/usr/local/share/kicad/demos/test_xil_95108;/usr/local/share/kicad/demos/tiny_tapeout;/usr/local/share/kicad/demos/video;/usr/local/share/kicad/demos/vme-wren")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "/usr/local/share/kicad/demos" TYPE DIRECTORY FILES
    "/opt/kicad/program/master/Tears_Master/demos/complex_hierarchy"
    "/opt/kicad/program/master/Tears_Master/demos/custom_pads_test"
    "/opt/kicad/program/master/Tears_Master/demos/ecc83"
    "/opt/kicad/program/master/Tears_Master/demos/flat_hierarchy"
    "/opt/kicad/program/master/Tears_Master/demos/interf_u"
    "/opt/kicad/program/master/Tears_Master/demos/kit-dev-coldfire-xilinx_5213"
    "/opt/kicad/program/master/Tears_Master/demos/microwave"
    "/opt/kicad/program/master/Tears_Master/demos/multichannel"
    "/opt/kicad/program/master/Tears_Master/demos/pic_programmer"
    "/opt/kicad/program/master/Tears_Master/demos/python_scripts_examples"
    "/opt/kicad/program/master/Tears_Master/demos/royalblue54L_feather"
    "/opt/kicad/program/master/Tears_Master/demos/simulation"
    "/opt/kicad/program/master/Tears_Master/demos/sonde xilinx"
    "/opt/kicad/program/master/Tears_Master/demos/stickhub"
    "/opt/kicad/program/master/Tears_Master/demos/test_pads_inside_pads"
    "/opt/kicad/program/master/Tears_Master/demos/test_xil_95108"
    "/opt/kicad/program/master/Tears_Master/demos/tiny_tapeout"
    "/opt/kicad/program/master/Tears_Master/demos/video"
    "/opt/kicad/program/master/Tears_Master/demos/vme-wren"
    )
endif()

