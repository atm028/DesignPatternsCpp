#include "connection.h"

Connection::Connection(int port, boost::asio::io_service &ios) {
    listen_ep = std::make_shared<boost::asio::ip::tcp::endpoint>(boost::asio::ip::address::from_string("127.0.0.1"), port);
    t_socket = std::make_shared<boost::asio::ip::tcp::socket>(ios);

    CStateType disconnected    = std::make_shared<ConnectionState>(STATE_DISC);
    CStateType connected       = std::make_shared<ConnectionState>(STATE_CONN);

    disc_event = std::make_shared<IEvent<CONN_STATE> >(STATE_DISC);
    conn_event = std::make_shared<IEvent<CONN_STATE> >(STATE_CONN);

    context = std::make_shared<Context<CONN_STATE> >(disconnected);
    context->addEdge(disconnected, conn_event, connected);
    context->addEdge(connected, disc_event, disconnected);
}

Connection::~Connection() {
    while(!msg_queue.empty()) {
        msg_queue.pop();
    }
}

std::shared_ptr<boost::asio::ip::tcp::endpoint> Connection::getRemoteEP() {
    if(STATE_CONN == context->getState()) {
        return remote_ep;
    }
    throw false;
}

void Connection::connect(std::string addr, int port) {
    boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address::from_string(addr.c_str()), port);
    t_socket->connect(ep);
}

void Connection::send_msg(const std::shared_ptr<CallerMsg> msg) {
    if(t_socket->is_open()) {
        try {
            std::string sout;
            if(msg->SerializeToString(&sout) != false) {
                t_socket->write_some(boost::asio::buffer(sout));
            }
        } catch(const boost::system::system_error &e) {
            throw e;
        }
    }
}

void Connection::receiveMsg() {
    if(t_socket->is_open()) {
        char buf[8192];
        t_socket->async_read_some(boost::asio::buffer(buf), boost::bind(&Connection::onReceive, this, buf, _1, _2));
    }
}

void Connection::onReceive(char *buf, const boost::system::error_code &error, std::size_t bytes) {
    if(!error) {
        std::string istr = std::string(buf);
        CallerMsg msg;
        msg.ParseFromString(istr);
        if(msg.req() == CallerMsg_Request_REQ_INFO && msg.rsp() == CallerMsg_Response_RSP_BUSY) {
            (*context)(conn_event);
            remote_ep = std::make_shared<boost::asio::ip::tcp::endpoint>(boost::asio::ip::address::from_string(t_socket->remote_endpoint().address().to_string()), msg.port());

            CallerMsg resp_msg;
            resp_msg.set_req(CallerMsg_Request_REQ_INFO);
            resp_msg.set_rsp(CallerMsg_Response_RSP_OK);
            resp_msg.set_port(listen_ep->port());
            send_msg(std::make_shared<CallerMsg>(resp_msg));
        }
        else if(msg.req() == CallerMsg_Request_REQ_INFO && msg.rsp() == CallerMsg_Response_RSP_OK) {
            (*context)(conn_event);
            remote_ep = std::make_shared<boost::asio::ip::tcp::endpoint>(boost::asio::ip::address::from_string(t_socket->remote_endpoint().address().to_string()), msg.port());
        }
        else {
            std::lock_guard<std::mutex> lock(msg_queue_mutex);
            msg_queue.push(std::make_shared<CallerMsg>(msg));
        }
    }
    receiveMsg();
}

std::shared_ptr<CallerMsg> Connection::getMsg() {
    std::lock_guard<std::mutex> lock(msg_queue_mutex);
    if(!msg_queue.empty()) {
        std::shared_ptr<CallerMsg> ret = msg_queue.front();
        msg_queue.pop();
        return ret;
    }
    throw false;
}
