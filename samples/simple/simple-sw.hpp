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

#ifndef META_FSM_SIMPLE_SW_HPP
#define META_FSM_SIMPLE_SW_HPP

#include <cstdint>
#include <array>

#include <gsl/gsl>

#include "states.hpp"
#include "events.hpp"

// NOLINTBEGIN(readability-function-size)
state next_state(state current_state, event next_event) {
  using
  enum state;
  using
  enum event;
  switch (current_state) {
    case A:
      switch (next_event) {
        case goB:
          return B;
        case goC:
          return C;
        case goD:
          return D;
        default:
          return A;
      }
    case B:
      switch (next_event) {
        case goA:
          return A;
        case goC:
          return C;
        case goD:
          return D;
        default:
          return B;
      }
    case C:
      switch (next_event) {
        case goA:
          return A;
        case goB:
          return B;
        case goD:
          return D;
        default:
          return C;
      }
    case D:
      switch (next_event) {
        case goA:
          return A;
        case goB:
          return B;
        case goC:
          return C;
        default:
          return D;
      }
  }
  std::unreachable();
}
// NOLINTEND(readability-function-size)


#endif //META_FSM_SIMPLE_SW_HPP
