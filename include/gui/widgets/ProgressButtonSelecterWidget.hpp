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


#ifndef PMSEXP_GUI_WIDGETS_PROGRESS_BUTTON_SELECTER_HPP
#define PMSEXP_GUI_WIDGETS_PROGRESS_BUTTON_SELECTER_HPP

#include "gui/widgets/ButtonSelecterWidget.hpp"

namespace gui::widgets
{
    class ProgressButtonSelecterWidget : public ButtonSelecterWidget
    {
        Q_OBJECT

        Q_PROPERTY(
            int progress
            READ progress
            WRITE setProgress
            RESET resetProgress
            NOTIFY progressChanged
        )

    public:
        explicit ProgressButtonSelecterWidget(
            QWidget* __parent = nullptr,
            Qt::WindowFlags   = {}
        );
    protected:
        void updateButtons(int __current_index) const override;
    public:
        int progress() const noexcept { return m_progress; }
        void setPageIndex(int) override;
    signals:
        void progressChanged(int);
        void run();
    private slots:
        virtual void updatePageIndex();
    public slots:
        void resetProgress() { emit progressChanged(m_progress = 0); }
        void setProgress(int __p) { emit progressChanged(m_progress = __p); }
    private:
        int m_progress;
    };
}

#endif // PMSEXP_GUI_WIDGETS_PROGRESS_BUTTON_SELECTER_HPP
