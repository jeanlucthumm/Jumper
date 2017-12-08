//
// Created by Jean-Luc Thumm on 11/26/17.
//

#ifndef PA4_EVENTLISTENER_H
#define PA4_EVENTLISTENER_H

#include "Event.h"

class EventListener {
public:
    virtual void receive(const Event &event) = 0;

    virtual void receive(int key) = 0; // for GLFW key strokes
};

#endif //PA4_EVENTLISTENER_H
