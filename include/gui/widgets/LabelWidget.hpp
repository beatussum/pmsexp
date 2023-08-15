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


#ifndef PMSEXP_GUI_WIDGETS_LABEL_WIDGET_HPP
#define PMSEXP_GUI_WIDGETS_LABEL_WIDGET_HPP

#include "core/core.hpp"

#include <QtWidgets/QLabel>

namespace gui::widgets
{
    class LabelWidget : public QLabel
    {
        Q_OBJECT
        Q_PROPERTY(QRect pixmapRect READ pixmapRect STORED false)

    public:
        explicit LabelWidget(
            const QPixmap&,
            QWidget* __parent = nullptr,
            Qt::WindowFlags   = {}
        );

        explicit LabelWidget(
            const cv::Mat& __m,
            QWidget* __parent   = nullptr,
            Qt::WindowFlags __f = {}
        )
            : LabelWidget(qpixmap_from_mat(__m), __parent, __f)
        {}

        explicit LabelWidget(
            QWidget* __parent   = nullptr,
            Qt::WindowFlags __f = {}
        )
            : LabelWidget(QPixmap(), __parent, __f)
        {}
    public:
        QRect pixmapRect() const;
    public slots:
        void setPixmap(const QPixmap& __p) { QLabel::setPixmap(__p); }

        void setPixmap(const cv::Mat& __m)
            { setPixmap(qpixmap_from_mat(__m)); }
    };
}

#endif // PMSEXP_GUI_WIDGETS_LABEL_WIDGET_HPP
