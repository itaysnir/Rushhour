#ifndef OBSERVER_H
#define OBSERVER_H

#include "OOP5EventException.h"

template<typename T>
class Observer
{
public:
    Observer() = default;
    virtual void handleEvent(const T&) = 0;
};

#endif
