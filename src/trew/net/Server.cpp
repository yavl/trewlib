#include "Server.hpp"
#include <trew/Logger.hpp>

using namespace trew;

constexpr auto LOGTAG = "Server";

Server::Server(int port) {
    if (enet_initialize() != 0) {
        logError(LOGTAG, "Failed to init ENet");
    }
    atexit(enet_deinitialize);

    ENetAddress address;
    address.host = ENET_HOST_ANY; // Слушать все интерфейсы
    address.port = port;

    // Создание сервера: адрес, макс. клиентов (32), каналов (2), входящий/исходящий лимит (0 = без лимита)
    server = enet_host_create(&address, 32, 2, 0, 0);
    if (server == nullptr) {
        logError(LOGTAG, "Failed to create host");
    }
    else {
        log(LOGTAG, fmt::format("Server started with port: {}", port));
    }
}

Server::~Server() {
    enet_host_destroy(server);
}

void Server::update() {
    if (server == nullptr) {
        return;
    }
    ENetEvent event;
    while (enet_host_service(server, &event, 0) > 0) {
        switch (event.type) {
        case ENET_EVENT_TYPE_CONNECT:
            printf("New connection: %x:%u\n", event.peer->address.host, event.peer->address.port);
            break;
        case ENET_EVENT_TYPE_RECEIVE:
            printf("Received from client: %s\n", (char*)event.packet->data);
            enet_packet_destroy(event.packet); // Очистка пакета
            break;
        case ENET_EVENT_TYPE_DISCONNECT:
            printf("Client disconnected.\n");
            break;
        }
    }
}
