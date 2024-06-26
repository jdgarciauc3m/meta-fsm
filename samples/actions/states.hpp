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

#ifndef META_FSM_STATES_HPP
#define META_FSM_STATES_HPP

#include "metafsm/enum.hpp"

#include <cstdint>
#include <string_view>

enum class state : std::uint8_t { A, B, C, D };

std::string_view state_name(state st) {
  using enum state;
  using namespace enum_meta;
  constexpr enum_names<state, A, B, C, D> names;
  return names[st];
}

#endif  // META_FSM_STATES_HPP
