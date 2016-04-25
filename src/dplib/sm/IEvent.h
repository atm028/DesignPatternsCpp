#pragma once

template <class Type>
class IEvent {
    Type event;
    public:
        IEvent(Type e) : event(e) {}
};
