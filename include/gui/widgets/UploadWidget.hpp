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


#ifndef PMSEXP_GUI_WIDGETS_UPLOAD_WIDGET_HPP
#define PMSEXP_GUI_WIDGETS_UPLOAD_WIDGET_HPP

#include "gui/widgets/ItemizeWidget.hpp"

namespace gui::widgets
{
    class UploadWidget : public ItemizeWidget
    {
        Q_OBJECT

        Q_PROPERTY(
            QString filePath
            READ filePath
            WRITE setFilePath
            RESET resetFilePath
            NOTIFY filePathUpdated
        )

        Q_PROPERTY(
            mime_checker_type mimeChecker
            READ mimeChecker
            WRITE setMimeChecker
        )

    public:
        using mime_checker_type = std::function<bool(const QString&)>;
    public:
        explicit UploadWidget(
            const QString& __info,
            mime_checker_type,
            QWidget* __parent = nullptr,
            Qt::WindowFlags   = {}
        );

        UploadWidget(QWidget* __parent = nullptr, Qt::WindowFlags __f = {})
            : UploadWidget(QString(), mime_checker_type(), __parent, __f)
        {}
    protected:
        void dragEnterEvent(QDragEnterEvent*) override;
        void dropEvent(QDropEvent*) override;
    private:
        bool checkFilePath(const QUrl&) const;
    public:
        QString filePath() const { return m_file_path; }
        bool isEmpty() const { return m_file_path.isEmpty(); }

        mime_checker_type mimeChecker() const { return m_mime_checker; }

        void setMimeChecker(mime_checker_type __c)
            { m_mime_checker = std::move(__c); }
    signals:
        void filePathUpdated(const QString& __new_file_path);
    public slots:
        void setFilePath(QString);
        void resetFilePath() { setFilePath(QString()); }
    private:
        QString           m_file_path;
        mime_checker_type m_mime_checker;
    };
}

#endif // PMSEXP_GUI_WIDGETS_UPLOAD_WIDGET_HPP
