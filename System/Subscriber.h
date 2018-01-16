//
// Created by sfaxi19 on 09.11.17.
//

#ifndef INFO_SYS2_SUBSCRIBER_H
#define INFO_SYS2_SUBSCRIBER_H

#include "iostream"
#include "Packet.h"
#include "list"

namespace subs {

    enum SubStates : uint8_t {
        SENDING,
        SLEEP,
        WAIT,
        EMPTY
    };

    class Subscriber {
    private:
        size_t m_id = 0;
        size_t m_window_to_send = 0;
        size_t m_windows_counter = 0;

        std::list<Packet *> m_buffer;
        SubStates m_state = WAIT;


        size_t m_width_min = 0;
        size_t m_width_max = 0;
        size_t m_width = 0;

        size_t m_packet_id = 0;

        size_t m_current_time = 0;

        double m_delay_sum = 0.0;
        size_t m_number_of_sent = 0;

        bool isTrace = false;

    public:
        Subscriber(size_t id, size_t width_min, size_t width_max, bool trace);

        virtual ~Subscriber();

        SubStates nextTime(double lambda, size_t time);

        void send();

        void fail();

        void success();

        double getDelayMean();

        size_t getNumberOfSent();

        std::string toString();

    };

    const char *toString(SubStates value);
}
#endif //INFO_SYS2_SUBSCRIBER_H
