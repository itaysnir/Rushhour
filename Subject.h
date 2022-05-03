#ifndef SUBJECT_H
#define SUBJECT_H

#include <vector>
#include <algorithm>
#include "OOP5EventException.h"
#include "Observer.h"

template<typename T>
class Subject
{
private:
    std::vector<Observer<T>*> _observers;

public:
    Subject() = default;
    void notify(const T&);
    void addObserver(Observer<T>&);
    void removeObserver(Observer<T>&);
    Subject<T>& operator+=(Observer<T>&);
    Subject<T>& operator-=(Observer<T>&);
    Subject<T>& operator()(const T&);
};

template<typename T>
void Subject<T>::notify(const T& arg)
{
    for (auto &observer : _observers)
    {
        (*observer).handleEvent(arg);
    }
}

template<typename T>
void Subject<T>::addObserver(Observer<T>& observer)
{
    if (std::find(_observers.begin(), _observers.end(), &observer) != _observers.end())
    {
        throw ObserverAlreadyKnownToSubject();
    }

    _observers.push_back(&observer);
}

template<typename T>
void Subject<T>::removeObserver(Observer<T>& observer)
{
    if (std::find(_observers.begin(), _observers.end(), &observer) == _observers.end())
    {
        throw ObserverUnknownToSubject();
    }

    _observers.erase(std::remove(_observers.begin(), _observers.end(), &observer), _observers.end());
}

template<typename T>
Subject<T>& Subject<T>::operator+=(Observer<T>& observer)
{
    addObserver(observer);

    return *this;
}

template<typename T>
Subject<T>& Subject<T>::operator-=(Observer<T>& observer)
{
    removeObserver(observer);

    return *this;
}

template<typename T>
Subject<T>& Subject<T>::operator()(const T& arg)
{
    notify(arg);

    return *this;
}

#endif
