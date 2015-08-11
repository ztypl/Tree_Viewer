#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlTableModel>
#include "TreeFile.h"
#include "DBManager.h"
#include <QVector>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void connectDB();
    void queryDB();
    void keyPressEvent(QKeyEvent  *event);
    //QSqlTableModel *model2;
    QStandardItemModel *model;

    ~MainWindow();
    void resizeEvent(QResizeEvent *event);

private slots:


    //void on_action_2_triggered();

    //void on_action_3_triggered();

   // void on_action_4_triggered();

    //void on_action_triggered();


    //void on_pushButton_clicked();

    //void on_tableView_doubleClicked(const QModelIndex &index);

    //void on_tableView_clicked(const QModelIndex &index);

    void on_action_move_triggered();

    void on_action_rotate_triggered();

    void on_action_zoom_triggered();

    void on_action_open_triggered();

    void on_action_new_triggered();

    void on_action_save_triggered();

    void on_action_othersave_triggered();

    void on_action_undo_triggered();

    void on_action_redo_triggered();

    void on_treeView_clicked(const QModelIndex &index);

	void on_pushed();

    void on_treeView_doubleClicked(const QModelIndex &index);

	void on_new_file(QString imagepath, QString projpath, QString projname);

    void on_action_output_triggered();

	void ButtomDock(QVector3D position, QVector3D angle, QVector2D scale, QString Mname);

private:
    Ui::MainWindow *ui;
    TreeFile thefile;
	QString projname="";
};

#endif // MAINWINDOW_H
