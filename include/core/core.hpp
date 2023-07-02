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


#ifndef PMSEXP_CORE_CORE_HPP
#define PMSEXP_CORE_CORE_HPP

#include "core/types.hpp"

#include <QtCore/QRect>

class QPixmap;
class QPolygon;

sorted_contours_type contours_from_mat(
    const cv::Mat& __frame,
    const cv::Rect& __roi,
    double __standard
);

inline sorted_contours_type contours_from_mat(
    const cv::Mat& __frame,
    const cv::Rect& __roi
)
    { return contours_from_mat(__frame, __roi, __roi.area()); }

full_position full_position_from_contour(contour_type);

QPixmap qpixmap_from_mat(const cv::Mat&);
QPolygon qpolygon_from_contour(contour_type);

inline cv::Rect rect_from_qrect(const QRect& __r)
    { return cv::Rect(__r.x(), __r.y(), __r.width(), __r.height()); }

inline double relative_difference(double __of, double __from)
    { return std::abs((__of - __from) / __from); }

#endif // PMSEXP_CORE_CORE_HPP
