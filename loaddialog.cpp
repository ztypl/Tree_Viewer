#include "loaddialog.h"
#include "ui_loaddialog.h"

#include <QTextCodec>
#include <QDebug>

LoadDialog::LoadDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoadDialog)
{
    ui->setupUi(this);
	QTextCodec* code = QTextCodec::codecForName("GB2312");
	//this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
	setWindowTitle("Loading Model");
	setWindowFlags(Qt::WindowTitleHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint);
	ui->label->setText(code->toUnicode("模型文件较大，初次加载需要数秒，请稍候"));

	this->close();
}

LoadDialog::~LoadDialog()
{
    delete ui;
}
