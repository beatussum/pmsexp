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


#include "core/core.hpp"

#include <opencv2/imgproc.hpp>
#include <QtGui/QPixmap>

sorted_contours_type contours_from_mat(
    const cv::Mat& __frame,
    const cv::Rect& __roi,
    double __standard
)
{
    sorted_contours_type ret((contour_compare(__standard)));

    cv::Mat split_first_frame[3];

    cv::split(cv::Mat(__frame, __roi), split_first_frame);

    cv::Mat binary_frame;

    cv::threshold(
        std::move(split_first_frame[2]),
        binary_frame,
        -1.,
        255.,
        cv::THRESH_BINARY | cv::THRESH_OTSU
    );

    contours_type contours;

    cv::findContours(
        std::move(binary_frame),
        contours,
        cv::RETR_LIST,
        cv::CHAIN_APPROX_NONE,
        __roi.tl()
    );

    for (contour_type c : contours) {
        ret.insert(std::move(c));
    }

    return ret;
}

full_position full_position_from_contour(contour_type __c)
{
    cv::Mat data(__c.size(), 2, CV_64F);

    for (int i = 0; i != data.rows; ++i) {
        data.at<double>(i, 0) = __c[i].x;
        data.at<double>(i, 1) = __c[i].y;
    }

    cv::PCA pca(data, cv::Mat(), cv::PCA::DATA_AS_ROW);
    cv::Point position(pca.mean.at<double>(0, 0), pca.mean.at<double>(0, 1));

    double angle = std::atan2(
        pca.eigenvectors.at<double>(0, 1),
        pca.eigenvectors.at<double>(0, 0)
    );

    return {angle, position};
}

QPixmap qpixmap_from_mat(const cv::Mat& __m)
{
    return QPixmap::fromImage(
        QImage(
            __m.data,
            __m.cols,
            __m.rows,
            QImage::Format_BGR888
        )
    );
}

QPolygon qpolygon_from_contour(contour_type __c)
{
    QVector<QPoint> vec;

    for (const cv::Point& p : __c) {
        vec.push_back(QPoint(p.x, p.y));
    }

    return vec;
}
