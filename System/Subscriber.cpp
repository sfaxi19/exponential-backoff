//
// Created by sfaxi19 on 09.11.17.
//

#include <cmath>
#include "Subscriber.h"
#include "../MyRandom.h"

using namespace subs;

#define TRACE if(isTrace)

Subscriber::Subscriber(size_t id, size_t width_min, size_t width_max, bool trace) {
    //MyRandom::init();
    this->isTrace = trace;
    m_id = id;
    m_width_min = width_min;
    m_width_max = width_max;
    m_width = width_min;
    //printf("m_width_min = %lu\t\twidth_max = %lu\n", width_min, width_max);
}

Subscriber::~Subscriber() {
    std::list<Packet *>::iterator node;
    for (node = this->m_buffer.begin(); node != this->m_buffer.end(); node++) {
        delete *node;
    }
    m_buffer.clear();
}

SubStates Subscriber::nextTime(double lambda, size_t time) {
    this->m_current_time = time;
    size_t cnt_packets = MyRandom::getPuassonValue(lambda);
    for (size_t i = 0; i < cnt_packets; i++) {
        m_buffer.push_back(new Packet(m_packet_id++, time));
    }
    switch (m_state) {
        case SubStates::WAIT:
            if (!m_buffer.empty()) send();
            break;
        case SubStates::SENDING:
            break;
        case SubStates::SLEEP:
            if (m_windows_counter == m_window_to_send) {
                m_state = SENDING;
            }
            m_windows_counter++;
            break;
        default:
            break;
    };
    return m_state;
}


void Subscriber::send() {
    m_state = SubStates::SENDING;
}

void Subscriber::fail() {
    if (m_state == SENDING) {
        m_state = SLEEP;
        m_width *= 2;
        m_window_to_send = (size_t) floor(MyRandom::getRand() * m_width);
        m_windows_counter = 0;
        if (m_width > m_width_max) m_width = m_width_max;
    }
}

void Subscriber::success() {
    if (m_state == SENDING) {
        m_buffer.front()->out = m_current_time;
        Packet *packet = m_buffer.front();
        m_delay_sum += (packet->out + 1) - packet->in;
        m_number_of_sent++;
        delete m_buffer.front();
        m_buffer.pop_front();
        m_width = m_width_min;
        m_state = WAIT;
        m_window_to_send = 0;
        m_windows_counter = 0;
    }
    //if (buffer.size() == 0) m_width = m_width_min;
}

double Subscriber::getDelayMean() {
    auto it = m_buffer.begin();
    size_t number_of_not_sent = 0;
    for (; it != m_buffer.end(); it++) {
        number_of_not_sent++;
        m_delay_sum += (m_current_time + 1 - (*it)->in);
        //printf("[%lu - %f]\n", current_time + 1, (*it)->in);
    }
    TRACE printf("\nDelay sum:            %f\n", m_delay_sum);
    TRACE printf("Number of sent:     %lu\n", m_number_of_sent);
    TRACE printf("Number of NOT sent: %lu\n", number_of_not_sent);
    if (m_number_of_sent + number_of_not_sent == 0) return 0;
    return (m_delay_sum) / (m_number_of_sent + number_of_not_sent);
}

size_t Subscriber::getNumberOfSent() {
    return m_number_of_sent;
}

std::string Subscriber::toString() {
    std::string line1;
    std::string line2;
    std::string line3;
    line1.append("+==================================================")
            .append(" Subscriber #")
            .append(std::to_string(this->m_id) + " [")
            .append(std::to_string(m_packet_id - m_buffer.size())).append("] ")
            .append("===================================================")
            .append("\n");
    size_t size_str = line1.size() - 1;
    line2.append("| W: ").append(std::to_string(m_width))
            .append(" | S: ").append(std::to_string(m_window_to_send))
            .append(" | C: ").append(std::to_string(m_windows_counter))
            .append(" |");
    line2.append("\t\tBuffer:\t");
    std::list<Packet *>::reverse_iterator node;
    if (this->m_buffer.begin() == this->m_buffer.end()) {
        line2.append(" | empty");
    }
    for (node = this->m_buffer.rbegin(); node != this->m_buffer.rend(); node++) {
        line2.append(" | ");
        line2.append("packet" + std::to_string((*node)->id)).append(
                " [" + std::to_string(m_current_time + 1 - (*node)->in) + "]");
    }
    if (this->m_state == SENDING) line2.append(" -> ");
    if (this->m_state == SLEEP) line2.append(" -?");
    if (this->m_state == WAIT) line2.append(" ... ");
    //if (this->action == Action::EMPTY) line2.append(" ( - )");

    line2.append("\n");
    line3.append("+");
    for (int i = 0; i < size_str - 1; i++) {
        line3.append("=");
    }
    line3.append("\n");
    line3.append(subs::toString(m_state)).append("\n");
    return line1 + line2 + line3;
}

const char *subs::toString(SubStates value) {
    switch (value) {
        case SubStates::SENDING:
            return "SENDING_STATE";
        case SubStates::SLEEP:
            return "SLEEP_STATE";
        case SubStates::WAIT:
            return "WAIT_STATE";
        default:
            return "Invalid_STATE";
    }
}
