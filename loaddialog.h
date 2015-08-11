#ifndef LOADDIALOG_H
#define LOADDIALOG_H

#include <QDialog>

namespace Ui {
class LoadDialog;
}

class LoadDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoadDialog(QWidget *parent = 0);
    ~LoadDialog();

private:
    Ui::LoadDialog *ui;
};

#endif // LOADDIALOG_H
