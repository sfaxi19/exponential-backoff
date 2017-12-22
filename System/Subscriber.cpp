//
// Created by sfaxi19 on 09.11.17.
//

#include <cmath>
#include "Subscriber.h"
#include "../MyRandom.h"

using namespace subs;

#define TRACE if(isTrace)

Subscriber::Subscriber(size_t id, size_t width_min, size_t width_max, bool trace) {
    MyRandom::init();
    this->isTrace = trace;
    m_id = id;
    m_width_min = width_min;
    m_width_max = width_max;
    m_width = width_min;
    //printf("m_width_min = %lu\t\twidth_max = %lu\n", width_min, width_max);
}

Subscriber::~Subscriber() {
    std::list<Packet *>::iterator node;
    for (node = this->buffer.begin(); node != this->buffer.end(); node++) {
        delete *node;
    }
    buffer.clear();
}

void Subscriber::sendPacket() {
    if (buffer.empty()) {
        m_width = m_width_min;
        this->window_to_send = 0;
        this->windows_counter = 0;
        action = EMPTY;
    } else {
        if (action == EMPTY) action = WAIT;
    }
    if (action == WAIT) {
        this->windows_counter = 0;
        this->window_to_send = (size_t) floor(MyRandom::getRand() * m_width);
        this->action = SLEEP;
    }
    if (action == SLEEP) {
        if (this->windows_counter == this->window_to_send) {
            action = SENDING;
        }
        this->windows_counter++;
    }
    if (action == EMPTY) this->windows_counter = 1;
}

Action Subscriber::refresh_buffer(double lambda, size_t time) {
    this->current_time = time;
    size_t cnt_packets = MyRandom::getPuassonValue(lambda);
    for (size_t i = 0; i < cnt_packets; i++) {
        buffer.push_back(new Packet(packet_id++, time));
    }
    sendPacket();

    return action;
}


std::string Subscriber::toString() {
    std::string line1;
    std::string line2;
    std::string line3;
    line1.append("+==================================================")
            .append(" Subscriber #")
            .append(std::to_string(this->m_id) + " [")
            .append(std::to_string(packet_id - buffer.size())).append("] ")
            .append("===================================================")
            .append("\n");
    size_t size_str = line1.size() - 1;
    line2.append("| W: ").append(std::to_string(m_width))
            .append(" | S: ").append(std::to_string(window_to_send + 1))
            .append(" | C: ").append(std::to_string(windows_counter))
            .append(" |");
    line2.append("\t\tBuffer:\t");
    std::list<Packet *>::reverse_iterator node;
    if (this->buffer.begin() == this->buffer.end()) {
        line2.append(" | empty");
    }
    for (node = this->buffer.rbegin(); node != this->buffer.rend(); node++) {
        line2.append(" | ");
        line2.append("packet" + std::to_string((*node)->id)).append(
                " [" + std::to_string(current_time + 1 - (*node)->in) + "]");
    }
    if (this->action == SENDING) line2.append(" -> ");
    if (this->action == SLEEP) line2.append(" -?");
    if (this->action == WAIT) line2.append(" ( ? )");
    //if (this->action == Action::EMPTY) line2.append(" ( - )");

    line2.append("\n");
    line3.append("+");
    for (int i = 0; i < size_str - 1; i++) {
        line3.append("=");
    }
    line3.append("\n");
    return line1 + line2 + line3;
}

void Subscriber::fail() {
    action = WAIT;
    m_width *= 2;
    if (m_width > m_width_max) m_width = m_width_max;
}

Packet Subscriber::success() {
    buffer.front()->out = current_time;
    Packet packet = *buffer.front();
    m_delay_sum += (packet.out + 1) - packet.in;
    m_number_of_sent++;
    delete buffer.front();
    buffer.pop_front();
    m_width = m_width_min;
    if (m_width > m_width_max) m_width = m_width_max;
    action = WAIT;
    if (buffer.size() == 0) m_width = m_width_min;
    return packet;
}

double Subscriber::getDelayMean() {
    auto it = buffer.begin();
    size_t number_of_not_sent = 0;
    for (; it != buffer.end(); it++) {
        number_of_not_sent++;
        m_delay_sum += (current_time + 1 - (*it)->in);
        //printf("[%lu - %f]\n", current_time + 1, (*it)->in);
    }
    TRACE printf("\nDelay:            %f\n", m_delay_sum);
    TRACE printf("Number of sent:     %lu\n", m_number_of_sent);
    TRACE printf("Number of NOT sent: %lu\n", number_of_not_sent);
    if (m_number_of_sent + number_of_not_sent == 0) return 0;
    return (m_delay_sum) / (m_number_of_sent + number_of_not_sent);
}

size_t Subscriber::getNumberOfSent() {
    return m_number_of_sent;
}



