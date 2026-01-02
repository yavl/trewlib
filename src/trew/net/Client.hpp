#pragma once

#include <enet/enet.h>

namespace trew {
	class Client {
	public:
		Client(const char* ip, int port);
		~Client();
		void update();
	private:
		ENetHost* client = nullptr;
		ENetPeer* peer = nullptr;
	};
}