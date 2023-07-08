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


#ifndef PMSEXP_GUI_MAIN_WINDOW_HPP
#define PMSEXP_GUI_MAIN_WINDOW_HPP

#include "ui_MainWindow.h"

#include "core/core.hpp"

#include <opencv2/videoio.hpp>
#include <QtCore/QFutureWatcher>

namespace cv
{
    class Tracker;
}

class QProgressBar;

namespace gui::widgets::pages
{
    class Calibration;
    class ContourSelection;
    class Selection;
    class Statistics;
    class Upload;
}

namespace gui
{
    class MainWindow : public QMainWindow
    {
        Q_OBJECT

    private:
        using future_watcher_type = QFutureWatcher<full_positions_data>;
    public:
        explicit MainWindow(QWidget* __parent = nullptr, Qt::WindowFlags = {});
        virtual ~MainWindow() { delete m_ui; }
    private:
        full_positions_data process();
    private slots:
        void update_size(int __current_index);
    private slots:
        void find_contours(const QRect& __new_selection);
        void load_selection(bool __new_status);
        void reset();
        void run();
        void show_results();
    private:
        cv::VideoCapture     m_capture;
        cv::Mat              m_first_frame;
        future_watcher_type  m_future_watcher;
        cv::Ptr<cv::Tracker> m_tracker;
        QProgressBar*        m_progress_bar;
        Ui::MainWindow*      m_ui;
        bool                 m_update_needed;

        gui::widgets::pages::Calibration*      m_calibration_page;
        gui::widgets::pages::ContourSelection* m_contour_selection_page;
        gui::widgets::pages::Selection*        m_selection_page;
        gui::widgets::pages::Statistics*       m_statistics_page;
        gui::widgets::pages::Upload*           m_upload_page;
    };
}

#endif // PMSEXP_GUI_MAIN_WINDOW_HPP
