#ifndef PLUGIN_BASIC_MAINWINDOW_H
#define PLUGIN_BASIC_MAINWINDOW_H

#include <map>
#include <string>

#include "common.h"

#include <QMainWindow>

BEGIN_PLUG_NAMESPACE(basic)

namespace Ui {
    class MainWindow;
}

class MainWindow;

END_PLUG_NAMESPACE

class binary::plugin::basic::MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = NULL);
    explicit MainWindow(std::map<std::string, std::string>,
            QWidget *parent = NULL);
    ~MainWindow();
private:
    Ui::MainWindow *ui;
};

#endif
