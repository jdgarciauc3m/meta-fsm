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

#include <iostream>
#include <format>

#include "simple-sw.hpp"

int main() try {
  constexpr int num_events = 10;
  state current = state::A;
  for (int i = 0; i < num_events; ++i) {
    auto ev = next_event(i);
    auto next = next_state(current, ev);
    std::cout << std::format("{}: {} -> {} =>{}\n",
                             i, state_name(current), event_name(ev), state_name(next));
    current = next;
  }
}
catch (std::exception &ex) {
  std::cerr << "Exception: " << ex.what() << '\n';
}