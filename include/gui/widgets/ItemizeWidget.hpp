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


#ifndef PMSEXP_GUI_WIDGETS_ITEMIZE_WIDGET_HPP
#define PMSEXP_GUI_WIDGETS_ITEMIZE_WIDGET_HPP

#include "ui_ItemizeWidget.h"

#include <QtGui/QIcon>

namespace gui::widgets
{
    class ItemizeWidget : public QWidget
    {
        Q_OBJECT

        Q_PROPERTY(QString info READ info WRITE setInfo)
        Q_PROPERTY(QPixmap pixmap READ pixmap WRITE setPixmap)

    public:
        explicit ItemizeWidget(
            const QString& __info,
            const QPixmap&,
            QWidget* __parent = nullptr,
            Qt::WindowFlags   = {}
        );

        explicit ItemizeWidget(
            const QString& __info,
            const QIcon& __i,
            QWidget* __parent   = nullptr,
            Qt::WindowFlags __f = {}
        )
            : ItemizeWidget(__info, __i.pixmap({32, 32}), __parent, __f)
        {}

        ItemizeWidget(QWidget* __parent = nullptr, Qt::WindowFlags __f = {})
            : ItemizeWidget(QString(), QPixmap(), __parent, __f)
        {}

        virtual ~ItemizeWidget() { delete m_ui; }
    public:
        QString info() const { return m_ui->m_info_label->text(); }
        QPixmap pixmap() const;
    public slots:
        void setInfo(const QString& __i) const
            { m_ui->m_info_label->setText(__i); }
    protected slots:
        void setPixmap(const QPixmap& __p) const
            { m_ui->m_pixmap_label->setPixmap(__p); }

        void setPixmap(const QIcon& __i) const
            { setPixmap(__i.pixmap({32, 32})); }
    private:
        Ui::ItemizeWidget* m_ui;
    };
}

#endif // PMSEXP_GUI_WIDGETS_ITEMIZE_WIDGET_HPP
