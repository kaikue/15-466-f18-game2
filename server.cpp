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
  int num_players = 0;

  Connection *p1 = nullptr;
  Connection *p2 = nullptr;

  auto then = std::chrono::high_resolution_clock::now();

	while (1) {
		server.poll([&](Connection *c, Connection::Event evt){
			if (evt == Connection::OnOpen) {
        if (num_players == 0) {
          std::cout << c << ": Got player 1." << std::endl;
          c->send_raw("p1", 2);
          p1 = c;
          num_players++;
        }
        else if (num_players == 1) {
          std::cout << c << ": Got player 2." << std::endl;
          c->send_raw("p2", 2);
          p2 = c;
          num_players++;
        }
        else {
          std::cout << c << ": Got player, but game is full..." << std::endl;
          c->send_raw("p0", 2);
        }
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
            if (fire) {
              std::cout << "fire..." << std::endl;
              if (is_p1) state.fire1 = true;
              else state.fire2 = true;
            }
						memcpy(is_p1 ? &state.paddle1 : &state.paddle2, c->recv_buffer.data() + 3, sizeof(float));
						c->recv_buffer.erase(c->recv_buffer.begin(), c->recv_buffer.begin() + 3 + sizeof(float));
					}
				}
			}
		}, 0.01);
		
    //based on https://stackoverflow.com/a/14391562
    auto now = std::chrono::high_resolution_clock::now();
    float diff = std::chrono::duration_cast<std::chrono::duration<float>>(now - then).count();
		if (diff > 0.015f) {
      state.update(diff);
			then = now;
      //send to p1 and p2, if not null
		}
	}
}
