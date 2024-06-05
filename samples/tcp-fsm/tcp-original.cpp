// Originally contributed by Jesus Martinez Cruz (jmcruz@uma.es)

#include <cstdint>
#include <iostream>

enum class FSMState { CLOSED, LISTEN, SYN_RCVD, SYN_SENT, ESTABLISHED };

enum class Event {
  syscall_connect,
  syscall_listen,
  syscall_close,
  seg_RST,
  seg_SYN,
  seg_SYN_ACK,
  seg_ACK,
  seg_FIN,
  NOT_SUPPORTED
};

enum class tcp_t { SYN, ACK, SYN_ACK, FIN, RST };

struct tcp_segment {
    tcp_t type;
    uint32_t seq;
    uint32_t ack;
};

struct test_item {
    Event ev;
    tcp_segment segment;
};

// X is the initial sequence of the local tcp entity
#define X 100
// Y is the initial sequence of the remote tcp entity
#define Y 200

// last cell must be Event::NOT_SUPPORTED to force finishing the fsm
test_item test_samples[] = {
  { Event::syscall_listen,                                               {}},
  {        Event::seg_SYN,                   {.type = tcp_t::SYN, .seq = Y}},
  {        Event::seg_ACK, {.type = tcp_t::ACK, .seq = Y + 1, .ack = X + 1}},
  {        Event::seg_FIN,               {.type = tcp_t::FIN, .seq = Y + 1}},

  {Event::syscall_connect,                                               {}},
  {        Event::seg_SYN,                   {.type = tcp_t::SYN, .seq = Y}},
  {        Event::seg_ACK, {.type = tcp_t::ACK, .seq = Y + 1, .ack = X + 1}},
  {        Event::seg_FIN,               {.type = tcp_t::FIN, .seq = Y + 1}},

  {Event::syscall_connect,                                               {}},
  {    Event::seg_SYN_ACK,     {.type = tcp_t::SYN, .seq = Y, .ack = X + 1}},
  {        Event::seg_FIN,               {.type = tcp_t::FIN, .seq = Y + 1}},

  {Event::syscall_connect,                                               {}},
  {    Event::seg_SYN_ACK,     {.type = tcp_t::SYN, .seq = Y, .ack = X + 1}},
  {        Event::seg_ACK,               {.type = tcp_t::ACK, .ack = X + 1}},
  {        Event::seg_FIN,               {.type = tcp_t::FIN, .seq = Y + 1}},

  {Event::syscall_connect,                                               {}},
  {        Event::seg_RST,                   {.type = tcp_t::SYN, .seq = Y}},

  {  Event::NOT_SUPPORTED,                                               {}}
};

auto wait_for_event() {
  static int i = 0;
  return test_samples[i++];
}

class fsm_tcp {
  public:
    fsm_tcp(FSMState init) : state{init} {
      isn                  = X;
      dup_acks             = 0;
      conn_failed_attempts = 0;
      snd_next             = isn;
    }

    void send(tcp_segment & segment) {
      std::cout << "segment sent: ";
      std::cout << "{flags: " << static_cast<int>(segment.type);  // ups
      std::cout << ",seq: " << segment.seq;
      std::cout << ",ack: " << segment.ack << "}\n";
    }

    // returns true is everything was ok, false otherwise
    bool process_event(Event e, tcp_segment const & rcv_segment) {
      if (e == Event::NOT_SUPPORTED) { return false; }

      switch (state) {
        case FSMState::CLOSED:
          switch (e) {
            case Event::syscall_listen:
              state = FSMState::LISTEN;
              break;
            case Event::syscall_connect:
              // envia SYN
              {
                tcp_segment seg{tcp_t::SYN, snd_next, 0};
                send(seg);
                snd_next++;
                state = FSMState::SYN_SENT;
              }
              break;
            default:
              // envia RST
              {
                tcp_segment seg{tcp_t::RST, 0, 0};
                send(seg);
              }
              break;
          }
          break;
        case FSMState::LISTEN:
          switch (e) {
            case Event::seg_SYN:
              {
                irn      = rcv_segment.seq;
                rcv_next = irn + 1;
                tcp_segment seg{tcp_t::SYN_ACK, snd_next, rcv_next};
                send(seg);
                snd_next++;
                state = FSMState::SYN_RCVD;
              }
              break;
            default:
              // envia RST
              {
                tcp_segment seg{tcp_t::RST, 0, 0};
                send(seg);
              }
              break;
          }
          break;
        case FSMState::SYN_RCVD:
          switch (e) {
            case Event::seg_ACK:
              if (rcv_segment.ack != snd_next) {
                dup_acks++;
                std::cout << "WARNING: dup ACK!\n";
              } else {
                dup_acks = 0;
                state    = FSMState::ESTABLISHED;
              }
              break;
            case Event::seg_RST:
              conn_failed_attempts++;
              snd_next = isn;
              dup_acks = 0;
              state    = FSMState::CLOSED;
              break;
            case Event::syscall_close:
              snd_next = isn;
              dup_acks = 0;
              // emulated
              state = FSMState::CLOSED;
              break;
            default:
              // envia RST
              {
                tcp_segment seg{tcp_t::RST, 0, 0};
                send(seg);
              }
              break;
          }
          break;
        case FSMState::SYN_SENT:
          switch (e) {
            case Event::seg_SYN:
              // simultaneous open
              {
                irn      = rcv_segment.seq;
                rcv_next = irn + 1;
                tcp_segment seg{tcp_t::SYN_ACK, snd_next - 1, rcv_next};
                send(seg);
                state = FSMState::SYN_RCVD;
              }
              break;
            case Event::seg_SYN_ACK:
              if (rcv_segment.ack != snd_next) {
                // envia RST
                tcp_segment seg{tcp_t::RST, 0, 0};
                send(seg);
                snd_next = isn;
                dup_acks = 0;
                state    = FSMState::CLOSED;
              } else {
                // envia ACK
                irn      = rcv_segment.seq;
                rcv_next = irn + 1;
                tcp_segment seg{tcp_t::ACK, snd_next, rcv_next};
                send(seg);
                state = FSMState::ESTABLISHED;
              }
              break;
            case Event::seg_RST:
              conn_failed_attempts++;
              snd_next = isn;
              dup_acks = 0;
              state    = FSMState::CLOSED;
              break;
            default:
              std::cout << "SYN_SENT: event ignored!\n";
              break;
          }
          break;
        case FSMState::ESTABLISHED:
          switch (e) {
            case Event::seg_ACK:
              if (rcv_segment.ack != snd_next) {
                dup_acks++;
                std::cout << "WARNING: dup ACK!\n";
              } else {
                dup_acks = 0;
              }
              break;
            case Event::seg_FIN:
              snd_next = isn;
              dup_acks = 0;
              state    = FSMState::CLOSED;
              break;
            case Event::syscall_close:
              snd_next = isn;
              dup_acks = 0;
              state    = FSMState::CLOSED;
              break;
            default:
              std::cout << "ESTABLISHED: event unexpected!\n";
              return false;
              break;
          }
          break;
      }
      return true;
    }

  private:
    FSMState state;  // current state

    // simulation params
    uint32_t isn;                 // initial sender seq number
    uint32_t irn;                 // initial recv seq number
    size_t dup_acks;              // number of duplicate acks received
    size_t conn_failed_attempts;  // failed connection attempts
    uint32_t snd_next;            // next seq to transmit
    uint32_t rcv_next;            // next expected byte
};

int main() {
  fsm_tcp machine{FSMState::CLOSED};
  bool go_ahead = true;
  while (go_ahead) {
    auto [ev, args] = wait_for_event();
    go_ahead        = machine.process_event(ev, args);
  }

  return 0;
}
