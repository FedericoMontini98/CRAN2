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

#include "Cell.h"

Define_Module(Cell);

void Cell::initialize()
{
    endToEndDelay_ = registerSignal("endToEndDelay");
}

void Cell::handleMessage(cMessage *msg)
{
    simtime_t elapsed_time = simTime() - m->getTimestamp();
    emit(endToEndDelay_, elapsed_time);
    //EV << elapsed_time;
    delete m;
    delete msg;
}
