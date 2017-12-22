//
// Created by sfaxi19 on 09.11.17.
//

#ifndef INFO_SYS2_ADVANCEDSYSTEM_H
#define INFO_SYS2_ADVANCEDSYSTEM_H

#include "../System/System.h"
#include "vector"
#include "../System/Subscriber.h"

namespace asys {

    struct AdvancedResult {
        double lambda = 0;
        size_t subscribers = 0;
        size_t sent_packets = 0;
        double out_stream = 0;
        double delay = 0;
    };

    class AdvancedSystem : public System {
    private:
        std::vector<subs::Subscriber *> m_subscribers;
        bool isTrace = false;
        double m_lambda;

    public:

        AdvancedSystem(size_t subscr_cnt, size_t width, double lambda, bool trace);

        AdvancedSystem(size_t subscr_cnt, size_t width_min, size_t width_max, double lambda, bool trace);

        void nextTime() override;

        void test_suite(int time_limit) override;

        size_t getTime() override;

        ~AdvancedSystem() override;

        AdvancedResult getResult();

        size_t getCountSentPackets() override;

        void print_status(size_t);

        std::string toString();

    };

    void print_results(std::vector<AdvancedResult> &results);

    void print_results(AdvancedResult &results);
}

#endif //INFO_SYS2_ADVANCEDSYSTEM_H
