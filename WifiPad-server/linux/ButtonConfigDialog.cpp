#include <QtGui/QKeyEvent>
#include "KeySyms.h"
#include "ui_ButtonConfigDialog.h"
#include "ButtonConfigDialog.h"

using namespace WifiPad;

ButtonConfigDialog::ButtonConfigDialog() : ui(new Ui::ButtonConfigDialog()), m_count(0)
{
    ui->setupUi(this);
}

ButtonConfigDialog::~ButtonConfigDialog()
{
    delete ui;
}

void ButtonConfigDialog::Default()
{
    done(2);
}

void ButtonConfigDialog::keyPressEvent ( QKeyEvent * event )
{
    int sym = event->nativeVirtualKey();
    if(sym >= 0 && !event->isAutoRepeat()) {
        switch(m_count) {
            case 0: ui->leKey1->setText(KeySyms::MapKey(sym).c_str()); break;
            case 1: ui->leKey2->setText(KeySyms::MapKey(sym).c_str()); break;
            case 2: ui->leKey3->setText(KeySyms::MapKey(sym).c_str()); break;
        }
        m_count++;
    }
}

void ButtonConfigDialog::keyReleaseEvent ( QKeyEvent * event )
{
    if(!event->isAutoRepeat()) m_count--;
    if(m_count <= 0) {
        key1 = KeySyms::MapSymbol(ui->leKey1->text().toStdString());
        key2 = KeySyms::MapSymbol(ui->leKey2->text().toStdString());
        key3 = KeySyms::MapSymbol(ui->leKey3->text().toStdString());
        accept();
    }
}

