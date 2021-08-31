#ifndef ROTOR_H
#define ROTOR_H

#include <QString>

class Rotor
{
public:
    static Rotor* load(const QString& path);  // 从文件载入

public:
    Rotor();

public:
    bool dump(const QString& path);  // 保存到文件
};

#endif // ROTOR_H
