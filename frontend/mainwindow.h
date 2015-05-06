/*
 * Copyright 2015 KANG Yuxuan
 *
 * mainwindow.h by KANG Yuxuan <stonekyx@gmail.com>
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "common.h"
#include "PluginManager.h"
#include <QMainWindow>
#include <QtCore/QString>
#include <QtGui/QKeyEvent>

#include "backend/Backend.h"

BEGIN_BIN_NAMESPACE(frontend)

namespace Ui {
class MainWindow;
}

class MainWindow;

END_BIN_NAMESPACE

class binary::frontend::MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void loadPlugins(const QString &);
    void loadPlugin(const QString &);
    void openFile(const QString &);

public slots:
    void openFile();
    void closeFile();
    void loadPlugin();
    void safeClose();

protected:
    virtual void keyPressEvent(QKeyEvent *);
    virtual void closeEvent(QCloseEvent *);

private:
    Ui::MainWindow *ui;
    BIN_NAMESPACE(backend)::Backend *_backend;
    PluginManager *_pm;
};

#endif // MAINWINDOW_H
