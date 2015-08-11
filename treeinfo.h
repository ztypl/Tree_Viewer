#ifndef TREEINFO_H
#define TREEINFO_H

#include <QString>

class TreeInfo
{
public:
    TreeInfo(const QString &name, const QString &path);
    ~TreeInfo();
    QString Name() { return name; }
    QString Path() { return path; }
    QString name;
    QString path;
};

#endif // TREEINFO_H
