cmake_minimum_required(VERSION 3.5)
project(SYNTH2 CXX)

set(SRC event.hpp event.cpp 
  track.hpp track.cpp 
  midi.hpp midi.cpp 
  signal.hpp signal.cpp
  instrument.hpp instrument.cpp
  synthesizer.hpp synthesizer.cpp
  wav.hpp wav.cpp)

set(TEST_SRC unittests.cpp
  event_test.cpp
  instrument_test.cpp
  midi_test.cpp
  signal_test.cpp
  synthesizer_test.cpp
  track_test.cpp
  wav_test.cpp)

# You should not need to edit below this line
#-----------------------------------------------------------------------
#-----------------------------------------------------------------------

set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR})

# require a C++11 compiler for all targets
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(UNIX AND STRICT)
  message("-- Enabling strict compilation mode")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

add_library(synthesizer ${SRC})

add_executable(synth main.cpp)
target_link_libraries(synth synthesizer)
INSTALL(TARGETS synth DESTINATION bin)

add_executable(unittests catch.hpp ${TEST_SRC} ${SRC})
INSTALL(TARGETS unittests DESTINATION bin)

# setup testing
set(TEST_FILE_DIR "${CMAKE_SOURCE_DIR}/tests")

configure_file(${CMAKE_SOURCE_DIR}/test_config.hpp.in 
  ${CMAKE_BINARY_DIR}/test_config.hpp)

include_directories(${CMAKE_BINARY_DIR})

enable_testing()

add_test(00_Install ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target install)
add_test(unittests unittests)

add_subdirectory(tests)

# In the reference environment enable coverage on tests
if(UNIX AND NOT APPLE AND CMAKE_COMPILER_IS_GNUCXX)
  message("-- Enabling test coverage")
  set(GCC_COVERAGE_COMPILE_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage")
  set_target_properties(unittests PROPERTIES COMPILE_FLAGS ${GCC_COVERAGE_COMPILE_FLAGS} )
  target_link_libraries(unittests gcov)
  add_custom_target(coverage
    COMMAND ${CMAKE_COMMAND} -E env "ROOT=${CMAKE_CURRENT_SOURCE_DIR}"
    ${CMAKE_CURRENT_SOURCE_DIR}/scripts/coverage.sh)
endif()

# In the reference environment enable memory checking on tests
if(UNIX AND NOT APPLE AND CMAKE_COMPILER_IS_GNUCXX)
  message("-- Enabling memory checks")
  add_custom_target(memtest
    COMMAND valgrind ${CMAKE_BINARY_DIR}/unittests)
endif()


