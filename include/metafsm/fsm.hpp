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

#include "metafsm/enum.hpp"

#include <concepts>
#include <format>
#include <functional>
#include <string>
#include <type_traits>

namespace fsm {

  template <auto event_id, auto target_state_id>
  struct to {
      static constexpr auto event = event_id;
      static constexpr auto state = target_state_id;

      static void run() { }
  };

  template <auto event_id, auto target_state_id, auto action_fun>
  struct to_doing : public to<event_id, target_state_id> {
      static constexpr auto action = action_fun;

      static void run() { std::invoke(action); }
  };

  template <typename T>
  concept transition = requires {
    T::event;
    T::state;
    requires(std::is_enum_v<decltype(T::event)>);
    requires(std::is_enum_v<decltype(T::state)>);
  };

  template <typename T>
  concept transition_action = transition<T> and requires { T::action(); };

  template <transition T>
  std::string to_string() {
    using namespace enum_meta;
    return std::format("transition: {} -> {}", enum_value_name<T::event>(),
                       enum_value_name<T::state>());
  }

  template <transition_action T>
  std::string to_string() {
    using namespace enum_meta;
    return std::format("transition: {} -> (action) -> {}", enum_value_name<T::event>(),
                       enum_value_name<T::state>());
  }

  template <auto state_id, typename T1, typename... T>
  struct state {
      static constexpr auto id = state_id;

      template <typename E>
      static auto next(E event) {
        if (event == T1::event) {
          T1::run();
          return T1::state;
        }
        if constexpr (sizeof...(T) > 0) { return state<state_id, T...>::next(event); }
        return id;
      }

      static constexpr auto to_string() {
        using namespace enum_meta;
        return std::format("{}\n{}\n", enum_value_name<id>(), fsm::to_string<T1>()) +
               (std::format("{}\n", fsm::to_string<T>()) + ...);
      }
  };

  namespace detail {
    template <typename ST1, typename... ST>
    auto next_state(auto current_state, auto next_event) {
      if (current_state == ST1::id) { return ST1::next(next_event); }
      if constexpr (sizeof...(ST) > 0) { return next_state<ST...>(current_state, next_event); }
      return current_state;
    }
  }  // namespace detail

  template <typename S, typename... ST>
  struct machine {
    public:
      machine(S initial_state) : current_state_{initial_state} { }

      void process_event(auto ev) {
        current_state_ = detail::next_state<ST...>(current_state_, ev);
      }

      [[nodiscard]] S current_state() const { return current_state_; }

      static auto to_string() {
        return std::format("State type = {}\n", enum_meta::type_name<S>()) +
               (ST::to_string() + ...);
      }

    private:
      S current_state_;
  };

}  // namespace fsm

#endif  // META_FSM_FSM_HPP
