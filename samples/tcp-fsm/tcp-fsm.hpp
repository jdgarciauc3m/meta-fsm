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

#ifndef META_FSM_TCP_FSM_HPP
#define META_FSM_TCP_FSM_HPP

#include "metafsm/fsm.hpp"
#include "protocol_status.hpp"
#include "tcp_event.hpp"
#include "tcp_state.hpp"

#include <cstdint>
#include <iostream>
#include <string_view>

// clang-format off
using automata = fsm::machine<
    tcp_state,
    fsm::state<tcp_state::closed,
        // fsm::other_self<[] (auto & ...) { send(tcp_segment{segment_kind::rst}); }>
        fsm::to<tcp_event::syscall_listen, tcp_state::listen>,
        fsm::to<tcp_event::syscall_connect, tcp_state::syn_sent,
            [](tcp_segment &, protocol_status & st) { st.send_syn(); }>
    >,
    fsm::state<tcp_state::listen,
        // fsm::other_self<[] (auto & ...) { status.send_syn_ack_on_listen(rcv_segment.seq);
        fsm::to<tcp_event::seg_syn, tcp_state::syn_rcvd,
            [](tcp_segment & seg, protocol_status & st) { st.send_syn_ack_on_listen(seg.seq);}>
    >,
    fsm::state<tcp_state::syn_rcvd,
//      fsm::other_self<[] (auto & ...) { send(tcp_segment{segment_kind::rst}); }>
//        fsm::when<tcp_event::seg_ack,
//            [] (tcp_segment & seg, protocol_status & st){ return st.is_next_send(seg.ack); },
//            fsm::next<tcp_state::established,
//                [](tcp_segment & seg, protocol_status & st) {status.reset_dup_acks();}>,
//            fsm::next_self<
//                [](tcp_segment & seg, protocol_status & st) {status.increase_dup_acks();}>
//        >,
        fsm::to<tcp_event::seg_rst, tcp_state::closed,
            [](tcp_segment & , protocol_status & st) { st.reset_on_failure(); }>,
        fsm::to<tcp_event::syscall_close, tcp_state::closed,
            [](tcp_segment & , protocol_status & st) { st.reset(); }>
    >,
    fsm::state<tcp_state::syn_sent,
        //fsm::other_self<[](auto &...) { std::cout << "SYN_SENT: event ignored!\n"; }>
        fsm::to<tcp_event::seg_syn, tcp_state::syn_rcvd,
            [](tcp_segment & seg, protocol_status & st) { st.simultaneous_open(seg.seq); }>,
//        fsm::when<tcp_event::seg_syn_ack,
//            [] (tcp_segment & seg, protocol_status & st){ return st.is_next_send(seg.ack); },
//            fsm::next<tcp_state::established,
//                [] (tcp_segment & seg, protocol_status & st){ st.send_ack_on_syn_sent(seg.seq); }>,
//            fsm::next<tcp_state::closed,
//                [] (tcp_segment & seg, protocol_status & st){ st.reset(); }>
//        >
         fsm::to<tcp_event::seg_rst, tcp_state::closed,
              [] (tcp_segment & , protocol_status & st){ st.reset_on_failure(); }>
    >,
    fsm::state<tcp_state::established,
          //fsm::other_self<[](auto &...) { std::cout << "ESTABLISHED : event unexpected\n"; }>
//        fsm::when<tcp_event::seg_ack,
//            [] (tcp_segment & seg, protocol_status & st){ return st.is_next_send(seg.ack); },
//            fsm::next<tcp_state::established,
//                [] (tcp_segment & seg, protocol_status & st){ st.reset_dup_acks(seg.seq); }>,
//            fsm::next<tcp_state::closed,
//                [] (tcp_segment & seg, protocol_status & st){ st.increase_dup_acks(); }>
//        >
        fsm::to<tcp_event::seg_fin, tcp_state::closed,
            [] (tcp_segment & , protocol_status & st){ st.reset(); }>,
        fsm::to<tcp_event::syscall_close, tcp_state::closed,
            [] (tcp_segment & , protocol_status & st){ st.reset(); }>
    >
>;
// clang-format on

#endif  // META_FSM_TCP_FSM_HPP
