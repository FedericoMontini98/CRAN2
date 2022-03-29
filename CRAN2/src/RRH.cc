//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "RRH.h"
Define_Module(RRH);

void RRH::initialize()
{
    timer_ = new cMessage("timer");

    packet_in_queue_ = registerSignal("rrhPacketInQueue");
    queueing_time_ = registerSignal("rrhQueueingTime");
    response_time_ = registerSignal("rrhResponseTime");
}

void RRH::handleMessage(cMessage *msg)
{
    if(msg->isSelfMessage()) {
        //decompression ended
        forwardPkt();

        if(queue.size() > 0){
            PktMessage *new_pkt = queue.front();
            decompressPkt(new_pkt);
        }
    } else {
        PktMessage* new_pkt = check_and_cast<PktMessage*>(msg);
        queue.push(new_pkt);
        if(queue.size() == 1)
            decompressPkt(new_pkt);
    }
}

void RRH::finish() {
    cancelAndDelete(timer_);

    while(!queue.empty()) {
        PktMessage *pkt = queue.front();
        queue.pop();
        delete pkt;
    }
}

void RRH::forwardPkt()
{
    PktMessage *to_transmit = queue.front();
    queue.pop();
    long in_queue = static_cast<long>(queue.size());
    emit(packet_in_queue_, in_queue);
    send(to_transmit, "out");
}

void RRH::decompressPkt(PktMessage *pkt)
{
    simtime_t queueing_t = simTime() - pkt->getArrivalTime();
    emit(queueing_time_, queueing_t);
    simtime_t decompression_time = 0;

    if(par("compression_used").boolValue()) {
        double alfa = par("alfa").doubleValue();
        double to_wait = (alfa * 50) * (par("compression_ratio").doubleValue());
        int new_size = ceil(pkt->getByteLength() / (1 - par("compression_ratio").doubleValue() / 100));
        pkt->setByteLength(new_size);

        decompression_time = SimTime(to_wait, (SimTimeUnit)-3);
    }

    scheduleAt(simTime() + decompression_time, timer_);

    emit(response_time_, queueing_t + decompression_time);
}
