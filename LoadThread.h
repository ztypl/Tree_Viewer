#ifndef LOAD_THREAD_H
#define LOAD_THREAD_H

#include <QThread>
#include <QObject>
#include "loaddialog.h"
#include "treeviewwidget.h"
#include <QString>
#include "treeinfo.h"
#include <QMutex>

class LoadThread : public QThread
{
	Q_OBJECT
public:
	LoadThread(QObject *parent = 0) : QThread(parent)
	{

	}
	~LoadThread()
	{

	}
	void SetWindow(LoadDialog *d)
	{
		dialog = d;
	}
protected:
	void run()
	{

	}
private:
	QMutex mutex;
	LoadDialog *dialog;
};


#endif