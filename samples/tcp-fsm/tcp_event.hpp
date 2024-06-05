
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

#ifndef META_FSM_TCP_EVENT_HPP
#define META_FSM_TCP_EVENT_HPP

#include "segment.hpp"

#include <cstdint>

enum class tcp_event : std::uint8_t {
  syscall_connect,
  syscall_listen,
  syscall_close,
  seg_rst,
  seg_syn,
  seg_syn_ack,
  seg_ack,
  seg_fin,
  not_supported
};

template <tcp_event event>
constexpr auto make_segment(std::uint32_t seq_id = 0, std::uint32_t ack_id = 0) {
  tcp_segment segment{};
  switch (event) {
    case tcp_event::seg_syn:
      segment = {segment_kind::syn, seq_id, ack_id};
    case tcp_event::seg_ack:
      segment = {segment_kind::ack, seq_id, ack_id};
    case tcp_event::seg_syn_ack:
      segment = {segment_kind::syn, seq_id, ack_id};
    case tcp_event::seg_fin:
      segment = {segment_kind::fin, seq_id, ack_id};
    case tcp_event::seg_rst:
      segment = {segment_kind::rst, seq_id, ack_id};
    default:
      break;
  }
  return std::tuple{event, segment};
}

// X is the initial sequence of the local tcp entity
constexpr int local_sequence_start = 100;
// Y is the initial sequence of the remote tcp entity
constexpr int remote_sequence_start = 200;

// last cell must be tcp_event::NOT_SUPPORTED to force finishing the fsm
constexpr std::array test_samples{
  make_segment<tcp_event::syscall_listen>(),
  make_segment<tcp_event::seg_syn>(remote_sequence_start),
  make_segment<tcp_event::seg_ack>(remote_sequence_start + 1, local_sequence_start + 1),
  make_segment<tcp_event::seg_fin>(remote_sequence_start + 1),

  make_segment<tcp_event::syscall_connect>(),
  make_segment<tcp_event::seg_syn>(remote_sequence_start),
  make_segment<tcp_event::seg_ack>(remote_sequence_start + 1, local_sequence_start + 1),
  make_segment<tcp_event::seg_fin>(remote_sequence_start + 1),

  make_segment<tcp_event::syscall_connect>(),
  make_segment<tcp_event::seg_syn_ack>(remote_sequence_start, local_sequence_start + 1),
  make_segment<tcp_event::seg_fin>(remote_sequence_start + 1),

  make_segment<tcp_event::syscall_connect>(),
  make_segment<tcp_event::seg_syn_ack>(remote_sequence_start, local_sequence_start + 1),
  make_segment<tcp_event::seg_ack>(0, local_sequence_start + 1),
  make_segment<tcp_event::seg_fin>(remote_sequence_start + 1),

  make_segment<tcp_event::syscall_connect>(),
  make_segment<tcp_event::seg_rst>(remote_sequence_start),

  make_segment<tcp_event::not_supported>()};

auto wait_for_event() {
  static std::size_t i = 0;
  return test_samples[i++];  // NOLINT
}

#endif  // META_FSM_TCP_EVENT_HPP
