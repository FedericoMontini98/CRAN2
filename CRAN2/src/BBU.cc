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
    tx_channel = NULL;  //gate("out")->getTransmissionChannel();
    pkt_queue = new cPacketQueue();
    timer_ = new cMessage();
    in_transit = false;
    gate_size = gateSize("out");
    EV << gate_size << " gate_size" << endl;

    occupation_queue_ = registerSignal("occupationQueue");
    queueing_time_ = registerSignal("queueingTime");
    response_time_ = registerSignal("responseTime");
    pkt_in_queue_ = registerSignal("packetInQueue");
    pkt_in_bbu_ = registerSignal("packetInBBU");
}

void BBU::handleMessage(cMessage *msg)
{
    if(msg->isSelfMessage()) {
        EV << simTime() << " msg->isSelf, queue length: " << pkt_queue->getLength() << endl;
        in_transit = tx_channel->isBusy();
        /* extract packet from queue and send*/
        //EV << pkt_queue->getLength() << " -> length" << endl;
        if(in_transit) {
            //EV << "in transit selfmsg " << simTime() << endl;
            //EV << tx_channel->getTransmissionFinishTime() << " getFinishTime selfmsg" << endl;
            cancelEvent(msg);
            scheduleAt(tx_channel->getTransmissionFinishTime(), timer_);
        } else if(pkt_queue->getLength() > 0) {
            //EV << " not in transit, queue > 0 " << simTime() << endl;
            in_transit = true;
            cPacket *pkt= pkt_queue->pop();
            //emit(pkt_in_queue_, pkt_queue->getLength());
            sendPacket(pkt);
        }
    } else {
        // new packet from AS
        PktMessage *pkt = check_and_cast<PktMessage*>(msg);
        pkt->setTimestamp();
        EV << pkt->getTimestamp() << " get timestamp arriving new pkt " << endl;
        EV << simTime() << " arriving time" << endl;

        long queue_length = static_cast<long>(pkt_queue->getByteLength());
        emit(pkt_in_bbu_, pkt_queue->getLength() + (int)in_transit);

        // the packet is queued only if the transmitted channel is busy or there are other packets in the queue
        if(in_transit || !pkt_queue->isEmpty()) {
            pkt_queue->insert(pkt);
            EV << simTime() << " insert in queue" << endl;
        } else {
            // idle channel and empty queue
            EV << simTime() << " empty queue " << endl;
            in_transit = true;
            sendPacket(pkt);
        }
        emit(occupation_queue_, queue_length);
    }
}

void BBU::finish()
{
    cancelAndDelete(timer_);

    pkt_queue->clear();
    delete pkt_queue;
}

int BBU::compressPacket(cPacket *pkt) {
    int new_size = ceil(pkt->getByteLength() * (1 - par("compression_ratio").doubleValue() / 100));
    pkt->setByteLength(new_size);
    //EV << pkt->getByteLength() << " compres " << endl;
    return new_size;
}


void BBU::sendPacket(cMessage *msg) {
    emit(pkt_in_queue_, pkt_queue->getLength());
    PktMessage *pkt = check_and_cast<PktMessage*>(msg);
    int index_gate = pkt->getTarget_cell();
    EV << index_gate << " index target " << endl;
    if(index_gate < 0 || index_gate >= gate_size) {
        error("BBU: no corresponding target gate");
    }

    tx_channel = gate("out", index_gate)->getTransmissionChannel();

    simtime_t queueing_t = simTime() - pkt->getTimestamp();
    emit(queueing_time_, queueing_t);

    if(par("compression_enabled").boolValue()) {

        int new_size = compressPacket(pkt);
        EV << "size compressed: " << new_size << endl;
    }
    simtime_t duration = tx_channel->calculateDuration(pkt);
    simtime_t response_t = queueing_t + duration;
    EV << "response_time: " << response_t << ", duration: " << duration <<endl;

    EV << simTime() << " time before send " << endl;
    send(pkt, "out", index_gate);
    emit(response_time_, response_t);
    in_transit = tx_channel->isBusy();
    simtime_t finish_time_ = tx_channel->getTransmissionFinishTime();
    EV << finish_time_ << " time after send" << endl;
    EV << simTime() + duration << " simTime + duration" << endl;
    if(finish_time_ < simTime()) {
        //EV << "finish < simTime, simTime: " << simTime() << endl;
        finish_time_ = simTime();
    }
    scheduleAt(finish_time_, timer_);

}






