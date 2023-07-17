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


#include "gui/widgets/UploadWidget.hpp"

#include <QtCore/QMimeData>
#include <QtCore/QMimeDatabase>

#include <QtGui/QDragEnterEvent>

namespace gui::widgets
{
    UploadWidget::UploadWidget(
        const QString& __info,
        mime_checker_type __c,
        QWidget* __parent,
        Qt::WindowFlags __f
    )
        : ItemizeWidget(
                __info,
                QIcon::fromTheme("document-new"),
                __parent,
                __f
            )

        , m_file_path()
        , m_mime_checker(std::move(__c))
    {
        setAcceptDrops(true);
    }

    void UploadWidget::dragEnterEvent(QDragEnterEvent* __e)
    {
        const QMimeData* data = __e->mimeData();

        if (data->hasUrls() && (data->urls().size() == 1))
        {
            QMimeDatabase db;

            if (
                !m_mime_checker ||

                m_mime_checker(
                    db.mimeTypeForUrl(__e->mimeData()->urls().first()).name()
                )
            )
            {
                __e->acceptProposedAction();
            }
        }

        ItemizeWidget::dragEnterEvent(__e);
    }

    void UploadWidget::dropEvent(QDropEvent* __e)
    {
        m_file_path = __e->mimeData()->urls().first().toLocalFile();

        emit file_path_updated(m_file_path);

        ItemizeWidget::dropEvent(__e);
    }

    void UploadWidget::set_file_path(QString __f)
    {
        m_file_path = std::move(__f);

        emit file_path_updated(m_file_path);
    }
}
