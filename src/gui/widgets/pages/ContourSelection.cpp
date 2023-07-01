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


#include "gui/widgets/pages/ContourSelection.hpp"

#include <opencv2/imgproc.hpp>
#include <QtGui/QPainter>

namespace gui::widgets::pages
{
    ContourSelection::ContourSelection(
        sorted_contours_type __c,
        QPixmap __p,
        QWidget* __parent,
        Qt::WindowFlags __f
    )
        : QWidget(__parent, __f)

        , m_contours(std::move(__c))
        , m_pixmap(std::move(__p))

        , m_current(m_contours.cbegin())
        , m_ui(new Ui::ContourSelection())
    {
        m_ui->setupUi(this);

        m_ui->m_contour_selection_widget->setAlignment(Qt::AlignCenter);

        m_ui->m_next_button->setEnabled(m_contours.size() > 1);
        m_ui->m_previous_button->setEnabled(m_contours.size() > 1);

        QObject::connect(
            m_ui->m_next_button,
            &QPushButton::clicked,
            this,
            &ContourSelection::next
        );

        QObject::connect(
            m_ui->m_previous_button,
            &QPushButton::clicked,
            this,
            &ContourSelection::previous
        );

        QObject::connect(
            this,
            &ContourSelection::current_changed,
            this,
            &ContourSelection::update_shown_contour
        );

        update_shown_contour();
    }

    double ContourSelection::get_current_area() const
    {
        return (m_contours.empty() ? -1. : cv::contourArea(*m_current));
    }

    void ContourSelection::update_shown_contour()
    {
        if (!m_pixmap.isNull()) {
            QPixmap pixmap = m_pixmap.copy();

            QPainter painter(&pixmap);

            painter.setPen(Qt::red);
            painter.drawPolygon(qpolygon_from_contour(*m_current));

            painter.end();

            m_ui->m_contour_area_label->setText(
                tr("Aire du contour : %1 px.").arg(get_current_area())
            );

            m_ui->m_contour_selection_widget->setPixmap(pixmap);
        } else {
            m_ui->m_contour_area_label->clear();
            m_ui->m_contour_selection_widget->clear();
        }
    }

    void ContourSelection::set_values(sorted_contours_type __c, QPixmap __p)
    {
        m_contours = std::move(__c);
        m_pixmap   = std::move(__p);
        m_current  = m_contours.cbegin();

        m_ui->m_next_button->setEnabled(m_contours.size() > 1);
        m_ui->m_previous_button->setEnabled(m_contours.size() > 1);

        emit current_changed(m_current);
    }

    void ContourSelection::next()
    {
        if (++m_current == m_contours.cend()) {
            m_current = m_contours.cbegin();
        }

        emit current_changed(m_current);
    }

    void ContourSelection::previous()
    {
        if (m_current-- == m_contours.cbegin()) {
            m_current = --m_contours.cend();
        }

        emit current_changed(m_current);
    }
}
