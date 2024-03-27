# Install script for directory: /opt/kicad/program/V5/Tears_V5_S

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

if(CMAKE_INSTALL_COMPONENT STREQUAL "resources" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/doc/kicad" TYPE DIRECTORY FILES "/opt/kicad/program/V5/Tears_V5_S/scripts" REGEX "/[^/]*\\.bat$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "resources" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share" TYPE DIRECTORY FILES "/opt/kicad/program/V5/Tears_V5_S/resources/linux/mime/icons")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "resources" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share" TYPE DIRECTORY FILES "/opt/kicad/program/V5/Tears_V5_S/resources/linux/mime/mime")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "resources" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share" TYPE DIRECTORY FILES "/opt/kicad/program/V5/Tears_V5_S/resources/linux/mime/applications")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "resources" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share" TYPE DIRECTORY FILES "/opt/kicad/program/V5/Tears_V5_S/build/resources/linux/appdata" FILES_MATCHING REGEX "/[^/]*appdata\\.xml$" REGEX "/[^/]*\\.in$" EXCLUDE)
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/opt/kicad/program/V5/Tears_V5_S/build/bitmaps_png/cmake_install.cmake")
  include("/opt/kicad/program/V5/Tears_V5_S/build/common/cmake_install.cmake")
  include("/opt/kicad/program/V5/Tears_V5_S/build/3d-viewer/cmake_install.cmake")
  include("/opt/kicad/program/V5/Tears_V5_S/build/cvpcb/cmake_install.cmake")
  include("/opt/kicad/program/V5/Tears_V5_S/build/eeschema/cmake_install.cmake")
  include("/opt/kicad/program/V5/Tears_V5_S/build/gerbview/cmake_install.cmake")
  include("/opt/kicad/program/V5/Tears_V5_S/build/dxflib_qcad/cmake_install.cmake")
  include("/opt/kicad/program/V5/Tears_V5_S/build/pcbnew/cmake_install.cmake")
  include("/opt/kicad/program/V5/Tears_V5_S/build/polygon/cmake_install.cmake")
  include("/opt/kicad/program/V5/Tears_V5_S/build/pagelayout_editor/cmake_install.cmake")
  include("/opt/kicad/program/V5/Tears_V5_S/build/potrace/cmake_install.cmake")
  include("/opt/kicad/program/V5/Tears_V5_S/build/bitmap2component/cmake_install.cmake")
  include("/opt/kicad/program/V5/Tears_V5_S/build/pcb_calculator/cmake_install.cmake")
  include("/opt/kicad/program/V5/Tears_V5_S/build/plugins/cmake_install.cmake")
  include("/opt/kicad/program/V5/Tears_V5_S/build/kicad/cmake_install.cmake")
  include("/opt/kicad/program/V5/Tears_V5_S/build/tools/cmake_install.cmake")
  include("/opt/kicad/program/V5/Tears_V5_S/build/utils/cmake_install.cmake")
  include("/opt/kicad/program/V5/Tears_V5_S/build/demos/cmake_install.cmake")
  include("/opt/kicad/program/V5/Tears_V5_S/build/template/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/opt/kicad/program/V5/Tears_V5_S/build/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
