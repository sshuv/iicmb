
--==============================================================================
--                                                                             |
--    Project: IIC Multiple Bus Controller (IICMB)                             |
--                                                                             |
--    Module:  Testbench for 'iicmb_m_wb'.                                     |
--    Version:                                                                 |
--             1.0,   April 29, 2016                                           |
--             1.1,   May   10, 2016 Changed i2c_slave_model instance          |
--                                   parameter interface                       |
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

library work;
use work.iicmb_pkg.all;
use work.test.all;


--==============================================================================
entity iicmb_m_wb_tb is
end entity iicmb_m_wb_tb;
--==============================================================================

--==============================================================================
architecture beh of iicmb_m_wb_tb is

  constant c_f_clk   : real      := 100000.0; -- in kHz
  constant c_f_scl_0 : real      :=    100.0; -- in kHz
  constant c_f_scl_1 : real      :=    100.0; -- in kHz
  constant c_f_scl_2 : real      :=    100.0; -- in kHz
  constant c_f_scl_3 : real      :=    100.0; -- in kHz
  constant c_p_clk   : time      := integer(1000000000.0/c_f_clk) * 1 ps;

  constant c_bus_num : positive  := 4;

  ------------------------------------------------------------------------------
  component iicmb_m_wb is
    generic
    (
      g_bus_num   :       positive range 1 to 16 := 1;
      g_f_clk     :       real                   := 100000.0;
      g_f_scl_0   :       real                   :=    100.0;
      g_f_scl_1   :       real                   :=    100.0;
      g_f_scl_2   :       real                   :=    100.0;
      g_f_scl_3   :       real                   :=    100.0;
      g_f_scl_4   :       real                   :=    100.0;
      g_f_scl_5   :       real                   :=    100.0;
      g_f_scl_6   :       real                   :=    100.0;
      g_f_scl_7   :       real                   :=    100.0;
      g_f_scl_8   :       real                   :=    100.0;
      g_f_scl_9   :       real                   :=    100.0;
      g_f_scl_a   :       real                   :=    100.0;
      g_f_scl_b   :       real                   :=    100.0;
      g_f_scl_c   :       real                   :=    100.0;
      g_f_scl_d   :       real                   :=    100.0;
      g_f_scl_e   :       real                   :=    100.0;
      g_f_scl_f   :       real                   :=    100.0
    );
    port
    (
      clk_i       : in    std_logic;
      rst_i       : in    std_logic;
      cyc_i       : in    std_logic;
      stb_i       : in    std_logic;
      ack_o       :   out std_logic;
      adr_i       : in    std_logic_vector(1 downto 0);
      we_i        : in    std_logic;
      dat_i       : in    std_logic_vector(7 downto 0);
      dat_o       :   out std_logic_vector(7 downto 0);
      irq         :   out std_logic;
      scl_i       : in    std_logic_vector(0 to g_bus_num - 1);
      sda_i       : in    std_logic_vector(0 to g_bus_num - 1);
      scl_o       :   out std_logic_vector(0 to g_bus_num - 1);
      sda_o       :   out std_logic_vector(0 to g_bus_num - 1)
    );
  end component iicmb_m_wb;
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
      I2C_ADR : integer
    );
    port
    (
      scl     : inout std_logic;
      sda     : inout std_logic
    );
  end component i2c_slave_model;
  ------------------------------------------------------------------------------

  ------------------------------------------------------------------------------
  function get_slave_addr(n : natural) return std_logic_vector is
    variable ret : std_logic_vector(6 downto 0);
  begin
    ret := "010" & std_logic_vector(to_unsigned(n, 4));
    return ret;
  end function get_slave_addr;
  ------------------------------------------------------------------------------

  signal   clk_i         : std_logic := '0';
  signal   rst_i         : std_logic := '1';
  signal   cyc_i         : std_logic := '0';
  signal   stb_i         : std_logic := '0';
  signal   ack_o         : std_logic;
  signal   adr_i         : std_logic_vector(1 downto 0) := "00";
  signal   we_i          : std_logic := '0';
  signal   dat_i         : std_logic_vector(7 downto 0) := "00000000";
  signal   dat_o         : std_logic_vector(7 downto 0);
                         
  signal   scl_o         : std_logic_vector(0 to c_bus_num - 1) := (others => '1');
  signal   scl           : std_logic_vector(0 to c_bus_num - 1) := (others => 'H');
  signal   sda_o         : std_logic_vector(0 to c_bus_num - 1) := (others => '1');
  signal   sda           : std_logic_vector(0 to c_bus_num - 1) := (others => 'H');

  type real_vector is array (natural range <>) of real;
  signal   scl_real      : real_vector(0 to c_bus_num - 1);
  signal   sda_real      : real_vector(0 to c_bus_num - 1);
  signal   scl_quant     : bit_vector(0 to c_bus_num - 1);
  signal   sda_quant     : bit_vector(0 to c_bus_num - 1);
  signal   scl_nquant    : bit_vector(0 to c_bus_num - 1) := (others => '1');
  signal   sda_nquant    : bit_vector(0 to c_bus_num - 1) := (others => '1');
  signal   irq           : std_logic;

  ---- Byte-wide commands:
  constant wb_m_set_bus  : std_logic_vector(7 downto 0) := "00000" & mcmd_set_bus;
  constant wb_m_write    : std_logic_vector(7 downto 0) := "00000" & mcmd_write;
  constant wb_m_read_ack : std_logic_vector(7 downto 0) := "00000" & mcmd_read_ack;
  constant wb_m_read_nak : std_logic_vector(7 downto 0) := "00000" & mcmd_read_nak;
  constant wb_m_start    : std_logic_vector(7 downto 0) := "00000" & mcmd_start;
  constant wb_m_stop     : std_logic_vector(7 downto 0) := "00000" & mcmd_stop;
  constant wb_m_wait     : std_logic_vector(7 downto 0) := "00000" & mcmd_wait;

begin

  clk_i <= not(clk_i) after c_p_clk / 2;
  rst_i <= '1', '0' after 113 ns;

  ------------------------------------------------------------------------------
  -- Wishbone bus activity process:
  process
    ----------------------------------------------------------------------------
    procedure wb_write(addr : in std_logic_vector(1 downto 0); data : in std_logic_vector(7 downto 0)) is
    begin
      cyc_i   <= '1';
      stb_i   <= '1';
      adr_i   <= addr;
      we_i    <= '1';
      dat_i   <= data;
      wait until rising_edge(clk_i)and(ack_o = '1');
      cyc_i   <= '0';
      stb_i   <= '0';
      print_string("Wishbone Write: 0x" & to_string(addr, "X", 2) & " : " & "0x" & to_string(data, "X", 2) & newline);
    end procedure wb_write;
    ----------------------------------------------------------------------------
    ----------------------------------------------------------------------------
    procedure wb_read(addr : in std_logic_vector(1 downto 0); data : out std_logic_vector(7 downto 0)) is
    begin
      cyc_i   <= '1';
      stb_i   <= '1';
      adr_i   <= addr;
      we_i    <= '0';
      wait until rising_edge(clk_i)and(ack_o = '1');
      data    := dat_o;
      cyc_i   <= '0';
      stb_i   <= '0';
      print_string("Wishbone Read : 0x" & to_string(addr, "X", 2) & " : " & "0x" & to_string(dat_o, "X", 2) & newline);
    end procedure wb_read;
    ----------------------------------------------------------------------------
    ----------------------------------------------------------------------------
    procedure wb_wait(n : in positive) is
    begin
      print_string("Wishbone Waiting for " & integer'image(n) & " cycles." & newline);
      cyc_i   <= '0';
      stb_i   <= '0';
      for i in 0 to n - 1 loop
        wait until rising_edge(clk_i);
      end loop;
    end procedure wb_wait;
    ----------------------------------------------------------------------------
    ----------------------------------------------------------------------------
    procedure wb_halt is
    begin
      print_string("Wishbone Halted" & newline);
      cyc_i   <= '0';
      stb_i   <= '0';
      wait;
    end procedure wb_halt;
    ----------------------------------------------------------------------------
    procedure i2c_write_byte(slave_addr : in std_logic_vector(6 downto 0); addr : in std_logic_vector(7 downto 0); data : in std_logic_vector(7 downto 0)) is
      variable v_tmp : std_logic_vector(7 downto 0);
    begin
      wb_write("10", wb_m_start);
      wait until rising_edge(clk_i)and(irq = '1');
      wb_read("10", v_tmp);
      assert (v_tmp(7) = '1') report "Something gone wrong" severity error;
      --
      wb_write("01", slave_addr & "0");
      wb_write("10", wb_m_write);
      wait until rising_edge(clk_i)and(irq = '1');
      wb_read("10", v_tmp);
      assert (v_tmp(7) = '1') report "Something gone wrong" severity error;
      --
      wb_write("01", addr);
      wb_write("10", wb_m_write);
      wait until rising_edge(clk_i)and(irq = '1');
      wb_read("10", v_tmp);
      assert (v_tmp(7) = '1') report "Something gone wrong" severity error;
      --
      wb_write("01", data);
      wb_write("10", wb_m_write);
      wait until rising_edge(clk_i)and(irq = '1');
      wb_read("10", v_tmp);
      assert (v_tmp(7) = '1') report "Something gone wrong" severity error;
      --
      wb_write("10", wb_m_stop);
      wait until rising_edge(clk_i)and(irq = '1');
      wb_read("10", v_tmp);
      assert (v_tmp(7) = '1') report "Something gone wrong" severity error;
    end procedure i2c_write_byte;
    ----------------------------------------------------------------------------
    ----------------------------------------------------------------------------
    procedure i2c_read_byte(slave_addr : in std_logic_vector(6 downto 0); addr : in std_logic_vector(7 downto 0); data : out std_logic_vector(7 downto 0)) is
      variable v_tmp : std_logic_vector(7 downto 0);
    begin
      wb_write("10", wb_m_start);
      wait until rising_edge(clk_i)and(irq = '1');
      wb_read("10", v_tmp);
      assert (v_tmp(7) = '1') report "Something gone wrong" severity error;
      --
      wb_write("01", slave_addr & "0");
      wb_write("10", wb_m_write);
      wait until rising_edge(clk_i)and(irq = '1');
      wb_read("10", v_tmp);
      assert (v_tmp(7) = '1') report "Something gone wrong" severity error;
      --
      wb_write("01", addr);
      wb_write("10", wb_m_write);
      wait until rising_edge(clk_i)and(irq = '1');
      wb_read("10", v_tmp);
      assert (v_tmp(7) = '1') report "Something gone wrong" severity error;
      --
      wb_write("10", wb_m_start);
      wait until rising_edge(clk_i)and(irq = '1');
      wb_read("10", v_tmp);
      assert (v_tmp(7) = '1') report "Something gone wrong" severity error;
      --
      wb_write("01", slave_addr & "1");
      wb_write("10", wb_m_write);
      wait until rising_edge(clk_i)and(irq = '1');
      wb_read("10", v_tmp);
      assert (v_tmp(7) = '1') report "Something gone wrong" severity error;
      --
      wb_write("10", wb_m_read_nak);
      wait until rising_edge(clk_i)and(irq = '1');
      wb_read("10", v_tmp);
      assert (v_tmp(7) = '1') report "Something gone wrong" severity error;
      wb_read("01", data);
      --
      wb_write("10", wb_m_stop);
      wait until rising_edge(clk_i)and(irq = '1');
      wb_read("10", v_tmp);
      assert (v_tmp(7) = '1') report "Something gone wrong" severity error;
    end procedure i2c_read_byte;
    ----------------------------------------------------------------------------
    variable v_data : std_logic_vector(7 downto 0);
  begin
    -- Initial delay:
    wb_wait(100);

    --
    wb_read("00", v_data);
    wb_read("01", v_data);                                           
    wb_read("10", v_data);                                           
    wb_wait(1);
    wb_read("11", v_data);                                           
    --
    wb_wait(10);
    -- Enable controller and interrupts
    wb_write("00", "11000000");
    wb_read("00", v_data);
    --
    -- Select Bus #1
    wb_wait(10);
    wb_write("01", "00000001");
    wb_write("10", wb_m_set_bus);
    wb_wait(1);
    wb_read("10", v_data);
    --

    --
    wb_wait(10);
    i2c_write_byte(get_slave_addr(1), x"00", x"4A");
    i2c_write_byte(get_slave_addr(1), x"01", x"67");
    --

    --
    wb_wait(10);
    i2c_read_byte(get_slave_addr(1), x"00", v_data);
    print_string("Data read: " & to_string(v_data, "X", 2) & newline);
    i2c_read_byte(get_slave_addr(1), x"01", v_data);
    print_string("Data read: " & to_string(v_data, "X", 2) & newline);
    --

    -- Halt bus activity
    wb_halt;
  end process;
  ------------------------------------------------------------------------------

  ------------------------------------------------------------------------------
  dut : iicmb_m_wb
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
      clk_i       => clk_i,
      rst_i       => rst_i,
      cyc_i       => cyc_i,
      stb_i       => stb_i,
      ack_o       => ack_o,
      adr_i       => adr_i,
      we_i        => we_i,
      dat_i       => dat_i,
      dat_o       => dat_o,
      irq         => irq,
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

    ----------------------------------------------------------------------------
    i2c_slave_model_inst0 : i2c_slave_model
      generic map
      (
        I2C_ADR => to_integer(unsigned(get_slave_addr(i)))
      )
      port map
      (
        scl     => scl(i),
        sda     => sda(i)
      );
    ----------------------------------------------------------------------------
  end generate bus_gen;
  --****************************************************************************

  scl <= (others => 'H'); -- Pull up
  sda <= (others => 'H'); -- Pull up

  scl_nquant <= to_bitvector(to_x01(scl));
  sda_nquant <= to_bitvector(to_x01(sda));

end architecture beh;
--==============================================================================

