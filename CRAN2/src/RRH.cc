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
    // TODO - Generated method body
    timer_ = new cMessage("timer");
}

void RRH::handleMessage(cMessage *msg)
{
    // TODO - Generated method body
    if(msg->isSelfMessage()){
        //decompression ended
        forwardPkt();

        if(queue.size() > 0)
            decompressPkt(new_pkt);
    }
    else{
        PktMessage* new_pkt = check_and_cast<PktMessage*>(msg);
        queue.push(new_pkt);
        if(queue.size() == 1)
            decompressPkt(new_pkt);
    }
}

void RRH::forwardPkt()
{
    PktMessage* to_transmit = queue.front();
    queue.pop();
    send(to_transmit, "out");
}

void RRH::decompressPkt(PktMessage *pkt)
{
    if(par("compression_used")){
        long long to_wait = ((long long)50) * ((long long)par("compression_ratio"));
        pkt->setByteLength(pkt->getByteLength() / ((100 - par("compression_ratio").intValue()) / 100));
        simtime_t decompression_time = SimTime(to_wait, (SimTimeUnit)-3);
        scheduleAt(simTime() + decompression_time, timer_);
    }
    else{
        scheduleAt(simTime(), timer_);
    }
}
