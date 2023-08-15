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


#include "gui/widgets/CalibrationWidget.hpp"

#include <QtGui/qevent.h>
#include <QtGui/QPainter>

namespace gui::widgets
{
    CalibrationWidget::CalibrationWidget(
        const QPixmap& __p,
        QWidget* __parent,
        Qt::WindowFlags __f
    )
        : LabelWidget(__parent, __f)

        , m_pixmap(__p)

        , m_end(-1., -1.)
        , m_origin(-1., -1.)
    {
        setAlignment(Qt::AlignCenter);
        LabelWidget::setPixmap(__p);
    }

    void CalibrationWidget::keyPressEvent(QKeyEvent* __e)
    {
        switch (__e->key()) {
            case Qt::Key_Enter:
            case Qt::Key_Return:
                if (m_end != QPoint(-1., -1.)) {
                    emit measureChanged(true);
                }

                break;
            case Qt::Key_Escape:
                resetMeasure();

                break;
            default:
                LabelWidget::keyPressEvent(__e);

                break;
        }
    }

    void CalibrationWidget::mousePressEvent(QMouseEvent* __e)
    {
        m_end    = QPoint(-1., -1.);
        m_origin = (__e->pos() - pixmapRect().topLeft());

        LabelWidget::setPixmap(m_pixmap);
        setFocus();

        emit measureChanged(false);

        LabelWidget::mousePressEvent(__e);
    }

    void CalibrationWidget::mouseMoveEvent(QMouseEvent* __e)
    {
        if (!m_pixmap.isNull() && (m_origin != QPoint(-1., -1.))) {
            QPixmap pixmap = m_pixmap.copy();
            QPainter painter(&pixmap);

            m_end = (__e->pos() - pixmapRect().topLeft());

            painter.setPen(Qt::red);
            painter.drawLine(m_origin, m_end);
            painter.end();

            LabelWidget::setPixmap(pixmap);
        }

        LabelWidget::mousePressEvent(__e);
    }

    double CalibrationWidget::measure() const
    {
        if (m_end != QPoint(-1., -1.)) {
            QPoint diff = (m_end - m_origin);

            return std::sqrt(std::pow(diff.x(), 2.) + std::pow(diff.y(), 2.));
        } else {
            return 0.;
        }
    }

    void CalibrationWidget::reset()
    {
        m_pixmap = QPixmap();

        resetMeasure();
    }

    void CalibrationWidget::resetMeasure()
    {
        m_end    = QPoint(-1., -1.);
        m_origin = QPoint(-1., -1.);

        LabelWidget::setPixmap(m_pixmap);

        emit measureChanged(false);
    }

    void CalibrationWidget::setPixmap(const QPixmap& __p)
    {
        resetMeasure();

        LabelWidget::setPixmap(m_pixmap = __p);
    }
}
