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

#include "action-fsm.hpp"
#include "metafsm/fsm.hpp"

#include <format>
#include <iostream>

int main() try {
  automata aut{state::A};
  std::cout << aut.to_string() << '\n';

  constexpr int num_events = 10;
  for (int i = 0; i < num_events; ++i) {
    auto ev = next_event(i);
    auto prev_state = aut.current_state();
    aut.process_event(ev);
    auto post_state = aut.current_state();
    std::cout << std::format("{}: {} -> {} =>{}\n", i, state_name(prev_state), event_name(ev), state_name(post_state));
  }
}
catch (std::exception &ex) {
  std::cerr << "Exception: " << ex.what() << '\n';
}
