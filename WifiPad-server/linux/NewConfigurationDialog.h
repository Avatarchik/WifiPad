#ifndef NEWCONFIGURATIONDIALOG_H
#define NEWCONFIGURATIONDIALOG_H

#include <QtGui/QDialog>

namespace Ui
{
    class NewConfigurationDialog;
}

class NewConfigurationDialog : public QDialog
{
    Q_OBJECT

private:
    Ui::NewConfigurationDialog *ui;
public:
    std::string configName;
    bool copyCurrentConfig;

    NewConfigurationDialog();
    virtual ~NewConfigurationDialog();
public slots:
    void DoneDialog( int result );
};

#endif // NEWCONFIGURATIONDIALOG_H
