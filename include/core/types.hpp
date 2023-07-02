/*
 * Copyright (C) 2023 Mattéo Rossillol‑‑Laruelle <beatussum@protonmail.com>
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


#ifndef PMSEXP_CORE_TYPES_HPP
#define PMSEXP_CORE_TYPES_HPP

#include <opencv2/core/types.hpp>

#include <map>
#include <set>
#include <vector>

struct full_position
{
    double angle       = 0.;
    cv::Point position = cv::Point(0., 0.);
};

using contour_type  = std::vector<cv::Point>;
using contours_type = std::vector<contour_type>;

class contour_compare
{
public:
    constexpr contour_compare() noexcept
        : m_standard(1.)
    {}

    contour_compare(double __standard);
public:
    bool operator()(const contour_type&, const contour_type&) const noexcept;
private:
    double m_standard;
};

using full_positions_data  = std::map<double, full_position>;
using sorted_contours_type = std::multiset<contour_type, contour_compare>;

#endif // PMSEXP_CORE_TYPES_HPP
