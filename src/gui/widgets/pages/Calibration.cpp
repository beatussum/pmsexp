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


#include "gui/widgets/pages/Calibration.hpp"

#include <QtGui/QDoubleValidator>

namespace gui::widgets::pages
{
    Calibration::Calibration(QWidget* __parent, Qt::WindowFlags __f)
        : QWidget(__parent, __f)
        , m_ui(new Ui::Calibration())
    {
        m_ui->setupUi(this);

        m_ui->m_help_label->setText(
            tr(
                "Use the mouse to select the area to be scaled. "
                "Press \"%1\" to validate the selection and \"%2\" to delete "
                "the previous one."
            )
                .arg(QKeySequence(Qt::Key_Enter).toString())
                .arg(QKeySequence(Qt::Key_Escape).toString())
        );

        m_ui->m_line_edit->setValidator(new QDoubleValidator());

        QObject::connect(
            m_ui->m_calibration_widget,
            &gui::widgets::CalibrationWidget::measure_changed,
            this,
            &Calibration::measure_changed
        );

        QObject::connect(
            m_ui->m_line_edit,
            &QLineEdit::textChanged,
            this,

            [&] (const QString& __text) {
                emit status_changed(!__text.isEmpty());
            }
        );

        QObject::connect(
            this,
            &Calibration::measure_changed,
            m_ui->m_group_box,
            &QGroupBox::setEnabled
        );

        QObject::connect(
            this,
            &Calibration::measure_changed,
            this,

            [&] (bool __status) {
                emit status_changed(
                    __status && !m_ui->m_line_edit->text().isEmpty()
                );
            }
        );
    }

    void Calibration::set_pixmap(const QPixmap& __p)
    {
        m_ui->m_calibration_widget->setPixmap(__p);
        m_ui->m_scroll_area->updateGeometry();
    }

    void Calibration::set_pixmap(const cv::Mat& __m)
    {
        m_ui->m_calibration_widget->setPixmap(__m);
        m_ui->m_scroll_area->updateGeometry();
    }

    double Calibration::get_ratio() const
    {
        if (get_status()) {
            return (get_real_measure() / get_pixmap_measure());
        } else {
            return 1.;
        }
    }

    bool Calibration::get_status() const
    {
        return (
            m_ui->m_calibration_widget->get_status() &&
            !m_ui->m_line_edit->text().isEmpty()
        );
    }

    void Calibration::reset() const
    {
        m_ui->m_calibration_widget->reset();
        m_ui->m_line_edit->clear();
    }
}
