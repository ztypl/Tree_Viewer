#include "newfrom.h"
#include "ui_newfrom.h"
#include<QFileDialog>
#include<QMessageBox>
#include<QTextCodec>
NewFrom::NewFrom(QWidget *parent) :
QWidget(parent),
ui(new Ui::NewFrom)
{
	ui->setupUi(this);
	resize(800, 600);
	this->image = new QImage();
	connect(ui->OpenImageBtn, SIGNAL(clicked()), this, SLOT(on_slotOpenImage_triggered()));
}
void NewFrom::on_slotOpenImage_triggered()
{
	QString fileName = QFileDialog::getOpenFileName(
		this, "open image file",
		".",
		"Image files (*.bmp *.jpg *.pbm *.pgm *.png *.ppm *.xbm *.xpm);;All files (*.*)");
	if (fileName != "")
	{
		ui->select->setText(fileName);
		if (image->load(fileName))
		{
			ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
			ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);


			QGraphicsScene *scene = new QGraphicsScene;
			QPixmap map = QPixmap::fromImage(*image);




			QPixmap sized = map.scaled(
				QSize(ui->graphicsView->width(),
				ui->graphicsView->height()),
				Qt::KeepAspectRatio);


			scene->addPixmap(sized);


			ui->graphicsView->setScene(scene);

			ui->graphicsView->show();
		}
	}
}
NewFrom::~NewFrom()
{
	delete image;
	delete ui;
}

/*void NewFrom::on_pushButton_3_clicked()
{
QString path = QFileDialog::getOpenFileName(this, tr("Open"), ".", tr("Image Files(*.jpg *.png)"));

}*/

void NewFrom::on_pushButton_3_clicked()
{
	QString newpath = QFileDialog::getExistingDirectory();
	ui->savePos->setText(newpath);
}

void NewFrom::on_pushButton_clicked()
{
	QTextCodec* code = QTextCodec::codecForName("GB2312");
	if (IsStringValid())
	{
		emit return_string(ui->select->text(), ui->savePos->text(), ui->proName->text());
		this->close();
	}
	else if (ui->select->text() == "")
	{
		QMessageBox::critical(0, "critical message", code->toUnicode("��ѡ����Ŀ����ͼƬ��"), QMessageBox::Ok, 0);
	}
	else if (ui->savePos->text() == "")
	{
		QMessageBox::critical(0, "critical message", code->toUnicode("��ѡ����Ŀ�洢�ļ��У�"), QMessageBox::Ok, 0);
	}
	else if (ui->proName->text() == "")
	{
		QMessageBox::critical(0, "critical message", code->toUnicode("��ѡ����Ŀ���ƣ�"), QMessageBox::Ok, 0);
	}
}

void NewFrom::on_pushButton_2_clicked()
{
	this->close();
}

bool NewFrom::IsStringValid()
{
	if (ui->select->text() != ""&&ui->savePos->text() != ""&&ui->proName->text() != "")
	{
		return true;
	}
	else return false;
}