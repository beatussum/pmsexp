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


#ifndef PMSEXP_GUI_WIDGETS_PAGES_CALIBRATION_HPP
#define PMSEXP_GUI_WIDGETS_PAGES_CALIBRATION_HPP

#include "ui_Calibration.h"

namespace gui::widgets::pages
{
    class Calibration : public QWidget
    {
        Q_OBJECT

        Q_PROPERTY(
            bool status
            READ get_status
            RESET reset_status
            NOTIFY status_changed
            STORED false
        )

    public:
        explicit Calibration(
            QWidget* __parent = nullptr,
            Qt::WindowFlags   = {}
        );

        virtual ~Calibration() { delete m_ui; }
    public:
        CalibrationWidget* get_calibration_widget() const noexcept
            { return m_ui->m_calibration_widget; }

        double get_real_measure() const
            { return m_ui->m_line_edit->text().toDouble(); }

        double get_ratio() const;

        bool get_status() const
            { return !m_ui->m_line_edit->text().isEmpty(); }
    signals:
        void status_changed(bool __new_status);
    public slots:
        void reset_status() const;
    private:
        Ui::Calibration* m_ui;
    };
}

#endif // PMSEXP_GUI_WIDGETS_PAGES_CALIBRATION_HPP
