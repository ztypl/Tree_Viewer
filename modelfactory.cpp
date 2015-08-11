#include "modelfactory.h"

ModelFactory* ModelFactory::inst = NULL;
ModelFactory::ModelFactory()
{

}

ModelFactory::~ModelFactory()
{

}

ModelFactory* ModelFactory::Instance()
{
    if(inst == NULL)
        inst = new ModelFactory();

    return inst;
}

OBJModel* ModelFactory::FetchModel(const TreeInfo &info)
{
    QMap<QString, OBJModel*>::iterator it;
    it = factory.find(info.name);
    if(it != factory.end() && it.key() == info.name)
    {
        // model already in factory
        QMap<QString, int>::iterator i = userCount.find(info.name);
        if(i!=userCount.end())
            i.value()++;
        else userCount.insert(info.name, 1);
        return it.value();
    }
    else
    {
        userCount.insert(info.name, 1);
        return loadModel(info.name, info.path);
    }
}

OBJModel* ModelFactory::loadModel(const TreeInfo &info)
{
    return loadModel(info.name, info.path);
}

OBJModel* ModelFactory::loadModel(const QString &name, const QString &filepath)
{
    OBJModel *mdl = new OBJModel();
    factory.insert(name, mdl);
    if(mdl->Load(filepath))
        return mdl;
    else
    {
        delete mdl;
        factory.remove(name);
        return NULL;
    }
}

void ModelFactory::FreeModel(const QString &name)
{
    QMap<QString, int>::iterator it;
    it = userCount.find(name);
    if(it != userCount.end() && it.key() == name)
        it.value()--;

    if(it.value()<=0)
    {
        userCount.remove(name);
        factory.remove(name);
    }
}
