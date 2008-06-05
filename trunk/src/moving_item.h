#ifndef MOVING_ITEM_H
#define MOVING_ITEM_H

#include "item.h"

class MovingItem : public Item
{
public:
    MovingItem();

    const QPointF &speed() const { return _speed; }
    void setSpeed(const QPointF &value) { if (_speed == value) return; speedChanged(value); }

    QPointF futurePosition() const;
    QRectF futureBoundingBox() const;

    virtual void update(int delta);

protected:
    virtual void speedChanged(const QPointF &value) { _speed = value; }

private:
    QPointF _speed;
};

#endif
