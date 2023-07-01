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


#include "gui/widgets/pages/Statistics.hpp"

#include <qcustomplot.h>

namespace gui::widgets::pages
{
    Statistics::Statistics(
        const full_positions_comp_data& __comp_data,
        const full_positions_ex_data& __ex_data,
        double __ratio,
        const cv::Size& __size,
        QWidget* __parent,
        Qt::WindowFlags __f
    )
        : ListSelecterWidget(__parent, __f)

        , m_trajectory(new QCustomPlot(this))
        , m_angular_difference(new QCustomPlot(this))
        , m_spatial_difference(new QCustomPlot(this))
    {
        add_page(
            QIcon::fromTheme("applications-science"),
            tr("Trajectoire"),
            m_trajectory
        );

        add_page(
            QIcon::fromTheme("applications-science"),
            tr("Écart en postion angulaire"),
            m_angular_difference
        );

        add_page(
            QIcon::fromTheme("applications-science"),
            tr("Écart en position spatiale"),
            m_spatial_difference
        );

        m_trajectory->addGraph();
        m_trajectory->graph(0)->setLineStyle(QCPGraph::lsNone);
        m_trajectory->graph(0)->setName(tr("Positions calculées"));
        m_trajectory->graph(0)->setPen(QPen(computed_data_color));

        m_trajectory->graph(0)->setScatterStyle(
            QCPScatterStyle(QCPScatterStyle::ssCircle, 4)
        );

        m_trajectory->addGraph();
        m_trajectory->graph(1)->setLineStyle(QCPGraph::lsNone);
        m_trajectory->graph(1)->setName(tr("Positions expérimentales"));
        m_trajectory->graph(1)->setPen(QPen(ex_data_color));

        m_trajectory->graph(1)->setScatterStyle(
            QCPScatterStyle(QCPScatterStyle::ssPlus, 4)
        );

        m_trajectory->addGraph();
        m_trajectory->graph(2)->setLineStyle(QCPGraph::lsNone);
        m_trajectory->graph(2)->setName(tr("Positions cibles"));
        m_trajectory->graph(2)->setPen(QPen(target_data_color));

        m_trajectory->graph(2)->setScatterStyle(
            QCPScatterStyle(QCPScatterStyle::ssCross, 4)
        );

        m_trajectory->legend->setVisible(true);
        m_trajectory->yAxis->setRangeReversed(true);

        set_data(__comp_data, __ex_data, __ratio, __size);
    }

    void Statistics::set_data(
        const full_positions_comp_data& __comp_data,
        const full_positions_ex_data& __ex_data,
        double __ratio,
        const cv::Size& __size
    ) const
    {
        reset_data();

        std::size_t size = __comp_data.size();
        QVector<double> x, y;

        x.reserve(size);
        y.reserve(size);

        for (auto i = __comp_data.cbegin(); i != (__comp_data.cend()); ++i) {
            x.push_back(i->second.first.position.x);
            y.push_back(i->second.first.position.y);
        }

        m_trajectory->graph(0)->setData(x, y, true);
        m_trajectory->graph(0)->rescaleAxes();

        x.resize(0);
        y.resize(0);

        for (auto i = __ex_data.cbegin(); i != (__ex_data.cend()); ++i) {
            x.push_back(i->second.position.x * __ratio);
            y.push_back(i->second.position.y * __ratio);
        }

        m_trajectory->graph(1)->setData(x, y, true);
        m_trajectory->graph(1)->rescaleAxes();

        x.resize(0);
        y.resize(0);

        for (auto i = __comp_data.cbegin(); i != (__comp_data.cend()); ++i) {
            x.push_back(i->second.second.position.x);
            y.push_back(i->second.second.position.y);
        }

        m_trajectory->graph(2)->setData(x, y, true);
        m_trajectory->graph(2)->rescaleAxes();

        m_trajectory->replot();
    }

    void Statistics::reset_data() const
    {
        for (int i = 0; i != 3; ++i) {
            m_trajectory->graph(i)->data()->clear();
        }
    }
}
