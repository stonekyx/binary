/*
 * Copyright 2015 KANG Yuxuan
 *
 * ui_MainWindow.cpp by KANG Yuxuan <stonekyx@gmail.com>
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

#include "GraphicsView.h"
#include "GVGraph.h"

#include "ui_MainWindow.h"

#define OBJNAME(widget) \
        do{ widget->setObjectName(QString::fromUtf8(#widget)); }while(0)

#define TRANS(raw) \
        QApplication::translate(_context, raw, 0, QApplication::UnicodeUTF8)

USE_PLUG_NAMESPACE(plugin_framework);

BEGIN_PLUG_NAMESPACE(flow)
namespace Ui {

MainWindow::MainWindow() :
    MWBase("PluginFlowMainWindow", "Flow Diagram"),
    _sliderMin(10), _sliderMax(1000)
{}

MainWindow::~MainWindow()
{
}

bool MainWindow::switchMode(bool file)
{
    if(file) {
        defaultLabel->hide();
        graphicsView->show();
        statusBar->show();
        rbtnAlgoFdp->show();
        rbtnAlgoDot->show();
        scaleSlider->show();
        btnResetSlider->show();
        return true;
    }
    graphicsView->hide();
    statusBar->hide();
    rbtnAlgoFdp->hide();
    rbtnAlgoDot->hide();
    scaleSlider->hide();
    btnResetSlider->hide();
    defaultLabel->show();
    return false;
}

void MainWindow::setupUi(QMainWindow *window) {
    MWBase::setupUi(window);

    graphicsView = new GraphicsView(centralWidget);
    OBJNAME(graphicsView);
    graphicsView->resetTransform();
    gridLayout->addWidget(graphicsView, 0, 0, 1, -1);

    statusBar = new QStatusBar(window);
    OBJNAME(statusBar);
    window->setStatusBar(statusBar);

    lblMousePos = new QLabel(statusBar);
    OBJNAME(lblMousePos);
    statusBar->addPermanentWidget(lblMousePos);

    lblLoopCnt = new QLabel(statusBar);
    OBJNAME(lblLoopCnt);
    statusBar->addPermanentWidget(lblLoopCnt);

    btnGrpAlgo = new QButtonGroup(centralWidget);
    OBJNAME(btnGrpAlgo);

    rbtnAlgoFdp = new QRadioButton("&Fdp", centralWidget);
    OBJNAME(rbtnAlgoFdp);
    btnGrpAlgo->addButton(rbtnAlgoFdp, GVGraph::GV_FDP);
    gridLayout->addWidget(rbtnAlgoFdp, 1, 0, 1, 1);

    rbtnAlgoDot = new QRadioButton("&Dot", centralWidget);
    OBJNAME(rbtnAlgoDot);
    btnGrpAlgo->addButton(rbtnAlgoDot, GVGraph::GV_DOT);
    gridLayout->addWidget(rbtnAlgoDot, 1, 1, 1, 1);

    rbtnAlgoFdp->setChecked(true);

    scaleSlider = new QSlider(Qt::Horizontal, centralWidget);
    OBJNAME(scaleSlider);
    scaleSlider->setRange(_sliderMin, _sliderMax);
    scaleSlider->setSingleStep(1);
    scaleSlider->setPageStep((_sliderMax-_sliderMin)/10);
    resetSlider();
    gridLayout->addWidget(scaleSlider, 1, 2, 1, 1);

    btnResetSlider = new QPushButton(centralWidget);
    OBJNAME(btnResetSlider);
    btnResetSlider->setFixedWidth(50);
    gridLayout->addWidget(btnResetSlider, 1, 3, 1, 1);

    QObject::connect(graphicsView, SIGNAL(mouseScenePos(const QString&)),
            lblMousePos, SLOT(setText(const QString&)));
    QObject::connect(btnResetSlider, SIGNAL(clicked()),
            this, SLOT(resetSlider()));
    QObject::connect(scaleSlider, SIGNAL(valueChanged(int)),
            this, SLOT(scaleView(int)));

    retranslateUi(window);
}

void MainWindow::retranslateUi(QMainWindow *) {
    btnResetSlider->setText(tr("Reset"));
}

void MainWindow::resetSlider()
{
    scaleSlider->setValue(_sliderMax);
}

void MainWindow::scaleView(int value)
{
    graphicsView->resetTransform();
    graphicsView->scale(value/(double)_sliderMax, value/(double)_sliderMax);
}

void MainWindow::setLoopCnt(int loopCnt)
{
    lblLoopCnt->setText(QString::number(loopCnt) + " loops");
}

}
END_PLUG_NAMESPACE
