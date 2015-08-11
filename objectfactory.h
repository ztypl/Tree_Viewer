#ifndef OBJECTFACTORY_H
#define OBJECTFACTORY_H

#include "object.h"
#include <QMap>
#include <QVector>
#include <QString>

class ObjectFactory
{
public:
    ObjectFactory();
    ~ObjectFactory();

    Object* FindObjectByName(const QString &name) const;
    void AddObject(Object *obj);
    void RemoveObject(Object *obj);
    void RemoveObject(const QString &name);
    const QMap<QString, Object*> *Factory() const {return &factory;}
private:
    int objectCount;
    QMap<QString, Object*> factory;
    //QList<Object*> objList;
};

#endif // OBJECTFACTORY_H
