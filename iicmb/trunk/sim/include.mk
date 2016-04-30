
# Should be defined externally:
#   IICMB_DIR    -- Directory of the IICMB project sources
#   LIB_IICMB    -- Design library for IICMB core
#   LIB_IICMB_TB -- Design library for IICMB testbenches


# We have to explicitly introduce both primary and secondary units in order to
# be able to consistently recompile the sources.
LIB_IICMB__iicmb_pkg                 = $(LIB_IICMB)/iicmb_pkg/_primary.dat
LIB_IICMB__iicmb_pkg__body           = $(LIB_IICMB)/iicmb_pkg/body.dat
LIB_IICMB__iicmb_int_pkg             = $(LIB_IICMB)/iicmb_int_pkg/_primary.dat
LIB_IICMB__iicmb_int_pkg__body       = $(LIB_IICMB)/iicmb_int_pkg/body.dat
LIB_IICMB__wishbone                  = $(LIB_IICMB)/wishbone/_primary.dat
LIB_IICMB__wishbone__rtl             = $(LIB_IICMB)/wishbone/rtl.dat
LIB_IICMB__avalon_mm                 = $(LIB_IICMB)/avalon_mm/_primary.dat
LIB_IICMB__avalon_mm__rtl            = $(LIB_IICMB)/avalon_mm/rtl.dat
LIB_IICMB__sequencer                 = $(LIB_IICMB)/sequencer/_primary.dat
LIB_IICMB__sequencer__rtl            = $(LIB_IICMB)/sequencer/rtl.dat
LIB_IICMB__regblock                  = $(LIB_IICMB)/regblock/_primary.dat
LIB_IICMB__regblock__rtl             = $(LIB_IICMB)/regblock/rtl.dat
LIB_IICMB__mbyte                     = $(LIB_IICMB)/mbyte/_primary.dat
LIB_IICMB__mbyte__rtl                = $(LIB_IICMB)/mbyte/rtl.dat
LIB_IICMB__mbit                      = $(LIB_IICMB)/mbit/_primary.dat
LIB_IICMB__mbit__rtl                 = $(LIB_IICMB)/mbit/rtl.dat
LIB_IICMB__bus_state                 = $(LIB_IICMB)/bus_state/_primary.dat
LIB_IICMB__bus_state__rtl            = $(LIB_IICMB)/bus_state/rtl.dat
LIB_IICMB__filter                    = $(LIB_IICMB)/filter/_primary.dat
LIB_IICMB__filter__rtl               = $(LIB_IICMB)/filter/rtl.dat
LIB_IICMB__conditioner               = $(LIB_IICMB)/conditioner/_primary.dat
LIB_IICMB__conditioner__str          = $(LIB_IICMB)/conditioner/str.dat
LIB_IICMB__conditioner_mux           = $(LIB_IICMB)/conditioner_mux/_primary.dat
LIB_IICMB__conditioner_mux__str      = $(LIB_IICMB)/conditioner_mux/str.dat
LIB_IICMB__iicmb_m                   = $(LIB_IICMB)/iicmb_m/_primary.dat
LIB_IICMB__iicmb_m__str              = $(LIB_IICMB)/iicmb_m/str.dat
LIB_IICMB__iicmb_m_wb                = $(LIB_IICMB)/iicmb_m_wb/_primary.dat
LIB_IICMB__iicmb_m_wb__str           = $(LIB_IICMB)/iicmb_m_wb/str.dat
LIB_IICMB__iicmb_m_av                = $(LIB_IICMB)/iicmb_m_av/_primary.dat
LIB_IICMB__iicmb_m_av__str           = $(LIB_IICMB)/iicmb_m_av/str.dat
LIB_IICMB__iicmb_m_sq                = $(LIB_IICMB)/iicmb_m_sq/_primary.dat
LIB_IICMB__iicmb_m_sq__str           = $(LIB_IICMB)/iicmb_m_sq/str.dat

# Testbench targets:
LIB_IICMB_TB__i2c_slave_model        = $(LIB_IICMB_TB)/i2c_slave_model/_primary.dat
LIB_IICMB_TB__test                   = $(LIB_IICMB_TB)/test/_primary.dat
LIB_IICMB_TB__test__body             = $(LIB_IICMB_TB)/test/body.dat
LIB_IICMB_TB__wire_mdl               = $(LIB_IICMB_TB)/wire_mdl/_primary.dat
LIB_IICMB_TB__wire_mdl__beh          = $(LIB_IICMB_TB)/wire_mdl/beh.dat
LIB_IICMB_TB__iicmb_m_tb             = $(LIB_IICMB_TB)/iicmb_m_tb/_primary.dat
LIB_IICMB_TB__iicmb_m_tb__beh        = $(LIB_IICMB_TB)/iicmb_m_tb/beh.dat
LIB_IICMB_TB__iicmb_m_wb_tb          = $(LIB_IICMB_TB)/iicmb_m_wb_tb/_primary.dat
LIB_IICMB_TB__iicmb_m_wb_tb__beh     = $(LIB_IICMB_TB)/iicmb_m_wb_tb/beh.dat
LIB_IICMB_TB__iicmb_m_sq_tb          = $(LIB_IICMB_TB)/iicmb_m_sq_tb/_primary.dat
LIB_IICMB_TB__iicmb_m_sq_tb__beh     = $(LIB_IICMB_TB)/iicmb_m_sq_tb/beh.dat
LIB_IICMB_TB__iicmb_m_sq_arb_tb      = $(LIB_IICMB_TB)/iicmb_m_sq_arb_tb/_primary.dat
LIB_IICMB_TB__iicmb_m_sq_arb_tb__beh = $(LIB_IICMB_TB)/iicmb_m_sq_arb_tb/beh.dat


$(LIB_IICMB) :
	vlib -type directory $(LIB_IICMB)

$(LIB_IICMB_TB) :
	vlib -type directory $(LIB_IICMB_TB)

$(LIB_IICMB__iicmb_pkg) $(LIB_IICMB__iicmb_pkg__body) : $(IICMB_DIR)/src/iicmb_pkg.vhd | $(LIB_IICMB)
	$(VCOM) -work $(LIB_IICMB) -2002 -O0 -quiet -explicit -check_synthesis $<

$(LIB_IICMB__iicmb_int_pkg) $(LIB_IICMB__iicmb_int_pkg__body) : $(IICMB_DIR)/src/iicmb_int_pkg.vhd | $(LIB_IICMB)
	$(VCOM) -work $(LIB_IICMB) -2002 -O0 -quiet -explicit -check_synthesis $<

$(LIB_IICMB__regblock) $(LIB_IICMB__regblock__rtl) : $(IICMB_DIR)/src/regblock.vhd $(LIB_IICMB__iicmb_pkg) | $(LIB_IICMB)
	$(VCOM) -work $(LIB_IICMB) -2002 -O0 -quiet -explicit -check_synthesis $<

$(LIB_IICMB__avalon_mm) $(LIB_IICMB__avalon_mm__rtl) : $(IICMB_DIR)/src/avalon_mm.vhd | $(LIB_IICMB)
	$(VCOM) -work $(LIB_IICMB) -2002 -O0 -quiet -explicit -check_synthesis $<

$(LIB_IICMB__sequencer) $(LIB_IICMB__sequencer__rtl) : $(IICMB_DIR)/src/sequencer.vhd $(LIB_IICMB__iicmb_pkg) | $(LIB_IICMB)
	$(VCOM) -work $(LIB_IICMB) -2002 -O0 -quiet -explicit -check_synthesis $<

$(LIB_IICMB__wishbone) $(LIB_IICMB__wishbone__rtl) : $(IICMB_DIR)/src/wishbone.vhd | $(LIB_IICMB)
	$(VCOM) -work $(LIB_IICMB) -2002 -O0 -quiet -explicit -check_synthesis $<

$(LIB_IICMB__mbyte) $(LIB_IICMB__mbyte__rtl) : $(IICMB_DIR)/src/mbyte.vhd $(LIB_IICMB__iicmb_pkg) $(LIB_IICMB__iicmb_int_pkg) | $(LIB_IICMB)
	$(VCOM) -work $(LIB_IICMB) -2002 -O0 -quiet -explicit -check_synthesis $<

$(LIB_IICMB__mbit) $(LIB_IICMB__mbit__rtl) : $(IICMB_DIR)/src/mbit.vhd $(LIB_IICMB__iicmb_int_pkg) | $(LIB_IICMB)
	$(VCOM) -work $(LIB_IICMB) -2002 -O0 -quiet -explicit -check_synthesis $<

$(LIB_IICMB__bus_state) $(LIB_IICMB__bus_state__rtl) : $(IICMB_DIR)/src/bus_state.vhd | $(LIB_IICMB)
	$(VCOM) -work $(LIB_IICMB) -2002 -O0 -quiet -explicit -check_synthesis $<

$(LIB_IICMB__filter) $(LIB_IICMB__filter__rtl) : $(IICMB_DIR)/src/filter.vhd | $(LIB_IICMB)
	$(VCOM) -work $(LIB_IICMB) -2002 -O0 -quiet -explicit -check_synthesis $<

$(LIB_IICMB__conditioner) $(LIB_IICMB__conditioner__str) : $(IICMB_DIR)/src/conditioner.vhd | $(LIB_IICMB)
	$(VCOM) -work $(LIB_IICMB) -2002 -O0 -quiet -explicit -check_synthesis $<

$(LIB_IICMB__conditioner_mux) $(LIB_IICMB__conditioner_mux__str) : $(IICMB_DIR)/src/conditioner_mux.vhd | $(LIB_IICMB)
	$(VCOM) -work $(LIB_IICMB) -2002 -O0 -quiet -explicit -check_synthesis $<

$(LIB_IICMB__iicmb_m) $(LIB_IICMB__iicmb_m__str) : $(IICMB_DIR)/src/iicmb_m.vhd $(LIB_IICMB__iicmb_pkg) $(LIB_IICMB__iicmb_int_pkg) | $(LIB_IICMB)
	$(VCOM) -work $(LIB_IICMB) -2002 -O0 -quiet -explicit -check_synthesis $<

$(LIB_IICMB__iicmb_m_wb) $(LIB_IICMB__iicmb_m_wb__str) : $(IICMB_DIR)/src/iicmb_m_wb.vhd | $(LIB_IICMB)
	$(VCOM) -work $(LIB_IICMB) -2002 -O0 -quiet -explicit -check_synthesis $<

$(LIB_IICMB__iicmb_m_av) $(LIB_IICMB__iicmb_m_av__str) : $(IICMB_DIR)/src/iicmb_m_av.vhd | $(LIB_IICMB)
	$(VCOM) -work $(LIB_IICMB) -2002 -O0 -quiet -explicit -check_synthesis $<

$(LIB_IICMB__iicmb_m_sq) $(LIB_IICMB__iicmb_m_sq__str) : $(IICMB_DIR)/src/iicmb_m_sq.vhd | $(LIB_IICMB)
	$(VCOM) -work $(LIB_IICMB) -2002 -O0 -quiet -explicit -check_synthesis $<


$(LIB_IICMB_TB__i2c_slave_model) : $(IICMB_DIR)/src_tb/i2c_slave_model.v $(IICMB_DIR)/src_tb/timescale.v | $(LIB_IICMB_TB)
	$(VLOG) -work $(LIB_IICMB_TB) -O0 -quiet +incdir+$(IICMB_DIR)/src_tb $<


$(LIB_IICMB_TB__test) $(LIB_IICMB_TB__test__body) : $(IICMB_DIR)/src_tb/test.vhd | $(LIB_IICMB_TB)
	$(VCOM) -work $(LIB_IICMB_TB) -2002 -O0 -quiet -explicit $<

$(LIB_IICMB_TB__wire_mdl) $(LIB_IICMB_TB__wire_mdl__beh) : $(IICMB_DIR)/src_tb/wire_mdl.vhd | $(LIB_IICMB_TB)
	$(VCOM) -work $(LIB_IICMB_TB) -2002 -O0 -quiet -explicit $<

$(LIB_IICMB_TB__iicmb_m_tb) $(LIB_IICMB_TB__iicmb_m_tb__beh) : $(IICMB_DIR)/src_tb/iicmb_m_tb.vhd $(LIB_IICMB__iicmb_pkg) $(LIB_IICMB__test) | $(LIB_IICMB_TB)
	$(VCOM) -work $(LIB_IICMB_TB) -2002 -O0 -quiet -explicit $<

$(LIB_IICMB_TB__iicmb_m_wb_tb) $(LIB_IICMB_TB__iicmb_m_wb_tb__beh) : $(IICMB_DIR)/src_tb/iicmb_m_wb_tb.vhd $(LIB_IICMB__iicmb_pkg) $(LIB_IICMB__test) | $(LIB_IICMB_TB)
	$(VCOM) -work $(LIB_IICMB_TB) -2002 -O0 -quiet -explicit $<

$(LIB_IICMB_TB__iicmb_m_sq_tb) $(LIB_IICMB_TB__iicmb_m_sq_tb__beh) : $(IICMB_DIR)/src_tb/iicmb_m_sq_tb.vhd $(LIB_IICMB__iicmb_pkg) $(LIB_IICMB__test) | $(LIB_IICMB_TB)
	$(VCOM) -work $(LIB_IICMB_TB) -2002 -O0 -quiet -explicit $<

$(LIB_IICMB_TB__iicmb_m_sq_arb_tb) $(LIB_IICMB_TB__iicmb_m_sq_arb_tb__beh) : $(IICMB_DIR)/src_tb/iicmb_m_sq_arb_tb.vhd $(LIB_IICMB__iicmb_pkg) $(LIB_IICMB__test) | $(LIB_IICMB_TB)
	$(VCOM) -work $(LIB_IICMB_TB) -2002 -O0 -quiet -explicit $<



IICMB_TGTS = \
	$(LIB_IICMB__avalon_mm)               $(LIB_IICMB__avalon_mm__rtl)               \
	$(LIB_IICMB__sequencer)               $(LIB_IICMB__sequencer__rtl)               \
	$(LIB_IICMB__wishbone)                $(LIB_IICMB__wishbone__rtl)                \
	$(LIB_IICMB__regblock)                $(LIB_IICMB__regblock__rtl)                \
	$(LIB_IICMB__mbyte)                   $(LIB_IICMB__mbyte__rtl)                   \
	$(LIB_IICMB__mbit)                    $(LIB_IICMB__mbit__rtl)                    \
	$(LIB_IICMB__bus_state)               $(LIB_IICMB__bus_state__rtl)               \
	$(LIB_IICMB__filter)                  $(LIB_IICMB__filter__rtl)                  \
	$(LIB_IICMB__conditioner)             $(LIB_IICMB__conditioner__str)             \
	$(LIB_IICMB__conditioner_mux)         $(LIB_IICMB__conditioner_mux__str)         \
	$(LIB_IICMB__iicmb_m)                 $(LIB_IICMB__iicmb_m__str)                 \
	$(LIB_IICMB__iicmb_m_wb)              $(LIB_IICMB__iicmb_m_wb__str)              \
	$(LIB_IICMB__iicmb_m_av)              $(LIB_IICMB__iicmb_m_av__str)              \
	$(LIB_IICMB__iicmb_m_sq)              $(LIB_IICMB__iicmb_m_sq__str)              \


IICMB_TB_TGTS = \
	$(LIB_IICMB_TB__i2c_slave_model)                                                 \
	$(LIB_IICMB_TB__test)                 $(LIB_IICMB_TB__test__body)                \
	$(LIB_IICMB_TB__wire_mdl)             $(LIB_IICMB_TB__wire_mdl__beh)             \
	$(LIB_IICMB_TB__iicmb_m_tb)           $(LIB_IICMB_TB__iicmb_m_tb__beh)           \
	$(LIB_IICMB_TB__iicmb_m_wb_tb)        $(LIB_IICMB_TB__iicmb_m_wb_tb__beh)        \
	$(LIB_IICMB_TB__iicmb_m_sq_tb)        $(LIB_IICMB_TB__iicmb_m_sq_tb__beh)        \
	$(LIB_IICMB_TB__iicmb_m_sq_arb_tb)    $(LIB_IICMB_TB__iicmb_m_sq_arb_tb__beh)    \



.PHONY : clean_iicmb
clean_iicmb :
	if [ -d $(LIB_IICMB) ];    then rm -rf $(LIB_IICMB);    fi;

.PHONY : clean_iicmb_tb
clean_iicmb_tb :
	if [ -d $(LIB_IICMB_TB) ]; then rm -rf $(LIB_IICMB_TB); fi;


ALL_TARGETS   := $(ALL_TARGETS) $(IICMB_TGTS) $(IICMB_TB_TGTS)
CLEAN_TARGETS := $(CLEAN_TARGETS) clean_iicmb clean_iicmb_tb

