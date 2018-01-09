#include <iostream>
#include <thread>
#include <WorldServer.hh>
#include <BusListener.hh>
#include <Authenticator.hh>
#include <TokenGenerator.hh>

using namespace fys::mq;
using namespace fys::ws;
using namespace fys::network;

using AuthBusListener = BusListener <buslistener::Authenticator, FysBus<fys::pb::FySMessage, BUS_QUEUES_SIZE>>;

int main(int argc, const char * const *argv) {
    GOOGLE_PROTOBUF_VERIFY_VERSION;
    try {
        boost::asio::io_service ios;
        boost::asio::io_service::work work(ios);
        Context ctx(argc, argv);
        std::cout << ctx << std::endl;
        auto fysBus = std::make_shared<FysBus<fys::pb::FySMessage, BUS_QUEUES_SIZE> > (fys::pb::Type_ARRAYSIZE);
        WorldServer::ptr worldServer = WorldServer::create(ctx, ios, fysBus);
        buslistener::Authenticator authenticator(worldServer);
        AuthBusListener authenticatorListener(authenticator);

        authenticatorListener.launchListenThread(fysBus);
        worldServer->connectToGateway(ctx);
        worldServer->runPlayerAccept();
        ios.run();
    }
    catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    google::protobuf::ShutdownProtobufLibrary();
    return 0;
}
