#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include "Server.h"

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(WifiPad::Server& server, QWidget *parent = 0);
    ~MainWindow();

    friend class GamePadView;
private:
    WifiPad::Server& m_server;
    WifiPad::Server::ConfigurationList m_configList;
    WifiPad::Server::GamePadList m_gamePadList;
    WifiPad::GamePadPtr m_gamePad;
    WifiPad::ConfigurationPtr m_configuration;

    Ui::MainWindow *ui;

    void ReloadConfigurationList();
    void ReloadGamepadList();
public slots:
    void ServerNameChanged(QString str);
    void ChangeGamePad(int index);
    void ChangeConfiguration(int index);
    void NewConfiguration();
    void DeleteConfiguration();
    void SaveConfiguration();
    void InstallGamepad();
    void DeleteGamepad();
	void SetMouseSpeed(int speed);
};

#endif // MAINWINDOW_H
