
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

#ifndef META_FSM_ENUM_HPP
#define META_FSM_ENUM_HPP

#include <string_view>

#include <gsl/gsl>

template<typename T>
constexpr std::string_view type_name() {
  constexpr std::string_view func_name{__PRETTY_FUNCTION__};
  constexpr std::string_view prefix = "[with T = ";
  constexpr auto prefix_idx = func_name.find(prefix) + prefix.length();
  constexpr auto name_length = func_name.find(';') - prefix_idx;
  return func_name.substr(prefix_idx, name_length);
}


template<auto value>
constexpr std::string_view enum_value_name() {
  constexpr std::string_view func_name{__PRETTY_FUNCTION__};
  constexpr std::string_view prefix = "[with auto value = ";
  constexpr auto prefix_idx = func_name.find(prefix) + prefix.length();
  constexpr auto name_length = func_name.find(';') - prefix_idx;
  return func_name.substr(prefix_idx, name_length);
}

template<typename E, E ... values>
class enum_names {
public:
  std::string_view operator[](E value) const {
    auto index = gsl::narrow_cast<std::size_t>(value);
    return gsl::span{names_}[index];
  }

private:
  std::array<std::string_view, sizeof...(values)> names_{enum_value_name<values>()...};
};


#endif //META_FSM_ENUM_HPP
