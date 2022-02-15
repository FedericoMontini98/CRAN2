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

#ifndef __C_RAN2_AS_H_
#define __C_RAN2_AS_H_

#include <omnetpp.h>
#include "PktMessage_m.h"

using namespace omnetpp;

// Index of the Random Number Generators
#define TIME_RNG     0
#define SIZE_RNG     1
#define TARGET_RNG     2

class AS : public cSimpleModule
{
  //Mean values
  double sizeMean=0;
  double timeMean=0;

  //Message to notify to the AS to start another packet generation
  cMessage *generate;


  //Methods
  void generate_delay();

  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    virtual void finish();
};

#endif
