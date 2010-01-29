#include <QtGui/QKeyEvent>
#include "KeySyms.h"
#include "ui_ButtonConfigDialog.h"
#include "ButtonConfigDialog.h"

using namespace WifiPad;

ButtonConfigDialog::ButtonConfigDialog() : ui(new Ui::ButtonConfigDialog()), m_count(0)
{
    ui->setupUi(this);
	QComboBox *c[] = { ui->cbKey1, ui->cbKey2, ui->cbKey3 };
	for(int i = 0; c[i]; i++) {
		c[i]->addItem("");
		c[i]->addItem("MOUSEUP");
		c[i]->addItem("MOUSEDOWN");
		c[i]->addItem("MOUSELEFT");
		c[i]->addItem("MOUSERIGHT");
		c[i]->addItem("LBUTTON");
		c[i]->addItem("RBUTTON");
		c[i]->addItem("MBUTTON");
	}
}

ButtonConfigDialog::~ButtonConfigDialog()
{
    delete ui;
}

void ButtonConfigDialog::Default()
{
    done(2);
}

void ButtonConfigDialog::Accept()
{
	key1 = KeySyms::MapSymbol(ui->cbKey1->currentText().toStdString());
	key2 = KeySyms::MapSymbol(ui->cbKey2->currentText().toStdString());
	key3 = KeySyms::MapSymbol(ui->cbKey3->currentText().toStdString());
    accept();
}

void ButtonConfigDialog::keyPressEvent ( QKeyEvent * event )
{
    int sym = event->nativeVirtualKey();
    if(sym >= 0 && !event->isAutoRepeat()) {
        switch(m_count) {
            case 0: ui->cbKey1->setEditText(KeySyms::MapKey(sym).c_str()); break;
            case 1: ui->cbKey2->setEditText(KeySyms::MapKey(sym).c_str()); break;
            case 2: ui->cbKey3->setEditText(KeySyms::MapKey(sym).c_str()); break;
        }
        m_count++;
    }
}

void ButtonConfigDialog::keyReleaseEvent ( QKeyEvent * event )
{
    if(!event->isAutoRepeat()) m_count--;
    if(m_count <= 0) {
        key1 = KeySyms::MapSymbol(ui->cbKey1->currentText().toStdString());
        key2 = KeySyms::MapSymbol(ui->cbKey2->currentText().toStdString());
        key3 = KeySyms::MapSymbol(ui->cbKey3->currentText().toStdString());
        accept();
    }
}

