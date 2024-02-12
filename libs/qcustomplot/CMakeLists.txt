# Copyright (C) 2023-2024 Mattéo Rossillol‑‑Laruelle <beatussum@protonmail.com>
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

project(
    QCusotmPlot
    VERSION 2.0.1
    DESCRIPTION "Qt C++ widget for plotting and data visualization"
    HOMEPAGE_URL "https://www.qcustomplot.com/"
    LANGUAGES CXX
)

option(USE_OPENGL "Enable support for OpenGL" OFF)

set(CMAKE_AUTOMOC ON)

find_package(Qt5 REQUIRED COMPONENTS Core Gui PrintSupport Widgets)

set(QCUSTOMPLOT_HPP qcustomplot.h)
set(QCUSTOMPLOT_CPP qcustomplot.cpp)

add_library(qcustomplot STATIC "${QCUSTOMPLOT_HPP}" "${QCUSTOMPLOT_CPP}")
target_include_directories(qcustomplot PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

target_link_libraries(
    qcustomplot

    PUBLIC
        Qt5::Core
        Qt5::Gui
        Qt5::PrintSupport
        Qt5::Widgets
)

if (USE_OPENGL)
    target_compile_definitions(qcustomplot PUBLIC QCUSTOMPLOT_USE_OPENGL)
endif()

add_library(QCustomPlot::QCustomPlot ALIAS qcustomplot)
