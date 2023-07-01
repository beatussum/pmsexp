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


#ifndef PMSEXP_GUI_WIDGETS_BUTTON_SELECTER_HPP
#define PMSEXP_GUI_WIDGETS_BUTTON_SELECTER_HPP

#include "ui_ButtonSelecterWidget.h"

#include "gui/widgets/SelecterWidget.hpp"

namespace gui::widgets
{
    class ButtonSelecterWidget : public SelecterWidget
    {
        Q_OBJECT

        Q_PROPERTY(
            int progress
            READ get_progress
            WRITE set_progress
            RESET reset_progress
            NOTIFY progress_changed
        )

    public:
        explicit ButtonSelecterWidget(
            QWidget* __parent = nullptr,
            Qt::WindowFlags   = {}
        );

        virtual ~ButtonSelecterWidget() { delete m_ui; }
    protected:
        void update_buttons(int __current_index) const override;
    public:
        int get_progress() const noexcept { return m_progress; }
        void set_page_index(int) override;
    signals:
        void progress_changed(int);
        void run();
    protected slots:
        virtual void update_page_index(int);
    public slots:
        void reset_progress() { emit progress_changed(m_progress = 0); }
        void set_progress(int __p) { emit progress_changed(m_progress = __p); }
    private:
        int                       m_progress;
        Ui::ButtonSelecterWidget* m_ui;
    };
}

#endif // PMSEXP_GUI_WIDGETS_BUTTON_SELECTER_HPP
