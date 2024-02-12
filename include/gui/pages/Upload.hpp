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


#ifndef PMSEXP_GUI_PAGES_UPLOAD_HPP
#define PMSEXP_GUI_PAGES_UPLOAD_HPP

#include "ui_Upload.h"

namespace gui::pages
{
    class Upload : public QWidget
    {
        Q_OBJECT

        Q_PROPERTY(
            bool uploadStatus
            READ isUploadValid
            RESET resetUploadStatus
            NOTIFY uploadStatusChanged
        )

    public:
        explicit Upload(QWidget* __parent = nullptr, Qt::WindowFlags = {});
        virtual ~Upload() { delete m_ui; }
    public:
        QString filePath() const
            { return m_ui->m_uploader->filePath(); }

        bool isUploadValid() const { return !m_ui->m_uploader->isEmpty(); }
    signals:
        void uploadStatusChanged(bool __new_status);
    public slots:
        void resetUploadStatus() const { m_ui->m_uploader->resetFilePath(); }
    private:
        Ui::Upload* m_ui;
    };
}

#endif // PMSEXP_GUI_PAGES_UPLOAD_HPP
