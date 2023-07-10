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

project(
    PMSExp
    VERSION 0.9.0
    DESCRIPTION "A software for recovering the position of an object in a video"
    HOMEPAGE_URL "https://github.com/beatussum/pmsexp/"
    LANGUAGES CXX
)

option(COMPRESS_DOC "Compress the documentation files if necessary" OFF)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

unset(CMAKE_CXX_FLAGS_RELEASE CACHE)

macro(add_gcc_cxx_flags __flags)
    if(CMAKE_CXX_COMPILER_ID EQUAL "GNU")
        string(APPEND CMAKE_CXX_FLAGS " ${__flags}")
    endif()
endmacro()

# Use ccache if installed

find_program(CCACHE_FOUND ccache)

if(CCACHE_FOUND)
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
    include/core/core.hpp
    include/core/types.hpp
    include/gui/MainWindow.hpp
    include/gui/widgets/ButtonSelecterWidget.hpp
    include/gui/widgets/CalibrationWidget.hpp
    include/gui/widgets/CheckWidget.hpp
    include/gui/widgets/ItemizeWidget.hpp
    include/gui/widgets/LabelWidget.hpp
    include/gui/widgets/ListSelecterWidget.hpp
    include/gui/widgets/ScrollArea.hpp
    include/gui/widgets/SelecterWidget.hpp
    include/gui/widgets/SelectionWidget.hpp
    include/gui/widgets/StackedWidget.hpp
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
    src/gui/widgets/ScrollArea.cpp
    src/gui/widgets/SelecterWidget.cpp
    src/gui/widgets/SelectionWidget.cpp
    src/gui/widgets/UploadWidget.cpp
    src/gui/widgets/pages/Calibration.cpp
    src/gui/widgets/pages/ContourSelection.cpp
    src/gui/widgets/pages/Selection.cpp
    src/gui/widgets/pages/Statistics.cpp
    src/gui/widgets/pages/Upload.cpp
)

set(PMSEXP_COMPRESSABLE_DOC_FILES ChangeLog README_fr.md README.md)
set(PMSEXP_UNCOMPRESSABLE_DOC_FILES copyright)

set(PMSEXP_LANGUAGES en fr_FR)
set(PMSEXP_TS i18n/pmsexp.ts)

foreach (language IN LISTS PMSEXP_LANGUAGES)
    list(APPEND PMSEXP_TS "i18n/pmsexp_${language}.ts")
endforeach()

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS "${CMAKE_CURRENT_SOURCE_DIR}/include/gui")

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")

find_package(Qt5 COMPONENTS Core Concurrent Gui LinguistTools Widgets REQUIRED)

find_package(OpenCV REQUIRED)
include_directories("${OpenCV_INCLUDE_DIRS}")

qt5_create_translation(
    PMSEXP_QM
    "${CMAKE_SOURCE_DIR}"
    "${PMSEXP_TS}"
    i18n/pmsexp.ts
)

add_executable(
    pmsexp
    "${PMSEXP_HPP}"
    "${PMSEXP_IPP}"
    "${PMSEXP_CPP}"
    "${PMSEXP_QM}"
)

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

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_gcc_cxx_flags(-O0)

    set(PMSEXP_I18N_DIR "${CMAKE_CURRENT_BINARY_DIR}")
    set(PMSEXP_ICON_DIR "${CMAKE_CURRENT_SOURCE_DIR}/share/icons")
    set(PMSEXP_THEME_DIR "${CMAKE_CURRENT_SOURCE_DIR}/share/icons")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_gcc_cxx_flags(-O2)
    add_gcc_cxx_flags(-Werror -Wfatal-errors)

    if (UNIX AND NOT APPLE)
        include(GNUInstallDirs)

        set(PMSEXP_BIN_DIR "${CMAKE_INSTALL_FULL_BINDIR}")
        set(PMSEXP_DATA_INSTALL_DIR "${CMAKE_INSTALL_FULL_DATADIR}")
        string(TOLOWER "${CMAKE_PROJECT_NAME}" PMSEXP_EXECUTABLE_NAME)
        set(PMSEXP_ICON_DIR "${PMSEXP_DATA_INSTALL_DIR}/icons/hicolor/256x256/apps")
        set(PMSEXP_LIB_DIR "${CMAKE_INSTALL_FULL_LIBDIR}")
        set(PMSEXP_DOC_DIR "${PMSEXP_DATA_INSTALL_DIR}/doc/${PMSEXP_EXECUTABLE_NAME}-${CMAKE_PROJECT_VERSION}")
    elseif (WIN32)
        set(PMSEXP_BIN_DIR ".")
        set(PMSEXP_DATA_INSTALL_DIR ".")
        set(PMSEXP_EXECUTABLE_NAME "${CMAKE_PROJECT_NAME}")
        set(PMSEXP_ICON_DIR "${PMSEXP_DATA_INSTALL_DIR}/${PMSEXP_EXECUTABLE_NAME}/icons")
        set(PMSEXP_LIB_DIR ".")
        set(PMSEXP_THEME_DIR "${PMSEXP_DATA_INSTALL_DIR}/${PMSEXP_EXECUTABLE_NAME}/icons")
        set(PMSEXP_DOC_DIR "${PMSEXP_DATA_INSTALL_DIR}/doc")

        find_program(
            WINDEPLOYQT_EXE
            windeployqt
            HINTS "${Qt5_PREFIX}/bin" "${Qt5_PREFIX}/tools/qt5/bin"
            ENV PATH
            DOC "A tool to automate deployment of the Qt-related dependencies."
            REQUIRED
        )

        if(Qt5Core_VERSION VERSION_LESS "5.14.1")
            set(WINDEPLOYQT_MODE "--release")
        endif()
    endif()

    set(PMSEXP_I18N_DIR "${PMSEXP_DATA_INSTALL_DIR}/${PMSEXP_EXECUTABLE_NAME}/translations")

    install(
        TARGETS pmsexp
        RUNTIME
        DESTINATION "${PMSEXP_BIN_DIR}"
        COMPONENT Runtime
    )

    if (COMPRESS_DOC)
        include(CompressIfNecessary)

        compress_if_necessary(
            FILES "${PMSEXP_COMPRESSABLE_DOC_FILES}"
            OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/doc"
            COMMENT "Compressing the documentation files…"
            TARGET compress_doc
            OUTPUT_VAR PMSEXP_PROCESSED_DOC_FILES
        )

        foreach (doc IN LISTS PMSEXP_UNCOMPRESSABLE_DOC_FILES)
            list(APPEND PMSEXP_PROCESSED_DOC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/${doc}")
        endforeach()

        install(
            FILES ${PMSEXP_PROCESSED_DOC_FILES}
            DESTINATION "${PMSEXP_DOC_DIR}"
            COMPONENT Documentation
        )
    endif()

    if (DEFINED PMSEXP_THEME_DIR)
        install(
            DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/share/icons"
            DESTINATION "${PMSEXP_THEME_DIR}"
            COMPONENT Theme
        )
    else()
        install(
            FILES "${CMAKE_CURRENT_SOURCE_DIR}/share/icons/com.github.PMSExp.png"
            DESTINATION "${PMSEXP_ICON_DIR}"
            COMPONENT Theme
        )
    endif()

    install(
        FILES ${PMSEXP_QM}
        DESTINATION "${PMSEXP_I18N_DIR}"
        COMPONENT Translations
    )

    set(CPACK_PACKAGE_NAME "${PMSEXP_EXECUTABLE_NAME}")
    set(CPACK_PACKAGE_VENDOR "Mattéo Rossillol‑‑Laruelle")
    #set(CPACK_PACKAGE_DESCRIPTION_FILE "")
    #set(CPACK_PACKAGE_ICON "")
    set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
    set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
    #set(CPACK_RESOURCE_FILE_WELCOME "")
    set(CPACK_GENERATOR "DEB;WIX")
    set(CPACK_PACKAGE_EXECUTABLES "${PMSEXP_EXECUTABLE_NAME};${CMAKE_PROJECT_NAME}")

    if (UNIX AND NOT APPLE)
        install(
            FILES "${CMAKE_CURRENT_SOURCE_DIR}/share/applications/com.github.PMSExp.desktop"
            DESTINATION "${PMSEXP_DATA_INSTALL_DIR}/applications/"
            COMPONENT Share
        )

        install(
            FILES "${CMAKE_CURRENT_SOURCE_DIR}/share/metainfo/com.github.PMSExp.metainfo.xml"
            DESTINATION "${PMSEXP_DATA_INSTALL_DIR}/metainfo/"
            COMPONENT Share
        )

        set(
            PMSEXP_DEBIAN_PACKAGE_DEPENDS
            "libqscintilla2-qt5-15"
            "libqt5core5a"
            "libqt5concurrent5"
            "libqt5gui5"
            "libqt5widgets5"
            "qttranslations5-l10n"
        )

        string(
            JOIN ", "
            CPACK_DEBIAN_PACKAGE_DEPENDS
            ${PMSEXP_DEBIAN_PACKAGE_DEPENDS}
        )

        set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Mattéo Rossillol‑‑Laruelle <beatussum@protonmail.com>")
        set(CPACK_DEBIAN_PACKAGE_SECTION "science")
        set(CPACK_DEBIAN_PACKAGE_CONTROL_STRICT_PERMISSION TRUE)
    elseif (WIN32)
        foreach (language IN LISTS PMSEXP_LANGUAGES)
            list(
                APPEND Qt5_QM
                "${Qt5_PREFIX}/share/qt5/translations/qtbase_${language}.qm"
            )
        endforeach()

        install(
            FILES "${Qt5_QM}"
            DESTINATION "${PMSEXP_I18N_DIR}"
            COMPONENT Translations
        )

        install(
            TARGETS QCustomPlot::QCustomPlot
            LIBRARY
            DESTINATION "${PMSEXP_LIB_DIR}"
            COMPONENT Runtime
        )

        install(
            SCRIPT "
                execute_process(
                    COMMAND \"${WINDEPLOYQT_EXE} ${PMSEXP_EXECUTABLE_NAME}.exe ${WINDEPLOYQT_MODE}\"
                    WORKING_DIRECTORY \${CMAKE_INSTALL_PREFIX}
                    OUTPUT_QUIET
                )
            "

            COMPONENT Runtime
        )

        set(CPACK_WIX_UPGRADE_GUID 8b7c850a-8e82-41e2-b584-b888f13e91ea)
        set(CPACK_WIX_PRODUCT_GUID 2362eeda-4124-401c-a6b2-70b35d177bf7)
        #set(CPACK_WIX_LICENSE_RTF "")
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

target_compile_definitions(
    pmsexp
    PRIVATE
    PMSEXP_I18N_DIR="${PMSEXP_I18N_DIR}"
)

if (DEFINED PMSEXP_THEME_DIR)
    target_compile_definitions(
        pmsexp
        PRIVATE
        PMSEXP_THEME_DIR="${PMSEXP_THEME_DIR}"
    )
endif()
