#ifndef PIETCOMPILER_H
#define PIETCOMPILER_H

#include <QObject>

class PietCompiler : public QObject
{
    Q_OBJECT
public:
    explicit PietCompiler(QObject *parent = nullptr);

signals:

};

#endif // PIETCOMPILER_H
