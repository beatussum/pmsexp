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


#ifndef PMSEXP_GUI_WIDGETS_PAGES_CONTOUR_SELECTION_HPP
#define PMSEXP_GUI_WIDGETS_PAGES_CONTOUR_SELECTION_HPP

#include "ui_ContourSelection.h"

#include "core/core.hpp"

namespace gui::widgets::pages
{
    class ContourSelection : public QWidget
    {
        Q_OBJECT

        Q_PROPERTY(
            sorted_contours_type contours
            READ get_contours
            RESET reset_contours
        )

        Q_PROPERTY(QPixmap pixmap READ get_pixmap)

        Q_PROPERTY(
            contour_type current
            READ get_current
            NOTIFY current_changed
            STORED false
        )

        Q_PROPERTY(double current_area READ get_current_area STORED false)

    public:
        explicit ContourSelection(
            sorted_contours_type,
            QPixmap,
            QWidget* __parent = nullptr,
            Qt::WindowFlags   = {}
        );

        explicit ContourSelection(
            sorted_contours_type __c,
            const cv::Mat& __m,
            QWidget* __parent   = nullptr,
            Qt::WindowFlags __f = {}
        )
            : ContourSelection(
                std::move(__c),
                qpixmap_from_mat(__m),
                __parent,
                __f
            )
        {}

        explicit ContourSelection(
            QWidget* __parent   = nullptr,
            Qt::WindowFlags __f = {}
        )
            : ContourSelection(
                sorted_contours_type(),
                QPixmap(),
                __parent,
                __f
            )
        {}

        virtual ~ContourSelection() { delete m_ui; }
    public:
        sorted_contours_type get_contours() const { return m_contours; }
        QPixmap get_pixmap() const { return m_pixmap; }
    public:
        contour_type get_current() const
            { return (m_contours.empty() ? contour_type() : *m_current); }

        double get_current_area() const;
    signals:
        void current_changed(
            sorted_contours_type::const_iterator __new_current
        );
    private slots:
        void update_shown_contour();
    public slots:
        void reset_contours()
            { set_values(sorted_contours_type(), QPixmap()); }

        void set_values(sorted_contours_type, QPixmap);

        void set_values(sorted_contours_type __c, const cv::Mat& __m)
            { set_values(std::move(__c), qpixmap_from_mat(__m)); }
    public slots:
        void next();
        void previous();
    private:
        sorted_contours_type m_contours;
        QPixmap              m_pixmap;

        sorted_contours_type::const_iterator m_current;
        Ui::ContourSelection*                m_ui;
    };
}

#endif // PMSEXP_GUI_WIDGETS_PAGES_CONTOUR_SELECTION_HPP
