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


#include "gui/widgets/ButtonSelecterWidget.hpp"

namespace gui::widgets
{
    ButtonSelecterWidget::ButtonSelecterWidget(
        QWidget* __parent,
        Qt::WindowFlags __f
    )
        : SelecterWidget(__parent, __f)
        , m_ui(new Ui::ButtonSelecterWidget())
    {
        m_ui->setupUi(this);
        setStackedWidget(m_ui->m_stacked_widget);

        QObject::connect(
            m_ui->m_next_button,
            &QPushButton::clicked,
            this,
            &widgets::SelecterWidget::next
        );

        QObject::connect(
            m_ui->m_previous_button,
            &QPushButton::clicked,
            this,
            &widgets::SelecterWidget::previous
        );
    }

    void ButtonSelecterWidget::updateButtons(int __current_index) const
    {
        m_ui->m_previous_button->setVisible(__current_index != 0);

        m_ui->m_next_button->setVisible(
            __current_index != (m_stacked_widget->count() - 1)
        );
    }
}
