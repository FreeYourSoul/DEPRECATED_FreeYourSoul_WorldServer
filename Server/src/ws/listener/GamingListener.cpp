//
// Created by FyS on 18/03/18.
//

#include <spdlog/spdlog.h>
#include <FySPlayerInteraction.pb.h>
#include <FySMessage.pb.h>
#include <WorldServer.hh>
#include "GamingListener.hh"

fys::ws::buslistener::GamingListener::GamingListener(std::shared_ptr<fys::ws::WorldServer> &ws) :
        _ws(ws), _worldEngine(ws->getWorldEngine()){
}

void fys::ws::buslistener::GamingListener::operator()(fys::mq::QueueContainer<pb::FySMessage> msg) {
    pb::PlayerInteract playerInteract;

    msg.getContained().content().UnpackTo(&playerInteract);
    if (_ws->getGamerConnections().isAuthenticated(msg.getIndexSession(), playerInteract.token())) {
        spdlog::get("c")->error("Bad authentication used in GamingListener user {} with token {}",
                                msg.getIndexSession(), playerInteract.token());
    }
    else if (pb::PlayerInteract_Type_IsValid(playerInteract.type())) {
        switch (playerInteract.type()) {

            case pb::PlayerInteract::MOVE_ON:
                changePlayerStateInteractionMove(std::move(playerInteract));
                break;

            case pb::PlayerInteract::MOVE_OFF:
                changePlayerStatInteractionStopMoving(std::move(playerInteract));
                break;

            case pb::PlayerInteract::ACTIVATE:
                playerInteractionWithWorldItem(std::move(playerInteract));
                break;

            case pb::PlayerInteract::REQUEST_INFO:
                playerRequestInformation(std::move(playerInteract));
                break;

            default:
                break;
        }
    }
}

void fys::ws::buslistener::GamingListener::changePlayerStateInteractionMove(fys::pb::PlayerInteract &&interact) {
    fys::pb::PlayerMove playerMove;
    interact.content().UnpackTo(&playerMove);
//    _worldEngine->changePlayerMovingState(, playerMove.angle());
}

void fys::ws::buslistener::GamingListener::changePlayerStatInteractionStopMoving(fys::pb::PlayerInteract &&interact) {
//    _worldEngine->changePlayerMovingState();
}

void fys::ws::buslistener::GamingListener::playerRequestInformation(fys::pb::PlayerInteract &&interact) {

}

void fys::ws::buslistener::GamingListener::playerInteractionWithWorldItem(fys::pb::PlayerInteract &&interact) {

}
