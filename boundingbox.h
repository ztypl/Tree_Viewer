#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include <QVector>
#include <QVector3D>
#include <QMatrix4x4>

class BoundingBox
{
public:
    BoundingBox();
    ~BoundingBox();

    bool Intersect(const QMatrix4x4 &modelview, const QVector3D &orig, const QVector3D &dir, bool cull_back = false);
    void Initialize(const QVector3D &min, const QVector3D &max);
    QVector<QVector3D> Triangles() { return triangles; }
private:
    const double EPSILON = 0.000001;
    void cross(QVector3D &dest,
               const QVector3D &v1,
               const QVector3D &v2);
    double dot(const QVector3D &v1,
               const QVector3D &v2);
    void sub(QVector3D &dest,
             const QVector3D &v1,
             const QVector3D &v2);
    bool intersect_triangle(const QVector3D &orig,
                            const QVector3D &dir,
                            const QVector3D &vert0,
                            const QVector3D &vert1,
                            const QVector3D &vert2,
                            bool cull_back = false);

    QVector<QVector3D> triangles;
};

#endif // BOUNDINGBOX_H
