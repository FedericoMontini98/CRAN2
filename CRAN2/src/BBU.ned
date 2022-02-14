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

package cran2;

simple BBU
{
    parameters:
        //signals for statistics and other parameters
        
        @signal[occupationQueue](type=long);
        @statistic[occupationQueueStat](source="occupationQueue"; record=vector);
        
        @signal[queueingTime];
        @statistic[queueingTimeStat](source="queueingTime"; record=mean,vector);
        
        @signal[responseTime];
        @statistic[responseTimeStat](source="responseTime"; record=max,vector);
        
        bool pkt_compression;
        int compression_rate;
    gates:
        input in;	// input from the AS
        output out[];	// outputs towards the RRHs
}
