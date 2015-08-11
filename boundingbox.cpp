#include "boundingbox.h"

BoundingBox::BoundingBox()
{

}

BoundingBox::~BoundingBox()
{

}

void BoundingBox::Initialize(const QVector3D &min, const QVector3D &max)
{
    triangles.clear();

    // 0,1,2 3,4,5 - front
    triangles << QVector3D(min[0], max[1], max[2])
              << QVector3D(min[0], min[1], max[2])
              << QVector3D(max[0], min[1], max[2]);
    triangles << QVector3D(max[0], min[1], max[2])
              << QVector3D(max[0], max[1], max[2])
              << QVector3D(min[0], max[1], max[2]);

    // 6,7,8 9,10,11 - back
    triangles << QVector3D(max[0], min[1], min[2])
              << QVector3D(min[0], min[1], min[2])
              << QVector3D(min[0], max[1], min[2]);
    triangles << QVector3D(min[0], max[1], min[2])
              << QVector3D(max[0], max[1], min[2])
              << QVector3D(max[0], min[1], min[2]);

    // 12,13,14 15,16,17 - left
    triangles << QVector3D(min[0], max[1], min[2])
              << QVector3D(min[0], min[1], min[2])
              << QVector3D(min[0], min[1], max[2]);
    triangles << QVector3D(min[0], min[1], max[2])
              << QVector3D(min[0], max[1], max[2])
              << QVector3D(min[0], max[1], min[2]);

    // 18,19,20 21,22,23 - right
    triangles << QVector3D(max[0], max[1], min[2])
              << QVector3D(max[0], max[1], max[2])
              << QVector3D(max[0], min[1], max[2]);
    triangles << QVector3D(max[0], min[1], max[2])
              << QVector3D(max[0], min[1], min[2])
              << QVector3D(max[0], max[1], min[2]);

    // 24,25,26 27,28,29 - top
    triangles << QVector3D(min[0], max[1], min[2])
              << QVector3D(min[0], max[1], max[2])
              << QVector3D(max[0], max[1], max[2]);
    triangles << QVector3D(max[0], max[1], max[2])
              << QVector3D(max[0], max[1], min[2])
              << QVector3D(min[0], max[1], min[2]);

    // 30,31,32 33,34,35 - bottom
    triangles << QVector3D(min[0], min[1], min[2])
              << QVector3D(max[0], min[1], min[2])
              << QVector3D(max[0], min[1], max[2]);
    triangles << QVector3D(max[0], min[1], max[2])
              << QVector3D(min[0], min[1], max[2])
              << QVector3D(min[0], min[1], min[2]);
}

bool BoundingBox::Intersect(const QMatrix4x4 &modelview, const QVector3D &orig,
                            const QVector3D &dir, bool cull_back)
{
    bool flag = false;

    for(int i=0;i<12;i++)
    {
        QVector4D v1,v2,v3;
        v1 = modelview * QVector4D(triangles[i*3], 1.0);
        v2 = modelview * QVector4D(triangles[i*3+1], 1.0);
        v3 = modelview * QVector4D(triangles[i*3+2], 1.0);
        flag = intersect_triangle(orig, dir, v1.toVector3D(), v2.toVector3D(), v3.toVector3D(), cull_back);
        if(flag)break;
    }
    return flag;
}

void BoundingBox::cross(QVector3D &dest,
           const QVector3D &v1,
           const QVector3D &v2)
{
    dest[0] = v1[1] * v2[2] - v1[2] * v2[1];
    dest[1] = v1[2] * v2[0] - v1[0] * v2[2];
    dest[2] = v1[0] * v2[1] - v1[1] * v2[0];
}


double BoundingBox::dot(const QVector3D &v1,
           const QVector3D &v2)
{
    return (v1[0]*v2[0] + v1[1]*v2[1] + v1[2]*v2[2]);
}


void BoundingBox::sub(QVector3D &dest,
         const QVector3D &v1,
         const QVector3D &v2)
{
    dest[0] = v1[0] - v2[0];
    dest[1] = v1[1] - v2[1];
    dest[2] = v1[2] - v2[2];
}

bool BoundingBox::intersect_triangle(const QVector3D &orig,
                        const QVector3D &dir,
                        const QVector3D &vert0,
                        const QVector3D &vert1,
                        const QVector3D &vert2,
                        bool cull_back)
{
    /* algorithm from MT97 by Tomas Moller */
    QVector3D edge1, edge2, tvec, pvec, qvec;
    double det, inv_det;
    double u, v;

    sub(edge1, vert1, vert0);
    sub(edge2, vert2, vert0);
    cross(pvec, dir, edge2);
    det = dot(edge1, pvec);

    if(cull_back)
    {
        if(det < EPSILON)
            return true;

        sub(tvec, orig, vert0);
        u = dot(tvec, pvec);
        if(u<0.0 || u>det)
            return false;

        cross(qvec, tvec, edge1);
        v = dot(dir, qvec);
        if(v<0.0 || u+v>det)
            return false;
    }
    else
    {
        if(det > -EPSILON && det < EPSILON)
            return false;

        inv_det = 1.0 / det;

        sub(tvec, orig, vert0);

        u = dot(tvec, pvec) * inv_det;
        if(u<0.0 || u>1.0)  // comparing method changed
            return false;

        cross(qvec, tvec, edge1);
        v = dot(dir, qvec) * inv_det;
        if(v<0.0 || u+v>1.0)
            return false;
    }
    return true;
}
