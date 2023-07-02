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
        inline static const QColor computed_data_color = Qt::green;
        inline static const QColor data_color          = Qt::black;
        inline static const QColor target_data_color   = Qt::red;
    public:
        explicit Statistics(
            const full_positions_data& __data,
            double __ratio,
            const cv::Size& __size,
            QWidget* __parent = nullptr,
            Qt::WindowFlags = {}
        );

        explicit Statistics(
            QWidget* __parent = nullptr,
            Qt::WindowFlags __f = {}
        )
            : Statistics(
                full_positions_data(),
                1.,
                cv::Size(),
                __parent,
                __f
            )
        {}
    public slots:
        void set_data(
            const full_positions_data& _data,
            double __ratio,
            const cv::Size& __size
        ) const;

        void reset_data() const;
    private:
    };
}

#endif // PMSEXP_GUI_WIDGETS_PAGES_STATISTICS_HPP
