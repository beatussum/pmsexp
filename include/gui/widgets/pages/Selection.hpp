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


#ifndef PMSEXP_GUI_WIDGETS_PAGES_SELECTION_HPP
#define PMSEXP_GUI_WIDGETS_PAGES_SELECTION_HPP

#include "ui_Selection.h"

namespace gui::widgets::pages
{
    class Selection : public QWidget
    {
        Q_OBJECT
        Q_PROPERTY(SelectionWidget* selection_widget READ get_selection_widget)

    public:
        explicit Selection(QWidget* __parent = nullptr, Qt::WindowFlags = {});
        virtual ~Selection() { delete m_ui; }
    public:
        const SelectionWidget* get_selection_widget() const noexcept
            { return m_ui->m_selection_widget; }

        SelectionWidget* get_selection_widget() noexcept
            { return m_ui->m_selection_widget; }
    private:
        Ui::Selection* m_ui;
    };
}

#endif // PMSEXP_GUI_WIDGETS_PAGES_SELECTION_HPP
