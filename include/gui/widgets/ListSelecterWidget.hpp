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


#ifndef PMSEXP_GUI_WIDGETS_LIST_SELECTER_WIDGET_HPP
#define PMSEXP_GUI_WIDGETS_LIST_SELECTER_WIDGET_HPP

#include "ui_ListSelecterWidget.h"

#include "gui/widgets/SelecterWidget.hpp"

namespace gui::widgets
{
    class ListSelecterWidget : public SelecterWidget
    {
        Q_OBJECT

    private:
        using map_type = std::unordered_map<QWidget*, QListWidgetItem*>;
    public:
        explicit ListSelecterWidget(
            QWidget* __parent = nullptr,
            Qt::WindowFlags   = {}
        );

        virtual ~ListSelecterWidget() { delete m_ui; }
    private:
        void addSelecter(const QIcon&, const QString&, QWidget*);
    protected:
        void updateButtons(int __current_index) const override;
    public:
        void addPage(const QIcon&, const QString&, QWidget*);

        void addPage(QWidget* __w) override
            { addPage(QIcon(), QString(), __w); }

        void insertPage(int __index, const QIcon&, const QString&, QWidget*);

        void insertPage(int __index, QWidget* __w) override
            { insertPage(__index, QIcon(), QString(), __w); }

        void removePage(QWidget*) override;
    private:
        map_type                m_map;
        Ui::ListSelecterWidget* m_ui;
    };
}

#endif // PMSEXP_GUI_WIDGETS_LIST_SELECTER_WIDGET_HPP
