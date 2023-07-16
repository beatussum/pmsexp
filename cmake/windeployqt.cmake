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


find_package(Qt5 REQUIRED)
get_filename_component(Qt5_PREFIX "${Qt5_DIR}/../../.." REALPATH)

macro (windeployqt __file)
    if (NOT DEFINED WINDEPLOYQT_EXE)
        find_program(
            WINDEPLOYQT_EXE
            windeployqt
            HINTS "${Qt5_PREFIX}/bin" "${Qt5_PREFIX}/tools/qt5/bin"
            ENV PATH
            DOC "A tool to automate deployment of the Qt-related dependencies"
            REQUIRED
        )
    endif()

    if (Qt5Core_VERSION VERSION_LESS 5.14.1)
        if (CMAKE_BUILD_TYPE STREQUAL "Debug")
            set(WINDEPLOYQT_MODE "--debug")
        elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
            set(WINDEPLOYQT_MODE "--release")
        endif()
    endif()

    execute_process(
        COMMAND ${WINDEPLOYQT_EXE} ${WINDEPLOYQT_MODE} "${__file}"
        WORKING_DIRECTORY "${CMAKE_INSTALL_PREFIX}"
    )
endmacro()
