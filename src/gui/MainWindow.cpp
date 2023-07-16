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


#include "gui/MainWindow.hpp"

#include "gui/widgets/SelectionWidget.hpp"

#include "gui/widgets/pages/Calibration.hpp"
#include "gui/widgets/pages/ContourSelection.hpp"
#include "gui/widgets/pages/Selection.hpp"
#include "gui/widgets/pages/Statistics.hpp"
#include "gui/widgets/pages/Upload.hpp"

#include <opencv2/video/tracking.hpp>

#include <QtConcurrent/QtConcurrent>
#include <QtWidgets/QProgressBar>

namespace gui
{
    MainWindow::MainWindow(QWidget* __parent, Qt::WindowFlags __f)
        : QMainWindow(__parent, __f)

        , m_capture()
        , m_first_frame()
        , m_future_watcher(this)
        , m_tracker()
        , m_progress_bar(new QProgressBar())
        , m_ui(new Ui::MainWindow())
        , m_update_needed(true)

        , m_calibration_page(new widgets::pages::Calibration())
        , m_contour_selection_page(new widgets::pages::ContourSelection())
        , m_selection_page(new widgets::pages::Selection())
        , m_statistics_page(new widgets::pages::Statistics())
        , m_upload_page(new widgets::pages::Upload())
    {
        m_ui->setupUi(this);

        m_ui->m_central_widget->add_page(m_upload_page);
        m_ui->m_central_widget->add_page(m_selection_page);
        m_ui->m_central_widget->add_page(m_contour_selection_page);
        m_ui->m_central_widget->add_page(m_calibration_page);
        m_ui->m_central_widget->add_page(m_statistics_page);

        m_progress_bar->hide();
        m_ui->m_status_bar->addWidget(m_progress_bar);

        QObject::connect(
            &m_future_watcher,
            &future_watcher_type::finished,
            this,
            &MainWindow::show_results
        );

        QObject::connect(
            &m_future_watcher,
            &future_watcher_type::progressValueChanged,
            m_progress_bar,
            &QProgressBar::setValue
        );

        QObject::connect(
            m_ui->m_action_reset,
            &QAction::triggered,
            this,
            &MainWindow::reset
        );

        QObject::connect(
            m_ui->m_central_widget,
            &gui::widgets::ButtonSelecterWidget::page_index_changed,
            this,
            &MainWindow::update_size
        );

        QObject::connect(
            m_ui->m_central_widget,
            &widgets::ButtonSelecterWidget::run,
            this,
            &MainWindow::run
        );

        QObject::connect(
            m_contour_selection_page,
            &widgets::pages::ContourSelection::current_changed,
            this,
            [&] { m_update_needed = true; }
        );

        QObject::connect(
            m_calibration_page,
            &widgets::pages::Calibration::status_changed,
            this,

            [&] (bool __new_status) {
                if (__new_status) {
                    m_ui->m_central_widget->set_progress(4);
                } else {
                    m_ui->m_central_widget->set_progress(3);
                }

                m_update_needed = true;
            }
        );

        QObject::connect(
            m_selection_page,
            &widgets::pages::Selection::selection_changed,
            this,
            &MainWindow::find_contours
        );

        QObject::connect(
            m_upload_page,
            &widgets::pages::Upload::upload_status_changed,
            this,
            &MainWindow::load_selection
        );

        update_size(0);
    }

    full_positions_data MainWindow::process()
    {
        full_positions_data ret;

        double current_area = m_contour_selection_page->get_current_area();

        full_position first_position =
            full_position_from_contour(
                m_contour_selection_page->get_current()
            );

        double first_timestamp = m_capture.get(cv::CAP_PROP_POS_MSEC);

        cv::Mat frame;

        int height   = m_selection_page->get_pixmap_rect().height();
        int progress = 1;
        double ratio = m_calibration_page->get_ratio();
        cv::Rect roi = rect_from_qrect(m_selection_page->get_selection());

        ret[0.] = {0., cv::Point(0., height * ratio)};
        emit m_future_watcher.progressValueChanged(progress);

        m_capture.set(cv::CAP_PROP_POS_FRAMES, 1.);

        while (m_capture.read(frame)) {
            m_tracker->update(frame, roi);

            full_position fp = full_position_from_contour(
                *contours_from_mat(frame, roi, current_area).begin()
            );

            fp.angle    -= first_position.angle;
            fp.position -= first_position.position;

            fp.position.y  = (height - fp.position.y);
            fp.position   *= ratio;

            ret[
                m_capture.get(cv::CAP_PROP_POS_MSEC) - first_timestamp
            ] = std::move(fp);

            emit m_future_watcher.progressValueChanged(++progress);
        }

        return ret;
    }

    void MainWindow::update_size(int __current_index)
    {
        QSize s  = size();
        QSize sh = sizeHint();

        switch (__current_index) {
            case 0:
                if (isMaximized()) {
                    showNormal();
                }

                setFixedSize(sh);
                m_ui->m_status_bar->setSizeGripEnabled(false);

                break;

            case 1:
                setFixedSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
                m_ui->m_status_bar->setSizeGripEnabled(true);

                break;

            case 3:
                setMinimumSize(0, 0);

                break;

            case 4:
                setMinimumSize(800, 400);

                break;
        }

        m_ui->m_central_widget->updateGeometry();

        if ((s.height() < sh.height()) || (s.width() < sh.width())) {
            adjustSize();
        }
    }

    void MainWindow::find_contours(const QRect& __new_selection)
    {
        if (__new_selection.isEmpty()) {
            m_ui->m_central_widget->set_progress(1);
        } else {
            sorted_contours_type contours = contours_from_mat(
                m_first_frame,
                rect_from_qrect(__new_selection)
            );

            if (std::size_t size = contours.size(); size == 0) {
                m_ui->m_central_widget->set_progress(1);

                m_ui->m_status_bar->showMessage(
                    tr("No contours found."), 2'000
                );
            } else {
                m_calibration_page->set_pixmap(m_first_frame);

                m_contour_selection_page->set_values(
                    std::move(contours),
                    m_first_frame
                );

                m_ui->m_central_widget->set_progress(3);

                m_ui->m_status_bar->showMessage(
                    tr("%n contour(s) found.", nullptr, size), 2'000
                );
            }
        }

        m_update_needed = true;
    }

    void MainWindow::load_selection(bool __new_status)
    {
        if (__new_status) {
            QString file_path = m_upload_page->get_file_path();

            if (m_capture.open(file_path.toStdString())) {
                m_capture.set(cv::CAP_PROP_POS_FRAMES, 0.);

                if (m_capture.read(m_first_frame)) {
                    m_selection_page->setPixmap(m_first_frame);
                    m_ui->m_central_widget->set_progress(1);
                } else {
                    m_ui->m_status_bar->showMessage(
                        tr("No frame can be retrieved."), 2'000
                    );
                }
            } else {
                m_ui->m_status_bar->showMessage(
                    tr("Error opening the file \"%1\".").arg(file_path), 2'000
                );
            }
        } else {
            m_ui->m_central_widget->set_progress(0);
        }

        m_update_needed = true;
    }

    void MainWindow::reset()
    {
        m_ui->m_central_widget->set_page_index(0);

        m_statistics_page->reset_data();
        m_calibration_page->reset();
        m_contour_selection_page->reset_values();
        m_selection_page->reset_selection();
        m_upload_page->reset_upload_status();

        m_update_needed = true;
    }

    void MainWindow::run()
    {
        if (m_update_needed) {
            m_tracker = cv::TrackerMIL::create();

            m_tracker->init(
                m_first_frame,
                rect_from_qrect(m_selection_page->get_selection())
            );

            m_ui->m_action_reset->setEnabled(false);
            m_ui->m_central_widget->setEnabled(false);
            m_ui->m_status_bar->clearMessage();

            m_progress_bar->reset();

            m_progress_bar->setRange(
                0, m_capture.get(cv::CAP_PROP_FRAME_COUNT)
            );

            m_progress_bar->show();

            m_future_watcher.setFuture(
                QtConcurrent::run(this, &MainWindow::process)
            );
        } else {
            m_ui->m_status_bar->showMessage(
                tr("Results already loaded!"), 2'000
            );
        }
    }

    void MainWindow::show_results()
    {
        full_positions_data result = m_future_watcher.result();

        m_progress_bar->hide();

        m_ui->m_action_reset->setEnabled(true);
        m_ui->m_central_widget->setEnabled(true);

        m_ui->m_status_bar->showMessage(
            tr("Data loading is complete."), 2'000
        );

        if (result.empty()) {
            m_ui->m_central_widget->previous();
        } else {
            m_statistics_page->set_data(result);

            m_update_needed = false;
        }

        m_ui->m_action_reset->setEnabled(true);
        m_ui->m_central_widget->setEnabled(true);

        m_progress_bar->hide();
    }
}
