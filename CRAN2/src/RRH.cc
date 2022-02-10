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
        forwardPkt(msg);

        if(queue.size() > 0)
            decompressPkt();
    }
    else{
        PktMessage* new_pkt = check_and_cast<PktMessage*>(msg);
        queue.push(new_pkt);
        if(queue.size() == 1)
            decompressPkt();
    }
}

void RRH::forwardPkt(cMessage *msg)
{
    PktMessage* to_transmit = queue.front();
    queue.pop();
    send(to_transmit, out);
}

void RRH::decompressPkt()
{
    if(par("compression_used")){
        long long to_wait = 50 * par("compression_ratio");
        simtime_t decompression_time = SimTime (to_wait, -3);
        scheduleAt(simTime() + decompression_time, timer_);
    }
    else{
        scheduleAt(simTime(), timer_);
    }
}
