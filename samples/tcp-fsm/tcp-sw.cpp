// Copyright (c) 2024.  Universidad Carlos III de Madrid
//
// Licensed under the Apache License, Version 2.0 (the "License");  you may not use
// this file except in compliance with the License.
//
// You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software distributed under
// the License is distributed on an "AS IS" BASIS,  WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied.  See the License for the
// specific language governing permissions and  limitations under the License.

#include "protocol_status.hpp"
#include "segment.hpp"
#include "tcp_event.hpp"
#include "tcp_state.hpp"

#include <iostream>

class fsm_tcp {
  public:
    fsm_tcp(tcp_state init) : state{init} { }

    // returns true is everything was ok, false otherwise
    // NOLINTBEGIN(readability-function-size)
    bool process_event(tcp_event e, tcp_segment const & rcv_segment) {
      if (e == tcp_event::not_supported) { return false; }

      switch (state) {
        case tcp_state::closed:
          switch (e) {
            case tcp_event::syscall_listen:
              state = tcp_state::listen;
              break;
            case tcp_event::syscall_connect:
              // send SYN
              {
                status.send_syn();
                state = tcp_state::syn_sent;
              }
              break;
            default:
              // send RST
              send(tcp_segment{segment_kind::rst});
              break;
          }
          break;
        case tcp_state::listen:
          switch (e) {
            case tcp_event::seg_syn:
              {
                status.send_syn_ack_on_listen(rcv_segment.seq);
                state = tcp_state::syn_rcvd;
              }
              break;
            default:
              // send RST
              send(tcp_segment{segment_kind::rst});
              break;
          }
          break;
        case tcp_state::syn_rcvd:
          switch (e) {
            case tcp_event::seg_ack:
              if (status.is_next_send(rcv_segment.ack)) {
                status.reset_dup_acks();
                state = tcp_state::established;
              } else {
                status.increase_dup_acks();
                // state remains the same
              }
              break;
            case tcp_event::seg_rst:
              status.reset_on_failure();
              state = tcp_state::closed;
              break;
            case tcp_event::syscall_close:
              status.reset();
              state = tcp_state::closed;
              break;
            default:
              // send RST
              send(tcp_segment{segment_kind::rst});
              break;
          }
          break;
        case tcp_state::syn_sent:
          switch (e) {
            case tcp_event::seg_syn:
              // simultaneous open
              {
                status.simultaneous_open(rcv_segment.seq);
                state = tcp_state::syn_rcvd;
              }
              break;
            case tcp_event::seg_syn_ack:
              if (status.is_next_send(rcv_segment.ack)) {
                status.send_ack_on_syn_sent(rcv_segment.seq);
                state = tcp_state::established;
              } else {
                status.send_reset();
                state = tcp_state::closed;
              }
              break;
            case tcp_event::seg_rst:
              status.reset_on_failure();
              state = tcp_state::closed;
              break;
            default:
              std::cout << "SYN_SENT: event ignored!\n";
              break;
          }
          break;
        case tcp_state::established:
          switch (e) {
            case tcp_event::seg_ack:
              if (status.is_next_send(rcv_segment.ack)) {
                status.reset_dup_acks();
              } else {
                status.increase_dup_acks();
              }
              break;
            case tcp_event::seg_fin:
            case tcp_event::syscall_close:
              status.reset();
              state = tcp_state::closed;
              break;
            default:
              std::cout << "ESTABLISHED: event unexpected!\n";
              return false;
          }
          break;
      }
      return true;
    }

    // NOLINTEND(readability-function-size)

  private:
    tcp_state state;  // current state
    protocol_status status;
};

int main() {
  fsm_tcp machine{tcp_state::closed};
  bool go_ahead = true;
  while (go_ahead) {
    auto [ev, segment] = wait_for_event();
    go_ahead           = machine.process_event(ev, segment);
  }

  return 0;
}
