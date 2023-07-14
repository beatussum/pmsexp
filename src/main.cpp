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


#include "config.hpp"
#include "gui/MainWindow.hpp"

#include <QtCore/QTranslator>

int main(int argc, char* argv[])
{
#ifdef PMSEXP_THEME_DIR
    QIcon::setFallbackSearchPaths(QStringList(PMSEXP_THEME_DIR));
#endif

    /* If the `QIcon::themeName()` is not set (for example, in a Windows
     * system, it is always true), one set a dummy theme name in order to force
     * `QIcon::fromTheme()` to use the fallback icons.
     */

    if (QIcon::themeName().isEmpty()) {
        QIcon::setThemeName(QString('d'));
    }

    QApplication app(argc, argv);

    QTranslator translator;

    translator.load(
        QLocale::system(),
        "pmsexp",
        QString('_'),
        PMSEXP_I18N_DIR
    );

    QApplication::installTranslator(&translator);

    gui::MainWindow main_window;
    main_window.show();

    return app.exec();
}
