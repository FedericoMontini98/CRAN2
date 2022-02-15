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

package cran2.simulations;

import cran2.AS;
import cran2.CellularSystem;


network CRAN2
{
    parameters:
        int N_cells;
    @display("bgb=393,343");
    submodules:
        as: AS {
            @display("p=194,65");
            N_target = N_cells;
        }
        cellularSystem: CellularSystem {
            @display("p=194,234");
            N = N_cells;
        }
    connections:
        as.out --> cellularSystem.in;
}
