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

#include "gui/widgets/StackedWidget.hpp"

namespace gui::widgets
{
    class SelecterWidget : public QWidget
    {
        Q_OBJECT

        Q_PROPERTY(
            int pageIndex
            READ pageIndex
            WRITE setPageIndex
            NOTIFY pageIndexChanged
            STORED false
        )

        Q_PROPERTY(
            StackedWidget* stackedWidget
            READ stackedWidget
            WRITE setStackedWidget
        )

    private:
        using connections_type = std::array<QMetaObject::Connection, 5>;
    public:
        explicit SelecterWidget(
            QWidget* __parent   = nullptr,
            Qt::WindowFlags __f = {}
        )
            : QWidget(__parent, __f)
            , m_stacked_widget(nullptr)
            , m_connections()
        {}

        explicit SelecterWidget(
            std::nullptr_t,
            QWidget* __parent   = nullptr,
            Qt::WindowFlags __f = {}
        )
            : SelecterWidget(__parent, __f)
        {}

        explicit SelecterWidget(
            StackedWidget* __s,
            QWidget* __parent   = nullptr,
            Qt::WindowFlags __f = {}
        )
            : SelecterWidget(__parent, __f)
        { setStackedWidget(__s); }
    public:
        const QWidget* currentPage() const
            { return m_stacked_widget->currentWidget(); }

        QWidget* currentPage()
            { return m_stacked_widget->currentWidget(); }

        int indexOf(QWidget* __w) const
            { return m_stacked_widget->indexOf(__w); }

        const QWidget* page(int __index) const
            { return m_stacked_widget->widget(__index); }

        QWidget* page(int __index)
            { return m_stacked_widget->widget(__index); }

        int pageIndex() const { return m_stacked_widget->currentIndex(); }

        virtual void setPageIndex(int __i)
            { m_stacked_widget->setCurrentIndex(__i); }

        const StackedWidget* stackedWidget() const { return m_stacked_widget; }
        StackedWidget* stackedWidget() { return m_stacked_widget; }
        void setStackedWidget(StackedWidget*);
    public:
        virtual void addPage(QWidget* __w)
            { emit pageAdded(m_stacked_widget->addWidget(__w)); }

        virtual void insertPage(int __index, QWidget* __w)
            { emit pageAdded(m_stacked_widget->insertWidget(__index, __w)); }

        virtual void removePage(QWidget* __w)
            { m_stacked_widget->removeWidget(__w); }
    signals:
        void pageAdded(int __index);
        void pageIndexChanged(int __current_index);
        void pageRemoved(int __index);
    public slots:
        void next();
        void previous();
    protected slots:
        virtual void updateButtons(int __current_index) const = 0;
    protected:
        StackedWidget* m_stacked_widget;
    private:
        connections_type m_connections;
    };
}

#endif // PMSEXP_GUI_WIDGETS_SELECTER_WIDGET_HPP
