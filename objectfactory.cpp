#include "objectfactory.h"
#include <QPair>
ObjectFactory::ObjectFactory()
{
    objectCount = 0;
    factory.clear();
}

ObjectFactory::~ObjectFactory()
{
    QMap<QString, Object*>::iterator it;
    for(it = factory.begin(); it != factory.end(); it++)
        delete it.value();
}

void ObjectFactory::AddObject(Object *obj)
{
    factory.insert(obj->Name(), obj);
}

void ObjectFactory::RemoveObject(Object *obj)
{
    QMap<QString, Object*>::iterator it;
    it = factory.find(obj->Name());
    delete it.value();
    factory.remove(obj->Name());
}

void ObjectFactory::RemoveObject(const QString &name)
{
    QMap<QString, Object*>::iterator it;
    it = factory.find(name);
    delete it.value();
    factory.remove(name);
}

Object* ObjectFactory::FindObjectByName(const QString &name) const
{
    QMap<QString, Object*>::const_iterator it;
    it = factory.find(name);
    if(it == factory.end())
        return NULL;
    return it.value();
}
