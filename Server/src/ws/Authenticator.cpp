//
// Created by FyS on 31/08/17.
//

#include <FySAuthenticationResponse.pb.h>
#include "Authenticator.hh"

fys::ws::buslistener::Authenticator::Authenticator(WorldServer::ptr& gtw) : _ws(gtw)
{}

void fys::ws::buslistener::Authenticator::operator()(mq::QueueContainer<pb::FySMessage> msg) {
    pb::LoginMessage authMessage;

    msg.getContained().content().UnpackTo(&authMessage);
    if (pb::LoginMessage_Type_IsValid(authMessage.typemessage())) {
        switch (authMessage.typemessage()) {

            case pb::LoginMessage_Type_NotifyNewPlayer :
                notifyPlayerIncoming(msg.getIndexSession(), std::move(authMessage));
                break;

            case pb::LoginMessage_Type_NotifyNewServer :
                notifyServer(std::move(authMessage));
                break;

            case pb::LoginMessage_Type_LoginPlayerOnGame:
                authPlayer(msg.getIndexSession(), std::move(authMessage));
                break;

            default:
                break;
        }
    }
}

void fys::ws::buslistener::Authenticator::notifyServer(fys::pb::LoginMessage &&loginMessage) {
    pb::NotifyServerIncoming notif;

    loginMessage.content().UnpackTo(&notif);
    _ws->connectAndAddWorldServerInCluster(notif.positionid(), notif.token(), notif.ip(), notif.port());
}

void fys::ws::buslistener::Authenticator::notifyPlayerIncoming(uint indexSession, fys::pb::LoginMessage &&loginMsg) {
    pb::NotifyPlayerIncoming notif;
    std::vector<char> data(notif.token().begin(), notif.token().end());

    loginMsg.content().UnpackTo(&notif);
    _ws->addIncomingPlayerInAcceptedIp(notif.ip(), data);
}

void fys::ws::buslistener::Authenticator::authPlayer(const uint indexSession, fys::pb::LoginMessage &&loginMessage) {

}

