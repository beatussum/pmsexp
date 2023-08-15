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


#include "gui/widgets/ProgressButtonSelecterWidget.hpp"

namespace gui::widgets
{
    ProgressButtonSelecterWidget::ProgressButtonSelecterWidget(
        QWidget* __parent,
        Qt::WindowFlags __f
    )
        : ButtonSelecterWidget(__parent, __f)
        , m_progress(0)
    {
        QObject::connect(
            this,
            &ProgressButtonSelecterWidget::pageAdded,
            this,
            &ProgressButtonSelecterWidget::updatePageIndex
        );

        QObject::connect(
            this,
            &ProgressButtonSelecterWidget::pageRemoved,
            this,
            &ProgressButtonSelecterWidget::updatePageIndex
        );

        QObject::connect(
            this,
            &ProgressButtonSelecterWidget::progressChanged,
            this,
            &ProgressButtonSelecterWidget::updatePageIndex
        );
    }

    void ProgressButtonSelecterWidget::updateButtons(int __current_index) const
    {
        bool is_ended =
            (__current_index == (m_stacked_widget->count() - 2));

        QPushButton* next_button = m_ui->m_next_button;

        next_button->setEnabled(m_progress > __current_index);

        next_button->setIcon(
            is_ended ?
            QIcon::fromTheme("system-run") :
            QIcon::fromTheme("go-next")
        );

        QKeySequence next_shortcut = next_button->shortcut();

        next_button->setText(is_ended ? tr("Run") : tr("Next"));
        next_button->setShortcut(next_shortcut);

        ButtonSelecterWidget::updateButtons(__current_index);
    }

    void ProgressButtonSelecterWidget::setPageIndex(int __i)
    {
        if (m_progress >= __i) {
            if (__i == (m_stacked_widget->count() - 1)) {
                emit run();
            }

            SelecterWidget::setPageIndex(__i);
        }
    }

    void ProgressButtonSelecterWidget::updatePageIndex()
    {
        int index = pageIndex();

        if (m_progress < index) {
            setPageIndex(m_progress);
        } else {
            updateButtons(index);
        }
    }
}
