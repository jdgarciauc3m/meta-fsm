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

#include "simple-fsm.hpp"

#include <chrono>
#include <format>
#include <iostream>

int main() try {
  constexpr int num_events = 100'000;

  automata machine{state::A};
  using namespace std::chrono;
  auto start = high_resolution_clock::now();
  for (int i = 0; i < num_events; ++i) {
    auto ev = next_event(i);
    machine.process_event(ev);
  }
  auto end = high_resolution_clock::now();
  std::cout << std::format("# Transitions: {} - Final state {}\n", num_events,
                           state_name(machine.current_state()));
  std::cout << std::format("Elapsed: {}\n", end - start);
} catch (std::exception & ex) { std::cerr << "Exception: " << ex.what() << '\n'; }
