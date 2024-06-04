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
#include "metafsm/fsm.hpp"

enum class state_id : std::uint8_t {
  off, on, running, stopped
};
enum class event_id : std::uint8_t {
  poweron, run, stop, poweroff
};

using basic_transition = fsm::to<event_id::poweron, state_id::on>;
static_assert(fsm::transition<basic_transition>);
static_assert(not fsm::transition_action<basic_transition>);

using output_transition = fsm::to_doing<event_id::poweron, state_id::on, []{}>;
static_assert(not fsm::transition_action<basic_transition>);
static_assert(fsm::transition<basic_transition>);

