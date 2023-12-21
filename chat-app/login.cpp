#include "login.h"
#include "./ui_login.h"

login::login (QWidget *parent) : QMainWindow (parent), ui (new Ui::login)
{
  ui->setupUi (this);
}

login::~login () { delete ui; }

void login::on_pushButton_login_clicked()
{
  QString Name = ui->lineEdit_Name->text();
  QString UserName = ui->lineEdit_User_Name->text();

  if (Name== "" || UserName == ""){
      QMessageBox::warning(this, "", "Name or Username cannot be left empty");
    }

  else {
      QMessageBox::information(this,UserName,"Welcome to Digital Chautari "+ UserName);
    }
}


void login::on_pushButton_exit_clicked()
{
  QMessageBox::StandardButton reply;
  reply = QMessageBox::question(this,"","Are you sure to close this application", QMessageBox:: Yes | QMessageBox:: No);

  if (reply == QMessageBox::Yes){
      QApplication::quit();
    }

}

