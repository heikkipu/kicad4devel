# CMake generated Testfile for 
# Source directory: /opt/kicad/program/master/Tears_Master/qa/tests
# Build directory: /opt/kicad/program/master/Tears_Master/build/qa/tests
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test([=[qa_python]=] "/usr/bin/python3" "-m" "pytest" "/opt/kicad/program/master/Tears_Master/qa/tests/pcbnewswig")
set_tests_properties([=[qa_python]=] PROPERTIES  ENVIRONMENT "PATH=/opt/kicad/program/master/Tears_Master/build/kicad/:/opt/kicad/program/master/Tears_Master/build/common/:/opt/kicad/program/master/Tears_Master/build/api/:/opt/kicad/program/master/Tears_Master/build/common/gal/:/opt/kicad/program/master/Tears_Master/build/pcbnew/:/opt/kicad/program/master/Tears_Master/build/eeschema/:/opt/kicad/program/master/Tears_Master/build/cvpcb/::/home/heikki/.local/bin:/home/heikki/bin:/usr/lib64/qt-3.3/bin:/usr/local/bin:/usr/bin:/bin:/usr/local/sbin:/usr/sbin:/sbin:/var/lib/snapd/snap/bin;PYTHONPATH=/opt/kicad/program/master/Tears_Master/build/pcbnew;;" WORKING_DIRECTORY "/opt/kicad/program/master/Tears_Master/qa/tests" _BACKTRACE_TRIPLES "/opt/kicad/program/master/Tears_Master/qa/tests/CMakeLists.txt;86;add_test;/opt/kicad/program/master/Tears_Master/qa/tests/CMakeLists.txt;0;")
add_test([=[qa_cli]=] "/usr/bin/python3" "-m" "pytest" "/opt/kicad/program/master/Tears_Master/qa/tests/cli")
set_tests_properties([=[qa_cli]=] PROPERTIES  ENVIRONMENT "PATH=/opt/kicad/program/master/Tears_Master/build/kicad/:/opt/kicad/program/master/Tears_Master/build/common/:/opt/kicad/program/master/Tears_Master/build/api/:/opt/kicad/program/master/Tears_Master/build/common/gal/:/opt/kicad/program/master/Tears_Master/build/pcbnew/:/opt/kicad/program/master/Tears_Master/build/eeschema/:/opt/kicad/program/master/Tears_Master/build/cvpcb/::/home/heikki/.local/bin:/home/heikki/bin:/usr/lib64/qt-3.3/bin:/usr/local/bin:/usr/bin:/bin:/usr/local/sbin:/usr/sbin:/sbin:/var/lib/snapd/snap/bin;KICAD_RUN_FROM_BUILD_DIR=1;PYTHONPATH=/opt/kicad/program/master/Tears_Master/build/pcbnew;;" WORKING_DIRECTORY "/opt/kicad/program/master/Tears_Master/qa/tests" _BACKTRACE_TRIPLES "/opt/kicad/program/master/Tears_Master/qa/tests/CMakeLists.txt;97;add_test;/opt/kicad/program/master/Tears_Master/qa/tests/CMakeLists.txt;0;")
subdirs("api")
subdirs("common")
subdirs("gerbview")
subdirs("eeschema")
subdirs("libs")
subdirs("pcbnew")
subdirs("spice")
