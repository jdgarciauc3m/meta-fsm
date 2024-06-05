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

#ifndef META_FSM_PROTOCOL_STATUS_HPP
#define META_FSM_PROTOCOL_STATUS_HPP

#include "segment.hpp"
#include "tcp_event.hpp"

#include <cstdint>
#include <iostream>

class protocol_status {
  public:
    protocol_status() = default;

    [[nodiscard]] bool is_next_send(std::uint32_t seq_id) const { return seq_id == snd_next; }

    void send_syn_ack_on_listen(std::uint32_t recv_seq) { send_syn_ack(recv_seq, snd_next++); }

    void send_ack_on_syn_sent(std::uint32_t recv_seq) { send_syn_ack(recv_seq, snd_next); }

    void simultaneous_open(std::uint32_t recv_seq) { send_syn_ack(recv_seq, snd_next - 1); }

    void increase_dup_acks() {
      dup_acks++;
      std::cout << "WARNING: dup ACK!\n";
    }

    void reset_dup_acks() { dup_acks = 0; }

    void send_syn() {
      send(tcp_segment{segment_kind::syn, snd_next});
      snd_next++;
    }

    void send_reset() {
      send(tcp_segment{segment_kind::rst, 0, 0});
      reset();
    }

    void reset_on_failure() {
      conn_failed_attempts++;
      reset();
    }

    void reset() {
      snd_next = protocol_status::sender_seq_id;
      dup_acks = 0;
    }

  private:
    void send_syn_ack(std::uint32_t recv_seq, std::uint32_t send_seq) {
      receiver_seq_id = recv_seq;
      rcv_next        = receiver_seq_id + 1;
      send(tcp_segment{segment_kind::syn_ack, send_seq, rcv_next});
    }

  private:
    static constexpr uint32_t sender_seq_id = local_sequence_start;  // initial sender seq number

    uint32_t receiver_seq_id    = 0;                     // initial recv seq number
    size_t dup_acks             = 0;                     // number of duplicate acks received
    size_t conn_failed_attempts = 0;                     // failed connection attempts
    uint32_t snd_next           = local_sequence_start;  // next seq to transmit
    uint32_t rcv_next           = 0;                     // next expected byte
};

#endif  // META_FSM_PROTOCOL_STATUS_HPP
