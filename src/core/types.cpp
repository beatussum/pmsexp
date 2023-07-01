/*
 * Copyright (C) 2022-2023 Mattéo Rossillol‑‑Laruelle <beatussum@protonmail.com>
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <https://www.gnu.org/licenses/>.
 */


#include "core/core.hpp"
#include <opencv2/imgproc.hpp>

contour_compare::contour_compare(double __standard)
    : m_standard(__standard)
{
    if (m_standard == 0.) {
        throw std::invalid_argument("`contour_compare`: division by zero.");
    }
}

bool contour_compare::operator()(
    const contour_type& __l,
    const contour_type& __r
) const noexcept
{
    return
        relative_difference(cv::contourArea(__l), m_standard) <
        relative_difference(cv::contourArea(__r), m_standard);
}
