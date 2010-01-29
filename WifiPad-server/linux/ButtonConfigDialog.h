#ifndef BUTTONCONFIGDIALOG_H
#define BUTTONCONFIGDIALOG_H

#include <QtGui/QDialog>

namespace Ui
{
    class ButtonConfigDialog;
}

class ButtonConfigDialog : public QDialog
{
    Q_OBJECT

private:
    Ui::ButtonConfigDialog *ui;
    int m_count;
protected:
    virtual void keyPressEvent ( QKeyEvent * event );
    virtual void keyReleaseEvent ( QKeyEvent * event );
public:
    int key1, key2, key3;
    
    ButtonConfigDialog();
    ~ButtonConfigDialog();

public slots:
    void Default();
	void Accept();
};

#endif // BUTTONCONFIGDIALOG_H
