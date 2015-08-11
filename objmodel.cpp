#include "objmodel.h"
#include "point3d.h"
#include "material.h"
#include <QFile>
#include <QTextStream>
#include <QVarLengthArray>
#include <QDebug>
#include <QtOpenGL>
#include <QImage>
#include <QTime>

OBJModel::OBJModel()
{
}

OBJModel::OBJModel(const QString &filepath)
{
    QTime t;
    t.start();
    Load(filepath);
    qDebug("Model Loaded in %d secs", t.elapsed());
}

double OBJModel::readNextDouble(char *str, int &s)
{
    int j = 0;
    char buf[20];
    while(str[s] == ' ' || str[s] == '\t')
        s++;
    while(str[s] && str[s] != ' ' && str[s] != '\t' && str[s] != '\n')
        buf[j++] = str[s++];

    buf[j] = 0;
    return atof(buf);
}

QString OBJModel::readNextString(char *buf, int &s)
{
    QString str = "";
    while(buf[s] == ' ' || buf[s] == '\t')
        s++;
    while(buf[s] && buf[s] != ' ' && buf[s] != '\t' && buf[s] != '\n')
        str.append(buf[s++]);

    return str;
}

bool OBJModel::Load(const QString &filepath)
{
    FILE *fp;
    fp = fopen(filepath.toLatin1().constData(), "r");
    if(fp == NULL)
    {
        qWarning("Failed to open obj file %s", filepath.toLatin1().constData());
        return false;
    }

    Point3d boundsMin( 1e9, 1e9, 1e9);
    Point3d boundsMax(-1e9,-1e9,-1e9);

    int faceCount = 0;

    char buf[256];
    while(fgets(buf, sizeof(buf), fp))
    {
        if(!buf[0] || buf[0] == '\n' || buf[0] == '#')
            continue;
        int s = 0;
        QString id = readNextString(buf, s);
        if(id=="v")
        {
            Point3d p;
            for(int i=0;i<3;i++)
            {
                p[i] = readNextDouble(buf, s);
                boundsMin[i] = qMin(boundsMin[i], p[i]);
                boundsMax[i] = qMax(boundsMax[i], p[i]);
            }
            m_points << p;
        }
        else if(id == "vt")
        {
            QVector2D t;
            for(int i=0;i<2;i++)
                t[i] = readNextDouble(buf, s);
            m_texcoords << t;
        }
        else if(id == "usemtl")
        {
            m_parts << faceCount;
            QString mtlname = readNextString(buf, s);

            for(int i=0;i<m_materials.size();i++)
                if(m_materials[i]->name == mtlname)
                {
                    m_matPointers<<m_materials[i];
                    break;
                }
        }
        else if(id == "mtllib")
        {
            QString mtlfp = readNextString(buf, s);
            readMTL(mtlfp);
        }
        else if(id == "f" || id == "fo")
        {
            faceCount++;

            QVarLengthArray<int, 4> p;
            QVarLengthArray<int, 4> t;
            while(buf[s] && buf[s] != '\n')
            {
                QString vertex = readNextString(buf, s);
                QStringList sp = vertex.split('/');

                const int vertexIndex = sp.value(0).toInt();
                if(vertexIndex != 0)
                    p.append(vertexIndex > 0 ? vertexIndex - 1 : m_points.size() + vertexIndex);
                if(sp.size()>1)
                {
                    const int texCoordIndex = sp.value(1).toInt();
                    if(texCoordIndex != 0)
                        t.append(texCoordIndex > 0 ? texCoordIndex - 1 : m_texcoords.size() + texCoordIndex);
                }
            }

            for(int i=0;i<3;i++)
                m_pointIndices << p[i];
            for(int i=0;i<3;i++)
                m_texcoordIndices << t[i];
        }

        const Point3d bounds = boundsMax - boundsMin;
    }

    min[0] = boundsMin[0]; min[1] = boundsMin[1]; min[2] = boundsMin[2];
    max[0] = boundsMax[0]; max[1] = boundsMax[1]; max[2] = boundsMax[2];

    if(m_pointIndices.size() == m_texcoordIndices.size())
    {
        for(int i=0;i<m_pointIndices.size();i++)
        {
            m_pointExpanded<<QVector3D(m_points[m_pointIndices.at(i)].x, m_points[m_pointIndices.at(i)].y, m_points[m_pointIndices.at(i)].z);
            m_texcoordExpanded<<QVector2D(m_texcoords[m_texcoordIndices.at(i)]);
        }
    }
    m_parts<<faceCount;

    return true;
}


void OBJModel::readMTL(const QString &filepath)
{
	/****************modified******************/
    QFile file("Model\\"+filepath);
	/****************modified******************/


    Material *currentMat = NULL;
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug("failed to open mtl file");
        return;
    }
    QTextStream in(&file);
    while(!in.atEnd())
    {
        QString input = in.readLine();
        QTextStream ts(&input);
        QString id;
        ts >> id;

        if(id == "newmtl")
        {
            if(currentMat!=NULL)
                m_materials<<currentMat;

            Material *mat = new Material();
            m_materials<<mat;
            QString matname;
            ts>>matname;
            mat->name = matname;
            currentMat = mat;
        }
        else if(id=="map_Kd")
        {
			/****************modified******************/
            QString fp = "Model\\";
            //ts>>fp;
			QString tmp; ts >> tmp;
			fp.append(tmp);
			/****************modified******************/

            QImage buf, tex;
            if(!buf.load(fp))
            {
                QImage dummy(128,128,QImage::Format_RGB32);
                dummy.fill(Qt::green);
                buf = dummy;
            }
            tex = QGLWidget::convertToGLFormat(buf);

            if(currentMat != NULL)
            {
                GLuint uid = 5;
                glGenTextures(1, &uid);
                glBindTexture(GL_TEXTURE_2D, uid);
                glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, tex.width(), tex.height(), 0,
                           GL_RGBA, GL_UNSIGNED_BYTE, tex.bits() );
                glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
                glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

                currentMat->map_Kd = uid;
            }
        }
    }
}

void OBJModel::Draw(int mode)
{
    if(mode == 0)
    {
        for(int part = 1; part<m_parts.size();part++)
        {
            glEnable(GL_DEPTH_TEST);
            glEnable(GL_ALPHA_TEST);
            glEnableClientState(GL_VERTEX_ARRAY);
            glEnableClientState(GL_NORMAL_ARRAY);
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            glEnable(GL_TEXTURE_2D);

            glBindTexture(GL_TEXTURE_2D, m_matPointers[part-1]->map_Kd);
            glVertexPointer(3, GL_FLOAT, 0, (float *)m_pointExpanded.data());
            glTexCoordPointer(2, GL_FLOAT, 0, (float *)m_texcoordExpanded.data());
            glNormalPointer(GL_FLOAT, 0, (float *)m_normals.data());

            glAlphaFunc(GL_GREATER, 0.5f);
            //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glDrawArrays(GL_TRIANGLES, m_parts[part-1] * ppf, (m_parts[part] - m_parts[part-1]) * ppf);
            //glDrawElements(GL_TRIANGLES, (m_parts[part] - m_parts[part-1]) * ppf, GL_UNSIGNED_INT, m_pointIndices.data() + m_parts[part-1] * ppf);

            glDisable(GL_TEXTURE_2D);
            glDisableClientState(GL_NORMAL_ARRAY);
            glDisableClientState(GL_VERTEX_ARRAY);
            glDisable(GL_ALPHA_TEST);
            glDisable(GL_DEPTH_TEST);
        }
    }

    if(mode == 1)
    {
        glEnable(GL_DEPTH_TEST);
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, (float *)m_points.data());

        glNormalPointer(GL_FLOAT, 0, (float *)m_normals.data());
        glDrawElements(GL_TRIANGLES, m_pointIndices.size(), GL_UNSIGNED_INT, m_pointIndices.data());
        glDisableClientState(GL_NORMAL_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisable(GL_DEPTH_TEST);
    }
}

OBJModel::~OBJModel()
{

}
