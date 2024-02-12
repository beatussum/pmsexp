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


#include "gui/pages/Upload.hpp"

namespace gui::pages
{
    Upload::Upload(QWidget* __parent, Qt::WindowFlags __f)
        : QWidget(__parent, __f)
        , m_ui(new Ui::Upload())
    {
        m_ui->setupUi(this);

        m_ui->m_uploader->setMimeChecker(
            [] (const QString& __m) {
                return (__m.section('/', 0, 0) == "video");
            }
        );

        QObject::connect(
            m_ui->m_uploader,
            &widgets::UploadWidget::filePathUpdated,
            this,

            [&] (const QString& __new_file_path) {
                bool status = !__new_file_path.isEmpty();

                m_ui->m_item->setStatus(status);

                emit uploadStatusChanged(status);
            }
        );
    }
}
