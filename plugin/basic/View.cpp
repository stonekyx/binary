#include <QtGui/QAction>
#include <QtGui/QApplication>

#include "View.h"

using namespace std;

BEGIN_PLUG_NAMESPACE(basic)

View::View() :
    _mainWindow(new MainWindow(NULL))
{
    _mainWindow->show();
}

View::View(map<string, string>) :
    _mainWindow(new MainWindow(NULL))
{
    _mainWindow->show();
}

View::~View()
{
    delete _mainWindow;
}

void View::update()
{
    _mainWindow->update();
}

END_PLUG_NAMESPACE
