#include "ui_NewConfigurationDialog.h"
#include "NewConfigurationDialog.h"

NewConfigurationDialog::NewConfigurationDialog() : ui(new Ui::NewConfigurationDialog)
{
    ui->setupUi(this);
}

NewConfigurationDialog::~NewConfigurationDialog()
{
    delete ui;
}

void NewConfigurationDialog::DoneDialog( int result )
{
    configName = ui->leConfigurationName->text().toStdString();
    copyCurrentConfig = ui->bCopyCurrentConfiguration->isChecked();
}
