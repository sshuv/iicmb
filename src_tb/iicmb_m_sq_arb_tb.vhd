
--==============================================================================
--                                                                             |
--    Project: IIC Multiple Bus Controller (IICMB)                             |
--                                                                             |
--    Module:  Testbench for iicmb_m_sq. Testing arbitration and clock         |
--             synchronization.                                                |
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
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

library iicmb;
use iicmb.iicmb_pkg.all;

use work.test.all;


--==============================================================================
entity iicmb_m_sq_arb_tb is
end entity iicmb_m_sq_arb_tb;
--==============================================================================

--==============================================================================
architecture beh of iicmb_m_sq_arb_tb is

  constant c_f_clk   : real      := 100000.0; -- in kHz
  constant c_f_scl_0 : real      :=    100.0; -- in kHz
  constant c_f_scl_1 : real      :=     66.0; -- in kHz
  constant c_p_clk   : time      := integer(1000000000.0/c_f_clk) * 1 ps; -- Period of 'clk' in ps.

  ------------------------------------------------------------------------------
  component iicmb_m_sq is
    generic
    (
      g_bus_num     :       positive range 1 to 16 := 1;
      g_f_clk       :       real                   := 100000.0;
      g_f_scl_0     :       real                   :=    100.0;
      g_f_scl_1     :       real                   :=    100.0;
      g_f_scl_2     :       real                   :=    100.0;
      g_f_scl_3     :       real                   :=    100.0;
      g_f_scl_4     :       real                   :=    100.0;
      g_f_scl_5     :       real                   :=    100.0;
      g_f_scl_6     :       real                   :=    100.0;
      g_f_scl_7     :       real                   :=    100.0;
      g_f_scl_8     :       real                   :=    100.0;
      g_f_scl_9     :       real                   :=    100.0;
      g_f_scl_a     :       real                   :=    100.0;
      g_f_scl_b     :       real                   :=    100.0;
      g_f_scl_c     :       real                   :=    100.0;
      g_f_scl_d     :       real                   :=    100.0;
      g_f_scl_e     :       real                   :=    100.0;
      g_f_scl_f     :       real                   :=    100.0;
      g_cmd         :       seq_cmd_type_array     := c_empty_array
    );
    port
    (
      clk           : in    std_logic;
      s_rst         : in    std_logic;
      cs_start      : in    std_logic;
      cs_busy       :   out std_logic;
      cs_status     :   out std_logic_vector(2 downto 0);
      scl_i         : in    std_logic_vector(0 to g_bus_num - 1);
      sda_i         : in    std_logic_vector(0 to g_bus_num - 1);
      scl_o         :   out std_logic_vector(0 to g_bus_num - 1);
      sda_o         :   out std_logic_vector(0 to g_bus_num - 1)
    );
  end component iicmb_m_sq;
  ------------------------------------------------------------------------------

  ------------------------------------------------------------------------------
  component wire_mdl is
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
  end component wire_mdl;
  ------------------------------------------------------------------------------

  ------------------------------------------------------------------------------
  component i2c_slave_model is
    generic
    (
      i2c_adr : std_logic_vector(6 downto 0)
    );
    port
    (
      scl     : inout std_logic;
      sda     : inout std_logic
    );
  end component i2c_slave_model;
  ------------------------------------------------------------------------------

  signal   clk         : std_logic := '0';
  signal   s_rst       : std_logic := '1';

  signal   cs_start_0  : std_logic := '0';
  signal   cs_busy_0   : std_logic;
  signal   cs_status_0 : std_logic_vector(2 downto 0);

  signal   cs_start_1  : std_logic := '0';
  signal   cs_busy_1   : std_logic;
  signal   cs_status_1 : std_logic_vector(2 downto 0);

  signal   scl_o_0     : std_logic_vector(0 to 0) := (others => '1');
  signal   sda_o_0     : std_logic_vector(0 to 0) := (others => '1');
  signal   scl_o_1     : std_logic_vector(0 to 0) := (others => '1');
  signal   sda_o_1     : std_logic_vector(0 to 0) := (others => '1');
  signal   scl         : std_logic_vector(0 to 0) := (others => 'H');
  signal   sda         : std_logic_vector(0 to 0) := (others => 'H');

  type real_vector is array (natural range <>) of real;
  signal   scl_real    : real_vector(0 to 0);
  signal   sda_real    : real_vector(0 to 0);
  signal   scl_quant   : bit_vector(0 to 0);
  signal   sda_quant   : bit_vector(0 to 0);
  signal   scl_nquant  : bit_vector(0 to 0) := (others => '1');
  signal   sda_nquant  : bit_vector(0 to 0) := (others => '1');

begin

  clk   <= not(clk) after c_p_clk / 2;
  s_rst <= '1', '0' after 113 ns;

  ------------------------------------------------------------------------------
  -- Generate signal to launch the sequencer #0
  process
  begin
    cs_start_0 <= '0';
    wait for 2000 ns;
    wait until rising_edge(clk);
    cs_start_0 <= '1';
    wait until rising_edge(clk);
    cs_start_0 <= '0';
    wait;
  end process;
  ------------------------------------------------------------------------------

  ------------------------------------------------------------------------------
  -- Generate signal to launch the sequencer #1
  process
  begin
    cs_start_1 <= '0';
    wait for 2000 ns;
    wait until rising_edge(clk);
    cs_start_1 <= '1';
    wait until rising_edge(clk);
    cs_start_1 <= '0';
    wait;
  end process;
  ------------------------------------------------------------------------------

  ------------------------------------------------------------------------------
  dut_inst0 : iicmb_m_sq
    generic map
    (
      g_bus_num   => 1,
      g_f_clk     => c_f_clk,
      g_f_scl_0   => c_f_scl_0,
      g_cmd       =>
        (
          scmd_wait(1),                             -- Wait for 1 ms
          scmd_set_bus(0),                          -- Select bus #0
          scmd_write_byte("0100001", x"03", x"4A"), -- Write byte
          scmd_write_byte("0100001", x"05", x"27")  -- Write byte
        )
    )
    port map
    (
      clk         => clk,
      s_rst       => s_rst,
      cs_start    => cs_start_0,
      cs_busy     => cs_busy_0,
      cs_status   => cs_status_0,
      scl_i       => to_stdlogicvector(scl_quant),
      sda_i       => to_stdlogicvector(sda_quant),
      scl_o       => scl_o_0,
      sda_o       => sda_o_0
    );
  ------------------------------------------------------------------------------

  ------------------------------------------------------------------------------
  dut_inst1 : iicmb_m_sq
    generic map
    (
      g_bus_num   => 1,
      g_f_clk     => c_f_clk,
      g_f_scl_0   => c_f_scl_1,
      g_cmd       =>
        (
          scmd_wait(1),                             -- Wait for 1 ms
          scmd_set_bus(0),                          -- Select bus #0
          scmd_write_byte("0100001", x"03", x"4A"), -- Write byte
          scmd_write_byte("0100001", x"05", x"67")  -- Write byte
        )
    )
    port map
    (
      clk         => clk,
      s_rst       => s_rst,
      cs_start    => cs_start_1,
      cs_busy     => cs_busy_1,
      cs_status   => cs_status_1,
      scl_i       => to_stdlogicvector(scl_quant),
      sda_i       => to_stdlogicvector(sda_quant),
      scl_o       => scl_o_1,
      sda_o       => sda_o_1
    );
  ------------------------------------------------------------------------------

  scl(0) <= '0' when (scl_o_0(0) = '0') else 'Z';
  sda(0) <= '0' when (sda_o_0(0) = '0') else 'Z';
  scl(0) <= '0' when (scl_o_1(0) = '0') else 'Z';
  sda(0) <= '0' when (sda_o_1(0) = '0') else 'Z';
  scl(0) <= 'H'; -- Pull-up
  sda(0) <= 'H'; -- Pull-up

  ----------------------------------------------------------------------------
  wire_mdl_inst_0 : wire_mdl
    generic map
    (
      g_resistance_0       => 40.0,
      g_resistance_1       => 4000.0,
      g_capacitance        => 200.0, -- In pF
      g_initial_level      => '1'
    )
    port map
    (
      sig_in               => scl_nquant(0),
      sig_out              => scl_real(0),
      sig_out_l            => scl_quant(0)
    );
  ----------------------------------------------------------------------------

  ----------------------------------------------------------------------------
  wire_mdl_inst_1 : wire_mdl
    generic map
    (
      g_resistance_0       => 40.0,
      g_resistance_1       => 4000.0,
      g_capacitance        => 200.0, -- In pF
      g_initial_level      => '1'
    )
    port map
    (
      sig_in               => sda_nquant(0),
      sig_out              => sda_real(0),
      sig_out_l            => sda_quant(0)
    );
  ----------------------------------------------------------------------------

  ----------------------------------------------------------------------------
  i2c_slave_model_inst0 : i2c_slave_model
    generic map
    (
      i2c_adr => "0100001"
    )
    port map
    (
      scl     => scl(0),
      sda     => sda(0)
    );
  ----------------------------------------------------------------------------

  scl_nquant <= to_bitvector(to_x01(scl));
  sda_nquant <= to_bitvector(to_x01(sda));

end architecture beh;
--==============================================================================

