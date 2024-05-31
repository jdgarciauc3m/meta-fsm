#include "metafsm/fsm.hpp"

#include <gtest/gtest.h>

enum class state_id : std::uint8_t {
  off, on
};
enum class event_id : std::uint8_t {
  poweron, poweroff
};

using automata = fsm::machine<state_id,
    fsm::state<state_id::off,
        fsm::to<event_id::poweron, state_id::on>,
        fsm::to<event_id::poweroff, state_id::off>>,
    fsm::state<state_id::on,
        fsm::to<event_id::poweron, state_id::on>,
        fsm::to<event_id::poweroff, state_id::off>>>;

TEST(machine, process_event) {
  automata m{state_id::off};
  m.process_event(event_id::poweron);
  m.process_event(event_id::poweron);
  EXPECT_EQ(state_id::on, m.current_state());
}


