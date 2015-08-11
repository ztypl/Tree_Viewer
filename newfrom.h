#ifndef NEWFROM_H
#define NEWFROM_H

#include <QWidget>
#include <QImage>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QFileDialog>
namespace Ui {
class NewFrom;
}

class NewFrom : public QWidget
{
    Q_OBJECT

public:
    explicit NewFrom(QWidget *parent = 0);
    ~NewFrom();

private slots:
    //void on_pushButton_3_clicked();
    void on_slotOpenImage_triggered();
    void on_pushButton_3_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

signals:
	void return_string(QString, QString, QString);
private:
    Ui::NewFrom *ui;
    QImage *image;
	bool IsStringValid();
};

#endif // NEWFROM_H
