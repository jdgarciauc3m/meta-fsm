#include "metafsm/enum.hpp"

#include <gtest/gtest.h>

TEST(type_name, primitive) {
  EXPECT_EQ("int", enum_meta::type_name<int>());
}

struct point {
  double x, y;
};
TEST(type_name, udt) {
  EXPECT_EQ("point", enum_meta::type_name<point>());
}

// NOLINTBEGIN(cppcoreguidelines-avoid-c-arrays,modernize-avoid-c-arrays)
// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
using array_type = double[10];
// NOLINTEND(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
// NOLINTEND(cppcoreguidelines-avoid-c-arrays,modernize-avoid-c-arrays)
TEST(type_name, array) {
  EXPECT_TRUE(("double [10]"==enum_meta::type_name<array_type>()) or 
              ("double[10]"==enum_meta::type_name<array_type>()));
}

enum class color : std::uint8_t { red, green, blue };
TEST(type_name, scoped_enum) {
  EXPECT_EQ("color",enum_meta::type_name<color>());
}

TEST(enum_value_name, value_red) {
  EXPECT_EQ("color::red", enum_meta::enum_value_name<color::red>());
}

TEST(enum_names, find_name) {
  using enum color;
  enum_meta::enum_names<color, red, green, blue> names;
  EXPECT_EQ("color::red", names[red]);
  EXPECT_EQ("color::green", names[green]);
  EXPECT_EQ("color::blue", names[blue]);
}