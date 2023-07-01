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


#include "gui/widgets/ListSelecterWidget.hpp"

namespace gui::widgets
{
    ListSelecterWidget::ListSelecterWidget(
        QWidget* __parent,
        Qt::WindowFlags __f
    )
        : SelecterWidget(__parent, __f)
        , m_ui(new Ui::ListSelecterWidget())
    {
        m_ui->setupUi(this);
        set_stacked_widget(m_ui->m_stacked_widget);

        QObject::connect(
            m_ui->m_scroll_down_button,
            &QToolButton::clicked,
            this,
            &ListSelecterWidget::next
        );

        QObject::connect(
            m_ui->m_scroll_up_button,
            &QToolButton::clicked,
            this,
            &ListSelecterWidget::previous
        );

        QObject::connect(
            m_ui->m_selecter,
            &QListWidget::currentRowChanged,
            this,
            &ListSelecterWidget::set_page_index
        );

        QObject::connect(
            this,
            &ListSelecterWidget::page_index_changed,
            m_ui->m_selecter,
            [&] (int __i) { m_ui->m_selecter->setCurrentRow(__i); }
        );
    }

    void ListSelecterWidget::add_selecter(
        const QIcon& __i,
        const QString& __s,
        QWidget* __w
    )
    {
        QListWidgetItem* item =
            new QListWidgetItem(__i, __s, m_ui->m_selecter);

        item->setSizeHint({m_ui->m_selecter->sizeHint().width(), 96});
        m_map[__w] = item;
    }

    void ListSelecterWidget::update_buttons(int __current_index) const
    {
        m_ui->m_scroll_down_button->setDisabled(
            __current_index == (m_ui->m_selecter->count() - 1)
        );

        m_ui->m_scroll_up_button->setDisabled(__current_index == 0);
    }

    void ListSelecterWidget::add_page(
        const QIcon& __i,
        const QString& __s,
        QWidget* __w
    )
    {
        add_selecter(__i, __s, __w);

        SelecterWidget::add_page(__w);
    }

    void ListSelecterWidget::insert_page(
        int __index,
        const QIcon& __i,
        const QString& __s,
        QWidget* __w
    )
    {
        add_selecter(__i, __s, __w);

        SelecterWidget::insert_page(__index, __w);
    }

    void ListSelecterWidget::remove_page(QWidget* __w)
    {
        QListWidgetItem* item = m_map.extract(__w).mapped();

        m_ui->m_selecter->removeItemWidget(item);
        delete item;

        SelecterWidget::remove_page(__w);
    }
}
