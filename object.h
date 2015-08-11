#ifndef OBJECT_H
#define OBJECT_H

#include <QVector>
#include <QString>
#include <QMatrix>
#include "objmodel.h"
#include "modelfactory.h"
#include "boundingbox.h"

class Object
{
public:
    Object();
    ~Object();

    bool Load(const TreeInfo &info);
    void Draw(int mode);
    void ToggleSelected(bool on);
    void Rotate(float angle, const QVector3D &ref);
    void Translate(const QVector3D &vec);
    void Translate2D(const QVector2D &vec, double kx, double ky);
    void Scale(const QVector3D &vec);
    void Scale(const QVector2D &vec_wh);

    QString Name() const { return name; }
    void setName(QString name){ this->name = name; }
    bool isSelected() const { return selected; }
    BoundingBox* Bounding() const {return boundingBox;}

    /* local transform */
    QMatrix4x4 ModelViewMatrix() const {return modelViewMatrix;}
    /* global transform */
    void SetPosition(const QVector3D &v);
    void SetEulerAngles(const QVector3D &v);
    void SetScale(const QVector3D &v);

	QVector3D GetPosition() const { return position; }
	QVector3D GetEulerAngles() const { return eulerAngles; }
	QVector3D GetScale() const { return scale; }
	QString GetName() const { return name; }
	QString GetModelName() const { return modelName; }

    void Free();
private:

    void updateModelViewMatrix();
    bool selected;
    void drawBoundingBox();
    QMatrix4x4 modelViewMatrix; // if we use local coodinates to do transformation

    /* if we use global cooridnates to do the trans */
    QVector3D scale;
    QVector3D eulerAngles;
    QVector3D position;

    QString name;
    BoundingBox *boundingBox = NULL;
    OBJModel* model;

    QString modelName;

};

#endif // OBJECT_H
