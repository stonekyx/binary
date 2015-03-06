#include <QtGui/QAction>
#include <QtGui/QApplication>

#include "View.h"

BEGIN_PLUG_NAMESPACE(basic)

View View::instance;

View::View() :
    _mainWindow(NULL)
{
}

View::~View()
{
}

void View::init(QWidget *parent)
{
    if(_mainWindow) {
        deinit();
    }
    _mainWindow = new MainWindow(parent);
    _mainWindow->hide();
}

void View::deinit()
{
    delete _mainWindow;
}

void View::update()
{
    _mainWindow->update();
}

extern "C"
QAction *createAction(QWidget *parent)
{
    View::instance.init(NULL);

    QAction *actionPluginBasic = new QAction(parent);
    actionPluginBasic->setObjectName("actionPluginBasic");
    actionPluginBasic->setText(QApplication::translate("PluginBasicMainWindow", "Basic", 0, QApplication::UnicodeUTF8));

    QObject::connect(actionPluginBasic, SIGNAL(triggered()),
            View::instance._mainWindow, SLOT(show()));
    return actionPluginBasic;
}

extern "C"
void destroyAction()
{
    View::instance.deinit();
}

END_PLUG_NAMESPACE
