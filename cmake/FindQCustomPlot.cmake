# Attempt to locate QCustomPlot
# Once done this will define:
#
#  QCUSTOMPLOT_FOUND - system has QCustomPlot
#  QCustomPlot_INCLUDE_DIR - the include directory for QCustomPlot
#  QCustomPlot_LIBRARY - Link this to use QCustomPlot
#
# Copyright (C) 2019, Scott Furry, <scott.wl.furry@gmail.com>
# Copyright (C) 2023, Mattéo Rossillol‑‑Laruelle, <beatussum@protonmail.com>
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


find_library(QCustomPlot_LIBRARY NAMES qcustomplot qcustomplot-qt5)
find_path(QCustomPlot_INCLUDE_DIR qcustomplot.h)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(
    QCustomPlot
    DEFAULT_MSG
    QCustomPlot_LIBRARY
    QCustomPlot_INCLUDE_DIR
)

if (QCustomPlot_FOUND AND NOT TARGET QCustomPlot::QCustomPlot)
    add_library(QCustomPlot::QCustomPlot UNKNOWN IMPORTED)

    set_target_properties(
        QCustomPlot::QCustomPlot PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${QCustomPlot_INCLUDE_DIR}"
        IMPORTED_LOCATION "${QCustomPlot_LIBRARY}"
    )
endif()
