/*
 * Copyright 2015 KANG Yuxuan
 *
 * ui_MainWindow.h by KANG Yuxuan <stonekyx@gmail.com>
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

#ifndef PLUGIN_FLOW_UI_MAINWINDOW_H
#define PLUGIN_FLOW_UI_MAINWINDOW_H

#include <QtGui/QStatusBar>
#include <QtGui/QLabel>
#include <QtGui/QSlider>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QButtonGroup>

#include "ui_MWBase.h"

BEGIN_PLUG_NAMESPACE(flow)

namespace Ui {
    class MainWindow;
    class GraphicsView;
}

END_PLUG_NAMESPACE

class binary::plugin::flow::Ui::MainWindow :
    public binary::plugin::plugin_framework::Ui::MWBase
{
    Q_OBJECT
public:
    GraphicsView *graphicsView;
    QStatusBar *statusBar;
    QLabel *lblMousePos;
    QLabel *lblLoopCnt;
    QButtonGroup *btnGrpAlgo;
    QRadioButton *rbtnAlgoFdp, *rbtnAlgoDot;
    QSlider *scaleSlider;
    QPushButton *btnResetSlider;

    MainWindow();
    ~MainWindow();

    virtual bool switchMode(bool file);

    virtual void setupUi(QMainWindow *window);

    void retranslateUi(QMainWindow *);
public slots:
    void resetSlider();
    void scaleView(int);
    void setLoopCnt(int);
private:
    int _sliderMin, _sliderMax;
};

#endif
