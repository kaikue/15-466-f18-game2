#include "Connection.hpp"
#include "Game.hpp"

#include <iostream>
#include <set>
#include <chrono>

int main(int argc, char **argv) {
	if (argc != 2) {
		std::cerr << "Usage:\n\t./server <port>" << std::endl;
		return 1;
	}
	
	Server server(argv[1]);

	Game state;

	while (1) {
		server.poll([&](Connection *c, Connection::Event evt){
			if (evt == Connection::OnOpen) {
			} else if (evt == Connection::OnClose) {
			} else { assert(evt == Connection::OnRecv);
				if (c->recv_buffer[0] == 'h') {
					c->recv_buffer.erase(c->recv_buffer.begin(), c->recv_buffer.begin() + 1);
					std::cout << c << ": Got hello." << std::endl;
				} else if (c->recv_buffer[0] == 's') {
					if (c->recv_buffer.size() < 3 + sizeof(float)) {
						return; //wait for more data
					} else {
            bool is_p1 = c->recv_buffer[1] == '1';
            bool fire = c->recv_buffer[2] == '1';
            if (is_p1) state.fire1 = fire;
            else state.fire2 = fire;
						memcpy(is_p1 ? &state.paddle1 : &state.paddle2, c->recv_buffer.data() + 3, sizeof(float));
						c->recv_buffer.erase(c->recv_buffer.begin(), c->recv_buffer.begin() + 3 + sizeof(float));
					}
				}
			}
		}, 0.01);
		//every second or so, dump the current paddle position:
		static auto then = std::chrono::steady_clock::now();
		auto now = std::chrono::steady_clock::now();
		if (now > then + std::chrono::seconds(1)) {
			then = now;
			std::cout << "Current ball position: " << state.ball.x << ", " << state.ball.y << std::endl;
		}
	}
}
