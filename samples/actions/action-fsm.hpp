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

#ifndef META_FSM_SIMPLE_FSM_HPP
#define META_FSM_SIMPLE_FSM_HPP

#include <cstdint>
#include <string_view>
#include <iostream>

#include "metafsm/fsm.hpp"

#include "events.hpp"
#include "states.hpp"

using automata = fsm::machine<
    state,
    fsm::state<state::A,
        fsm::to_doing<event::goB, state::B, [] { std::cout<< "  A -> (action) ->B\n"; }>,
        fsm::to_doing<event::goC, state::C, [] { std::cout<< "  A -> (action) ->C\n"; }>,
        fsm::to<event::goD, state::D>>,
    fsm::state<state::B,
        fsm::to<event::goA, state::A>,
        fsm::to_doing<event::goC, state::C, [] { std::cout<< "  B -> (action) ->C\n"; }>,
        fsm::to<event::goD, state::D>>,
    fsm::state<state::C,
        fsm::to<event::goA, state::A>,
        fsm::to_doing<event::goB, state::B, [] { std::cout<< "  C -> (action) ->B\n"; }>,
        fsm::to<event::goD, state::D>>,
    fsm::state<state::D,
        fsm::to<event::goA, state::A>,
        fsm::to_doing<event::goB, state::B, [] { std::cout<< "  D -> (action) ->B\n"; }>,
        fsm::to_doing<event::goC, state::C, [] { std::cout<< "  D -> (action) ->C\n"; }>
        >
>;


#endif //META_FSM_SIMPLE_FSM_HPP
