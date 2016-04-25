#pragma once
#include "IEvent.h"

#include <memory>

template <class EventType>
class IAutomationInterface {
    public:
        virtual void operator() (const std::shared_ptr<IEvent<EventType> > event) = 0;
        virtual EventType getState() const = 0;
};
