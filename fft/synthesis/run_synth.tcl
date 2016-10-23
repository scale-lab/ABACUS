
set search_path 	[list . $env(TECH_LIB_DIR)]
set target_library 	[list tcbn65gplustc.db]
set link_library 	[list {*} tcbn65gplustc.db]

set DESIGN_NAME $env(DESIGN_NAME)
set files_loc $env(SOURCE_FOLDER)
puts ${files_loc}
set SUB_MODULE [list ${files_loc}/fft.v ${files_loc}/DRUM6_10_16.v]
analyze -format verilog $SUB_MODULE

set command_status [elaborate $DESIGN_NAME] 
if ($command_status==0) {quit}
set command_status [link] 
if ($command_status==0) {quit}
set command_status [create_clock clk -period 5 -waveform {0 2.5}]
if ($command_status==0) {quit}


set_propagated_clock  clk
set_cost_priority -delay
set_fix_hold clk

set_wire_load_mode top
set_wire_load_model -name TSMC64K_Lowk_Conservative
set_wire_load_selection -name TSMC64K_Lowk_Conservative


#set_operating_conditions typical 
#-min fast -max slow 
set command_status [compile]
if ($command_status==0) {quit}
set netlist_ext _netlist.v
set sdf_ext _sdf.sdf
set output_folder_name $env(SYNTH_DIR) 
write -format verilog -hierarchy -output $output_folder_name/$DESIGN_NAME$netlist_ext
report_timing -max_paths 500 > $output_folder_name/sta_stp_rpt
report_power > $output_folder_name/pwr_rpt
report_area > $output_folder_name/area_rpt
report_timing -delay min -max_paths 500 > $output_folder_name/sta_hld_rpt
report_timing -path full_clock_expanded > $output_folder_name/clk_ntwk_rpt

