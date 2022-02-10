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
    sending_pkt = NULL;
    tx_channel = gate("out")->getTransmissionChannel();
    msg_timer = new cMessage();
    gate_size = gateSize("out");

    occupation_queue_ = registerSignal("occupationQueue");
    queueing_time_ = registerSignal("queueingTime");
    response_time_ = registerSignal("responseTime");
    //emit(occupation_Queue_, pktQueue.getByteLength());
    //msg = new PktMessage();
    //msg->setByteLength(5);

}

// TODO not finish
void BBU::handleMessage(cMessage *msg)
{
    if(msg->isSelfMessage()) {
        // extract packet from queue and send
    } else {
        // new packet from AS
    }
    PktMessage *pkt = check_and_cast<PktMessage*>(msg);

    // the packet is queued only if there are other packets in the queue or the transmitted channel is busy
    if(!pkt_queue->empty() || tx_channel->isBusy()) {
        simtime_t enqueue_time = simTime();
        pkt->setTimestamp(enqueue_time);
        pkt->setEnqueue_time(enqueue_time);
        pkt_queue->insert(pkt);
    }
    emit(occupation_queue_, pkt_queue->getByteLength());

    // the channel towards the rrh is idle
    if(!sending_pkt) {
        sending_pkt = pkt_queue->empty() ? pkt : check_and_cast<PktMessage*>(pkt_queue->pop());
        int index_gate = sending_pkt->getTarget_cell();
        if(index_gate < 0 || index_gate >= gate_size) {
            error("BBU: no corresponding target gate");
        }
        //simtime_t queueing_t = simTime() -  pkt->getEnqueue_time();
        simtime_t queueing_t = simTime() -  pkt->getTimestamp();
        emit(queueing_time_, queueing_t);

        if(par("pkt_compression").boolValue()) {
            int new_size = compressPacket(sending_pkt);
            EV << "size compressed: " << new_size << endl;
        }

        send(sending_pkt, "out");
        scheduleAt(simTime() + tx_channel->getTransmissionFinishTime(), msg_timer);

    }
}

void BBU::finish()
{

}

int BBU::compressPacket(cPacket *pkt) {
    int new_size = ceil(pkt->getByteLength() * ((100 - par("compression_rate").intValue()) / 100));
    pkt->setByteLength(new_size);
    return new_size;
}
