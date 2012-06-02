%  Copyright (C) 2009, Robert Tang <opensource@robotang.co.nz>
%
%  This is free software; you can redistribute it and/or
%  modify it under the terms of the GNU Lesser General Public
%  License as published by the Free Software Foundation; either
%  version 2.1 of the License, or (at your option) any later version.
%
%  This program is distributed in the hope that it will be useful,
%  but WITHOUT ANY WARRANTY; without even the implied warranty of
%  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
%  Lesser General Public License for more details.
%
%  You should have received a copy of the GNU Lesser General Public Licence
%  along with this library; if not, see <http://www.gnu.org/licenses/>.

function thresh = srf_decay_threshold(n0, lambda, b, n, write)

t = 1:n;
thresh = int32((n0-b)*exp(-1*t/lambda) + b);
thresh = thresh';

if write
    fid = fopen('../src/sonar_threshold.h', 'w');
    fprintf(fid, '#ifndef SONAR_THRESHOLD_H\n');
    fprintf(fid, '#define SONAR_THRESHOLD_H\n');
    fprintf(fid, '\n');
    fprintf(fid, '#include <stdint.h>\n');
    fprintf(fid, '\n');
    fprintf(fid, 'static uint8_t __attribute__ ((progmem)) sonar_threshold[] = {');    
    fprintf(fid, '%d', thresh(1));
    for i = 2:n
        fprintf(fid, ', %d', thresh(i));
    end
    fprintf(fid, '};\n');
    fprintf(fid, '\n');
    fprintf(fid, '#endif\n');
    fprintf(fid, '\n');
    fclose(fid);
end