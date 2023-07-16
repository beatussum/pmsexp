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


find_program(
    MAGICK_EXE magick
    DOC "A free, open-source software suite, used for editing and manipulating digital images"
    REQUIRED
)

function(add_image_conversion)
    set(options)
    set(one_value_args FILE OUTPUT_FILE)
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
        COMMAND ${MAGICK_EXE} convert ${ADD_IMAGE_CONVERSION_ARGS} "${ADD_IMAGE_CONVERSION_FILE}" "${ADD_IMAGE_CONVERSION_OUTPUT_FILE}"
        MAIN_DEPENDENCY "${ADD_IMAGE_CONVERSION_FILE}"
        COMMENT "Generating \"${ADD_IMAGE_CONVERSION_OUTPUT_FILE}\" from \"${ADD_IMAGE_CONVERSION_FILE}\"…"
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    )
endfunction()
