/*
 * Copyright (C) 2022-2023 Mattéo Rossillol‑‑Laruelle <beatussum@protonmail.com>
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

#include <opencv2/tracking.hpp>

#include <QtConcurrent/QtConcurrent>
#include <QtWidgets/QProgressBar>

#include <valijson/adapters/qtjson_adapter.hpp>
#include <valijson/utils/qtjson_utils.hpp>
#include <valijson/schema.hpp>
#include <valijson/schema_parser.hpp>
#include <valijson/validator.hpp>

namespace gui
{
    MainWindow::MainWindow(QWidget* __parent, Qt::WindowFlags __f)
        : QMainWindow(__parent, __f)

        , m_capture()
        , m_first_frame()
        , m_future_watcher_comp(this)
        , m_future_watcher_ex(this)
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
            &m_future_watcher_comp,
            &future_watcher_comp_type::finished,
            this,
            &MainWindow::show_results
        );

        QObject::connect(
            &m_future_watcher_comp,
            &future_watcher_comp_type::progressTextChanged,
            this,

            [&] (const QString& __progress_text) {
                m_ui->m_status_bar->showMessage(__progress_text, 2'000);
            }
        );

        QObject::connect(
            &m_future_watcher_ex,
            &future_watcher_ex_type::finished,
            this,

            [&] {
                m_progress_bar->reset();
                m_progress_bar->setRange(0, 0);

                m_ui->m_status_bar->showMessage(
                    tr(
                        "Le chargement des données expérimentales est terminé."
                    ),

                    2'000
                );

                m_future_watcher_comp.setFuture(
                    QtConcurrent::run(this, &MainWindow::process_comp)
                );
            }
        );

        QObject::connect(
            &m_future_watcher_ex,
            &future_watcher_ex_type::progressValueChanged,
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
            m_selection_page->get_selection_widget(),
            &widgets::SelectionWidget::selection_changed,
            this,
            &MainWindow::find_contours
        );

        QObject::connect(
            m_upload_page,
            &widgets::pages::Upload::upload_status_changed,
            this,
            &MainWindow::load_selection
        );
    }

    full_positions_comp_data MainWindow::process_comp()
    {
        full_positions_comp_data ret;
        QString file_path = m_upload_page->get_comp_file_path();

        QJsonValue schema_value;

        if (
            valijson::utils::loadDocument(
                PMSEXP_SCHEMA_FILE_PATH,
                schema_value
            )
        )
        {
            valijson::adapters::QtJsonAdapter schema_adaptater(schema_value);
            valijson::Schema schema;
            valijson::SchemaParser schema_parser;

            schema_parser.populateSchema(schema_adaptater, schema);

            QJsonValue data;

            if (
                valijson::utils::loadDocument(file_path.toStdString(), data)
            )
            {
                valijson::adapters::QtJsonAdapter data_adaptater(data);
                valijson::Validator validator;

                if (validator.validate(schema, data_adaptater, nullptr)) {
                    QJsonArray data_arr = data["data"].toArray();

                    double first_timestamp =
                        data_arr.first()["timestamp"].toDouble();

                    full_position first_position =
                        full_position_from_qjsonvalue(data_arr.first());

                    ret[0.].first  = full_position();
                    ret[0.].second = full_position();

                    for (
                        auto i = (data_arr.begin() + 1);
                        i < data_arr.end();
                        ++i
                    )
                    {
                        QJsonObject datum = i->toObject();
                        double timestamp  = datum["timestamp"].toDouble();

                        ret[timestamp - first_timestamp].first =
                            full_position_from_qjsonvalue(
                                datum["computed"],
                                first_position
                            );

                        ret[timestamp - first_timestamp].second =
                            full_position_from_qjsonvalue(
                                datum["target"],
                                first_position
                            );
                    }
                } else {
                    emit m_future_watcher_comp.progressTextChanged(
                        tr("Le fichier « %1 » n'a pas le bon format.")
                            .arg(file_path)
                    );
                }
            } else {
                emit m_future_watcher_comp.progressTextChanged(
                    tr("La lecture du fichier « %1 » est impossible.")
                        .arg(file_path)
                );
            }
        } else {
            emit m_future_watcher_comp.progressTextChanged(
                tr("La lecture du fichier schéma « %1 » est impossible.")
                    .arg(PMSEXP_SCHEMA_FILE_PATH)
            );
        }

        return ret;
    }

    full_positions_ex_data MainWindow::process_ex()
    {
        full_positions_ex_data ret;

        full_position first_position =
            full_position_from_contour(
                m_contour_selection_page->get_current()
            );

        double first_timestamp = m_capture.get(cv::CAP_PROP_POS_MSEC);
        int progress           = 1;
        ret[0.]                = full_position();

        emit m_future_watcher_ex.progressValueChanged(progress);

        double current_area = m_contour_selection_page->get_current_area();
        cv::Mat frame;

        cv::Rect roi = rect_from_qrect(
            m_selection_page->get_selection_widget()->get_selection()
        );

        m_capture.set(cv::CAP_PROP_POS_FRAMES, 1.);

        while (m_capture.read(frame)) {
            m_tracker->update(frame, roi);

            full_position fp = full_position_from_contour(
                *contours_from_mat(frame, roi, current_area).begin()
            );

            fp.angle     -= first_position.angle;
            fp.position  -= first_position.position;

            ret[
                m_capture.get(cv::CAP_PROP_POS_MSEC) - first_timestamp
            ] = std::move(fp);

            emit m_future_watcher_ex.progressValueChanged(++progress);
        }

        return ret;
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
                    tr("Aucun contour trouvé."), 2'000
                );
            } else {
                m_calibration_page->get_calibration_widget()->setPixmap(
                    m_first_frame
                );

                m_contour_selection_page->set_values(
                    std::move(contours),
                    m_first_frame
                );

                m_ui->m_central_widget->set_progress(3);

                m_ui->m_status_bar->showMessage(
                    tr("%n contour(s) trouvé(s).", nullptr, size), 2'000
                );
            }
        }

        m_update_needed = true;
    }

    void MainWindow::load_selection(bool __new_status)
    {
        if (__new_status) {
            QString file_path = m_upload_page->get_ex_file_path();

            if (m_capture.open(file_path.toStdString())) {
                m_capture.set(cv::CAP_PROP_POS_FRAMES, 0.);

                if (m_capture.read(m_first_frame)) {
                    m_selection_page->get_selection_widget()->setPixmap(
                        m_first_frame
                    );

                    m_ui->m_central_widget->set_progress(1);
                } else {
                    m_ui->m_status_bar->showMessage(
                        tr("Aucun « frame » n'a pu ếtre récupéré."), 2'000
                    );
                }
            } else {
                m_ui->m_status_bar->showMessage(
                    tr("Erreur lors de l'ouverture du fichier « %1 ».")
                        .arg(file_path),

                    2'000
                );
            }
        } else {
            m_ui->m_central_widget->set_progress(0);
        }

        m_update_needed = true;
    }

    void MainWindow::reset()
    {
        m_statistics_page->reset_data();
        m_calibration_page->reset_status();
        m_contour_selection_page->reset_contours();
        m_selection_page->get_selection_widget()->reset_selection();
        m_upload_page->reset_upload_status();

        m_update_needed = true;
    }

    void MainWindow::run()
    {
        if (m_update_needed) {
            m_tracker = cv::TrackerKCF::create();

            m_tracker->init(
                m_first_frame,

                rect_from_qrect(
                    m_selection_page->get_selection_widget()->get_selection()
                )
            );

            m_ui->m_action_reset->setEnabled(false);
            m_ui->m_central_widget->setEnabled(false);
            m_ui->m_status_bar->clearMessage();

            m_progress_bar->reset();

            m_progress_bar->setRange(
                0, m_capture.get(cv::CAP_PROP_FRAME_COUNT)
            );

            m_progress_bar->show();

            m_future_watcher_ex.setFuture(
                QtConcurrent::run(this, &MainWindow::process_ex)
            );
        } else {
            m_ui->m_status_bar->showMessage(
                tr("Résultats déjà chargés !"), 2'000
            );
        }
    }

    void MainWindow::show_results()
    {
        full_positions_comp_data result_comp = m_future_watcher_comp.result();
        full_positions_ex_data result_ex     = m_future_watcher_ex.result();

        if (result_comp.empty() || result_ex.empty()) {
            m_ui->m_central_widget->previous();
        } else {
            m_ui->m_status_bar->showMessage(
                tr("Le chargement des données calculées est terminé."), 2'000
            );

            m_statistics_page->set_data(
                result_comp,
                result_ex,
                m_calibration_page->get_ratio(),
                m_first_frame.size()
            );

            m_update_needed = false;
        }

        m_ui->m_action_reset->setEnabled(true);
        m_ui->m_central_widget->setEnabled(true);

        m_progress_bar->hide();
    }
}
