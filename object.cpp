#include "object.h"

Object::Object()
{
    model = NULL;
    name = "undefined";
    selected = false;
    boundingBox = new BoundingBox();

    scale = QVector3D(1.0,1.0,1.0);
    position = QVector3D(0.0,0.0,-10.0);
    eulerAngles = QVector3D(0.0,0.0,0.0);

    updateModelViewMatrix();
}

Object::~Object()
{
    if(boundingBox != NULL)
        delete boundingBox;

    if(modelName != "")
        ModelFactory::Instance()->FreeModel(modelName);
}

bool Object::Load(const TreeInfo &info)
{
    if(model != NULL)
        return false;

    model = ModelFactory::Instance()->FetchModel(info);
    if(model == NULL) return false;

    modelName = info.name;
    boundingBox->Initialize(model->Min(), model->Max());
    return true;
}

void Object::Draw(int mode)
{
    //only if there's LoadIdentity outside it would be correct !
    glPushMatrix();
    glMultMatrixf(modelViewMatrix.constData());

    if(model != NULL)
        model->Draw(0);

    if(selected)
        drawBoundingBox();
    glPopMatrix();
}

void Object::ToggleSelected(bool on)
{
    selected = on;
}

void Object::drawBoundingBox()
{
    // draw OBJModel's bounding box;
    QVector<QVector3D> triangles = boundingBox->Triangles();

    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_LINE_STRIP);
        for(int i=0;i<36;i++)
        {
            GLdouble t[3]{ triangles[i][0], triangles[i][1], triangles[i][2] };
            glVertex3dv(t);
        }
    glEnd();
    glColor3f(1.0, 1.0, 1.0);
}

void Object::Rotate(float angle, const QVector3D &ref)
{
    if(ref.x() - 1.0 > -0.000001 && ref.x() - 1.0 <= 0.000001)
        eulerAngles[0] += angle;

    if(ref.y() - 1.0 > -0.000001 && ref.y() - 1.0 <= 0.000001)
        eulerAngles[1] += angle;

    updateModelViewMatrix();
}

void Object::Translate(const QVector3D &vec)
{
    position += vec;
    updateModelViewMatrix();
}

void Object::Translate2D(const QVector2D &vec, double kx, double ky)
{
    Translate(QVector3D(vec.x()*kx*position.z(), vec.y()*ky*position.z(), 0));
}

void Object::Scale(const QVector3D &vec)
{
    scale = vec;
    updateModelViewMatrix();
}

void Object::Scale(const QVector2D &vec_wh)
{
    scale = QVector3D(vec_wh.x(), vec_wh.y(), vec_wh.x());
    updateModelViewMatrix();
}

void Object::updateModelViewMatrix()
{
    modelViewMatrix.setToIdentity();
    modelViewMatrix.translate(position);
    modelViewMatrix.rotate(eulerAngles.x(), 1.0, 0.0, 0.0);
    modelViewMatrix.rotate(eulerAngles.y(), 0.0, 1.0, 0.0);
    modelViewMatrix.scale(scale);
}


void Object::SetPosition(const QVector3D &v)
{
    position = v;
    updateModelViewMatrix();
}

void Object::SetEulerAngles(const QVector3D &v)
{
    eulerAngles = v;
    updateModelViewMatrix();
}

void Object::SetScale(const QVector3D &v)
{
    scale = v;
    updateModelViewMatrix();
}
