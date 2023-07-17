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


#ifndef PMSEXP_GUI_WIDGETS_PAGES_STATISTICS_HPP
#define PMSEXP_GUI_WIDGETS_PAGES_STATISTICS_HPP

#include "core/types.hpp"
#include "gui/widgets/ListSelecterWidget.hpp"

class QCustomPlot;

namespace gui::widgets::pages
{
    class Statistics : public ListSelecterWidget
    {
        Q_OBJECT

    public:
        explicit Statistics(
            const full_positions_data& __data = {},
            QWidget* __parent                 = nullptr,
            Qt::WindowFlags                   = {}
        );
    public slots:
        void set_data(const full_positions_data& _data) const;
        void reset_data() const;
    private:
        QCustomPlot* m_heading;
        QCustomPlot* m_position;
        QCustomPlot* m_trajectory;
    };
}

#endif // PMSEXP_GUI_WIDGETS_PAGES_STATISTICS_HPP
