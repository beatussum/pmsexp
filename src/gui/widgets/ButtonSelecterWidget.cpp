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
        , m_progress(0)
    {
        m_ui->setupUi(this);
        set_stacked_widget(m_ui->m_stacked_widget);

        QObject::connect(
            m_ui->m_next_button,
            &QPushButton::clicked,
            this,
            &SelecterWidget::next
        );

        QObject::connect(
            m_ui->m_previous_button,
            &QPushButton::clicked,
            this,
            &SelecterWidget::previous
        );

        QObject::connect(
            this,
            &ButtonSelecterWidget::page_added,
            this,
            &ButtonSelecterWidget::update_page_index
        );

        QObject::connect(
            this,
            &ButtonSelecterWidget::page_removed,
            this,
            &ButtonSelecterWidget::update_page_index
        );

        QObject::connect(
            this,
            &ButtonSelecterWidget::progress_changed,
            this,
            &ButtonSelecterWidget::update_page_index
        );
    }

    void ButtonSelecterWidget::update_buttons(int __current_index) const
    {
        QPushButton* next_button = m_ui->m_next_button;

        bool is_ended =
            (__current_index == (m_ui->m_stacked_widget->count() - 2));

        m_ui->m_previous_button->setVisible(__current_index != 0);

        next_button->setIcon(
            is_ended ?
            QIcon::fromTheme("system-run") :
            QIcon::fromTheme("go-next")
        );

        QKeySequence next_shortcut = m_ui->m_next_button->shortcut();

        next_button->setEnabled(m_progress > __current_index);

        next_button->setVisible(
            __current_index != (m_ui->m_stacked_widget->count() - 1)
        );

        next_button->setText(is_ended ? tr("Run") : tr("Next"));
        next_button->setShortcut(next_shortcut);
    }

    void ButtonSelecterWidget::set_page_index(int __i)
    {
        if (m_progress >= __i) {
            SelecterWidget::set_page_index(__i);

            if (__i == (m_ui->m_stacked_widget->count() - 1)) {
                emit run();
            }
        }
    }

    void ButtonSelecterWidget::update_page_index(int __p)
    {
        int index = get_page_index();

        if (__p < index) {
            set_page_index(__p);
        } else {
            update_buttons(index);
        }
    }
}
