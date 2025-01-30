# Install script for directory: /opt/kicad/program/V5/Tears_V5_S/demos

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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/kicad/demos" TYPE DIRECTORY FILES
    "/opt/kicad/program/V5/Tears_V5_S/demos/complex_hierarchy"
    "/opt/kicad/program/V5/Tears_V5_S/demos/custom_pads_test"
    "/opt/kicad/program/V5/Tears_V5_S/demos/ecc83"
    "/opt/kicad/program/V5/Tears_V5_S/demos/electric"
    "/opt/kicad/program/V5/Tears_V5_S/demos/flat_hierarchy"
    "/opt/kicad/program/V5/Tears_V5_S/demos/interf_u"
    "/opt/kicad/program/V5/Tears_V5_S/demos/kit-dev-coldfire-xilinx_5213"
    "/opt/kicad/program/V5/Tears_V5_S/demos/microwave"
    "/opt/kicad/program/V5/Tears_V5_S/demos/pic_programmer"
    "/opt/kicad/program/V5/Tears_V5_S/demos/python_scripts_examples"
    "/opt/kicad/program/V5/Tears_V5_S/demos/simulation"
    "/opt/kicad/program/V5/Tears_V5_S/demos/sonde xilinx"
    "/opt/kicad/program/V5/Tears_V5_S/demos/test_pads_inside_pads"
    "/opt/kicad/program/V5/Tears_V5_S/demos/test_xil_95108"
    "/opt/kicad/program/V5/Tears_V5_S/demos/video"
    )
endif()

