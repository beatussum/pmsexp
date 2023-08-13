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
        , m_stacked_layout(new StackedWidget())
    {
        QObject::connect(
            m_stacked_layout,
            &StackedWidget::currentChanged,
            this,
            &SelecterWidget::pageIndexChanged
        );

        QObject::connect(
            m_stacked_layout,
            &StackedWidget::widgetRemoved,
            this,
            &SelecterWidget::pageRemoved
        );

        QObject::connect(
            this,
            &SelecterWidget::pageAdded,
            this,
            [&] (int __i) { updateButtons(pageIndex()); }
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
            [&] (int __i) { updateButtons(pageIndex()); }
        );
    }

    void SelecterWidget::next()
    {
        setPageIndex(
            std::min(
                m_stacked_layout->count() - 1,
                m_stacked_layout->currentIndex() + 1
            )
        );
    }

    void SelecterWidget::previous()
    {
        setPageIndex(std::max(0, m_stacked_layout->currentIndex() - 1));
    }
}

