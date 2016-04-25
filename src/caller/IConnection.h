#pragma once
#include <string>
#include <iostream>
#include <functional>
#include <mutex>
#include <memory>
#include <queue>

#include "tmorozov/sm/Context.h"

struct ConnectionAddr {
    std::string address;
    int port;
};


template <typename TransportType, typename CType, typename MsgType>
class IConnection {
    protected:
        TransportType tr;
        std::mutex msg_mutex;
        std::queue<std::shared_ptr<MsgType> > msg_queue;
        void queue_put(std::shared_ptr<MsgType> msg) {
            std::lock_guard<std::mutex> ilk(msg_mutex);
            msg_queue.push(msg);
        }
        std::shared_ptr<MsgType> queue_get() {
            if(!msg_queue.empty()) {
                std::shared_ptr<MsgType> msg = msg_queue.front();
                msg_queue.pop();
                return msg;
            }
            return std::make_shared<MsgType>();
        }

        virtual void receiveMsg() = 0;
        std::shared_ptr<Context<CType> > context;

    public:
        IConnection() {}
        virtual ~IConnection() {
            while(!msg_queue.empty()) {
                std::shared_ptr<MsgType> msg = msg_queue.front();
                msg_queue.pop();
            }
        }

        virtual CType connect(const ConnectionAddr &addr) = 0;
        virtual void disconnect() = 0;

        template<typename AcceptorType>
        void accept(std::shared_ptr<AcceptorType> acceptor) {
            acceptor->accept(*tr);
        }

        virtual void sendMsg(const std::shared_ptr<MsgType> msg) = 0;
        std::shared_ptr<MsgType> getMsg() {
            std::shared_ptr<MsgType> msg;
            if(!msg_queue.empty()) {
                std::lock_guard<std::mutex> lk(msg_mutex);
                msg = msg_queue.front();
                msg_queue.pop();
            }
            return msg;
        }
};

