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


macro(compress_if_necessary)
    set(options)

    set(
        one_value_args
        COMMENT
        EXTRA_OPTIONS
        OUTPUT_DIR
        OUTPUT_VAR
        TARGET
        WORKING_DIRECTORY
    )

    set(multi_value_args FILES)

    cmake_parse_arguments(
        COMPRESS_IF_NECESSARY
        "${options}"
        "${one_value_args}"
        "${multi_value_args}"
        ${ARGN}
    )

    if (NOT DEFINED COMPRESS_IF_NECESSARY_WORKING_DIRECTORY)
        set(WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
    endif()

    find_program(gzip gzip)

    if (NOT gzip)
        message(FATAL_ERROR "`gzip` not found!")
    else()
        set("${COMPRESS_IF_NECESSARY_OUTPUT_VAR}")

        file(MAKE_DIRECTORY "${COMPRESS_IF_NECESSARY_OUTPUT_DIR}")

        foreach(file IN LISTS COMPRESS_IF_NECESSARY_FILES)
            string(PREPEND file "${WORKING_DIRECTORY}/")
            get_filename_component(file_name "${file}" NAME)
            file(SIZE "${file}" file_size)
            set(output_file "${COMPRESS_IF_NECESSARY_OUTPUT_DIR}/${file_name}")

            file(COPY "${file}" DESTINATION "${COMPRESS_IF_NECESSARY_OUTPUT_DIR}")

            if (file_size GREATER_EQUAL 4096)
                list(APPEND "${COMPRESS_IF_NECESSARY_OUTPUT_VAR}" "${output_file}.gz")

                add_custom_command(
                    OUTPUT "${output_file}.gz"
                    COMMAND ${gzip} -9nf ${COMPRESS_IF_NECESSARY_EXTRA_OPTIONS} "${output_file}"
                    WORKING_DIRECTORY "${WORKING_DIRECTORY}"
                    DEPENDS "${file}"
                    COMMENT "Compressing `${output_file}`…"
                )
            else()
                list(APPEND "${COMPRESS_IF_NECESSARY_OUTPUT_VAR}" "${output_file}")
            endif()
        endforeach()

        add_custom_target(
            "${COMPRESS_IF_NECESSARY_TARGET}"
            ALL
            DEPENDS ${${COMPRESS_IF_NECESSARY_OUTPUT_VAR}}
            COMMENT "${COMPRESS_IF_NECESSARY_COMMENT}"
        )
    endif()
endmacro()
