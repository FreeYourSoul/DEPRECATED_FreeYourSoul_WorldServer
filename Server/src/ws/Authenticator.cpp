//
// Created by FyS on 31/08/17.
//

#include <FySAuthenticationResponse.pb.h>
#include "Authenticator.hh"

fys::ws::buslistener::Authenticator::Authenticator(WorldServer::ptr& gtw) : _gtw(gtw)
{}

void fys::ws::buslistener::Authenticator::operator()(mq::QueueContainer<pb::FySMessage> msg) {
    pb::LoginMessage authMessage;

    msg.getContained().content().UnpackTo(&authMessage);
    if (pb::LoginMessage_Type_IsValid(authMessage.typemessage())) {
        switch (authMessage.typemessage()) {
            case pb::LoginMessage_Type_NotifyNewPlayer :
                notifyNewPlayer(std::move(authMessage));
                break;

            case pb::LoginMessage_Type_LoginPlayerOnGame:
                authPlayer(msg.getIndexSession(), std::move(authMessage));
                break;

            default:
                break;
        }
    }
}

void fys::ws::buslistener::Authenticator::notifyNewPlayer(fys::pb::LoginMessage &&indexSession) {

}

void fys::ws::buslistener::Authenticator::authPlayer(const uint indexSession, fys::pb::LoginMessage &&loginMessage) {

}

