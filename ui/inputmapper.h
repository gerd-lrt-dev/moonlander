#ifndef INPUTMAPPER_H
#define INPUTMAPPER_H

#include <QObject>

class inputmapper : public QObject
{
    Q_OBJECT
public:
    explicit inputmapper(QObject *parent = nullptr);

signals:
};

#endif // INPUTMAPPER_H
