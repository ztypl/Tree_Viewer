#ifndef OBJMODEL_H
#define OBJMODEL_H

#include <QString>
#include <QtOpenGL>
#include <QOpenGLShaderProgram>
#include "point3d.h"
#include "material.h"

class OBJModel
{
public:
    OBJModel();
    OBJModel(const QString &filepath);
    bool Load(const QString &filepath);
    //void SetUniformValues(GLuint *names, void **values);
    void Draw(int mode = 0);
    QVector3D Min() const { return min; }
    QVector3D Max() const { return max; }
    ~OBJModel();
private:
    GLuint bufferId;
    QVector<Point3d> m_points;
    QVector<QVector2D> m_texcoords;
    QVector<Point3d> m_normals;
    QVector<int> m_edgeIndices;
    QVector<int> m_texcoordIndices;
    QVector<QVector2D> m_texcoordExpanded;
    QVector<int> m_pointIndices;
    QVector<QVector3D> m_pointExpanded;
    QVector<int> m_parts;
    QVector<Material*> m_matPointers;
    QVector<Material*> m_materials;
    void readMTL(const QString &filepath);
    int ppf = 3;

    double readNextDouble(char *buf, int &s);
    QString readNextString(char *buf, int &s);

    QVector3D max, min;
};

#endif // OBJMODEL_H
