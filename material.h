#ifndef MATERIAL_H
#define MATERIAL_H

#include <QtOpenGL>

class Material
{
public:
    Material();
    ~Material();
    GLuint map_Kd;
    QString name;
};

#endif // MATERIAL_H
