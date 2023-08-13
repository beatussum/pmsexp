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


#ifndef PMSEXP_GUI_WIDGETS_SELECTION_WIDGET_HPP
#define PMSEXP_GUI_WIDGETS_SELECTION_WIDGET_HPP

#include "gui/widgets/LabelWidget.hpp"

#include <QtWidgets/QRubberBand>

namespace gui::widgets
{
    class SelectionWidget : public LabelWidget
    {
        Q_OBJECT

        Q_PROPERTY(
            QRect selection
            READ selection
            WRITE setSelection
            RESET resetSelection
            NOTIFY selectionChanged
        )

    public:
        explicit SelectionWidget(
            const QPixmap&,
            QRect __selection,
            QWidget* __parent = nullptr,
            Qt::WindowFlags   = {}
        );

        explicit SelectionWidget(
            const cv::Mat& __m,
            QRect __selection,
            QWidget* __parent   = nullptr,
            Qt::WindowFlags __f = {}
        )
            : SelectionWidget(
                qpixmap_from_mat(__m),
                std::move(__selection),
                __parent,
                __f
            )
        {}

        explicit SelectionWidget(
            QWidget* __parent   = nullptr,
            Qt::WindowFlags __f = {}
        )
            : SelectionWidget(QPixmap(), QRect(), __parent, __f)
        {}
    private:
        void updateRubberBandGeometry();
    protected:
        void keyPressEvent(QKeyEvent*) override;
        void mousePressEvent(QMouseEvent*) override;
        void mouseMoveEvent(QMouseEvent*) override;
        void resizeEvent(QResizeEvent*) override;
        void showEvent(QShowEvent*) override;
    public:
        QRect selection() const noexcept { return m_selection; }
        bool hasSelection() const noexcept { return m_selection.isValid(); }
    signals:
        void selectionChanged(const QRect& __new_selection);
    public slots:
        void setPixmap(const QPixmap&);

        void setPixmap(const cv::Mat& __m)
            { setPixmap(qpixmap_from_mat(__m)); }

        void setSelection(QRect) noexcept;
        void resetSelection() noexcept;
    private:
        QRect m_selection;

        QPoint      m_origin;
        QRubberBand m_rubber_band;
    };
}

#endif // PMSEXP_GUI_WIDGETS_SELECTION_WIDGET_HPP
