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

set(LINUX (UNIX AND NOT APPLE))

project(
    PMSExp
    VERSION 0.9.0
    DESCRIPTION "A software for recovering the position of an object in a video"
    HOMEPAGE_URL "https://github.com/beatussum/pmsexp/"
    LANGUAGES CXX
)

option(WITH_QCUSTOMPLOT "Use Built-in QCustomPlot" OFF)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

macro(add_gcc_cxx_flags __flags)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        foreach (flag IN LISTS __flags)
            string(APPEND CMAKE_CXX_FLAGS " ${__flags}")
        endforeach()
    endif()
endmacro()

# Use ccache if installed

find_program(CCACHE_FOUND ccache)

if (CCACHE_FOUND)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
endif()

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

set(
    PMSEXP_HPP
    core/core.hpp
    core/types.hpp
    gui/MainWindow.hpp
    gui/widgets/ButtonSelecterWidget.hpp
    gui/widgets/CalibrationWidget.hpp
    gui/widgets/CheckWidget.hpp
    gui/widgets/ItemizeWidget.hpp
    gui/widgets/LabelWidget.hpp
    gui/widgets/ListSelecterWidget.hpp
    gui/widgets/ScrollArea.hpp
    gui/widgets/SelecterWidget.hpp
    gui/widgets/SelectionWidget.hpp
    gui/widgets/StackedWidget.hpp
    gui/widgets/UploadWidget.hpp
    gui/widgets/pages/Calibration.hpp
    gui/widgets/pages/ContourSelection.hpp
    gui/widgets/pages/Selection.hpp
    gui/widgets/pages/Statistics.hpp
    gui/widgets/pages/Upload.hpp
)

list(TRANSFORM PMSEXP_HPP PREPEND "include/")

set(
    PMSEXP_CPP
    main.cpp
    core/core.cpp
    core/types.cpp
    gui/MainWindow.cpp
    gui/widgets/ButtonSelecterWidget.cpp
    gui/widgets/CalibrationWidget.cpp
    gui/widgets/CheckWidget.cpp
    gui/widgets/ItemizeWidget.cpp
    gui/widgets/LabelWidget.cpp
    gui/widgets/ListSelecterWidget.cpp
    gui/widgets/ScrollArea.cpp
    gui/widgets/SelecterWidget.cpp
    gui/widgets/SelectionWidget.cpp
    gui/widgets/UploadWidget.cpp
    gui/widgets/pages/Calibration.cpp
    gui/widgets/pages/ContourSelection.cpp
    gui/widgets/pages/Selection.cpp
    gui/widgets/pages/Statistics.cpp
    gui/widgets/pages/Upload.cpp
)

list(TRANSFORM PMSEXP_CPP PREPEND "src/")

set(PMSEXP_DOC ChangeLog copyright README_fr.md README.md)
list(TRANSFORM PMSEXP_DOC PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/")

set(PMSEXP_LANGUAGES en fr_FR)
set(PMSEXP_TS i18n/pmsexp.ts)

foreach (language IN LISTS PMSEXP_LANGUAGES)
    list(APPEND PMSEXP_TS "i18n/pmsexp_${language}.ts")
endforeach()

set(
    PMSEXP_THEME_ICONS
    application-exit.svg
    go-next.svg
    go-previous.svg
    system-run.svg
    view-refresh.svg
)

list(
    TRANSFORM PMSEXP_THEME_ICONS
    PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/share/icons/"
)

set(CMAKE_AUTOMOC TRUE)
set(CMAKE_AUTOUIC TRUE)
set(CMAKE_AUTOUIC_SEARCH_PATHS include/gui)

include_directories(include)

find_package(OpenCV REQUIRED)
include_directories("${OpenCV_INCLUDE_DIRS}")

if (WITH_QCUSTOMPLOT)
    add_subdirectory(libs/qcustomplot)
else()
    find_package(QCustomPlot REQUIRED)
endif()

find_package(Qt5 COMPONENTS Core Concurrent Gui LinguistTools Widgets REQUIRED)
get_filename_component(Qt5_PREFIX "${Qt5_DIR}/../../.." REALPATH)

qt5_create_translation(
    PMSEXP_QM
    include src "${PMSEXP_TS}"
    OPTIONS -no-obsolete
)

add_executable(
    pmsexp
    "${PMSEXP_HPP}"
    "${PMSEXP_IPP}"
    "${PMSEXP_CPP}"
    "${PMSEXP_QM}"
)

if (WIN32)
    set_target_properties(pmsexp PROPERTIES WIN32_EXECUTABLE TRUE)
endif()

target_include_directories(pmsexp PRIVATE "${CMAKE_CURRENT_BINARY_DIR}")

target_link_libraries(
    pmsexp
    "${OpenCV_LIBS}"
    QCustomPlot::QCustomPlot
    Qt5::Core
    Qt5::Concurrent
    Qt5::Gui
    Qt5::Widgets
)

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

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(PMSEXP_I18N_DIR "${CMAKE_CURRENT_BINARY_DIR}")
    set(PMSEXP_ICON_DIR "${CMAKE_CURRENT_SOURCE_DIR}/share/icons")
    set(PMSEXP_THEME_DIR "${CMAKE_CURRENT_SOURCE_DIR}/share/icons")

    if (WIN32)
        windeployqt("$<TARGET_FILE:pmsexp>")
    endif()
elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
    if (LINUX)
        include(GNUInstallDirs)

        set(PMSEXP_BIN_DIR "${CMAKE_INSTALL_FULL_BINDIR}")
        set(PMSEXP_DATA_INSTALL_DIR "${CMAKE_INSTALL_FULL_DATADIR}")
        string(TOLOWER "${CMAKE_PROJECT_NAME}" PMSEXP_EXECUTABLE_NAME)
        set(PMSEXP_I18N_DIR "${PMSEXP_DATA_INSTALL_DIR}/${PMSEXP_EXECUTABLE_NAME}/translations")
        set(PMSEXP_ICON_DIR "${PMSEXP_DATA_INSTALL_DIR}/icons/hicolor/256x256/apps")
        set(PMSEXP_DOC_DIR "${PMSEXP_DATA_INSTALL_DIR}/doc/${PMSEXP_EXECUTABLE_NAME}-${CMAKE_PROJECT_VERSION}")
    elseif (WIN32)
        set(PMSEXP_BIN_DIR ".")
        set(PMSEXP_DATA_INSTALL_DIR ".")
        set(PMSEXP_EXECUTABLE_NAME "${CMAKE_PROJECT_NAME}")
        set(PMSEXP_I18N_DIR "${PMSEXP_DATA_INSTALL_DIR}/translations")
        set(PMSEXP_ICON_DIR "${PMSEXP_DATA_INSTALL_DIR}/icons")
        set(PMSEXP_THEME_DIR "${PMSEXP_DATA_INSTALL_DIR}/icons")
        set(PMSEXP_DOC_DIR "${PMSEXP_DATA_INSTALL_DIR}/doc")
    endif()

    install(
        FILES ${PMSEXP_DOC}
        DESTINATION "${PMSEXP_DOC_DIR}"
        COMPONENT Documentation
    )

    if (WITH_QCUSTOMPLOT)
        install(
            FILES "${CMAKE_CURRENT_SOURCE_DIR}/libs/qcustomplot/changelog.txt"
            DESTINATION "${PMSEXP_DOC_DIR}/QCustomPlot"
            COMPONENT Documentation
        )
    endif()

    install(
        TARGETS pmsexp
        RUNTIME
        DESTINATION "${PMSEXP_BIN_DIR}"
        COMPONENT Runtime
    )

    install(
        FILES "${CMAKE_CURRENT_SOURCE_DIR}/share/icons/com.github.PMSExp.png"
        DESTINATION "${PMSEXP_ICON_DIR}"
        COMPONENT Theme
    )

    if (DEFINED PMSEXP_THEME_DIR)
        install(
            FILES ${PMSEXP_THEME_ICONS}
            DESTINATION "${PMSEXP_THEME_DIR}"
            COMPONENT Theme
        )
    endif()

    install(
        FILES ${PMSEXP_QM}
        DESTINATION "${PMSEXP_I18N_DIR}"
        COMPONENT Translations
    )

    set(CPACK_COMPONENT_RUNTIME_REQUIRED TRUE)
    set(CPACK_PACKAGE_NAME "${PMSEXP_EXECUTABLE_NAME}")
    #set(CPACK_PACKAGE_DESCRIPTION_FILE "")
    set(CPACK_PACKAGE_ICON "${CMAKE_CURRENT_SOURCE_DIR}/share/icons/com.github.pmsexp.png")
    set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
    #set(CPACK_RESOURCE_FILE_WELCOME "")
    set(CPACK_GENERATOR "WIX")
    set(CPACK_PACKAGE_EXECUTABLES "${PMSEXP_EXECUTABLE_NAME};${CMAKE_PROJECT_NAME}")

    if (LINUX)
        install(
            FILES "${CMAKE_CURRENT_SOURCE_DIR}/share/applications/com.github.PMSExp.desktop"
            DESTINATION "${PMSEXP_DATA_INSTALL_DIR}/applications"
            COMPONENT Share
        )

        install(
            FILES "${CMAKE_CURRENT_SOURCE_DIR}/share/metainfo/com.github.PMSExp.metainfo.xml"
            DESTINATION "${PMSEXP_DATA_INSTALL_DIR}/metainfo"
            COMPONENT Share
        )

        set(CPACK_COMPONENT_SHARE_REQUIRED TRUE)
        set(CPACK_PACKAGE_VENDOR "Mattéo Rossillol‑‑Laruelle")
        set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
    elseif (WIN32)
        install(
            FILES "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE"
            DESTINATION "${PMSEXP_DOC_DIR}"
            COMPONENT Documentation
        )

        install(
            FILES "${CMAKE_CURRENT_SOURCE_DIR}/libs/qcustomplot/GPL.txt"
            DESTINATION "${PMSEXP_DOC_DIR}/QCustomPlot"
            COMPONENT Documentation
        )

        install(
            CODE "windeployqt(\"\${CMAKE_INSTALL_PREFIX}/${PMSEXP_EXECUTABLE_NAME}.exe\")"
            COMPONENT Runtime
        )

        set(CPACK_COMPONENT_THEME_REQUIRED TRUE)
        set(CPACK_PACKAGE_INSTALL_DIRECTORY "${CMAKE_PROJECT_NAME}")
        set(CPACK_PACKAGE_VENDOR "Mattéo Rossillol--Laruelle")
        set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/distrib/windows/LICENSE.rtf")

        set(CPACK_WIX_UPGRADE_GUID 8b7c850a-8e82-41e2-b584-b888f13e91ea)
        set(CPACK_WIX_PRODUCT_GUID 2362eeda-4124-401c-a6b2-70b35d177bf7)
        #set(CPACK_WIX_PRODUCT_ICON "")
        #set(CPACK_WIX_UI_BANNER "")
        #set(CPACK_WIX_UI_DIALOG "")
        #set(CPACK_WIX_CULTURES "")
    endif()

    include(CPack)
endif()

set_target_properties(
    pmsexp
    PROPERTIES
    OUTPUT_NAME "${PMSEXP_EXECUTABLE_NAME}"
)

configure_file(include/config.hpp.in "${CMAKE_CURRENT_BINARY_DIR}/config.hpp")
