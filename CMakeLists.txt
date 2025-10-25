# ================================================================================
#
# Copyright (c) 2025 Devin DeLong
# SPDX-License-Identifier: BSD-3-clause
#
# Licensed under the BSD 3-Clause License.
# See the license file in the project root for full license information.
#
# ================================================================================

cmake_minimum_required (VERSION 3.21)

project (slice_view
  LANGUAGES CXX
)

# Set aliases for the project directories.
set (SLICE_VIEW_LIBRARY_NAME         "dd")
set (SLICE_VIEW_ROOT_PATH            "${PROJECT_SOURCE_DIR}")
set (SLICE_VIEW_ROOT_INCLUDE_PATH    "${SLICE_VIEW_ROOT_PATH}/include")
set (SLICE_VIEW_LIBRARY_INCLUDE_PATH "${SLICE_VIEW_ROOT_INCLUDE_PATH}/${SLICE_VIEW_LIBRARY_NAME}")
set (SLICE_VIEW_TEST_PATH            "${SLICE_VIEW_ROOT_PATH}/tests")
set (SLICE_VIEW_EXAMPLES_PATH        "${SLICE_VIEW_ROOT_PATH}/examples")

# Create a variable for C++ version to use globally.
set (SLICE_VIEW_CXX_VERSION cxx_std_23)

# Library name
set (SLICE_VIEW_LIBRARY ${SLICE_VIEW_LIBRARY_NAME})

# Create header only library.
add_library (${SLICE_VIEW_LIBRARY} INTERFACE)
target_include_directories (${SLICE_VIEW_LIBRARY} INTERFACE ${SLICE_VIEW_ROOT_INCLUDE_PATH})
target_compile_features (${SLICE_VIEW_LIBRARY} INTERFACE ${SLICE_VIEW_CXX_VERSION})

# Testing
enable_testing()
add_subdirectory ("${SLICE_VIEW_TEST_PATH}")
add_subdirectory ("${SLICE_VIEW_EXAMPLES_PATH}")
