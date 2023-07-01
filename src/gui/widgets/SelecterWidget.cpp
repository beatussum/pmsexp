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


#include "gui/widgets/SelecterWidget.hpp"

namespace gui::widgets
{
    SelecterWidget::SelecterWidget(
        QStackedWidget* __stacked_widget,
        QWidget* __parent,
        Qt::WindowFlags __f
    )
        : QWidget(__parent, __f)
        , m_stacked_widget(__stacked_widget)
    {
        __set_stacked_widget(m_stacked_widget);

        QObject::connect(
            this,
            &SelecterWidget::page_added,
            this,
            [&] (int __i) { update_buttons(get_page_index()); }
        );

        QObject::connect(
            this,
            &SelecterWidget::page_index_changed,
            this,
            &SelecterWidget::update_buttons
        );

        QObject::connect(
            this,
            &SelecterWidget::page_removed,
            this,
            [&] (int __i) { update_buttons(get_page_index()); }
        );
    }

    void SelecterWidget::__set_stacked_widget(QStackedWidget* __s) const
    {
        if (__s != nullptr) {
            QObject::connect(
                __s,
                &QStackedWidget::currentChanged,
                this,
                &SelecterWidget::page_index_changed
            );

            QObject::connect(
                __s,
                &QStackedWidget::widgetRemoved,
                this,
                &SelecterWidget::page_removed
            );
        }
    }

    void SelecterWidget::next()
    {
        set_page_index(
            std::min(
                m_stacked_widget->count() - 1,
                m_stacked_widget->currentIndex() + 1
            )
        );
    }

    void SelecterWidget::previous()
    {
        set_page_index(std::max(0, m_stacked_widget->currentIndex() - 1));
    }
}

