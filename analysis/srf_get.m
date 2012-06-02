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

function data = srf_get(hrealterm, fid, c)

max_attempts = 5;

for i = 1:max_attempts
    invoke(hrealterm, 'putstring', c);
    pause(2.5);
    str = fscanf(fid, '%s');
    
    if ~isempty(str)
       break; 
    end
end

data = textscan(str, '%d', 450, 'delimiter', ',');
data = data{1};
