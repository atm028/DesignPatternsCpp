#pragma once
#include "IAutomationInterface.h"
#include <map>
#include <memory>
#include <functional>
#include <string>
#include <iostream>

template <typename EventType>
class Context : public IAutomationInterface<EventType> {

    typedef std::shared_ptr<IEvent<EventType> > IEventType;
    typedef std::shared_ptr<IAutomationInterface<EventType> > IFuncType;
    protected:
        std::map<IFuncType, std::map<IEventType, IFuncType > > edges;
        IFuncType state;

    public:
        Context() {}
        Context(IFuncType st) : state(st) { }
        virtual ~Context() { }

        void addEdge(IFuncType state_1, IEventType event, IFuncType state_2) {
            auto edge = edges.find(state_1);
            auto node = std::make_pair(event, state_2);
        
            if(edge == edges.end()) {
                std::map<IEventType, IFuncType> map;
                map.insert(node);
                auto pair = std::make_pair(state_1, map);
                edges.insert(pair);
            } else {
                edge->second.insert(node);
            }
        }

        void delState(IFuncType state) {
            auto edge = edges.find(state);
            if(edge != edges.end()) {
                edges.erase(edge);
            }
        }

        void delEdge(IFuncType state, IEventType event) {
            auto edge = edges.find(state);
            if(edge != edges.end()) {
                auto node = edge->second.find(event);
                if(node != edge->second.end()) {
                    edge->second.erase(node);
                }
            }
        }

        virtual void operator() (const IEventType event) {
            auto l_node = edges.find(state);
            if(l_node != edges.end()) {
                auto n_state = l_node->second.find(event);
                if(n_state != l_node->second.end()) {
                    state = n_state->second;
                }
            }
            (*state.get())(event);
        }
        EventType getState() const { return state->getState(); }
};
