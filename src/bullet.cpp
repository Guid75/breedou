#include "bullet.h"

Bullet::Bullet() : MovingItem()
{
    setSize(QSizeF(0.040, 0.010));
    setDestructible(false);
    setCollisionDamages(1);
}

void Bullet::speedChanged(const QPointF &value)
{
    MovingItem::speedChanged(value);

    // Update rotation
    QLineF vector(0.0, 0.0, speed().x(), speed().y());
    setRotation(-vector.angle());
}
