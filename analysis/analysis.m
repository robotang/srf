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

clc; close all;

SERIAL_CFG = 0;
CALIBRATE = 'c';
READ = 'r';
FNAME = 'c:\temp\srf.txt';

threshold = srf_decay_threshold(30, 100, 4, 450, 0);

% use realterm to communicate to serial port
if SERIAL_CFG == 1
    hrealterm = actxserver('realterm.realtermintf');
    hrealterm.baud = 115200;
    hrealterm.Port = '3';
    hrealterm.PortOpen = 3;
    hrealterm.CaptureFile = FNAME;
    invoke(hrealterm,'startcapture');
    fid = fopen(FNAME,'r');
end

figure;

% do an initial calibration
calibration = srf_get(hrealterm, fid, CALIBRATE);

while true
    % read a keyboard press from the figure pane
    c = get(gcf, 'CurrentCharacter');
    set(gcf, 'CurrentCharacter', ' ');
    
    % breakout if escape key
    if c == 27
        break;
    elseif c == CALIBRATE 
        calibration = srf_get(hrealterm, fid, CALIBRATE);
        fprintf('calibration performed\n');
    elseif c == READ        
        data = srf_get(hrealterm, fid, READ);
        fprintf('reading performed\n');
        
        % plot difference
        subplot(1, 2, 1);
        plot(data - calibration);
        hold on; plot(threshold, 'r'); hold off;
        xlabel('Time (100us)');
        ylabel('ADC reading');
        title('SRF difference waveform');
        set(legend('Data', 'Threshold'), 'Location', 'Best');
        
        % plot raw data and calibration
        subplot(1, 2, 2); 
        plot(data, 'b'); 
        hold on; plot(calibration, 'r'); hold off;
        xlabel('Time (100us)');
        ylabel('ADC reading');
        title('SRF raw data');
        set(legend('Raw', 'Calibration'), 'Location', 'Best');
        
        % calculate distance
        [pulse, num_pulses] = srf_calculate(data, calibration, threshold);
        
        if num_pulses
            pulse(1).distance
        end
    end    
    
    % block until user hits a key on the figure pane
    fprintf('waiting for a button press\n');
    waitforbuttonpress;
end

fprintf('quitting\n');

if SERIAL_CFG == 1
    fclose(fid);
    invoke(hrealterm,'stopcapture');
    invoke(hrealterm,'close'); 
    delete(hrealterm);
end