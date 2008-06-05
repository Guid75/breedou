#include "moving_item.h"

MovingItem::MovingItem()
    : Item()
{
}

void MovingItem::update(int delta)
{
    Item::update(delta);

    // Collisions?
    

    setPosition(QPointF(position().x() + _speed.x(),
                        position().y() + _speed.y()));
}

QPointF MovingItem::futurePosition() const
{
    return QPointF(position().x() + _speed.x(),
                   position().y() + _speed.y());
}

QRectF MovingItem::futureBoundingBox() const
{
    QPointF pos = futurePosition();
    return QRectF(pos.x() - size().width() / 2.0,
                  pos.y() - size().height() / 2.0,
                  size().width(),
                  size().height());
}
