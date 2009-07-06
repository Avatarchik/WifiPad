#include <QtGui/QApplication>
#include <QMessageBox>
#include "Server.h"
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    try {
        using namespace WifiPad;
        Server server("0.0.0.0",8989);
        server.Start();

        MainWindow w(server);

        w.show();
        return a.exec();
    } catch(const std::runtime_error& e) {
        QMessageBox::critical(NULL,"Error",e.what());
    }
    return -1;
}
