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
    //Parameters initialization
    numTarget = par("N_target").intValue();

    sizeMean = par("sizeMean");

    //Checking the validity
    if(sizeMean < 0){
        error("Error in Size Mean Value Extraction: The value is negative");
    }

    sizeVariance = par("sizeVariance");
    if(sizeVariance < 0){
        error("Error in Size Variance Value Extraction: The value is negative");
    }

    timeMean = par("timeMean");
    if(timeMean < 0){
        error("Error in Time Mean Value Extraction: The value is negative");
    }

    sizeDistribution = par("sizeDistribution").intValue();
    timeDistribution = par("timeDistribution").intValue();

    // System initialization: Create a message that simulate the generation of a new packet -> represents a timer
    generate = new cMessage();
    generate_delay();
}

/*
 * function executed after the generation time
 * create and prepare a packet to send on the "out" link to the BBU
 * after the forwarding, call generate_delay to 'generate' another packet
 */
void AS::handleMessage(cMessage *msg)
{
    int size;

    // Generating a size for the packet with a proper distribution
    if(sizeDistribution == 1) {
        // exponential
        size = (int)exponential(sizeMean, SIZE_RNG);
    } else if(sizeDistribution == 0) {
        // log-normal
        size = (int)lognormal(sizeMean, sizeVariance, SIZE_RNG);
    } else {
        // constant
        size = sizeMean;
    }

    // create a new packet with specified bytes size and destination cell in the interval [0, N-1]
    PktMessage* pkt = new PktMessage();
    pkt->setByteLength(size);
    pkt->setTarget_cell(intuniform(0, numTarget-1, TARGET_RNG));

    send(pkt, "out");   // send the message to the BBU (only one link)
    generate_delay();   // generation of the waiting time for another new packet
}

/*
 * function that generates an amount of time to wait between the creation of two packets to send to the BBU
 */
void AS::generate_delay() {
    simtime_t time;

    if(timeDistribution == 1) { // exponential case: the only one actually evaluated
        // generating a "random" amount of time to wait
        time = (simtime_t)exponential(timeMean, TIME_RNG);
    } else { // constant generating time
        time = timeMean;
    }

    // Send to itself a msg to simulate that the packet generation has been completed
    scheduleAt(simTime() + time, generate);
    EV << "generated time: " << time << endl;
}

void AS::finish() {
    cancelAndDelete(generate);
}
