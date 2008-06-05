#ifndef ROBOT_H
#define ROBOT_H

#include "moving_item.h"

class Robot : public MovingItem
{
public:
    static const int fullHealth = 20;

    Robot();

    //! \brief Represents the minimum time between two fires in milliseconds
    int firePeriod() const { return _firePeriod; }

private:
    int _firePeriod;
};

#endif
