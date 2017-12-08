//
// Created by Jean-Luc Thumm on 11/26/17.
//

#ifndef PA4_EVENT_H
#define PA4_EVENT_H


#include "Ray.h"

class Event {
public:
    enum Type {
        PRESS, RELEASE, DRAG
    };

    Event(Type type, Ray ray)
        : type{type}, ray{ray} {}

    const Ray ray;
    const Type type;
};


#endif //PA4_EVENT_H
