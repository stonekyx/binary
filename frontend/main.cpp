/*
 * Copyright 2015 KANG Yuxuan
 *
 * main.cpp by KANG Yuxuan <stonekyx@gmail.com>
 *
 * This file is part of Binary.
 *
 * Binary is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Binary is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Binary.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "mainwindow.h"
#include <QtCore/QStringList>
#include <QtCore/QCoreApplication>
#include <QApplication>
#include <QtGui/QCleanlooksStyle>

USE_BIN_NAMESPACE(frontend);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow *w = new MainWindow();
    QStringList args = QCoreApplication::arguments();
    for(int i=1; i<args.size(); i++) {
        if(args.at(i) == QString::fromUtf8("--plugins") &&
                i+1 < args.size())
        {
            w->loadPlugins(args.at(i+1));
        }
        if(args.at(i) == QString::fromUtf8("--file") &&
                i+1 < args.size())
        {
            w->openFile(args.at(i+1));
        }
    }
    QApplication::setStyle(new QCleanlooksStyle());
    w->show();

    return a.exec();
}
