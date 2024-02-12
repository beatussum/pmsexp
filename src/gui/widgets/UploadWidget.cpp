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


#include "gui/widgets/UploadWidget.hpp"

#include <QtCore/QMimeData>
#include <QtCore/QMimeDatabase>

#include <QtGui/qevent.h>
#include <QtWidgets/QFileDialog>

namespace gui::widgets
{
    void UploadWidget::dragEnterEvent(QDragEnterEvent* __e)
    {
        const QMimeData* data = __e->mimeData();

        if (
            data->hasUrls() &&
            (data->urls().size() == 1) &&
            checkFilePath(__e->mimeData()->urls().first())
        )
        {
            __e->acceptProposedAction();
        }

        ItemizeWidget::dragEnterEvent(__e);
    }

    void UploadWidget::dropEvent(QDropEvent* __e)
    {
        m_file_path = __e->mimeData()->urls().first().toLocalFile();

        emit filePathUpdated(m_file_path);

        ItemizeWidget::dropEvent(__e);
    }

    void UploadWidget::mousePressEvent(QMouseEvent* __e)
    {
        if (__e->button() == Qt::LeftButton) {
            setFilePath(
                QFileDialog::getOpenFileName(
                    this,
                    tr("Open the video file"),
                    QDir::homePath(),
                    tr("Video files (*.mkv *.mp4);;Any files (*)")
                )
            );
        }

        ItemizeWidget::mousePressEvent(__e);
    }

    bool UploadWidget::checkFilePath(const QUrl& __f) const
    {
        QMimeDatabase db;

        return (
            !m_mime_checker ||
            m_mime_checker(db.mimeTypeForUrl(__f).name())
        );
    }

    void UploadWidget::setFilePath(QString __f)
    {
        if (__f.isEmpty() || checkFilePath(__f)) {
            m_file_path = std::move(__f);

            emit filePathUpdated(m_file_path);
        }
    }
}
