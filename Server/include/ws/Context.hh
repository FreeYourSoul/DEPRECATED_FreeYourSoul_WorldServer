//
// Created by FyS on 26/05/17.
//

#ifndef FREESOULS_CONTEXT_HH
#define FREESOULS_CONTEXT_HH

#include <string>
#include <ostream>
#include "GameServerInstance.hh"

// -- Config File Keys
static constexpr char GTW_INI_PORT[] = "server.port";
static constexpr char GTW_INI_GTW_PORT[] = "gateway.port";
static constexpr char GTW_INI_GTW_IP[] = "gateway.ip";
static constexpr char GTW_INI_ASIO_THREADS[] = "server.asioThread";
static constexpr char GTW_INI_BUS_PATH[] = "bus.iniPath";
static constexpr char GTW_QUEUES_SIZE[] = "bus.queuesSize";

namespace fys::ws {

    enum { BUS_QUEUES_SIZE = 1024 };

    class Context {

    public:
        ~Context() = default;
        Context(const int ac, const char *const *av);
        Context(const Context&);
        Context(Context&&) noexcept;

        friend std::ostream &operator<<(std::ostream &os, const Context &context);

        u_short getPort() const;
        const std::string &getBusIniFilePath() const;
        std::size_t getQueuesSize() const;
        size_t getAsioThread() const;
        u_short getGtwPort() const;
        bool isVerbose() const;
        const std::string &getGtwIp() const;

        void setBusIniFilePath(std::string &&busIniFilePath) noexcept;
        void setPort(u_short port);
        void setAsioThread(size_t asioThread);
        void setVerbose(bool verbose);
        void setQueuesSize(std::size_t queuesSize);
        void setGtwPort(u_short gtwPort);
        void setGtwIp(std::string&& gtwIp) noexcept;

    private:
        void initializeFromIni(const std::string &iniPath);

    private:
        u_short _port;
        u_short _gtwPort;
        std::string _gtwIp;
        std::size_t _asioThread;
        std::string _busIniFilePath;
        std::size_t _queuesSize;
        bool _verbose;

        std::vector<ws::GameServerInstance> _gameServers;
    };

}


#endif //FREESOULS_CONTEXT_HH
