#include "ConnectionEndpoint.h"

Endpoint::Endpoint(int port) : t_port(port), t_state(STATE_DISC), t_run(true) {
    t_ios = std::make_shared<boost::asio::io_service>();
    t_local_ep = std::make_shared<boost::asio::ip::tcp::endpoint>(boost::asio::ip::address::from_string("127.0.0.1"), port);
    std::shared_ptr<Connection> conn = std::make_shared<Connection>(t_port, *t_ios);
    t_acceptor = std::make_shared<boost::asio::ip::tcp::acceptor>(*t_ios, *t_local_ep);
    t_acceptor->async_accept(*(conn->getSocket()), boost::bind(&Endpoint::on_accept, this, conn, boost::asio::placeholders::error));
    t_io_thread = std::thread(boost::bind(&boost::asio::io_service::run, t_ios));
    t_msg_receiver_thread = std::thread(&Endpoint::msgReceiver, this);
}

Endpoint::~Endpoint() {
    t_acceptor->cancel();
    t_acceptor.reset();
    t_ios->stop();
    t_io_thread.join();
    t_run = false;
    t_msg_receiver_thread.join();
    while(!t_msg_queue.empty()) {
        t_msg_queue.pop();
    }
}

void Endpoint::on_accept(std::shared_ptr<Connection> conn, const boost::system::error_code &error) {
    if(!error) {
        conn->receiveMsg();
        CallerMsg resp_msg;
        resp_msg.set_req(CallerMsg_Request_REQ_INFO);
        resp_msg.set_rsp(CallerMsg_Response_RSP_BUSY);
        resp_msg.set_port(t_port);
        conn->send_msg(std::make_shared<CallerMsg>(resp_msg));
        std::lock_guard<std::mutex> lock(t_conn_list_mutex);
        t_conn_list.push_back(conn);
        t_state = STATE_CONN;
        std::shared_ptr<Connection> conn = std::make_shared<Connection>(t_port, *t_ios);
        t_acceptor->async_accept(*(conn->getSocket()), boost::bind(&Endpoint::on_accept, this, conn, boost::asio::placeholders::error));
    }
}

void Endpoint::connect(std::string address, int port) {
    std::shared_ptr<Connection> conn = std::make_shared<Connection>(t_port, *t_ios);
    try {
        conn->connect(address, port);
        conn->receiveMsg();
        std::lock_guard<std::mutex> lock(t_conn_list_mutex);
        t_conn_list.push_back(conn);
        t_state = STATE_CONN;
    } catch(boost::system::system_error e) {
        throw e;
    }
}

void Endpoint::sendMsg(const Endpoint *target, std::shared_ptr<CallerMsg> msg) {
    auto ep = *(target->t_local_ep);
    std::lock_guard<std::mutex> lock(t_conn_list_mutex);
    for(auto it : t_conn_list) {
        try {
            auto conn_ep = *(it->getRemoteEP());
            if(ep == conn_ep) {
                it->send_msg(msg);
                break;
            }
        } catch(bool e) { throw e; }
    }
}

std::shared_ptr<CallerMsg> Endpoint::getMsg() {
    if(!t_msg_queue.empty()) {
        auto msg = t_msg_queue.front();
        std::lock_guard<std::mutex> lock(t_msg_queue_mutex);
        t_msg_queue.pop();
        return msg;
    }
    throw false;
}

void Endpoint::msgReceiver() {
    while(t_run) {
        std::unique_lock<std::mutex> lock(t_conn_list_mutex);
        for(auto it : t_conn_list) {
            try {
                auto msg = it->getMsg();
                std::lock_guard<std::mutex> lock(t_msg_queue_mutex);
                t_msg_queue.push(msg);
                break;
            } catch(bool e) {}
        }
        lock.unlock();
    }
}
