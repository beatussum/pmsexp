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


#include "gui/pages/Statistics.hpp"

#include <qcustomplot.h>

namespace gui::pages
{
    Statistics::Statistics(
        const full_positions_data& __data,
        QWidget* __parent,
        Qt::WindowFlags __f
    )
        : ListSelecterWidget(__parent, __f)

        , m_heading(new QCustomPlot(this))
        , m_position(new QCustomPlot(this))
        , m_trajectory(new QCustomPlot(this))
    {
        add_page(
            QIcon::fromTheme("applications-science"),
            tr("Heading"),
            m_heading
        );

        add_page(
            QIcon::fromTheme("applications-science"),
            tr("Position"),
            m_position
        );

        add_page(
            QIcon::fromTheme("applications-science"),
            tr("Trajectory"),
            m_trajectory
        );

        m_heading->addGraph();
        m_heading->legend->setVisible(true);
        m_heading->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
        m_heading->xAxis->setLabel(tr("Time (in ms)"));
        m_heading->yAxis->setLabel(tr("Heading (in rad)"));
        m_heading->graph(0)->setName(tr("Time evolution of the heading"));
        m_heading->graph(0)->setLineStyle(QCPGraph::lsNone);
        m_heading->graph(0)->setPen(QPen(Qt::red));
        m_heading->graph(0)->setScatterStyle(QCPScatterStyle::ssCross);

        m_position->plotLayout()->clear();
        m_position->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

        auto legend = new QCPLegend();

        auto top_axis = new QCPAxisRect(m_position);
        top_axis->axis(QCPAxis::atBottom)->setLabel(tr("Time (in ms)"));
        top_axis->axis(QCPAxis::atLeft)->setLabel(tr("Abscissa (in cm)"));

        top_axis->insetLayout()->addElement(
            legend,
            Qt::AlignTop | Qt::AlignRight
        );

        m_position->legend->setLayer("legend");

        auto bottom_axis = new QCPAxisRect(m_position);
        bottom_axis->axis(QCPAxis::atBottom)->setLabel(tr("Time (in ms)"));
        bottom_axis->axis(QCPAxis::atLeft)->setLabel(tr("Ordinate (in cm)"));

        m_position->plotLayout()->addElement(0, 0, top_axis);
        m_position->plotLayout()->addElement(1, 0, bottom_axis);

        auto margin_group = new QCPMarginGroup(m_position);
        top_axis->setMarginGroup(QCP::msLeft, margin_group);
        bottom_axis->setMarginGroup(QCP::msLeft, margin_group);

        auto top_graph = m_position->addGraph(
            top_axis->axis(QCPAxis::atBottom),
            top_axis->axis(QCPAxis::atLeft)
        );

        top_graph->setLineStyle(QCPGraph::lsNone);
        top_graph->setName(tr("Time evolution of the abscissa"));
        top_graph->setPen(QPen(Qt::black));
        top_graph->setScatterStyle(QCPScatterStyle::ssCross);

        auto bottom_graph = m_position->addGraph(
            bottom_axis->axis(QCPAxis::atBottom),
            bottom_axis->axis(QCPAxis::atLeft)
        );

        bottom_graph->setLineStyle(QCPGraph::lsNone);
        bottom_graph->setName(tr("Time evolution of the ordinate"));
        bottom_graph->setPen(QPen(Qt::red));
        bottom_graph->setScatterStyle(QCPScatterStyle::ssCross);

        m_trajectory->addGraph();
        m_trajectory->legend->setVisible(true);
        m_trajectory->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
        m_trajectory->xAxis->setLabel(tr("Abscissa (in cm)"));
        m_trajectory->yAxis->setLabel(tr("Ordinate (in cm)"));
        m_trajectory->graph(0)->setLineStyle(QCPGraph::lsNone);
        m_trajectory->graph(0)->setName(tr("Evolution of the position"));
        m_trajectory->graph(0)->setPen(QPen(Qt::red));
        m_trajectory->graph(0)->setScatterStyle(QCPScatterStyle::ssCross);

        set_data(__data);
    }

    void Statistics::set_data(const full_positions_data& __data) const
    {
        std::size_t size = __data.size();

        QVector<double> x(size), y(size);

        for (auto i = __data.cbegin(); i != __data.cend(); ++i) {
            x.push_back(i->first);
            y.push_back(i->second.angle);
        }

        m_heading->graph(0)->setData(x, y);
        m_heading->rescaleAxes();
        m_heading->replot();

        x.resize(0);
        y.resize(0);

        for (auto i = __data.cbegin(); i != __data.cend(); ++i) {
            x.push_back(i->first);
            y.push_back(i->second.position.x);
        }

        m_position->graph(0)->setData(x, y);
        m_position->graph(0)->rescaleAxes();

        x.resize(0);
        y.resize(0);

        for (auto i = __data.cbegin(); i != __data.cend(); ++i) {
            x.push_back(i->first);
            y.push_back(i->second.position.y);
        }

        m_position->graph(1)->setData(x, y);
        m_position->graph(1)->rescaleAxes();

        m_position->replot();

        x.resize(0);
        y.resize(0);

        for (auto i = __data.cbegin(); i != __data.cend(); ++i) {
            x.push_back(i->second.position.x);
            y.push_back(i->second.position.y);
        }

        m_trajectory->graph(0)->setData(x, y);
        m_trajectory->rescaleAxes();
        m_trajectory->replot();
    }

    void Statistics::reset_data() const
    {
        m_heading->graph(0)->data()->clear();
        m_heading->replot();

        m_position->graph(0)->data()->clear();
        m_position->graph(1)->data()->clear();
        m_position->replot();

        m_trajectory->graph(0)->data()->clear();
        m_trajectory->replot();
    }
}
