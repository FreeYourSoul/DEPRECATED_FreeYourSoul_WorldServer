//
// Created by FyS on 28/08/17.
//

#ifndef FREESOULS_GAMESERVERINSTANCE_HH
#define FREESOULS_GAMESERVERINSTANCE_HH

#include <string>

namespace fys::ws {

    class GameServerInstance {
    public:
        ~GameServerInstance() = default;
        GameServerInstance(const ushort port, const std::string &ip, const std::string &positionId);
        GameServerInstance(const GameServerInstance&) = default;
        GameServerInstance(GameServerInstance&&) noexcept;
        GameServerInstance &operator=(GameServerInstance);

        void setPort(const ushort port);
        void setIp(const std::string &ip);
        void setPositionId(const std::string &positionId);

        const std::string &getIp() const;
        const std::string &getPositionId() const;
        ushort getPort() const;


    private:
        std::string _ip;
        std::string _positionId;
        ushort _port;
    };

}


#endif //FREESOULS_GAMESERVERINSTANCE_HH
