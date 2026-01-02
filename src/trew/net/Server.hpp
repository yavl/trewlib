#pragma once

#include <enet/enet.h>

namespace trew {
	class Server {
	public:
		Server(int port);
		~Server();
		void update();
	private:
		ENetHost* server = nullptr;
	};
}