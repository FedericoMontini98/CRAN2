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
    //Initializations
    tx_channel = NULL;  //gate("out")->getTransmissionChannel();
    pkt_queue = new cPacketQueue();
    timer_ = new cMessage();
    in_transit = false;
    gate_size = gateSize("out");
    //EV << gate_size << " gate_size" << endl;

    // Signals
    occupation_queue_ = registerSignal("occupationQueue");
    queueing_time_ = registerSignal("queueingTime");
    response_time_ = registerSignal("responseTime");
    pkt_in_queue_ = registerSignal("packetInQueue");
    pkt_in_bbu_ = registerSignal("packetInBBU");

    //Now idle, until AS sends a new packet
}

//Function that receives AS packets and forward them to the RRH, the packet may be inside the queue, from which we take it using a FIFO policy or,
//if the queue is empty, we may go idle of queue is empty or forward the packet that has been just sent from the AS
void BBU::handleMessage(cMessage *msg)
{
    //Two options: The message can arrive from the AS or from the BBU itself
    //If the packet is a self message the service the actual packet that was in transmission has been sent, we can proceed with the next packet
    if(msg->isSelfMessage()) {
        // Check on the state of the channel
        in_transit = tx_channel->isBusy();
        if(in_transit) {
            // If the channel is still busy -> fix the timer
            cancelEvent(msg);
            scheduleAt(tx_channel->getTransmissionFinishTime(), timer_);
        }
        // Pop a packet from the queue and forward to the RRH
        else if(pkt_queue->getLength() > 0) {
            in_transit = true;
            cPacket *pkt= pkt_queue->pop();
            sendPacket(pkt);
        }
    }
    // Otherwise the packet came from the AS and we have to understand if the channel is free or busy with another packet transmission
    else {
        //Cast of a cMessage to a PktMessage
        PktMessage *pkt = check_and_cast<PktMessage*>(msg);
        pkt->setTimestamp();

        //Emit of the number of packets statistic
        long queue_length = static_cast<long>(pkt_queue->getByteLength());
        emit(pkt_in_bbu_, pkt_queue->getLength() + (int)in_transit);

        //The packet is queued only if the transmitted channel is busy or there are other packets in the queue
        if(in_transit || !pkt_queue->isEmpty()) {
            pkt_queue->insert(pkt);
            /*EV << simTime() << " insert in queue" << endl;*/
        } else {
            // idle channel and empty queue, we can forward it to the RRH
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

//Function that perform the packet compression with the chosen ratio "compression_ratio"
int BBU::compressPacket(cPacket *pkt) {
    int new_size = ceil(pkt->getByteLength() * (1 - par("compression_ratio").doubleValue() / 100));
    pkt->setByteLength(new_size);
    return new_size;
}

//Function that perform the forwarding of the packet to the RRH
void BBU::sendPacket(cMessage *msg) {
    emit(pkt_in_queue_, pkt_queue->getLength());
    PktMessage *pkt = check_and_cast<PktMessage*>(msg);
    //Extract the target cell
    int index_gate = pkt->getTarget_cell();
    // Check the existence of that cell
    if(index_gate < 0 || index_gate >= gate_size) {
        error("BBU: no corresponding target gate");
    }

    tx_channel = gate("out", index_gate)->getTransmissionChannel();

    simtime_t queueing_t = simTime() - pkt->getTimestamp();
    emit(queueing_time_, queueing_t);

    //If the compression is enabled perform it
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

    //If the time needed to forward is larger than the time left for the simulation
    if(finish_time_ < simTime()) {
        finish_time_ = simTime();
    }
    scheduleAt(finish_time_, timer_);

}
