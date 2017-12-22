//
// Created by sfaxi19 on 09.11.17.
//

#ifndef INFO_SYS2_SUBSCRIBER_H
#define INFO_SYS2_SUBSCRIBER_H

#include "iostream"
#include "Packet.h"
#include "list"

namespace subs {

    enum Action : uint8_t {
        SENDING,
        SLEEP,
        WAIT,
        EMPTY
    };

    class Subscriber {
    private:
        size_t m_id = 0;
        size_t window_to_send = 0;
        size_t windows_counter = 0;

        std::list<Packet *> buffer;
        Action action = WAIT;


        size_t m_width_min = 0;
        size_t m_width_max = 0;
        size_t m_width = 0;

        size_t packet_id = 0;

        size_t current_time = 0;

        double m_delay_sum = 0;
        size_t m_number_of_sent = 0;

        bool isTrace = false;

    public:
        Subscriber(size_t id, size_t width_min, size_t width_max, bool trace);

        virtual ~Subscriber();

        Action refresh_buffer(double lambda, size_t time);

        void sendPacket();

        std::string toString();

        void fail();

        Packet success();

        double getDelayMean();

        size_t getNumberOfSent();

    };

}
#endif //INFO_SYS2_SUBSCRIBER_H
