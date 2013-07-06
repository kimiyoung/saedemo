#include "rpc/RpcServer.hpp"
#include "search.hpp"

using namespace std;

void setup_services(RpcServer* server) {
    server->addMethod("EntitySearch", &demoserver::EntitySearch);
}

int main() {
    RpcServer* server = RpcServer::CreateServer(40111, 20);
    setup_services(server);
    server->run();
    return 0;
}
