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

  struct do_nothing {
      void operator()(auto &&...) const { }
  };

  template <typename T>
  struct is_null_action : std::bool_constant<
      std::is_same_v<std::remove_cv_t<T>, do_nothing>> {};

  template <typename T>
  static constexpr bool is_null_action_v = is_null_action<T>::value;

  template <auto event_id, auto target_state_id, auto action_fun = do_nothing{}>
  struct on {
      static constexpr auto event = event_id;
      static constexpr auto state = target_state_id;

      using action_type            = decltype(action_fun);
      static constexpr auto action = action_fun;

      static void run(auto... data) {
        if constexpr (not std::is_same_v<action_type, do_nothing>) { std::invoke(action, data...); }
      }
  };

  template <typename T>
  concept transition = requires {
    typename T::action_type;
    T::event;
    T::state;
    requires(std::is_enum_v<decltype(T::event)>);
    requires(std::is_enum_v<decltype(T::state)>);
  };

  template <transition T>
  std::string to_string() {
    using namespace enum_meta;
    if constexpr (is_null_action_v<typename T::action_type>) {
      return std::format("transition: {} -> {}", enum_value_name<T::event>(),
                         enum_value_name<T::state>());
    } else {
      return std::format("transition: {} -> {} -> {}", enum_value_name<T::event>(),
                         type_name<typename T::action_type>(), enum_value_name<T::state>());
    }
  }

  template <auto state_id, typename T1, typename... T>
  struct state {
      static constexpr auto id = state_id;

      static auto next(auto event, auto... data) {
        if (event == T1::event) {
          T1::run(data...);
          return T1::state;
        }
        if constexpr (sizeof...(T) > 0) { return state<state_id, T...>::next(event, data...); }
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
    auto next_state(auto current_state, auto next_event, auto... data) {
      if (current_state == ST1::id) { return ST1::next(next_event, data...); }
      if constexpr (sizeof...(ST) > 0) {
        return next_state<ST...>(current_state, next_event, data...);
      }
      return current_state;
    }
  }  // namespace detail

  template <typename S, typename... ST>
  struct machine {
    public:
      machine(S initial_state) : current_state_{initial_state} { }

      void process_event(auto ev, auto... data) {
        current_state_ = detail::next_state<ST...>(current_state_, ev, data...);
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
