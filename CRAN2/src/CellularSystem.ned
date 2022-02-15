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

import ned.DatarateChannel;


module CellularSystem
{
    parameters:
        int N = default(5);
        bool compression_on = default(false);
        int compression_rate = default(0);
        double dr @unit(kbps) = default(5kbps);

        @display("bgb=887,280");
    gates:
        input in;	// from AS
    submodules:
        bbu: BBU {
            compression_used = compression_on;
            compression_ratio = compression_rate;
            @display("p=149.24251,138.105");
        }
        rrh[N]: RRH {
            @display("p=431,139");
            compression_used = compression_on;
            compression_ratio = compression_rate;
        }
        cell[N]: Cell {
            @display("p=725,139");
        }
    connections:
        in --> bbu.in;
        for i=0..N-1 {
            bbu.out++ --> {  datarate = dr; } --> rrh[i].in;
            cell[i].in <-- rrh[i].out;
        }
}
