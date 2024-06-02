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

#ifndef META_FSM_FSM_HPP
#define META_FSM_FSM_HPP

#include <type_traits>
#include <concepts>
#include <string>
#include <format>

#include "metafsm/enum.hpp"

namespace fsm {

  template<auto event_id, auto target_state_id>
  struct to {
    static constexpr auto event = event_id;
    static constexpr auto state = target_state_id;
  };

  template<typename T>
  concept transition = requires {
    T::event;
    T::state;
    requires(std::is_enum_v<decltype(T::event)>);
    requires(std::is_enum_v<decltype(T::state)>);
  };

  template<transition T>
  std::string to_string() {
    using namespace enum_meta;
    return std::format("transition: {} -> {}",
                       enum_value_name<T::event>(),
                       enum_value_name<T::state>());
  }

  template<auto state_id, typename T1, typename ... T>
  struct state {
    static constexpr auto id = state_id;

    template <typename E>
    static auto next(E event) {
      if (event == T1::event) { return T1::state; }
      if constexpr (sizeof...(T) > 0) {
        return state<state_id, T...>::next(event);
      }
      return id;
    }

    static constexpr auto to_string() {
      using namespace enum_meta;
      return std::format("{}\n{}\n",enum_value_name<id>(), fsm::to_string<T1>()) +
      (std::format("{}\n",fsm::to_string<T>()) + ...);
    }

  };


  namespace detail {
    template<typename ST1, typename ... ST>
    auto next_state(auto current_state, auto next_event) {
      if (current_state == ST1::id) { return ST1::next(next_event); }
      if constexpr (sizeof...(ST) > 0) { return next_state<ST...>(current_state, next_event); }
      return current_state;
    }
  }

  template <typename S, typename... ST>
  struct machine {
  public:
    machine(S initial_state) : current_state_{initial_state} { }

    void process_event(auto ev) {
      current_state_ = detail::next_state<ST...>(current_state_, ev);
    }

    [[nodiscard]] S current_state() const { return current_state_; }

  private:
    S current_state_;
  };

}

#endif //META_FSM_FSM_HPP
