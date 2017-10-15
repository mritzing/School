# CMake generated Testfile for 
# Source directory: /vagrant
# Build directory: /vagrant
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(00_Install "/usr/bin/cmake" "--build" "/vagrant" "--target" "install")
add_test(unittests "unittests")
subdirs(tests)
