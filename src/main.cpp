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

#include <QtCore/QTranslator>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    QTranslator translator;

    translator.load(
        QLocale::system(),
        QString("%1/pmsexp").arg(PMSEXP_I18N_FILE_PATH),
        QString('_')
    );

    app.installTranslator(&translator);

    gui::MainWindow main_window;
    main_window.show();

    return app.exec();
}
