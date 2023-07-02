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


#ifndef PMSEXP_GUI_WIDGETS_SELECTER_WIDGET_HPP
#define PMSEXP_GUI_WIDGETS_SELECTER_WIDGET_HPP

#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QWidget>

namespace gui::widgets
{
    class SelecterWidget : public QWidget
    {
        Q_OBJECT

        Q_PROPERTY(
            int page_index
            READ get_page_index
            WRITE set_page_index
            NOTIFY page_index_changed
            STORED false
        )

    public:
        explicit SelecterWidget(
            QWidget* __parent = nullptr,
            Qt::WindowFlags   = {}
        );
    public:
        int get_index_of(QWidget* __w) const
            { return m_stacked_widget->indexOf(__w); }

        QWidget* get_page(int __index) const
            { return m_stacked_widget->widget(__index); }

        int get_page_index() const { return m_stacked_widget->currentIndex(); }

        virtual void set_page_index(int __i)
            { m_stacked_widget->setCurrentIndex(__i); }
    public:
        virtual void add_page(QWidget* __w)
            { emit page_added(m_stacked_widget->addWidget(__w)); }

        virtual void insert_page(int __index, QWidget* __w)
            { emit page_added(m_stacked_widget->insertWidget(__index, __w)); }

        virtual void remove_page(QWidget* __w)
            { m_stacked_widget->removeWidget(__w); }
    signals:
        void page_added(int __index);
        void page_index_changed(int __current_index);
        void page_removed(int __index);
    public slots:
        void next();
        void previous();
    protected slots:
        virtual void update_buttons(int __current_index) const = 0;
    protected:
        QStackedWidget* m_stacked_widget;
    };
}

#endif // PMSEXP_GUI_WIDGETS_SELECTER_WIDGET_HPP
