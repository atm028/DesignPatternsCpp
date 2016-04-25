#include <string>
#include <iostream>
#include <functional>

#include "google/gtest/gtest.h"
#include <boost/asio.hpp>
#include "ConnectionEndpoint.h"

TEST(CreateEndpoint, Normal) {
    Endpoint ep1(21350);
    EXPECT_EQ(ep1.getState(), STATE_DISC);
}

TEST(SimpleConnect, Normal) {
    Endpoint ep1(21350);
    Endpoint ep2(21351);
    EXPECT_EQ(ep1.getState(), STATE_DISC);
    EXPECT_EQ(ep2.getState(), STATE_DISC);

    ep1.connect("127.0.0.1", 21351);
    ::sleep(5);
    EXPECT_EQ(ep1.getState(), STATE_CONN);
    EXPECT_EQ(ep2.getState(), STATE_CONN);
}

TEST(ThreeConnections, Normal) {
    Endpoint ep1(21350);
    Endpoint ep2(21351);
    Endpoint ep3(21352);


    EXPECT_EQ(ep1.getState(), STATE_DISC);
    EXPECT_EQ(ep2.getState(), STATE_DISC);
    EXPECT_EQ(ep3.getState(), STATE_DISC);
    
    ep2.connect("127.0.0.1", 21350);
    ::sleep(2);
    EXPECT_EQ(ep1.getState(), STATE_CONN);
    EXPECT_EQ(ep2.getState(), STATE_CONN);
    EXPECT_EQ(ep3.getState(), STATE_DISC);

    ep3.connect("127.0.0.1", 21350);
    ::sleep(2);
    EXPECT_EQ(ep1.getState(), STATE_CONN);
    EXPECT_EQ(ep2.getState(), STATE_CONN);
    EXPECT_EQ(ep3.getState(), STATE_CONN);
}

TEST(SimpleDataSend, Normal) {
    Endpoint ep1(21350);
    Endpoint ep2(21351);
    EXPECT_EQ(ep1.getState(), STATE_DISC);
    EXPECT_EQ(ep2.getState(), STATE_DISC);

    ep1.connect("127.0.0.1", 21351);
    ::sleep(2);
    EXPECT_EQ(ep1.getState(), STATE_CONN);
    EXPECT_EQ(ep2.getState(), STATE_CONN);

    CallerMsg out_msg;
    out_msg.set_req(CallerMsg_Request_REQ_INVITE);
    out_msg.set_rsp(CallerMsg_Response_RSP_OK);
    ::sleep(2);
    ep1.sendMsg(&ep2, std::make_shared<CallerMsg>(out_msg));
    ::sleep(2);
    std::shared_ptr<CallerMsg> in_msg = ep2.getMsg();
    EXPECT_EQ(out_msg.req(), in_msg->req());
    EXPECT_EQ(out_msg.rsp(), in_msg->rsp());
}

TEST(SimpleDataSend1, Normal) {
    Endpoint ep1(21350);
    Endpoint ep2(21351);
    EXPECT_EQ(ep1.getState(), STATE_DISC);
    EXPECT_EQ(ep2.getState(), STATE_DISC);

    ep1.connect("127.0.0.1", 21351);
    ::sleep(2);
    EXPECT_EQ(ep1.getState(), STATE_CONN);
    EXPECT_EQ(ep2.getState(), STATE_CONN);

    CallerMsg out_msg;
    out_msg.set_req(CallerMsg_Request_REQ_INVITE);
    out_msg.set_rsp(CallerMsg_Response_RSP_OK);
    ep2.sendMsg(&ep1, std::make_shared<CallerMsg>(out_msg));
    ::sleep(2);
    std::shared_ptr<CallerMsg> in_msg = ep1.getMsg();
    EXPECT_EQ(out_msg.req(), in_msg->req());
    EXPECT_EQ(out_msg.rsp(), in_msg->rsp());
}

TEST(SimpleEcho, Normal) {
    Endpoint ep1(21350);
    Endpoint ep2(21351);
    EXPECT_EQ(ep1.getState(), STATE_DISC);
    EXPECT_EQ(ep2.getState(), STATE_DISC);

    ep1.connect("127.0.0.1", 21351);
    ::sleep(2);
    EXPECT_EQ(ep1.getState(), STATE_CONN);
    EXPECT_EQ(ep2.getState(), STATE_CONN);

    CallerMsg out_msg;
    out_msg.set_req(CallerMsg_Request_REQ_INVITE);
    out_msg.set_rsp(CallerMsg_Response_RSP_OK);
    ep1.sendMsg(&ep2, std::make_shared<CallerMsg>(out_msg));
    ::sleep(2);
    std::shared_ptr<CallerMsg> in_msg = ep2.getMsg();
    EXPECT_EQ(out_msg.req(), in_msg->req());
    EXPECT_EQ(out_msg.rsp(), in_msg->rsp());

    ep2.sendMsg(&ep1, in_msg);
    ::sleep(2);
    std::shared_ptr<CallerMsg> in_msg1 = ep1.getMsg();
    EXPECT_EQ(in_msg->req(), in_msg1->req());
    EXPECT_EQ(in_msg->rsp(), in_msg1->rsp());
}

TEST(ThreeConnectionDataSend, Normal) {
    Endpoint ep1(21351);
    Endpoint ep2(21352);
    Endpoint ep3(21353);
    EXPECT_EQ(ep1.getState(), STATE_DISC);
    EXPECT_EQ(ep2.getState(), STATE_DISC);
    EXPECT_EQ(ep3.getState(), STATE_DISC);

    ep1.connect("127.0.0.1", 21352);
    ::sleep(2);
    EXPECT_EQ(ep1.getState(), STATE_CONN);
    EXPECT_EQ(ep2.getState(), STATE_CONN);

    ep2.connect("127.0.0.1", 21353);
    ::sleep(2);
    EXPECT_EQ(ep2.getState(), STATE_CONN);
    EXPECT_EQ(ep3.getState(), STATE_CONN);

    ep3.connect("127.0.0.1", 21351);
    ::sleep(2);
    EXPECT_EQ(ep3.getState(), STATE_CONN);
    EXPECT_EQ(ep1.getState(), STATE_CONN);

    CallerMsg out_msg;
    out_msg.set_req(CallerMsg_Request_REQ_INVITE);
    out_msg.set_rsp(CallerMsg_Response_RSP_OK);
    ep1.sendMsg(&ep2, std::make_shared<CallerMsg>(out_msg));
    ::sleep(2);
    std::shared_ptr<CallerMsg> in_msg = ep2.getMsg();
    EXPECT_EQ(out_msg.req(), in_msg->req());
    EXPECT_EQ(out_msg.rsp(), in_msg->rsp());

    ep2.sendMsg(&ep3, in_msg);
    ::sleep(2);
    std::shared_ptr<CallerMsg> in_msg1 = ep3.getMsg();
    EXPECT_EQ(in_msg->req(), in_msg1->req());
    EXPECT_EQ(in_msg->rsp(), in_msg1->rsp());


    ep3.sendMsg(&ep1, in_msg1);
    ::sleep(2);
    std::shared_ptr<CallerMsg> in_msg2 = ep1.getMsg();
    EXPECT_EQ(in_msg1->req(), in_msg2->req());
    EXPECT_EQ(in_msg1->rsp(), in_msg2->rsp());
}

TEST(ThreeConnectionSimultaneousDataSend, Normal) {
    Endpoint ep1(21351);
    Endpoint ep2(21352);
    Endpoint ep3(21353);
    EXPECT_EQ(ep1.getState(), STATE_DISC);
    EXPECT_EQ(ep2.getState(), STATE_DISC);
    EXPECT_EQ(ep3.getState(), STATE_DISC);

    ep1.connect("127.0.0.1", 21352);
    ::sleep(2);
    EXPECT_EQ(ep1.getState(), STATE_CONN);
    EXPECT_EQ(ep2.getState(), STATE_CONN);

    ep2.connect("127.0.0.1", 21353);
    ::sleep(2);
    EXPECT_EQ(ep2.getState(), STATE_CONN);
    EXPECT_EQ(ep3.getState(), STATE_CONN);

    ep3.connect("127.0.0.1", 21351);
    ::sleep(2);
    EXPECT_EQ(ep3.getState(), STATE_CONN);
    EXPECT_EQ(ep1.getState(), STATE_CONN);

    CallerMsg out_msg;
    out_msg.set_req(CallerMsg_Request_REQ_INVITE);
    out_msg.set_rsp(CallerMsg_Response_RSP_OK);
    ep1.sendMsg(&ep3, std::make_shared<CallerMsg>(out_msg));
    ep2.sendMsg(&ep3, std::make_shared<CallerMsg>(out_msg));

    ::sleep(2);
    std::shared_ptr<CallerMsg> in_msg_1 = ep3.getMsg();
    EXPECT_EQ(out_msg.req(), in_msg_1->req());
    EXPECT_EQ(out_msg.rsp(), in_msg_1->rsp());

    std::shared_ptr<CallerMsg> in_msg_2 = ep3.getMsg();
    EXPECT_EQ(out_msg.req(), in_msg_2->req());
    EXPECT_EQ(out_msg.rsp(), in_msg_2->rsp());
}
