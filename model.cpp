#include "model.h"

#include <QFile>
#include <QTextStream>
#include <QVarLengthArray>
#include <QDebug>
#include <QtOpenGL>

Model::Model(const QString &filePath)
    : m_fileName(QFileInfo(filePath).fileName())
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug("failed to open file");
        return;
    }
    Point3d boundsMin( 1e9, 1e9, 1e9);
    Point3d boundsMax(-1e9,-1e9,-1e9);

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString input = in.readLine();
        if (input.isEmpty() || input[0] == '#')
            continue;

        QTextStream ts(&input);
        QString id;
        ts >> id;
        if (id == "v") {
            Point3d p;
            for (int i = 0; i < 3; ++i) {
                ts >> p[i];
                boundsMin[i] = qMin(boundsMin[i], p[i]);
                boundsMax[i] = qMax(boundsMax[i], p[i]);
            }
            m_points << p;
        } else if (id == "f" || id == "fo") {
            QVarLengthArray<int, 4> p;

            while (!ts.atEnd()) {
                QString vertex;
                ts >> vertex;
                QStringList sp = vertex.split('/');
                const int vertexIndex = sp.value(0).toInt();
                if(sp.size()>1)
                {
                    const int texCoordIndex = sp.value(1).toInt();
                    if (texCoordIndex)
                        ;//p.append(texCoordIndex > 0 ? texCoordIndex - 1 : m_points.size() + texCoordIndexIndex);
                }
                if (vertexIndex)
                    p.append(vertexIndex > 0 ? vertexIndex - 1 : m_points.size() + vertexIndex);
            }

            for (int i = 0; i < p.size(); ++i) {
                const int edgeA = p[i];
                const int edgeB = p[(i + 1) % p.size()];

                if (edgeA < edgeB)
                    m_edgeIndices << edgeA << edgeB;
            }

            for (int i = 0; i < 3; ++i)
                m_pointIndices << p[i];

            if (p.size() == 4)
                for (int i = 0; i < 3; ++i)
                    m_pointIndices << p[(i + 2) % 4];
        }
    }

    const Point3d bounds = boundsMax - boundsMin;
    const qreal scale = 1 / qMax(bounds.x, qMax(bounds.y, bounds.z));
    for (int i = 0; i < m_points.size(); ++i)
        m_points[i] = (m_points[i] - (boundsMin + bounds * 0.5)) * scale;

    m_normals.resize(m_points.size());
    for (int i = 0; i < m_pointIndices.size(); i += 3) {
        const Point3d a = m_points.at(m_pointIndices.at(i));
        const Point3d b = m_points.at(m_pointIndices.at(i+1));
        const Point3d c = m_points.at(m_pointIndices.at(i+2));

        const Point3d normal = cross(b - a, c - a).normalize();

        for (int j = 0; j < 3; ++j)
            m_normals[m_pointIndices.at(i + j)] += normal;
    }

    for (int i = 0; i < m_normals.size(); ++i)
        m_normals[i] = m_normals[i].normalize();
}

void Model::render(bool wireframe, bool normals) const
{
    qDebug("rendering %d vertices", m_pointIndices.size());
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_ALPHA_TEST);
    glEnableClientState(GL_VERTEX_ARRAY);
    if (wireframe) {
        glVertexPointer(3, GL_FLOAT, 0, (float *)m_points.data());
        glDrawElements(GL_LINES, m_edgeIndices.size(), GL_UNSIGNED_INT, m_edgeIndices.data());
    } else {
        glEnableClientState(GL_NORMAL_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, (float *)m_points.data());
        glNormalPointer(GL_FLOAT, 0, (float *)m_normals.data());
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDrawElements(GL_TRIANGLES, m_pointIndices.size(), GL_UNSIGNED_INT, m_pointIndices.data());

        glDisableClientState(GL_NORMAL_ARRAY);
    }
    if (normals) {
        QVector<Point3d> normals;
        for (int i = 0; i < m_normals.size(); ++i)
            normals << m_points.at(i) << (m_points.at(i) + m_normals.at(i) * 0.02f);
        glVertexPointer(3, GL_FLOAT, 0, (float *)normals.data());
        glDrawArrays(GL_LINES, 0, normals.size());
    }
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisable(GL_ALPHA_TEST);
    glDisable(GL_DEPTH_TEST);
}
