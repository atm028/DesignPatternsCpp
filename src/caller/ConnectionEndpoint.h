#include <list>
#include <thread>
#include <queue>

#include "connection.h"
#include "tmorozov/sm/Context.h"

class Endpoint {
    protected:
        std::queue<std::shared_ptr<CallerMsg> > t_msg_queue;
        std::mutex t_msg_queue_mutex;
        std::thread t_io_thread, t_msg_receiver_thread;
        std::shared_ptr<boost::asio::ip::tcp::endpoint> t_local_ep;
        bool t_run;
        CONN_STATE t_state;
        int t_port;
        std::shared_ptr<boost::asio::io_service> t_ios;
        std::shared_ptr<boost::asio::ip::tcp::acceptor> t_acceptor;
        std::list<std::shared_ptr<Connection> > t_conn_list;
        std::mutex t_conn_list_mutex;

        void on_accept(std::shared_ptr<Connection> conn, const boost::system::error_code &error);
        void msgReceiver();

    public:
        Endpoint(int port);
        ~Endpoint();
        void connect(std::string address, int port);
        CONN_STATE getState() { return t_state; }
        void sendMsg(const Endpoint *target, std::shared_ptr<CallerMsg> msg);
        std::shared_ptr<CallerMsg> getMsg();
};

