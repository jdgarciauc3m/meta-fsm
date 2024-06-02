
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

#include <gsl/gsl>
#include <string_view>

namespace enum_meta {

namespace detail {

  template <typename T>
  constexpr std::string_view function_name() {
#if defined(__GNUC__) or defined(__clang__)
    return __PRETTY_FUNCTION__;
#elif defined(_MSC_VER)
    return __FUNCSIG__;
#else
#error "Unsupported compiler"
#endif
  }

  template <auto value>
  constexpr std::string_view function_value_name() {
#if defined(__GNUC__) or defined(__clang__)
    return __PRETTY_FUNCTION__;
#elif defined(_MSC_VER)
    return __FUNCSIG__;
#else
  #error "Unsupported compiler"
#endif
  }

  consteval std::string_view type_prefix() {
#if defined(__GNUC__) or defined(__clang__)
    return "[with T = ";
#elif defined(_MSC_VER)
    return "function_name<";
#else
  #error "Unsupported compiler"
#endif
  }

  consteval std::string_view type_suffix() {
#if defined(__GNUC__) or defined(__clang__)
    return ";";
#elif defined(_MSC_VER)
    return ">(void)";
#else
  #error "Unsupported compiler"
#endif
  }

  consteval std::string_view enum_prefix() {
#if defined(__GNUC__) or defined(__clang__)
    return "[with auto value = ";
#elif defined(_MSC_VER)
    return "function_value_name<";
#else
  #error "Unsupported compiler"
#endif
  }

  consteval std::string_view enum_suffix() {
#if defined(__GNUC__) or defined(__clang__)
    return ";";
#elif defined(_MSC_VER)
    return ">(void)";
#else
  #error "Unsupported compiler"
#endif
  }

}

template <typename T>
constexpr std::string_view type_name() {
  constexpr std::string_view func_name = detail::function_name<T>();
  constexpr std::string_view prefix = detail::type_prefix();
  constexpr auto prefix_idx = func_name.find(prefix) + prefix.length();
  constexpr auto name_length = func_name.find(detail::type_suffix()) - prefix_idx;

  constexpr auto complete_name              = func_name.substr(prefix_idx, name_length);

  constexpr std::string_view key_enum = "enum ";
  constexpr auto key_enum_idx = complete_name.find(key_enum);
  if constexpr (key_enum_idx != std::string_view::npos) {
    return complete_name.substr(key_enum_idx + key_enum.length(), name_length - key_enum.length());
  }

  constexpr std::string_view key_struct = "struct ";
  constexpr auto key_struct_idx = complete_name.find(key_struct);
  if constexpr (key_struct_idx != std::string_view::npos) {
    return complete_name.substr(key_struct_idx + key_struct.length(), name_length - key_struct.length());
  }

  return func_name.substr(prefix_idx, name_length);
}

template <auto value>
constexpr std::string_view enum_value_name() {
  constexpr std::string_view func_name = detail::function_value_name<value>();
  constexpr std::string_view prefix = detail::enum_prefix();
  constexpr auto prefix_idx = func_name.find(prefix) + prefix.length();
  constexpr auto name_length = func_name.find(detail::enum_suffix()) - prefix_idx;
  return func_name.substr(prefix_idx, name_length);
}

template <typename E, E... values>
class enum_names {
  public:
    std::string_view operator[](E value) const {
      auto index = gsl::narrow_cast<std::size_t>(value);
      return gsl::span{names_}[index];
    }

  private:
    std::array<std::string_view, sizeof...(values)> names_{enum_value_name<values>()...};
};

}

#endif  // META_FSM_ENUM_HPP
