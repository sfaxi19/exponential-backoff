//
// Created by sfaxi19 on 09.11.17.
//

#include "AdvancedSystem.h"
#include "../log.h"

#define TRACE if(isTrace)
extern bool isLog;

using namespace asys;

void AdvancedSystem::nextTime() {
    TRACE printf("Time: %lu\n\n", m_time_scale);
    LOG log2file::out << "Time: " << std::to_string(m_time_scale) << "\n\n";
    subs::SubStates state;
    //std::list<size_t> senders;
    size_t sending_count = 0;
    size_t firstSenderID = 0;
    for (size_t i = 0; i < m_subscribers.size(); i++) {
        state = m_subscribers[i]->nextTime(m_lambda / m_subscribers.size(), m_time_scale);
        TRACE std::cout << m_subscribers[i]->toString();
        LOG log2file::out << m_subscribers[i]->toString();
        if (state == subs::SENDING) {
            if (sending_count == 0) firstSenderID = i;
            else m_subscribers[i]->fail();
            sending_count++;
        }
    }
    if (sending_count == 1) m_subscribers[firstSenderID]->success();
    else m_subscribers[firstSenderID]->fail();
    TRACE print_status(sending_count);
    TRACE std::cout << "\n";
    m_time_scale++;
}

void AdvancedSystem::test_suite(int time_limit) {
    for (int t = 0; t < time_limit; t++) {
        this->nextTime();
    }
    //std::cout << toString();
}

size_t AdvancedSystem::getTime() {
    return m_time_scale;
}

AdvancedSystem::~AdvancedSystem() {
    for (size_t i = 0; i < m_subscribers.size(); i++) {
        delete m_subscribers[i];
    }
    m_subscribers.clear();
}

AdvancedSystem::AdvancedSystem(size_t subscr_cnt, size_t width,
                               double lambda, bool trace) : AdvancedSystem(subscr_cnt, width, width, lambda, trace) {};

AdvancedSystem::AdvancedSystem(size_t subscr_cnt, size_t width_min,
                               size_t width_max, double lambda, bool trace) : System() {
    for (size_t i = 0; i < subscr_cnt; i++) {
        m_subscribers.push_back(new subs::Subscriber(i, width_min, width_max, trace));
    }
    m_lambda = lambda;
    isTrace = trace;
}

size_t AdvancedSystem::getCountSentPackets() {
    return 0;
}

void AdvancedSystem::print_status(size_t size) {
    switch (size) {
        case 0: {
            std::string line1;
            line1.append("                  +---------+\n");
            line1.append("                  |         |\n");
            line1.append("                  +---------+\n");
            std::cout << line1;
            break;
        }
        case 1: {
            std::string line1;
            line1.append("                  +---------+\n");
            line1.append("                  | SUCCESS |\n");
            line1.append("                  +---------+\n");
            std::cout << line1;
            break;
        }
        default: {
            std::string line1;
            line1.append("                  +---------+\n");
            line1.append("                  | FAILURE |\n");
            line1.append("                  +---------+\n");
            std::cout << line1;
            break;
        }
    }
}

AdvancedResult AdvancedSystem::getResult() {
    double delay_mean_sum = 0;
    size_t number_of_sent = 0;
    size_t bad_subs = 0; // it's subscriber which return mean delay as 0
    for (int i = 0; i < m_subscribers.size(); i++) {
        double delay_mean = m_subscribers[i]->getDelayMean();
        if (delay_mean == 0) bad_subs++;
        delay_mean_sum += delay_mean;
        TRACE printf("Delay mean:          %f\n", delay_mean);
        number_of_sent += m_subscribers[i]->getNumberOfSent();
    }
    TRACE printf("\n!!!\t Delay mean sum:          %f\n", delay_mean_sum);
    TRACE printf(  "!!!\t Delay mean mean:         %f\n", delay_mean_sum / (m_subscribers.size() - bad_subs));
    AdvancedResult m_result;
    m_result.lambda = m_lambda;
    m_result.subscribers = m_subscribers.size();
    m_result.sent_packets = number_of_sent;
    m_result.delay = delay_mean_sum / (m_subscribers.size() - bad_subs);
    //printf("\n\n%f / %d\n\n", delay_mean_sum, m_subscribers.size());
    m_result.out_stream = (double) m_result.sent_packets / m_time_scale;
    return m_result;
}

std::string AdvancedSystem::toString() {
    std::string str;
    for (size_t i = 0; i < m_subscribers.size(); i++) {
        str.append(m_subscribers[i]->toString());
    }
    return str;
}

void asys::print_results(AdvancedResult &result) {
    std::vector<AdvancedResult> results;
    results.push_back(result);
    print_results(results);
}

void asys::print_results(std::vector<AdvancedResult> &results) {
    printf("+-----------------------+"
                   "-----------------------+"
                   "-----------------------+"
                   "-----------------------+"
                   "-----------------------+\n");

    printf("|%20s\t|%20s\t|%20s\t|%20s\t|%20s\t|\n", "Lambda", "Subscribers", "Delay", "Sent Packets", "Out Stream");
    printf("+-----------------------+"
                   "-----------------------+"
                   "-----------------------+"
                   "-----------------------+"
                   "-----------------------+\n");
    for (int i = 0; i < results.size(); i++) {
        printf("|%20f\t|%20ld\t|%20f\t|%20ld\t|%20f\t|\n",
               results[i].lambda,
               results[i].subscribers,
               results[i].delay,
               results[i].sent_packets,
               results[i].out_stream);
    }
    printf("+-----------------------+"
                   "-----------------------+"
                   "-----------------------+"
                   "-----------------------+"
                   "-----------------------+\n");
}



