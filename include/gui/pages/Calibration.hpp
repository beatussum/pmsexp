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


#ifndef PMSEXP_GUI_PAGES_CALIBRATION_HPP
#define PMSEXP_GUI_PAGES_CALIBRATION_HPP

#include "ui_Calibration.h"

namespace gui::pages
{
    class Calibration : public QWidget
    {
        Q_OBJECT

        Q_PROPERTY(QPixmap pixmap READ pixmap WRITE setPixmap STORED false)
        Q_PROPERTY(double pixmapMeasure READ pixmapMeasure STORED false)
        Q_PROPERTY(double ratio READ ratio STORED false)
        Q_PROPERTY(double realMeasure READ realMeasure STORED false)

        Q_PROPERTY(
            bool status
            READ status
            RESET reset
            NOTIFY statusChanged
            STORED false
        )

    public:
        explicit Calibration(QWidget* __parent = nullptr, Qt::WindowFlags = {});
        virtual ~Calibration() { delete m_ui; }
    public:
        QPixmap pixmap() const
            { return m_ui->m_calibration_widget->pixmap(Qt::ReturnByValue); }

        void setPixmap(const QPixmap&);
        void setPixmap(const cv::Mat&);

        double pixmapMeasure() const
            { return m_ui->m_calibration_widget->measure(); }

        double ratio() const;

        double realMeasure() const
            { return m_ui->m_line_edit->text().toDouble(); }

        bool status() const;
    signals:
        void measureChanged(bool __status);
        void statusChanged(bool __new_status);
    public slots:
        void reset() const;
    private:
        Ui::Calibration* m_ui;
    };
}

#endif // PMSEXP_GUI_PAGES_CALIBRATION_HPP
