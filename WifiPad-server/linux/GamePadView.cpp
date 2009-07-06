#include <QtGui/QPainter>
#include <QtGui/QMouseEvent>
#include <QtGui/QMessageBox>
#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "Image.h"
#include "KeySyms.h"
#include "ButtonConfigDialog.h"
#include "GamePadView.h"

using namespace WifiPad;

GamePadView::GamePadView(QWidget *parent) : QWidget(parent), m_selectedButton(-1)
{
    setMouseTracking(true);
}

void GamePadView::paintEvent ( QPaintEvent * event )
{
    QPainter painter(this);

    painter.setBackgroundMode(Qt::OpaqueMode);
    painter.setBackground(QBrush(QColor(214,214,214)));

    GamePadPtr gamePad = m_main->m_gamePad;
    ConfigurationPtr configuration = m_main->m_configuration;
    if(gamePad) {
        ImagePtr gamepadImage = gamePad->GetMainImage();

        if(gamepadImage) painter.drawImage(0,0,*gamepadImage);

        // get checked state of tap zones
        bool showTapZones = m_main->ui->bShowTapZones->isChecked();
        bool showKeys = m_main->ui->bShowKeys->isChecked();

        for(int i = 0; i < gamePad->GetNumButtons(); i++) {
            ButtonInfo& button = gamePad->GetButton(i);
             QRect rect(button.x,button.y,button.width,button.height);

            // blit tap zone
            if(showTapZones || m_selectedButton == i)
            {
                painter.fillRect(rect,QColor(0,0,255,128));
            }

            // blit keynames
            if(showKeys) {
                // displays keys for the given device's configuration
                ConfigurationKey pkey;
                ConfigurationValue pvalue;
                int keyCode[3];

                pkey.gamepadUUID = gamePad->GetUuid();
                pkey.buttonNo = i;

                if(configuration && configuration->GetValue(pkey,&pvalue)) {
                        keyCode[0] = pvalue.key[0];
                        keyCode[1] = pvalue.key[1];
                        keyCode[2] = pvalue.key[2];
                } else {
                        keyCode[0] = button.defaultKey[0];
                        keyCode[1] = button.defaultKey[1];
                        keyCode[2] = button.defaultKey[2];
                }

                std::string key = (keyCode[0] >= 0 ? KeySyms::MapKey(keyCode[0]) : "") +
                        (keyCode[1] >= 0 ? "+" + KeySyms::MapKey(keyCode[1]) : "") +
                        (keyCode[2] >= 0 ? "+" + KeySyms::MapKey(keyCode[2]) : "");

                painter.drawText(rect,Qt::AlignCenter | Qt::TextDontClip,key.c_str());
            }
        }

    }

    painter.end();
}

void GamePadView::mouseMoveEvent ( QMouseEvent * event )
{
    GamePadPtr gamePad = m_main->m_gamePad;
    if(!gamePad) return;

    // blit-tap zones
    for(int i = 0; i < gamePad->GetNumButtons(); i++) {
        ButtonInfo& button = gamePad->GetButton(i);

        if(event->x() >= button.x && event->x() <= button.x + button.width &&
           event->y() >= button.y && event->y() <= button.y + button.height) {
            m_selectedButton = i;
            repaint();
            return;
        }
    }

    if(m_selectedButton != -1) {
        m_selectedButton = -1;
        repaint();
    }
}

void GamePadView::mousePressEvent ( QMouseEvent * event )
{
    GamePadPtr gamePad = m_main->m_gamePad;
    if(!gamePad) return;

    if(m_selectedButton != -1) {
        int selected = m_selectedButton;
        ButtonInfo& button = gamePad->GetButton(selected);
        if(event->x() >= button.x && event->x() <= button.x + button.width &&
           event->y() >= button.y && event->y() <= button.y + button.height) {
           if(!m_main->m_configuration) {
                QMessageBox::information(NULL,"Cannot reconfigure the default configuration","To configure keys, please create or select a configuration.");
                return;
            }

            ButtonConfigDialog dialog;
            int ret = dialog.exec();
            if(ret == QDialog::Accepted) {
                ConfigurationKey pkey;
                ConfigurationValue pvalue;

                pkey.gamepadUUID = gamePad->GetUuid();
                pkey.buttonNo = selected;

                if(!m_main->m_configuration->GetValue(pkey,&pvalue)) {
                    ButtonInfo& button = gamePad->GetButton(selected);
                    pvalue.key[0] = button.defaultKey[0];
                    pvalue.key[1] = button.defaultKey[1];
                    pvalue.key[2] = button.defaultKey[2];
                }


                pvalue.key[0] = dialog.key1;
                pvalue.key[1] = dialog.key2;
                pvalue.key[2] = dialog.key3;

                m_main->m_configuration->SetValue(pkey,pvalue);
            } else if(ret == 2) {
                ConfigurationKey pkey;

                pkey.gamepadUUID = gamePad->GetUuid();
                pkey.buttonNo = selected;

                m_main->m_configuration->UnsetValue(pkey);
            }
            repaint();
        }
    }
}

void GamePadView::leaveEvent ( QEvent * event )
{
    m_selectedButton = -1;
    repaint();
}

void GamePadView::Refresh()
{
    repaint();
}
