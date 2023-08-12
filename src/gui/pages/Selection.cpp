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


#include "gui/pages/Selection.hpp"

namespace gui::pages
{
    Selection::Selection(QWidget* __parent, Qt::WindowFlags __f)
        : QWidget(__parent, __f)
        , m_ui(new Ui::Selection())
    {
        m_ui->setupUi(this);

        m_ui->m_help_label->setText(
            tr(
                "Use the mouse to select the area to be monitored. "
                "Press \"%1\" to validate the selection and \"%2\" to delete "
                "the previous one."
            )
                .arg(QKeySequence(Qt::Key_Enter).toString())
                .arg(QKeySequence(Qt::Key_Escape).toString())
        );

        QObject::connect(
            m_ui->m_selection_widget,
            &widgets::SelectionWidget::selection_changed,
            this,
            &Selection::selection_changed
        );
    }
}
