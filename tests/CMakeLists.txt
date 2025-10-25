# ==============================================================================
#
# Copyright (c) 2025 Devin DeLong
# SPDX-License-Identifier: BSD-3-clause
#
# Licensed under the BSD 3-Clause License.
# See the license file in the project root for full license information.
#
# ==============================================================================

set (SLICE_VIEW_TESTS "neonxx_tests")

include (FetchContent)
FetchContent_Declare (
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.8.1
)
FetchContent_MakeAvailable (Catch2)

include (CTest)
include (Catch)

file (GLOB_RECURSE
  SLICE_VIEW_TEST_SOURCES
  CONFIGURE_DEPENDS
  "*.cpp"
)

add_executable (
  ${SLICE_VIEW_TESTS}
  ${SLICE_VIEW_TEST_SOURCES}
)

target_link_libraries (
  ${SLICE_VIEW_TESTS} PRIVATE
  ${SLICE_VIEW_LIBRARY_NAME}
  Catch2::Catch2WithMain
)

add_test (
  NAME ${SLICE_VIEW_TESTS}
  COMMAND ${SLICE_VIEW_TESTS}
 )

 catch_discover_tests (${SLICE_VIEW_TESTS})
