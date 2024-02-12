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


#[========================================================================[.md:

# ImageMagick

Finds the ImageMagick executable and defines convenient functions.

## Result variables

This will define the following variables:

- `ImageMagick_FOUND`: True if the system has the ImageMagick executable.

If ImageMagick is found, the following are added:

- `ImageMagick_VERSION`: Version of the ImageMagick executable.
- `ImageMagick_VERSION_MAJOR`: Major version of the ImageMagick executable.
- `ImageMagick_VERSION_MINOT`: Minor version of the ImageMagick executable.
- `ImageMagick_VERSION_PATCH`: Patch version of the ImageMagick executable.

## Cache variables

The following cache variables are defined:

- `IMAGEMAGICK_EXECUTABLE`: The ImageMagick executable.

#]========================================================================]

cmake_minimum_required(VERSION 3.25)

find_program(
    IMAGEMAGICK_EXECUTABLE magick
    DOC "A free, open-source software suite, used for editing and manipulating digital images"
)

if (IMAGEMAGICK_EXECUTABLE)
    execute_process(
        COMMAND ${IMAGEMAGICK_EXECUTABLE} -version
        OUTPUT_VARIABLE ImageMagick_VERSION
    )

    string(
        REGEX MATCH "Version: ImageMagick ([0-9]+).([0-9]+).([0-9]+)"
        _
        "${ImageMagick_VERSION}"
    )

    set(ImageMagick_VERSION_MAJOR ${CMAKE_MATCH_1})
    set(ImageMagick_VERSION_MINOR ${CMAKE_MATCH_2})
    set(ImageMagick_VERSION_PATCH ${CMAKE_MATCH_3})

    string(
        JOIN "."
        ImageMagick_VERSION
        ${ImageMagick_VERSION_MAJOR}
        ${ImageMagick_VERSION_MINOR}
        ${ImageMagick_VERSION_PATCH}
    )
endif()

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(
    ImageMagick
    REQUIRED_VARS IMAGEMAGICK_EXECUTABLE
    VERSION_VAR ImageMagick_VERSION
)

include(FeatureSummary)

set_package_properties(
    ImageMagick

    PROPERTIES
        URL "https://www.imagemagick.org/"
        DESCRIPTION "A collection of tools and libraries for many image formats"
        PURPOSE "Allow to generate some PNG and ICO files from their corresponding SVG file"
)

macro(add_image_conversion)
    set(options)
    set(one_value_args FILE OUTPUT_FILE OUTPUT_VAR)
    set(multi_value_args ARGS)

    cmake_parse_arguments(
        ADD_IMAGE_CONVERSION
        "${options}"
        "${one_value_args}"
        "${multi_value_args}"
        ${ARGN}
    )

    add_custom_command(
        OUTPUT "${ADD_IMAGE_CONVERSION_OUTPUT_FILE}"
        COMMAND ${IMAGEMAGICK_EXECUTABLE} convert ${ADD_IMAGE_CONVERSION_ARGS} "${ADD_IMAGE_CONVERSION_FILE}" "${ADD_IMAGE_CONVERSION_OUTPUT_FILE}"
        MAIN_DEPENDENCY "${ADD_IMAGE_CONVERSION_FILE}"
        COMMENT "Generating \"${ADD_IMAGE_CONVERSION_OUTPUT_FILE}\" from \"${ADD_IMAGE_CONVERSION_FILE}\"…"
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    )

    if (DEFINED ADD_IMAGE_CONVERSION_OUTPUT_VAR)
        list(
            APPEND ${ADD_IMAGE_CONVERSION_OUTPUT_VAR}
            "${ADD_IMAGE_CONVERSION_OUTPUT_FILE}"
        )
    endif()
endmacro()
