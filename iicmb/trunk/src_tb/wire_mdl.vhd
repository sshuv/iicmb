
--==============================================================================
--                                                                             |
--    Project: IIC Multiple Bus Controller (IICMB)                             |
--                                                                             |
--    Module:  Wire model.                                                     |
--    Version:                                                                 |
--             1.0,   April 29, 2016                                           |
--                                                                             |
--    Author:  Sergey Shuvalkin, (sshuv2@opencores.org)                        |
--                                                                             |
--==============================================================================
--==============================================================================
-- Copyright (c) 2016, Sergey Shuvalkin                                        |
-- All rights reserved.                                                        |
--                                                                             |
-- Redistribution and use in source and binary forms, with or without          |
-- modification, are permitted provided that the following conditions are met: |
--                                                                             |
-- 1. Redistributions of source code must retain the above copyright notice,   |
--    this list of conditions and the following disclaimer.                    |
-- 2. Redistributions in binary form must reproduce the above copyright        |
--    notice, this list of conditions and the following disclaimer in the      |
--    documentation and/or other materials provided with the distribution.     |
--                                                                             |
-- THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" |
-- AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE   |
-- IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE  |
-- ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE    |
-- LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR         |
-- CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF        |
-- SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS    |
-- INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN     |
-- CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)     |
-- ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE  |
-- POSSIBILITY OF SUCH DAMAGE.                                                 |
--==============================================================================


library ieee;
use ieee.math_real.uniform;
use ieee.math_real.exp;


--==============================================================================
entity wire_mdl is
  generic
  (
    g_resistance_0       :       real       := 1.0; -- In Ohms
    g_resistance_1       :       real       := 1.0; -- In Ohms
    g_capacitance        :       real       := 1.0; -- In pF
    g_initial_level      :       bit        := '0'
  );
  port
  (
    sig_in               : in    bit;
    sig_out              :   out real;
    sig_out_l            :   out bit
  );
end entity wire_mdl;
--==============================================================================

--==============================================================================
architecture beh of wire_mdl is

  -- We use picoseconds and picofarads in calculations
  constant c_time_delta      : time := 2011 ps;
  constant c_time_delta_real : real := real(c_time_delta / 1 ps);

  function to_real(a : bit) return real is
  begin
    if (a = '0') then
      return 0.0;
    else
      return 1.0;
    end if;
  end function to_real;

  signal   u_c          : real := to_real(g_initial_level); -- current voltage

begin

  ------------------------------------------------------------------------------
  sig_out_proc:
  process
    variable v_target     : real;
    variable v_resistance : real;
    variable seed_0       : positive  := 1137938;
    variable seed_1       : positive  := 1010110111;
    variable random_value : real;
  begin
    -- Get target voltage
    v_target := to_real(sig_in);

    -- Get resistance, depending on target voltage
    if (sig_in = '1') then
      v_resistance := g_resistance_1;
    else
      v_resistance := g_resistance_0;
    end if;

    -- Calculate next voltage level:
    u_c <= v_target - (v_target - u_c)*exp((-c_time_delta_real)/(g_capacitance * v_resistance));

    ieee.math_real.uniform(seed_0, seed_1, random_value);
    if (u_c >= random_value) then
      sig_out_l <= '1';
    else
      sig_out_l <= '0';
    end if;
    wait for c_time_delta;
  end process sig_out_proc;
  ------------------------------------------------------------------------------

  sig_out <= u_c;

end architecture beh;
--==============================================================================

