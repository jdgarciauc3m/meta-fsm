#include "metafsm/fsm.hpp"

#include <gtest/gtest.h>

enum class state_id : std::uint8_t {
  off, on, running, stopped
};
enum class event_id : std::uint8_t {
  poweron, run, stop, poweroff
};

using on_state = fsm::state<state_id::on,
    fsm::on<event_id::run, state_id::running>,
    fsm::on<event_id::poweroff, state_id::off>
>;

TEST(state, next_default) {
  EXPECT_EQ(state_id::on, on_state::next(event_id::poweron));
  EXPECT_EQ(state_id::running, on_state::next(event_id::run));
  EXPECT_EQ(state_id::on, on_state::next(event_id::stop));
  EXPECT_EQ(state_id::off, on_state::next(event_id::poweroff));
}

TEST(state, to_string) {
  std::string const result = "state_id::on\n"
                             "transition: event_id::run -> state_id::running\n"
                             "transition: event_id::poweroff -> state_id::off\n";
  EXPECT_EQ(result, on_state::to_string());
}