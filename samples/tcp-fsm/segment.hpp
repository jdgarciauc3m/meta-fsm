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

#ifndef META_FSM_TCP_SEGMENT_HPP
#define META_FSM_TCP_SEGMENT_HPP

#include "metafsm/enum.hpp"

#include <cstdint>
#include <format>
#include <iostream>
#include <string_view>

// Kinds of segments
enum class segment_kind : std::uint8_t { syn, ack, syn_ack, fin, rst };

std::string_view kind_name(segment_kind kind) {
  using enum segment_kind;
  using namespace enum_meta;
  constexpr enum_names<segment_kind, syn, ack, syn_ack, fin, rst> names;
  return names[kind];
}

struct tcp_segment {
    segment_kind kind = segment_kind::syn;
    std::uint32_t seq = 0;
    std::uint32_t ack = 0;

    constexpr tcp_segment() = default;

    constexpr tcp_segment(segment_kind type_init) : tcp_segment{type_init, 0, 0} { }

    constexpr tcp_segment(segment_kind type_init, std::uint32_t seq_init)
      : tcp_segment{type_init, seq_init, 0} { }

    constexpr tcp_segment(segment_kind type_init, std::uint32_t seq_init, std::uint32_t ack_init)
      : kind{type_init}, seq{seq_init}, ack{ack_init} { }
};

std::ostream & operator<<(std::ostream & out, tcp_segment const & segment) {
  out << std::format("segment sent: {{flags: {}, seq: {}, ack: {}}}", kind_name(segment.kind),
                     segment.seq, segment.ack);
  return out;
}

void send(tcp_segment const & segment) {
  std::cout << segment << '\n';
}

#endif  // META_FSM_TCP_SEGMENT_HPP
