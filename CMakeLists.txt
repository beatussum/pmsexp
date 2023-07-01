# Copyright (C) 2023 Mattéo Rossillol‑‑Laruelle <beatussum@protonmail.com>
#
# This program is free software: you can redistribute it and/or modify it under
# the terms of the GNU General Public License as published by the Free Software
# Foundation, either version 3 of the License, or (at your option) any later
# version.
#
# This program is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
# FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
# details.
#
# You should have received a copy of the GNU General Public License along with
# this program. If not, see <https://www.gnu.org/licenses/>.


cmake_minimum_required(VERSION 3.25)
cmake_policy(SET CMP0048 NEW)

project(pmsexp
        VERSION 0.9.0
        DESCRIPTION ""
        HOMEPAGE_URL "https://github.com/beatussum/pms/"
        LANGUAGES CXX)

option(WITH_DOC "Build the documentation" OFF)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

unset(CMAKE_CXX_FLAGS_RELEASE CACHE)

macro(add_gcc_cxx_flags _flags)
    if(CMAKE_CXX_COMPILER_ID EQUAL "GNU")
         set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${_flags}")
    endif()
endmacro()

# Use ccache if installed

find_program(CCACHE_FOUND ccache)

if(CCACHE_FOUND)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
endif()

set(
    EXPORT_DIR
    "${CMAKE_BINARY_DIR}/exports"
    CACHE PATH
    "The path where the export header is generated"
)

if (WITH_DOC)
    set(
        DOC_DIR
        "${CMAKE_BINARY_DIR}/doc"
        CACHE PATH
        "The path where the documentation is built"
    )

    find_package(Doxygen REQUIRED)

    set(DOXYGEN_OUTPUT_DIRECTORY "${DOC_DIR}")
    set(DOXYGEN_INLINE_INHERITED_MEMB YES)
    set(DOXYGEN_ALIASES "pexample=@par Example")
    set(DOXYGEN_HIDE_UNDOC_CLASSES YES)
    set(DOXYGEN_HIDE_IN_BODY_DOCS YES)
    set(DOXYGEN_WARN_NO_PARAMDOC YES)
    set(DOXYGEN_WARN_LOGFILE "${DOC_DIR}/log")
endif()

macro(add_to_doc _headers)
    if (WITH_DOC)
        doxygen_add_docs(
            doc
            "${_headers}"
            COMMENT "Generating documentation…"
        )
    endif()
endmacro()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS FALSE)

add_gcc_cxx_flags(
    -fmessage-length=75
    -fno-gnu-keywords
    -fno-operator-names
    -funsigned-char
    -pipe
    -Wall
    -Wconversion
    -Wdouble-promotion
    -Wduplicated-branches
    -Wduplicated-cond
    -Wfloat-equal
    -Wignored-qualifiers
    -Wimplicit-fallthrough=5
    -Wmissing-field-initializers
    -Wmissing-include-dirs
    -Wno-div-by-zero
    -Wnoexcept
    -Wold-style-cast
    -Woverloaded-virtual
    -Wpedantic
    -Wsuggest-attribute=const
    -Wsuggest-attribute=noreturn
    -Wsuggest-attribute=pure
    -Wsuggest-final-methods
    -Wsuggest-final-types
    -Wsuggest-final-types
    -Wsuggest-override
    -Wsuggest-override
    -Wundef
    -Wuninitialized
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_gcc_cxx_flags(-O0)

    add_definitions(
        -DPMSEXP_SCHEMA_FILE_PATH="${CMAKE_SOURCE_DIR}/schema.json"
    )
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_gcc_cxx_flags(-O2)
    add_gcc_cxx_flags(-Werror -Wfatal-errors)
endif()

set(
    PMSEXP_HPP
    include/core/core.hpp
    include/core/types.hpp
    include/gui/MainWindow.hpp
    include/gui/widgets/ButtonSelecterWidget.hpp
    include/gui/widgets/CalibrationWidget.hpp
    include/gui/widgets/CheckWidget.hpp
    include/gui/widgets/ItemizeWidget.hpp
    include/gui/widgets/LabelWidget.hpp
    include/gui/widgets/ListSelecterWidget.hpp
    include/gui/widgets/SelecterWidget.hpp
    include/gui/widgets/SelectionWidget.hpp
    include/gui/widgets/UploadWidget.hpp
    include/gui/widgets/pages/Calibration.hpp
    include/gui/widgets/pages/ContourSelection.hpp
    include/gui/widgets/pages/Selection.hpp
    include/gui/widgets/pages/Statistics.hpp
    include/gui/widgets/pages/Upload.hpp
)

set(PMSEXP_IPP)

set(
    PMSEXP_CPP
    src/main.cpp
    src/core/core.cpp
    src/core/types.cpp
    src/gui/MainWindow.cpp
    src/gui/widgets/ButtonSelecterWidget.cpp
    src/gui/widgets/CalibrationWidget.cpp
    src/gui/widgets/CheckWidget.cpp
    src/gui/widgets/ItemizeWidget.cpp
    src/gui/widgets/LabelWidget.cpp
    src/gui/widgets/ListSelecterWidget.cpp
    src/gui/widgets/SelecterWidget.cpp
    src/gui/widgets/SelectionWidget.cpp
    src/gui/widgets/UploadWidget.cpp
    src/gui/widgets/pages/Calibration.cpp
    src/gui/widgets/pages/ContourSelection.cpp
    src/gui/widgets/pages/Selection.cpp
    src/gui/widgets/pages/Statistics.cpp
    src/gui/widgets/pages/Upload.cpp
)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS "${CMAKE_CURRENT_SOURCE_DIR}/include/gui")

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include/")

find_package(Qt5 COMPONENTS Core Concurrent Gui Widgets REQUIRED)

find_package(OpenCV REQUIRED)
include_directories("${OpenCV_INCLUDE_DIRS}")

add_executable(pmsexp "${PMSEXP_HPP}" "${PMSEXP_IPP}" "${PMSEXP_CPP}")
target_include_directories(pmsexp PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")

target_link_libraries(
    pmsexp
    "${OpenCV_LIBS}"
    qcustomplot
    Qt5::Core
    Qt5::Concurrent
    Qt5::Gui
    Qt5::Widgets
)

add_to_doc("${PMSEXP_HPP}")
