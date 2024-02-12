/*
 * Copyright (C) 2023-2024 Mattéo Rossillol‑‑Laruelle <beatussum@protonmail.com>
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


#include "gui/widgets/SelectionWidget.hpp"

#include <QtGui/qevent.h>

namespace gui::widgets
{
    SelectionWidget::SelectionWidget(
        const QPixmap& __p,
        QRect __selection,
        QWidget* __parent,
        Qt::WindowFlags __f
    )
        : LabelWidget(__parent, __f)

        , m_selection(std::move(__selection))

        , m_origin()
        , m_rubber_band(QRubberBand::Rectangle, this)
    {
        setAlignment(Qt::AlignCenter);
        LabelWidget::setPixmap(__p);
    }

    void SelectionWidget::updateRubberBandGeometry()
    {
        if (m_rubber_band.isVisible()) {
            m_rubber_band.move(
                pixmapRect().topLeft() + m_selection.topLeft()
            );
        }
    }

    void SelectionWidget::keyPressEvent(QKeyEvent* __e)
    {
        switch (__e->key()) {
            case Qt::Key_Enter:
            case Qt::Key_Return:
                if (
                    m_rubber_band.isVisible() &&
                    (m_selection.size() != QSize())
                )
                {
                    emit selectionChanged(m_selection);
                }

                break;
            case Qt::Key_Escape:
                resetSelection();

                break;
            default:
                LabelWidget::keyPressEvent(__e);

                break;
        }
    }

    void SelectionWidget::mousePressEvent(QMouseEvent* __e)
    {
        QPoint origin = __e->pos();

        setFocus();

        if (pixmapRect().contains(origin)) {
            if (!m_selection.isEmpty()) {
                resetSelection();
            }

            m_rubber_band.setGeometry(
                QRect(m_origin = std::move(origin), QSize())
            );

            m_rubber_band.show();
        }

        LabelWidget::mousePressEvent(__e);
    }

    void SelectionWidget::mouseMoveEvent(QMouseEvent* __e)
    {
        QPoint bottom_right = __e->pos();
        QRect pixmap_rect   = pixmapRect();

        if (pixmap_rect.contains(bottom_right)) {
            QRect rubber_band_rect = QRect(
                m_origin,
                bottom_right
            ).normalized();

            m_selection = QRect(
                rubber_band_rect.x() - pixmap_rect.x(),
                rubber_band_rect.y() - pixmap_rect.y(),
                rubber_band_rect.width(),
                rubber_band_rect.height()
            );

            m_rubber_band.setGeometry(rubber_band_rect);
        }

        LabelWidget::mouseMoveEvent(__e);
    }

    void SelectionWidget::resizeEvent(QResizeEvent* __e)
    {
        updateRubberBandGeometry();

        LabelWidget::resizeEvent(__e);
    }

    void SelectionWidget::showEvent(QShowEvent* __e)
    {
        updateRubberBandGeometry();

        LabelWidget::showEvent(__e);
    }

    void SelectionWidget::setPixmap(const QPixmap& __p)
    {
        resetSelection();

        LabelWidget::setPixmap(__p);
    }

    void SelectionWidget::setSelection(QRect __s) noexcept
    {
        m_selection = std::move(__s);

        emit selectionChanged(m_selection);
    }

    void SelectionWidget::resetSelection() noexcept
    {
        m_selection = QRect();

        m_rubber_band.hide();

        emit selectionChanged(m_selection);
    }
}
