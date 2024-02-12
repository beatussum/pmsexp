# Copyright (C) 2019, Scott Furry, <scott.wl.furry@gmail.com>
# Copyright (C) 2023-2024, Mattéo Rossillol‑‑Laruelle, <beatussum@protonmail.com>
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# 1. Redistributions of source code must retain the copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
# 3. The name of the author may not be used to endorse or promote products
#    derived from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
# IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
# OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
# IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
# NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
# THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


#[========================================================================[.md:

# QCustomPlot

Finds the QCustomPlot library.

## Imported targets

This module provides the following imported targets, if found:

- `QCustomPlot::QCustomPlot`: The QCustomPlot library.

## Result variables

This will define the following variables:

- `QCustomPlot_FOUND`: True if the system has the QCustomPlot library.
- `QCustomPlot_INCLUDE_DIRS`: Include directories needed to use QCustomPlot.
- `QCustomPlot_LIBRARIES`: Libraries needed to link to QCustomPlot.

## Cache variables

The following cache variables are defined:

- `QCustomPlot_INCLUDE_DIR`: The directory containing `qcustomplot.h`.
- `QCustomPlot_LIBRARY`: The path to the QCustomPlot library.

#]========================================================================]

cmake_minimum_required(VERSION 3.25)

find_path(QCustomPlot_INCLUDE_DIR qcustomplot.h)
find_library(QCustomPlot_LIBRARY NAMES qcustomplot)

set(QCustomPlot_INCLUDE_DIRS "${QCustomPlot_INCLUDE_DIR}")
set(QCustomPlot_LIBRARIES "${QCustomPlot_LIBRARY}")

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(
    QCustomPlot
    DEFAULT_MSG
    QCustomPlot_INCLUDE_DIR
    QCustomPlot_LIBRARY
)

include(FeatureSummary)

set_package_properties(
    QCustomPlot

    PROPERTIES
        URL "https://www.qcustomplot.com/"
        DESCRIPTION "Qt C++ widget for plotting and data visualization"
)

if (QCustomPlot_FOUND AND NOT TARGET QCustomPlot::QCustomPlot)
    add_library(QCustomPlot::QCustomPlot UNKNOWN IMPORTED)

    set_target_properties(
        QCustomPlot::QCustomPlot

        PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES "${QCustomPlot_INCLUDE_DIRS}"
            IMPORTED_LOCATION "${QCustomPlot_LIBRARIES}"
    )
endif()
