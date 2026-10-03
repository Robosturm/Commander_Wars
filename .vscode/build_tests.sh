#!/bin/bash
# Configures, builds and runs the standalone test project via the
# desktop-tests-debug preset (binaryDir build_tests/).
set -e
cd "$(dirname "$0")/.."
CMAKE=/home/robosturm/Qt/Tools/CMake/bin/cmake
CTEST=/home/robosturm/Qt/Tools/CMake/bin/ctest
if [ ! -x "$CMAKE" ]; then
    CMAKE=cmake
    CTEST=ctest
fi
"$CMAKE" --preset desktop-tests-debug
"$CMAKE" --build --preset desktop-tests-debug
"$CTEST" --test-dir build_tests --output-on-failure
