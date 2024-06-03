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

#ifndef META_FSM_EVENTS_HPP
#define META_FSM_EVENTS_HPP

#include <cstdint>
#include <string_view>

#include "metafsm/enum.hpp"

enum class event : std::uint8_t {
  goA, goB, goC, goD
};

std::string_view event_name(event ev) {
  // clang-format off
  using
  enum event;
  // clang-format on
  using namespace enum_meta;
  constexpr enum_names<event, goA, goB, goC, goD> names;
  return names[ev];
}

constexpr std::array events{
    event::goA, event::goC, event::goD, event::goA,
    event::goB, event::goC, event::goA, event::goD,
};

event next_event(int i) {
  auto idx = gsl::narrow<std::size_t>(i) % events.size();
  return gsl::narrow<event>(gsl::span{events}[idx]);
}

#endif //META_FSM_EVENTS_HPP
