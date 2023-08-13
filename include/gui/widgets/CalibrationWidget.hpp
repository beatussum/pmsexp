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


#ifndef PMSEXP_GUI_WIDGETS_CALIBRATION_WIDGET_HPP
#define PMSEXP_GUI_WIDGETS_CALIBRATION_WIDGET_HPP

#include "gui/widgets/LabelWidget.hpp"

namespace gui::widgets
{
    class CalibrationWidget : public LabelWidget
    {
        Q_OBJECT

        Q_PROPERTY(
            double measure
            READ measure
            RESET resetMeasure
            NOTIFY measureChanged
            STORED false
        )

    public:
        explicit CalibrationWidget(
            const QPixmap&,
            QWidget* __parent = nullptr,
            Qt::WindowFlags   = {}
        );

        explicit CalibrationWidget(
            const cv::Mat& __m,
            QWidget* __parent   = nullptr,
            Qt::WindowFlags __f = {}
        )
            : CalibrationWidget(qpixmap_from_mat(__m), __parent, __f)
        {}

        explicit CalibrationWidget(
            QWidget* __parent   = nullptr,
            Qt::WindowFlags __f = {}
        )
            : CalibrationWidget(QPixmap(), __parent, __f)
        {}
    protected:
        void keyPressEvent(QKeyEvent*) override;
        void mousePressEvent(QMouseEvent*) override;
        void mouseMoveEvent(QMouseEvent*) override;
    public:
        double measure() const;
        bool status() const { return (m_end != QPoint(-1., -1.)); }
    signals:
        void measureChanged(bool __status);
    public slots:
        void reset();
        void resetMeasure();

        void setPixmap(const QPixmap&);

        void setPixmap(const cv::Mat& __m)
            { setPixmap(qpixmap_from_mat(__m)); }
    private:
        QPixmap m_pixmap;

        QPoint m_end;
        QPoint m_origin;
    };
}

#endif // PMSEXP_GUI_WIDGETS_CALIBRATION_WIDGET_HPP
