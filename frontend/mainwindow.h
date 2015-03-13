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

protected:
    virtual void keyPressEvent(QKeyEvent *);

private:
    Ui::MainWindow *ui;
    BIN_NAMESPACE(backend)::Backend *_backend;
    PluginManager *_pm;
};

#endif // MAINWINDOW_H
