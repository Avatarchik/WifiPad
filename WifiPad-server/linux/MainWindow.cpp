#include <QtGui/QMessageBox>
#include <QtGui/QFileDialog>
#include <QtCore/QFileInfo>
#include "Server.h"
#include "OS.h"
#include "NewConfigurationDialog.h"
#include "MainWindow.h"
#include "ui_MainWindow.h"

using namespace WifiPad;

MainWindow::MainWindow(Server& server,QWidget *parent)
    : QMainWindow(parent), m_server(server), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->gvGamepadView->SetMainWindow(this);
    ReloadConfigurationList();
    ReloadGamepadList();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ReloadConfigurationList()
{
    ui->cbConfiguration->clear();
    ui->cbConfiguration->addItem("<Default configuration>");
    ui->cbConfiguration->setCurrentIndex(0);
    m_server.GetConfigurationList(&m_configList);
    for(Server::ConfigurationList::const_iterator it = m_configList.begin(); it != m_configList.end(); ++it) {
        ui->cbConfiguration->addItem(it->c_str());
    }
}

void MainWindow::ReloadGamepadList()
{
    ui->cbGamepads->clear();
	m_server.LoadGamePads();
    m_server.GetGamePadList(&m_gamePadList);
    for(Server::GamePadList::const_iterator it = m_gamePadList.begin(); it != m_gamePadList.end(); ++it) {
        ui->cbGamepads->addItem(it->gamePad->GetName().c_str());
    }
    ui->cbGamepads->setCurrentIndex(0);
}

void MainWindow::ServerNameChanged(QString str)
{
    m_server.SetServerName(str.toStdString());
}

void MainWindow::ChangeGamePad(int index)
{
    try {
        if(index >= 0 && index < m_gamePadList.size()) {
            FileDataSource source(m_gamePadList[index].path);
            m_gamePad.reset(new GamePad(source));
            repaint();
        }
    } catch(const std::runtime_error& e) {
        QMessageBox::critical(this,"Error selecting gamepad.",e.what());
    }
}

void MainWindow::ChangeConfiguration(int index)
{
    try {
        if(index <= 0) m_configuration.reset();
        else {
            m_configuration = m_server.LoadConfiguration(m_configList[index - 1]);
        }
        repaint();
    } catch(const std::runtime_error& e) {
        QMessageBox::critical(this,"Error loading configuration.",e.what());
        ReloadConfigurationList();
    }
}

void MainWindow::NewConfiguration()
{
    NewConfigurationDialog dialog;
    int code = dialog.exec();
    if(code == QDialog::Accepted) {
        try {
            std::string configName = dialog.configName;

            if(configName == "") {
                throw std::runtime_error("Configuration name is empty.");
            }

            // check if configuration exists first
            if(OS::FileExists(m_server.GetConfigurationsPath() + "/" + configName + ".config")) {
                throw std::runtime_error("Could not create configuration because configuration already exists.");
            }

            ConfigurationPtr config(new Configuration(configName));
            // copy configuration if checked
            if(dialog.copyCurrentConfig) {
                if(m_configuration) {
                        *config = *m_configuration;
                }
            }

            m_server.SaveConfiguration(config,configName);
            // reload configuration
            ReloadConfigurationList();
        } catch(const std::runtime_error& e) {
            QMessageBox::critical(this,"Error creating configuration.",e.what());
        }
    }
}

void MainWindow::DeleteConfiguration()
{
     try {
        int selected = ui->cbConfiguration->currentIndex();
        if(selected <= 0) throw std::runtime_error("You cannot delete the default configuration.");

        int code = QMessageBox::question(this,"Confirm",("Do you wish to delete the configuration \"" + m_configList[selected - 1] + "\"?").c_str(),QMessageBox::Yes | QMessageBox::No);
        if(code == QMessageBox::Yes) {
            OS::DeleteFile(m_server.GetConfigurationsPath() + "/" + m_configList[selected - 1] + ".config");
            QMessageBox::information(this,"","Configuration deleted.");
            ReloadConfigurationList();
        }
    } catch(const std::runtime_error& e) {
        QMessageBox::critical(this,"Error creating configuration.",e.what());
    }
}

void MainWindow::SaveConfiguration()
{
    try {
        int selected = ui->cbConfiguration->currentIndex();
        if(selected <= 0) throw std::runtime_error("You cannot save over the default configuration.");
        m_server.SaveConfiguration(m_configuration,m_configList[selected - 1]);

        // reload configurations for each device
        Server::DeviceList devices;
        m_server.GetDeviceList(&devices);
        for(Server::DeviceList::iterator device = devices.begin(); device != devices.end(); ++device)
        {
            ConfigurationPtr config = (*device)->GetConfiguration();
            if(config && config->GetName() == m_configList[selected - 1]) {
                (*device)->SetConfiguration(m_server.LoadConfiguration(m_configList[selected - 1]));
            }
        }

        QMessageBox::information(this,"","Configuration saved.");
    } catch(const std::runtime_error& e) {
        QMessageBox::critical(this,"Error creating configuration.",e.what());
    }
}

void MainWindow::InstallGamepad()
{
    QString filename = QFileDialog::getOpenFileName(this,"Choose a gamepad to install...","","Gamepads (*.gamepad)");
    if(!filename.isNull())
    {
        QFileInfo fileinfo(filename);
        try {
            // load gamepad to make sure it's valid
            GamePadPtr checkPad(new GamePad());
            try {
                FileDataSource source(filename.toStdString());
                checkPad->LoadGamePad(source,0);
            } catch(const std::runtime_error&) {
                throw std::runtime_error("Failed to install gamepad. The selected gamepad is invalid.");
            }

            // check for duplicate uuid
            try {
                m_server.GetGamePadPathByUuid(checkPad->GetUuid());
                QMessageBox::critical(this,"Error","This gamepad is already installed. (Or matches a UUID of an existing gamepad.)");
                return;
            } catch(const std::runtime_error&) {
            }

            std::string baseFilename = (fileinfo.baseName() + "." + fileinfo.suffix()).toStdString();
			char curtime[32];
			sprintf(curtime,"%d",(int)time(NULL));

			OS::CopyFile(filename.toStdString(),m_server.GetGamePadsPath() + "/" + curtime + "." + baseFilename);

            QMessageBox::information(this,"","Gamepad installed.");

            ReloadGamepadList();
        } catch(const std::runtime_error& e) {
            QMessageBox::critical(this,"Error installing gamepad",e.what());
        }
    }
}

void MainWindow::DeleteGamepad()
{
    try {
        int selected = ui->cbGamepads->currentIndex();
        if(selected <= 0) throw std::runtime_error("You cannot delete the default gamepad.");

        int code = QMessageBox::question(this,"Confirm",("Do you want to delete the gamepad \"" + m_gamePadList[selected].gamePad->GetName() + "\"?").c_str(),QMessageBox::Yes | QMessageBox::No);
        if(code == QMessageBox::Yes) {
            OS::DeleteFile(m_gamePadList[selected].path);
            ReloadGamepadList();
        }
    } catch(const std::runtime_error& e) {
        QMessageBox::critical(this,"Error deleting gamepad",e.what());
    }
}
