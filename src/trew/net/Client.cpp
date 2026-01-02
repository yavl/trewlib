#include "Client.hpp"
#include <trew/Logger.hpp>

using namespace trew;

constexpr auto LOGTAG = "Client";

Client::Client(const char* ip, int port) {
    if (enet_initialize() != 0) {
        logError(LOGTAG, "Failed to init ENet");
    }
    atexit(enet_deinitialize);

    client = enet_host_create(NULL, 1, 2, 0, 0);
    ENetAddress address;
    ENetEvent event;

    enet_address_set_host(&address, ip);
    address.port = port;

    peer = enet_host_connect(client, &address, 2, 0);
    if (peer == nullptr) {
        logError(LOGTAG, "Failed to connect to server");
    }
}

Client::~Client() {
    enet_host_destroy(client);
}

void Client::update() {
    if (client == nullptr) return;

    ENetEvent event;
    // Опрашиваем события (таймаут 0 — не блокируем игру)
    while (enet_host_service(client, &event, 0) > 0) {
        switch (event.type) {
        case ENET_EVENT_TYPE_CONNECT:
            printf("Connected to server!\n");

            // Отправляем приветствие ТОЛЬКО ОДИН РАЗ при подключении
            {
                const char* msg = "Hello Server";
                ENetPacket* packet = enet_packet_create(msg, strlen(msg) + 1, ENET_PACKET_FLAG_RELIABLE);
                enet_peer_send(peer, 0, packet);
            }
            break;

        case ENET_EVENT_TYPE_RECEIVE:
            printf("Received from server: %s\n", (char*)event.packet->data);
            enet_packet_destroy(event.packet);
            break;

        case ENET_EVENT_TYPE_DISCONNECT:
            printf("Disconnected from server.\n");
            peer = nullptr; // Чтобы не использовать невалидный указатель
            break;
        }
    }
}