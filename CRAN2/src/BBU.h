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

#ifndef __C_RAN2_BBU_H_
#define __C_RAN2_BBU_H_

#include <omnetpp.h>

#include "PktMessage_m.h"

using namespace omnetpp;

class BBU : public cSimpleModule
{
    cPacketQueue *pkt_queue;
    cChannel *tx_channel;
    cMessage *timer_;
    bool in_transit;

    //Number of RRH/cell pairs: Used to check that the destination cell really exists
    int gate_size;

    //Signals
    simsignal_t occupation_queue_;
    simsignal_t queueing_time_;
    simsignal_t response_time_;
    simsignal_t pkt_in_queue_;
    simsignal_t pkt_in_bbu_;

  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    virtual void finish();

    int compressPacket(cPacket *pkt);
    void handleNewMessage(cMessage *msg);
    void sendPacket(cMessage *pkt);

};

#endif
