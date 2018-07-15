//
// Created by FyS on 31/08/17.
//

#include <spdlog/spdlog.h>
#include <FySAuthenticationLoginMessage.pb.h>
#include <FySAuthenticationResponse.pb.h>
#include <FySMessage.pb.h>
#include <WorldServer.hh>
#include "listener/Authenticator.hh"

namespace fys::ws {
    
    
buslistener::Authenticator::Authenticator(std::shared_ptr<WorldServer> &ws) : _ws(ws)
{}

void buslistener::Authenticator::operator()(mq::QueueContainer<pb::FySMessage> msg) {
    pb::LoginMessage authMessage;

    msg.getContained().content().UnpackTo(&authMessage);
    if (pb::LoginMessage_Type_IsValid(authMessage.typemessage())) {
        switch (authMessage.typemessage()) {

            case pb::LoginMessage_Type_NotifyNewPlayer :
                notifyPlayerIncoming(std::move(authMessage));
                break;

            case pb::LoginMessage_Type_LoginPlayerOnGame:
                authPlayer(msg.getIndexSession(), std::move(authMessage));
                break;

            case pb::LoginMessage_Type_NotifyNewServer :
                notifyServerIncoming(std::move(authMessage));
                break;

            case pb::LoginMessage_Type_LoginWorldServerInCluster :
                authWorldServer(msg.getIndexSession(), std::move(authMessage));
                break;

            default:
                break;
        }
    }
}

void buslistener::Authenticator::notifyPlayerIncoming(fys::pb::LoginMessage &&loginMsg) {
    pb::NotifyPlayerIncoming notif;
    loginMsg.content().UnpackTo(&notif);
    network::Token token{notif.token().begin(), notif.token().end()};

    _ws->getGamerConnections().addIncomingPlayer(notif.ip(), std::move(token));
}

void buslistener::Authenticator::authPlayer(uint indexSession, fys::pb::LoginMessage &&loginMessage) {
    pb::LogingPlayerOnGame loginPlayerOnGame;
    const std::string &actualToken = _ws->getGamerConnections().getConnectionToken(indexSession);
    const std::string &token = loginPlayerOnGame.tokengameserver();

    loginMessage.content().UnpackTo(&loginPlayerOnGame);
    if (actualToken.empty())
        spdlog::get("c")->error("The player {} at index {}, token is {} isn't awaited by server", indexSession, loginMessage.user(), token);
    else if (std::equal(token.begin(), token.end(), actualToken.begin())) {
        _ws->getGamerConnections().connectPlayerWithToken(indexSession, {token.begin(), token.end()});
        _ws->initPlayerPosition(indexSession, 0.0, 0.0); // TODO: change position (x0.0, y0.0) info by the one got from data server
        spdlog::get("c")->info("A new player ({} at index {}) connected on server", loginMessage.user(), indexSession);
    }
    else
        spdlog::get("c")->error("Mismatch has been found {}:{}, token is {} and should be {}", indexSession, loginMessage.user(), token, actualToken);
}

void buslistener::Authenticator::notifyServerIncoming(fys::pb::LoginMessage &&loginMessage) {
    pb::NotifyServerIncoming notif;
    loginMessage.content().UnpackTo(&notif);
    network::Token token{notif.token().begin(), notif.token().end()};

    _ws->getWorldServerCluster().addIncomingWorldServer(notif.positionid(), notif.ip(), std::move(token));
}

void buslistener::Authenticator::authWorldServer(uint indexSession, fys::pb::LoginMessage &&loginMessage) {
    pb::LoginWorldServerInCluster loginGameServer;
    const std::string &actualToken = _ws->getGamerConnections().getConnectionToken(indexSession);
    const std::string &token = loginGameServer.tokengameserver();

    loginMessage.content().UnpackTo(&loginGameServer);
    if (actualToken.empty())
        spdlog::get("c")->error("The WorldServer {} at index {}, token is {} isn't awaited by server", indexSession, loginMessage.user(), token);
    else if (std::equal(token.begin(), token.end(), actualToken.begin())) {
        network::Token tkn{token.begin(), token.end()};
        _ws->connectAndAddWorldServerInCluster(loginMessage.user(), loginGameServer.ip(), loginGameServer.port(),
                                               std::move(tkn), indexSession);
        spdlog::get("c")->info("A WorldServer ({} at index {}) connected on server", loginMessage.user(), indexSession);
    }
    else
        spdlog::get("c")->error("Mismatch has been found {}:{}, token is {} and should be {}", indexSession, loginMessage.user(), token, actualToken);
}

}