#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "common.h"
#include <QMainWindow>

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

public slots:
    void openFile();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
