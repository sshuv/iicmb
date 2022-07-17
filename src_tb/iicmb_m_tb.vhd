
--==============================================================================
--                                                                             |
--    Project: IIC Multiple Bus Controller (IICMB)                             |
--                                                                             |
--    Module:  Testbench for 'iicmb_m'.                                        |
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

library work;
use work.iicmb_pkg.all;
use work.test.all;


--==============================================================================
entity iicmb_m_tb is
end entity iicmb_m_tb;
--==============================================================================

--==============================================================================
architecture beh of iicmb_m_tb is

  constant c_f_clk   : real      := 100000.0; -- in kHz
  constant c_f_scl_0 : real      :=    100.0; -- in kHz
  constant c_f_scl_1 : real      :=    100.0; -- in kHz
  constant c_f_scl_2 : real      :=    100.0; -- in kHz
  constant c_f_scl_3 : real      :=    100.0; -- in kHz
  constant c_p_clk   : time      := integer(1000000000.0/c_f_clk) * 1 ps;

  constant c_bus_num : positive  := 1;

  ------------------------------------------------------------------------------
  component iicmb_m is
    generic
    (
      g_bus_num :       positive range 1 to 16 := 1;
      g_f_clk   :       real                   := 100000.0;
      g_f_scl_0 :       real                   :=    100.0;
      g_f_scl_1 :       real                   :=    100.0;
      g_f_scl_2 :       real                   :=    100.0;
      g_f_scl_3 :       real                   :=    100.0;
      g_f_scl_4 :       real                   :=    100.0;
      g_f_scl_5 :       real                   :=    100.0;
      g_f_scl_6 :       real                   :=    100.0;
      g_f_scl_7 :       real                   :=    100.0;
      g_f_scl_8 :       real                   :=    100.0;
      g_f_scl_9 :       real                   :=    100.0;
      g_f_scl_a :       real                   :=    100.0;
      g_f_scl_b :       real                   :=    100.0;
      g_f_scl_c :       real                   :=    100.0;
      g_f_scl_d :       real                   :=    100.0;
      g_f_scl_e :       real                   :=    100.0;
      g_f_scl_f :       real                   :=    100.0
    );
    port
    (
      clk         : in    std_logic;
      s_rst       : in    std_logic;
      busy        :   out std_logic;
      captured    :   out std_logic;
      bus_id      :   out std_logic_vector(3 downto 0);
      bit_state   :   out std_logic_vector(3 downto 0);
      byte_state  :   out std_logic_vector(3 downto 0);
      mcmd_wr     : in    std_logic;
      mcmd_id     : in    std_logic_vector(2 downto 0);
      mcmd_data   : in    std_logic_vector(7 downto 0);
      mrsp_wr     :   out std_logic;
      mrsp_id     :   out std_logic_vector(2 downto 0);
      mrsp_data   :   out std_logic_vector(7 downto 0);
      scl_i       : in    std_logic_vector(0 to g_bus_num - 1);
      sda_i       : in    std_logic_vector(0 to g_bus_num - 1);
      scl_o       :   out std_logic_vector(0 to g_bus_num - 1);
      sda_o       :   out std_logic_vector(0 to g_bus_num - 1)
    );
  end component iicmb_m;
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

  signal   clk         : std_logic := '0';
  signal   s_rst       : std_logic := '1';

  signal   busy        : std_logic;
  signal   captured    : std_logic;
  signal   bus_id      : std_logic_vector(3 downto 0);
  signal   bit_state   : std_logic_vector(3 downto 0);
  signal   byte_state  : std_logic_vector(3 downto 0);
  signal   mcmd_wr     : std_logic;
  signal   mcmd_id     : std_logic_vector(2 downto 0);
  signal   mcmd_data   : std_logic_vector(7 downto 0);
  signal   mrsp_wr     : std_logic;
  signal   mrsp_id     : std_logic_vector(2 downto 0);
  signal   mrsp_data   : std_logic_vector(7 downto 0);
  signal   scl_o       : std_logic_vector(0 to c_bus_num - 1);
  signal   scl         : std_logic_vector(0 to c_bus_num - 1);
  signal   sda_o       : std_logic_vector(0 to c_bus_num - 1);
  signal   sda         : std_logic_vector(0 to c_bus_num - 1);

  type real_vector is array (natural range <>) of real;
  signal   scl_real    : real_vector(0 to c_bus_num - 1);
  signal   sda_real    : real_vector(0 to c_bus_num - 1);
  signal   scl_quant   : bit_vector(0 to c_bus_num - 1);
  signal   sda_quant   : bit_vector(0 to c_bus_num - 1);
  signal   scl_nquant  : bit_vector(0 to c_bus_num - 1);
  signal   sda_nquant  : bit_vector(0 to c_bus_num - 1);

begin

  clk <= not(clk) after c_p_clk / 2;
  s_rst <= '1', '0' after 113 ns;

  ------------------------------------------------------------------------------
  process
    procedure issue_command(a : std_logic_vector(2 downto 0)) is
    begin
      mcmd_wr   <= '1';
      mcmd_id   <= a;
      wait until rising_edge(clk);
      mcmd_wr   <= '0';
      wait until rising_edge(clk)and(mrsp_wr = '1');
    end procedure issue_command;
    procedure send_byte(a : std_logic_vector(7 downto 0)) is
    begin
      mcmd_wr   <= '1';
      mcmd_id   <= mcmd_write;
      mcmd_data <= a;
      wait until rising_edge(clk);
      mcmd_wr   <= '0';
      wait until rising_edge(clk)and(mrsp_wr = '1');
    end procedure send_byte;
  begin
    -- Initial delay:
    mcmd_wr   <= '0';
    mcmd_id   <= mcmd_set_bus;
    mcmd_data <= (others => '0');
    wait for 20000 ns;

    --
    issue_command(mcmd_start);
    send_byte(x"5A");
    issue_command(mcmd_start);
    send_byte(x"AA");
    issue_command(mcmd_read_ack);
    issue_command(mcmd_read_nak);
    issue_command(mcmd_stop);

    -- Halt:
    mcmd_wr   <= '0';
    mcmd_id   <= mcmd_set_bus;
    mcmd_data <= (others => '0');
    wait;
  end process;
  ------------------------------------------------------------------------------

  ------------------------------------------------------------------------------
  dut : iicmb_m
    generic map
    (
      g_bus_num   => c_bus_num,
      g_f_clk     => c_f_clk,
      g_f_scl_0   => c_f_scl_0,
      g_f_scl_1   => c_f_scl_1,
      g_f_scl_2   => c_f_scl_2,
      g_f_scl_3   => c_f_scl_3
    )
    port map
    (
      clk         => clk,
      s_rst       => s_rst,
      busy        => busy,
      captured    => captured,
      bus_id      => bus_id,
      bit_state   => bit_state,
      byte_state  => byte_state,
      mcmd_wr     => mcmd_wr,
      mcmd_id     => mcmd_id,
      mcmd_data   => mcmd_data,
      mrsp_wr     => mrsp_wr,
      mrsp_id     => mrsp_id,
      mrsp_data   => mrsp_data,
      scl_i       => to_stdlogicvector(scl_quant),
      sda_i       => to_stdlogicvector(sda_quant),
      scl_o       => scl_o,
      sda_o       => sda_o
    );
  ------------------------------------------------------------------------------

  --****************************************************************************
  bus_gen:
  for i in 0 to c_bus_num - 1 generate
    scl(i) <= '0' when (scl_o(i) = '0') else 'Z';
    sda(i) <= '0' when (sda_o(i) = '0') else 'Z';

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
        sig_in               => scl_nquant(i),
        sig_out              => scl_real(i),
        sig_out_l            => scl_quant(i)
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
        sig_in               => sda_nquant(i),
        sig_out              => sda_real(i),
        sig_out_l            => sda_quant(i)
      );
    ----------------------------------------------------------------------------
  end generate bus_gen;
  --****************************************************************************

  scl <= (others => 'H'); -- Pull up
  sda <= (others => 'H'); -- Pull up

  scl_nquant <= to_bitvector(to_x01(scl));
  sda_nquant <= to_bitvector(to_x01(sda));

  ------------------------------------------------------------------------------
  process(clk)
  begin
    if rising_edge(clk) then
      if (s_rst = '1') then
      else
        if (mrsp_wr = '1') then
          case (mrsp_id) is
            when mrsp_done     => print_string("Response: Done" & newline);
            when mrsp_nak      => print_string("Response: Nak" & newline);
            when mrsp_arb_lost => print_string("Response: Arbitration lost" & newline);
            when mrsp_error    => print_string("Response: Error" & newline);
            when mrsp_byte     => print_string("Response: Byte received: " & "0x" & to_string(mrsp_data, "X") & newline);
            when others        => print_string("Undefined response!!!" & newline);
          end case;
        end if;
      end if;
    end if;
  end process;

end architecture beh;
--==============================================================================

