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

#include "BBU.h"

Define_Module(BBU);

void BBU::initialize()
{
    // Initializations
    tx_channel = NULL;
    pkt_queue = new cPacketQueue();
    timer_ = new cMessage();
    in_transit = false;     // at the beginnig, the channel is idle
    gate_size = gateSize("out");
    //EV << gate_size << " gate_size" << endl;

    // Signals
    occupation_queue_ = registerSignal("occupationQueue");
    queueing_time_ = registerSignal("queueingTime");
    response_time_ = registerSignal("responseTime");
    pkt_in_queue_ = registerSignal("packetInQueue");
    pkt_in_bbu_ = registerSignal("packetInBBU");

    // now idle, until AS sends a new packet
}

/*
 * receives AS packets and forwards them to the RRHs
 * two options: the message can arrive from the AS or from the BBU itself
 * packetQueue handled using a FIFO policy
 */
void BBU::handleMessage(cMessage *msg)
{
    if(msg->isSelfMessage()) {      // timer message, from itself
        in_transit = tx_channel->isBusy();
        if(in_transit) {    // channel is still busy -> reschedule the timer
            cancelEvent(msg);
            scheduleAt(tx_channel->getTransmissionFinishTime(), timer_);
        } else if(pkt_queue->getLength() > 0) {   // idle channel: if there is at least one packet queued, pop it and forward to the RRH
            in_transit = true;
            cPacket *pkt= pkt_queue->pop();
            sendPacket(pkt);
        }
    } else {   // new packet from the AS
        PktMessage *pkt = check_and_cast<PktMessage*>(msg);     // message cast to PktMessage type
        pkt->setTimestamp();    // timestamp used to compute the queueing time, set to the current sim-time

        long queue_length = static_cast<long>(pkt_queue->getByteLength());  // queue length in bytes
        emit(pkt_in_bbu_, pkt_queue->getLength() + (int)in_transit);        // number of packet in the BBU

        // the packet is queued only if the transmitted channel is busy or there are other packets in the queue
        if(in_transit || !pkt_queue->isEmpty()) {
            pkt_queue->insert(pkt);
            // EV << simTime() << " insert in queue" << endl;

        } else { // idle channel and empty queue -> new packet sent to the RRH without queueing
            EV << simTime() << " empty queue " << endl;
            in_transit = true;
            sendPacket(pkt);
        }
        emit(occupation_queue_, queue_length);      // signal for queue length in bytes
    }
}

void BBU::finish()
{
    cancelAndDelete(timer_);

    pkt_queue->clear();
    delete pkt_queue;
}

/*
 * function for compression of a packet, using the given level "compression_ratio"
 */
int BBU::compressPacket(cPacket *pkt) {
    int new_size = ceil(pkt->getByteLength() * (1 - par("compression_ratio").doubleValue() / 100));
    pkt->setByteLength(new_size);
    return new_size;
}

/*
 * forwards of the packet to the correct RRH,
 * records some statistics,
 * calls the compression function,
 * schedules a new timer
 */
void BBU::sendPacket(cMessage *msg) {
    emit(pkt_in_queue_, pkt_queue->getLength());
    PktMessage *pkt = check_and_cast<PktMessage*>(msg);

    int index_gate = pkt->getTarget_cell();     // read the target cell

    if(index_gate < 0 || index_gate >= gate_size) {     // if the target cell does not exists -> error
        error("BBU: no corresponding target gate");
    }

    tx_channel = gate("out", index_gate)->getTransmissionChannel();     // reference to the busy channel towards RRH

    simtime_t queueing_t = simTime() - pkt->getTimestamp();
    emit(queueing_time_, queueing_t);

    if(par("compression_enabled").boolValue()) {    // if compression enabled -> compress the packet
        int new_size = compressPacket(pkt);
        EV << "size compressed: " << new_size << endl;
    }
    simtime_t duration = tx_channel->calculateDuration(pkt);    // the transmission duration corresponds to the BBU service time
    simtime_t response_t = queueing_t + duration;

    // EV << "response_time: " << response_t << ", duration: " << duration <<endl;
    // EV << simTime() << " time before send " << endl;

    send(pkt, "out", index_gate);
    emit(response_time_, response_t);
    in_transit = tx_channel->isBusy();
    simtime_t finish_time_ = tx_channel->getTransmissionFinishTime();

    // EV << finish_time_ << " time after send" << endl;

    if(finish_time_ < simTime()) {      // "synchronize" the timer with the sim-time
        finish_time_ = simTime();
    }
    scheduleAt(finish_time_, timer_);

}
