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

function [pulse, num_pulses] = srf_calculate(raw, calibration, threshold)

MIN_PULSE_WIDTH = 2;

data = (raw - calibration) - threshold;

index = 1;
pulse(1).distance = 0;
pulse(1).peak_index = 0; 
pulse(1).start_index = 0;
pulse(1).end_index = 0;
num_pulses = 0;
max_index = size(data, 1);
for i = 1:10 % looks for up to 10 pulse segments
    [peak_index, start_index, end_index] = find_pulse(data, index, max_index);
        
    if end_index > start_index + MIN_PULSE_WIDTH
        pulse(i).distance = index_to_distance(peak_index);
        pulse(i).peak_index = peak_index;
        pulse(i).start_index = start_index;
        pulse(i).end_index = end_index;
        num_pulses = num_pulses + 1;
    end
    
    index = end_index + 5; % slight hysteresis
    
    if end_index >= max_index
        break;
    end
end

% subfunction
function [peak_index, start_index, end_index] = find_pulse(data, index, max_index)

start_index = index;
while data(start_index) < 0
    if start_index >= max_index
        break;
    end
    
    start_index = start_index + 1; 
end

peak = 0;
peak_index = 1;
end_index = start_index;
while data(end_index) > 0
    if end_index >= max_index
        break;
    end
    
    if data(end_index) > peak
        peak_index = end_index;
        peak = data(peak_index);
    end
    
    end_index = end_index + 1;
end

% subfunction
function d = index_to_distance(index)

SOUND_MS = 340;
PING_OFFSET = 60;

t = (PING_OFFSET + 100*index) / (2*10000);
d = t * SOUND_MS;
