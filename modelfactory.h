#ifndef MODELFACTORY_H
#define MODELFACTORY_H

#include "objmodel.h"
#include "treeinfo.h"
#include <QString>
#include <QMap>

class ModelFactory
{
public:
    static ModelFactory *Instance();
    static void FreeFactory();
    OBJModel* FetchModel(const TreeInfo &info);
    void FreeModel(const QString &name);
private:
    static ModelFactory *inst;
    OBJModel* loadModel(const TreeInfo &info);
    OBJModel* loadModel(const QString &name,
                        const QString &filepath);
    QMap<QString, OBJModel*> factory;
    QMap<QString, int>       userCount;
    ModelFactory();
    ~ModelFactory();
};

#endif // MODELFACTORY_H
