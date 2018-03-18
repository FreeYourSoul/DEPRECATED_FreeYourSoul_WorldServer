//
// Created by FyS on 18/03/18.
//

#include <FySPlayerInteraction.pb.h>
#include <FySMessage.pb.h>
#include "listener/GamingListener.hh"

fys::ws::buslistener::GamingListener::GamingListener(std::shared_ptr<fys::ws::WorldServer> &ws) : _ws(ws) {
}

void fys::ws::buslistener::GamingListener::operator()(fys::mq::QueueContainer<pb::FySMessage> msg) {
    pb::PlayerInteract playerInteract;

    msg.getContained().content().UnpackTo(&playerInteract);
    if (pb::PlayerInteract_Type_IsValid(playerInteract.type())) {
        switch (playerInteract.type()) {

            case pb::PlayerInteract::MOVE_ON:
                break;

            case pb::PlayerInteract::MOVE_OFF:
                break;

            case pb::PlayerInteract::ACTIVATE:
                break;

            case pb::PlayerInteract::REQUEST_INFO:
                break;

            default:
                break;
        }
    }
}
