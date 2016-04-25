#include "boost/asio.hpp"
#include "boost/bind.hpp"
#include <memory>
#include <queue>
#include <iostream>
#include "message.pb.h"

#include "IConnection.h"

#define DEBUG(id)   std::cout << id << ":"  << __FILE__ << ":" << __LINE__ << std::endl

enum CONN_STATE {
    STATE_NONE = 0,
    STATE_DISC,
    STATE_CONN
};

typedef std::shared_ptr<IEvent<CONN_STATE> > CEventType;
typedef std::shared_ptr<IAutomationInterface<CONN_STATE> > CStateType;

class Connection {
    protected:
        std::queue<std::shared_ptr<CallerMsg> > msg_queue;
        std::mutex msg_queue_mutex;
        std::shared_ptr<boost::asio::ip::tcp::socket> t_socket;
        void onReceive(char *buf, const boost::system::error_code &error, std::size_t bytes);
        std::shared_ptr<boost::asio::ip::tcp::endpoint> remote_ep, listen_ep;

        CEventType conn_event, disc_event;
        class ConnectionState: public IAutomationInterface<CONN_STATE> {
            private:
                CEventType event_;
                CONN_STATE state_;
            public:
                ConnectionState(CONN_STATE state) : state_(state) {}
                void operator() (const CEventType event) { event_ = event; }
                CEventType getEvent() { return event_; }
                CONN_STATE getState() const { return state_; }
        };
        std::shared_ptr<Context<CONN_STATE> > context;

    public:
        Connection(int port, boost::asio::io_service &ios);
        ~Connection();
        void connect(std::string addr, int port);
        std::shared_ptr<boost::asio::ip::tcp::socket> getSocket() { return t_socket; }
        void send_msg(const std::shared_ptr<CallerMsg> msg);
        std::shared_ptr<CallerMsg> getMsg();
        void receiveMsg();
        std::shared_ptr<boost::asio::ip::tcp::endpoint> getRemoteEP();
        CONN_STATE isConnected() { return context->getState(); }
};
