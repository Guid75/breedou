#ifndef BULLET_H
#define BULLET_H

#include "moving_item.h"

class Bullet : public MovingItem
{
public:
    Bullet();

protected:
    virtual void speedChanged(const QPointF &value);

private:
    static QPixmap _pixmap;
};

#endif
