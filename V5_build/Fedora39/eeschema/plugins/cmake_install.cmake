# Install script for directory: /opt/kicad/program/V5/Tears_V5_S/eeschema/plugins

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

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "binary" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/kicad/plugins" TYPE FILE FILES
    "/opt/kicad/program/V5/Tears_V5_S/eeschema/plugins/xsl_scripts/bom2csv.xsl"
    "/opt/kicad/program/V5/Tears_V5_S/eeschema/plugins/xsl_scripts/bom2grouped_csv.xsl"
    "/opt/kicad/program/V5/Tears_V5_S/eeschema/plugins/xsl_scripts/netlist_form_cadstar-RINF.xsl"
    "/opt/kicad/program/V5/Tears_V5_S/eeschema/plugins/xsl_scripts/netlist_form_cadstar.xsl"
    "/opt/kicad/program/V5/Tears_V5_S/eeschema/plugins/xsl_scripts/netlist_form_OrcadPcb2.xsl"
    "/opt/kicad/program/V5/Tears_V5_S/eeschema/plugins/xsl_scripts/netlist_form_pads-pcb.asc.xsl"
    "/opt/kicad/program/V5/Tears_V5_S/eeschema/plugins/xsl_scripts/bom_with_title_block_2_csv.xsl"
    "/opt/kicad/program/V5/Tears_V5_S/eeschema/plugins/python_scripts/README-bom.txt"
    "/opt/kicad/program/V5/Tears_V5_S/eeschema/plugins/python_scripts/kicad_netlist_reader.py"
    "/opt/kicad/program/V5/Tears_V5_S/eeschema/plugins/python_scripts/bom_csv_grouped_by_value.py"
    "/opt/kicad/program/V5/Tears_V5_S/eeschema/plugins/python_scripts/bom_csv_grouped_by_value_with_fp.py"
    "/opt/kicad/program/V5/Tears_V5_S/eeschema/plugins/python_scripts/bom_csv_sorted_by_ref.py"
    "/opt/kicad/program/V5/Tears_V5_S/eeschema/plugins/python_scripts/bom_html_grouped_by_value.py"
    "/opt/kicad/program/V5/Tears_V5_S/eeschema/plugins/python_scripts/bom_html_with_advanced_grouping.py"
    "/opt/kicad/program/V5/Tears_V5_S/eeschema/plugins/python_scripts/bom_sorted_by_ref.py"
    )
endif()

