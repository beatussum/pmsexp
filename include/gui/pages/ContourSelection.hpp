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


#ifndef PMSEXP_GUI_PAGES_CONTOUR_SELECTION_HPP
#define PMSEXP_GUI_PAGES_CONTOUR_SELECTION_HPP

#include "ui_ContourSelection.h"

#include "core/core.hpp"

namespace gui::pages
{
    class ContourSelection : public QWidget
    {
        Q_OBJECT

        Q_PROPERTY(sorted_contours_type contours READ contours)
        Q_PROPERTY(QPixmap pixmap READ pixmap)
        Q_PROPERTY(contour_type current READ current NOTIFY currentChanged)
        Q_PROPERTY(double currentArea READ currentArea STORED false)

    public:
        using current_type = sorted_contours_type::const_iterator;
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
        sorted_contours_type contours() const { return m_contours; }
        QPixmap pixmap() const { return m_pixmap; }
    public:
        contour_type current() const
            { return (m_contours.empty() ? contour_type() : *m_current); }

        double currentArea() const;
    signals:
        void currentChanged(current_type __new_current);
    private slots:
        void updateShownContour();
    public slots:
        void resetValues() { setValues(sorted_contours_type(), QPixmap()); }
        void setValues(sorted_contours_type, QPixmap);

        void setValues(sorted_contours_type __c, const cv::Mat& __m)
            { setValues(std::move(__c), qpixmap_from_mat(__m)); }
    public slots:
        void next();
        void previous();
    private:
        sorted_contours_type m_contours;
        QPixmap              m_pixmap;

        current_type          m_current;
        Ui::ContourSelection* m_ui;
    };
}

#endif // PMSEXP_GUI_PAGES_CONTOUR_SELECTION_HPP
