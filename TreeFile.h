#ifndef TREEFILE
#define TREEFILE

#include <QDomDocument>
#include "objectfactory.h"
#include "treeviewwidget.h"
#include <exception>
#include <stdexcept>
#include "DBManager.h"
using std::range_error;
class TreeFile
{
private:
	QDomDocument doc;
	QString path;
	QString otherpath;
	QString imagepath;
public:
	void SetPath(QString p)
	{
		path = p;
	}
	QString GetPath()
	{
		return path;
	}
	void SetOtherPath(QString p)
	{
		otherpath = p;
	}
	QString GetOtherPath()
	{
		return otherpath;
	}
	void SetImagePath(QString p)
	{
		imagepath = p;
	}
	QString GetImagePath()
	{
		return imagepath;
	}
	TreeFile(QString p, QString ip) :path(p), imagepath(ip)
	{

	}
	TreeFile(QString p) :path(p)
	{

	}
	TreeFile(){}
	void CreateDocument(ObjectFactory* trees)
	{
		doc.clear();
		doc.appendChild(doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\""));
		QDomElement root = doc.createElement("File");
		doc.appendChild(root);
		QDomElement background = doc.createElement("background");
		QDomText back_text = doc.createTextNode(imagepath);
		background.appendChild(back_text);
		root.appendChild(background);

		QDomElement forest = doc.createElement("forest");
		const QMap<QString, Object*> * treemap = trees->Factory();
		QMap<QString, Object*>::ConstIterator it;
		for (it = treemap->begin(); it != treemap->end(); ++it)
		{
			QDomElement tree = doc.createElement("tree");

			QDomElement name = doc.createElement("name");
			QDomText name_text = doc.createTextNode(it.value()->GetName());
			name.appendChild(name_text);
			tree.appendChild(name);

			QDomElement modelname = doc.createElement("modelname");
			QDomText modelname_text = doc.createTextNode(it.value()->GetModelName());
			modelname.appendChild(modelname_text);
			tree.appendChild(modelname);

			QDomElement position = doc.createElement("position");
			QDomElement px = doc.createElement("x");
			QDomText px_text = doc.createTextNode(QString("%1").arg(it.value()->GetPosition().x()));
			px.appendChild(px_text);
			QDomElement py = doc.createElement("y");
			QDomText py_text = doc.createTextNode(QString("%1").arg(it.value()->GetPosition().y()));
			py.appendChild(py_text);
			QDomElement pz = doc.createElement("z");
			QDomText pz_text = doc.createTextNode(QString("%1").arg(it.value()->GetPosition().z()));
			pz.appendChild(pz_text);
			position.appendChild(px);
			position.appendChild(py);
			position.appendChild(pz);
			tree.appendChild(position);

			QDomElement angle = doc.createElement("angle");
			QDomElement ax = doc.createElement("x");
			QDomText ax_text = doc.createTextNode(QString("%1").arg(it.value()->GetEulerAngles().x()));
			ax.appendChild(ax_text);
			QDomElement ay = doc.createElement("y");
			QDomText ay_text = doc.createTextNode(QString("%1").arg(it.value()->GetEulerAngles().y()));
			ay.appendChild(ay_text);
			QDomElement az = doc.createElement("z");
			QDomText az_text = doc.createTextNode(QString("%1").arg(it.value()->GetEulerAngles().z()));
			az.appendChild(az_text);
			angle.appendChild(ax);
			angle.appendChild(ay);
			angle.appendChild(az);
			tree.appendChild(angle);

			QDomElement scale = doc.createElement("scale");
			QDomElement sx = doc.createElement("x");
			QDomText sx_text = doc.createTextNode(QString("%1").arg(it.value()->GetScale().x()));
			sx.appendChild(sx_text);
			QDomElement sy = doc.createElement("y");
			QDomText sy_text = doc.createTextNode(QString("%1").arg(it.value()->GetScale().y()));
			sy.appendChild(sy_text);
			QDomElement sz = doc.createElement("z");
			QDomText sz_text = doc.createTextNode(QString("%1").arg(it.value()->GetScale().z()));
			sz.appendChild(sz_text);
			scale.appendChild(sx);
			scale.appendChild(sy);
			scale.appendChild(sz);
			tree.appendChild(scale);

			forest.appendChild(tree);
		}
		root.appendChild(forest);

		QDomElement globalsetting = doc.createElement("globalsetting");
		root.appendChild(globalsetting);
	}

	void CreateXMLFile(QString filepath)
	{
		QString xmlFileName =  filepath;
		QFile file(xmlFileName);
		if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QFile::Text))
		{
			qDebug() << xmlFileName;
			file.close();
			return;
		}
		QTextStream out(&file);
		out.setCodec("UTF-8");
		doc.save(out, 4, QDomNode::EncodingFromTextStream);
		file.close();
	}

	void CreateXMLFile()
	{
		CreateXMLFile(this->path);
	}

	void ReadXMLFile(ObjectFactory* trees, TreeViewWidget* widget)
	{
		QFile file(path);
		if (!file.open(QFile::ReadOnly | QFile::Text)) {
			qDebug() << path;
			file.close();
			return;
		}
		QString strError;
		int errLin = 0, errCol = 0;
		if (!doc.setContent(&file, false, &strError, &errLin, &errCol)) {
			qDebug() << "parse file failed at line " << errLin << " column " << errCol << ", error: " << strError << " !";
			throw range_error("Parse File failed!");
		}
		if (doc.isNull())
		{
			FileIsBrokenExp();
		}

		QDomElement root = doc.documentElement();
		if (root.tagName()!="File")
		{
			FileIsBrokenExp();
		}

		QDomElement background = root.firstChildElement();
		if (!ElementIsLeagal(background, "background"))
		{
			FileIsBrokenExp();
		}
		imagepath = background.text();
		widget->LoadBGImage(background.text());
		QDomElement forest = background.nextSiblingElement();
		if (!ElementIsLeagal(forest, "forest"))
		{
			FileIsBrokenExp();
		}
		
		QDomElement tree = forest.firstChildElement();
		widget->SetTreeCount(0);
		while (ElementIsLeagal(tree, "tree"))
		{
			QDomElement name = tree.firstChildElement();
			if (!ElementIsLeagal(name, "name"))
			{
				FileIsBrokenExp();
			}
			QDomElement modelname = name.nextSiblingElement();
			if (!ElementIsLeagal(modelname, "modelname"))
			{
				FileIsBrokenExp();
			}
			QDomElement position = modelname.nextSiblingElement();
			if (!ElementIsLeagal(position, "position"))
			{
				FileIsBrokenExp();
			}
			QDomElement px = position.firstChildElement();
			if (!ElementIsLeagal(px, "x"))
			{
				FileIsBrokenExp();
			}
			QDomElement py = px.nextSiblingElement();
			if (!ElementIsLeagal(py, "y"))
			{
				FileIsBrokenExp();
			}
			QDomElement pz = py.nextSiblingElement();
			if (!ElementIsLeagal(pz, "z"))
			{
				FileIsBrokenExp();
			}
			QDomElement angle = position.nextSiblingElement();
			if (!ElementIsLeagal(angle, "angle"))
			{
				FileIsBrokenExp();
			}
			QDomElement ax = angle.firstChildElement();
			if (!ElementIsLeagal(ax, "x"))
			{
				FileIsBrokenExp();
			}
			QDomElement ay = ax.nextSiblingElement();
			if (!ElementIsLeagal(ay, "y"))
			{
				FileIsBrokenExp();
			}
			QDomElement az = ay.nextSiblingElement();
			if (!ElementIsLeagal(az, "z"))
			{
				FileIsBrokenExp();
			}
			QDomElement scale = angle.nextSiblingElement();
			if (!ElementIsLeagal(scale, "scale"))
			{
				FileIsBrokenExp();
			}
			QDomElement sx = scale.firstChildElement();
			if (!ElementIsLeagal(sx, "x"))
			{
				FileIsBrokenExp();
			}
			QDomElement sy = sx.nextSiblingElement();
			if (!ElementIsLeagal(sy, "y"))
			{
				FileIsBrokenExp();
			}
			QDomElement sz = sy.nextSiblingElement();
			if (!ElementIsLeagal(sz, "z"))
			{
				FileIsBrokenExp();
			}
			Object* obj = new Object();
			
			DBManager manager;
			manager.ConnectToDB();
			QString path = manager.FindPathByTreeName(modelname.text());
			
			if (obj->Load(TreeInfo(modelname.text(),path)))
			{
				obj->setName("TREE" + QString::number(widget->GetTreeCount()));
				widget->SetTreeCount(widget->GetTreeCount() + 1);
				obj->SetPosition(QVector3D(px.text().toFloat(), py.text().toFloat(), pz.text().toFloat()));
				obj->SetEulerAngles(QVector3D(ax.text().toFloat(), ay.text().toFloat(), az.text().toFloat()));
				obj->SetScale(QVector3D(sx.text().toFloat(), sy.text().toFloat(), sz.text().toFloat()));
				trees->AddObject(obj);
			}
			else
			{
				qDebug() << "Load Model Failed!";
				throw range_error("Load Model Failed!");
			}
			tree = tree.nextSiblingElement();
		}
	}
	void FileIsBrokenExp()
	{
		qDebug() << "File is broken!";
		throw range_error("File is broken!");
	}

	bool ElementIsLeagal(QDomElement x, QString s)
	{
		return !x.isNull() && x.tagName() == s;
	}
};

#endif