#ifndef GAMECNTROLLER_H
#define GAMECNTROLLER_H

#include "Utils/Utils.h"


class GameController : public QObject {
    Q_OBJECT

public:
    explicit GameController(QObject *parent = nullptr)
        : QObject(parent)
    {}

signals:


public slots:

private:

};


#endif // GAMECNTROLLER_H
