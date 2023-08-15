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
    SelecterWidget::SelecterWidget(QWidget* __parent, Qt::WindowFlags __f)
        : QWidget(__parent, __f)
        , m_stacked_widget(nullptr)
        , m_connections()
    {
        QObject::connect(
            this,
            &SelecterWidget::pageAdded,
            this,
            [&] { updateButtons(pageIndex()); }
        );

        QObject::connect(
            this,
            &SelecterWidget::pageIndexChanged,
            this,
            &SelecterWidget::updateButtons
        );

        QObject::connect(
            this,
            &SelecterWidget::pageRemoved,
            this,
            [&] { updateButtons(pageIndex()); }
        );
    }

    void SelecterWidget::setStackedWidget(StackedWidget* __s)
    {
        if (m_stacked_widget != nullptr) {
            for (const QMetaObject::Connection& conn : m_connections) {
                QObject::disconnect(conn);
            }
        }

        m_stacked_widget = __s;

        if (m_stacked_widget != nullptr) {
            m_connections[0] = QObject::connect(
                m_stacked_widget,
                &StackedWidget::currentChanged,
                this,
                &SelecterWidget::pageIndexChanged
            );

            m_connections[1] = QObject::connect(
                m_stacked_widget,
                &StackedWidget::widgetRemoved,
                this,
                &SelecterWidget::pageRemoved
            );
        }
    }

    void SelecterWidget::next()
    {
        if (m_stacked_widget != nullptr) {
            setPageIndex(m_stacked_widget->currentIndex() + 1);
        }
    }

    void SelecterWidget::previous()
    {
        if (m_stacked_widget != nullptr) {
            setPageIndex(m_stacked_widget->currentIndex() - 1);
        }
    }
}

