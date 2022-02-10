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

#include "AS.h"

Define_Module(AS);

void AS::initialize()
{
    this->SizeMean=par("SizeMean");
    if(s<0){
        error("Error in  Size Mean Value Extraction: The value is negative");
    }
    pkt_generation_delay();
}

void AS::handleMessage(cMessage *msg)
{
    //After the wait for the Generate to arrive (Is a signal that notice to the AS to generate a packet to send to the BBU)
    //I proceed to create and send it
    int size;
    //Generating a random amount of time to wait
    if(par("Size_Distribution")==1){
        size=exponential(this->SizeMean,/* Find a way to generate a seed */0);
    }
    else{
        size=lognormal(this->SizeMean,/* Same*/0);
    }
    //I create a new packet with the specified ID, size and the cell to reach in the interval [0, N-1]
    this->pkt = new PktMessage(this->curr_pkt_id++,size,intuniform(0,par("N")-1,0));
    //I send the generated pkt
    send(this->pkt);
    //I proceed to wait another pkt generation cycle
    generate_delay();
}

void AS::generate_delay(){
    //I Create a msg
    this->Generate= new cMessage();
    int time;
    //Generating a random amount of time to wait
    if(par("Size_Distribution")==1){
        time=exponential(this->TimeMean,/* Find a way to generate a seed */0);
    }
    else{
        time=lognormal(this->TimeMean,/* Same*/0);
    }
    //I send to myself a msg to notify that i have to send a packet to the BBU
    scheduleAt(simTime()+ time, this->Generate);
}

//Function to stop the packet generation
void AS::finish(){
    cancelEvent(this->Generate);
    cancelAndDelete(this->Generate);
}
