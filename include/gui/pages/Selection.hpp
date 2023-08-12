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


#ifndef PMSEXP_GUI_PAGES_SELECTION_HPP
#define PMSEXP_GUI_PAGES_SELECTION_HPP

#include "ui_Selection.h"

namespace gui::pages
{
    class Selection : public QWidget
    {
        Q_OBJECT

        Q_PROPERTY(
            QRect selection
            READ get_selection
            WRITE set_selection
            RESET reset_selection
            NOTIFY selection_changed
        )

    public:
        explicit Selection(QWidget* __parent = nullptr, Qt::WindowFlags = {});
        virtual ~Selection() { delete m_ui; }
    public:
        QRect get_selection() const noexcept
            { return m_ui->m_selection_widget->get_selection(); }

        bool has_selection() const noexcept { return has_selection(); }

        QRect get_pixmap_rect() const
            { return m_ui->m_selection_widget->get_pixmap_rect(); }
    signals:
        void selection_changed(const QRect& __new_selection);
    public slots:
        void setPixmap(const QPixmap& __p)
            { m_ui->m_selection_widget->setPixmap(__p); }

        void setPixmap(const cv::Mat& __m)
            { m_ui->m_selection_widget->setPixmap(__m); }

        void set_selection(QRect __r) noexcept
            { m_ui->m_selection_widget->set_selection(std::move(__r)); }

        void reset_selection() noexcept
            { m_ui->m_selection_widget->reset_selection(); }
    private:
        Ui::Selection* m_ui;
    };
}

#endif // PMSEXP_GUI_PAGES_SELECTION_HPP
