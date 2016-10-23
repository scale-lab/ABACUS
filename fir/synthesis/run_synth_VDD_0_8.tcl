
set search_path 	[list . $env(TECH_LIB_DIR)]
set target_library 	[list tcbn65gplustc0d8.db]
set link_library 	[list {*} tcbn65gplustc0d8.db]

set DESIGN_NAME $env(DESIGN_NAME)
set files_loc $env(SYNTH_DIR)

set file_name ${DESIGN_NAME}_netlist
set vdd 0_8

set SUB_MODULE [list ${files_loc}/$file_name.v]
analyze -format verilog $SUB_MODULE
#check_design


set command_status [elaborate $DESIGN_NAME -update] 
if ($command_status==0) {quit}
set command_status [link] 
if ($command_status==0) {quit}
set command_status [create_clock clk -period 2 -waveform {0 1}]
if ($command_status==0) {quit}


set_propagated_clock  clk
set_cost_priority -delay
set_fix_hold clk


set_wire_load_mode top
set_wire_load_model -name TSMC64K_Lowk_Conservative
set_wire_load_selection -name TSMC64K_Lowk_Conservative

set netlist_ext _netlist.v
set sdf_ext _sdf.sdf

report_timing -max_paths 1 > $files_loc/sta_stp_rpt_$vdd
report_power > $files_loc/pwr_rpt_$vdd
report_area > $files_loc/area_rpt_$vdd
report_timing -delay min -max_paths 1 > $files_loc/sta_hld_rpt_$vdd
report_timing -path full_clock_expanded > $files_loc/clk_ntwk_rpt_$vdd

