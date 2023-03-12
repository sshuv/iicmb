# SDC Example
#

create_clock -name clk100 -period 10.000 [get_ports {clk_i}]

derive_clock_uncertainty

set_output_delay -clock [get_clocks {clk100}] -max 5 [get_ports {scl_o* sda_o*}] -add_delay;    #
set_output_delay -clock [get_clocks {clk100}] -min 1 [get_ports {scl_o* sda_o*}] -add_delay;    #

set_input_delay  -clock [get_clocks {clk100}] -max 5 [get_ports {scl_i* sda_i*}] -add_delay;    #
set_input_delay  -clock [get_clocks {clk100}] -min 1 [get_ports {scl_i* sda_i*}] -add_delay;    #

set_multicycle_path \
    -from [get_clocks {clk100}] \
    -through [get_pins -compatibility_mode -no_duplicates scl_i*] \
    -to [get_clocks {clk100}] \
    -setup -end 5;  # filter.vhd: g_cycles = 10

set_multicycle_path \
    -from [get_clocks {clk100}] \
    -through [get_pins -compatibility_mode -no_duplicates scl_i*] \
    -to [get_clocks {clk100}] \
    -hold -start 5

set_multicycle_path \
    -from [get_clocks {clk100}] \
    -through [get_pins -compatibility_mode -no_duplicates sda_i*] \
    -to [get_clocks {clk100}] \
    -setup -end 5;  # filter.vhd: g_cycles = 10

set_multicycle_path \
    -from [get_clocks {clk100}] \
    -through [get_pins -compatibility_mode -no_duplicates sda_i*] \
    -to [get_clocks {clk100}] \
    -hold -start 5

set_multicycle_path \
    -from [get_clocks {clk100}] \
    -through [get_pins -compatibility_mode -no_duplicates scl_o*] \
    -to [get_clocks {clk100}] \
    -setup -end 5;  # filter.vhd: g_cycles = 10

set_multicycle_path \
    -from [get_clocks {clk100}] \
    -through [get_pins -compatibility_mode -no_duplicates scl_o*] \
    -to [get_clocks {clk100}] \
    -hold -start 5

set_multicycle_path \
    -from [get_clocks {clk100}] \
    -through [get_pins -compatibility_mode -no_duplicates sda_o*] \
    -to [get_clocks {clk100}] \
    -setup -end 5;  # filter.vhd: g_cycles = 10

set_multicycle_path \
    -from [get_clocks {clk100}] \
    -through [get_pins -compatibility_mode -no_duplicates sda_o*] \
    -to [get_clocks {clk100}] \
    -hold -start 5
