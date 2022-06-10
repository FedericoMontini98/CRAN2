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

package CRAN2.src;

simple RRH
{
    parameters:
                
        @signal[packetInRRH];
        @statistic[packetInRRHStat](source="packetInRRH"; record=vector);
        
        @signal[rrhPacketInQueue](type=long);
        @statistic[rrhPacketInQueueStat](source="rrhPacketInQueue"; record=max,vector);
        
        @signal[rrhQueueingTime];
        @statistic[rrhQueueingTimeStat](source="rrhQueueingTime"; record=vector);
        
        @signal[rrhResponseTime];
        @statistic[rrhResponseTimeStat](source="rrhResponseTime"; record=vector);
        
        bool compression_used;
        double compression_ratio;
        double alfa = default(1);	// delay compression factor
    gates:
        input in;	// from the BBU
        output out;	// towards the related cell
}
