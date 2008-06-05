#include "robot.h"

Robot::Robot() : MovingItem(), _firePeriod(100)
{
    setSize(QSizeF(0.1, 0.1));
    setDestructible(true);
    setHealth(fullHealth);
    setCollisionDamages(5);
}
