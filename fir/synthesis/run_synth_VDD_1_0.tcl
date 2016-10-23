
set search_path 	[list . ~/LIBRARY/]
set target_library 	[list tcbn65gplustc.db]
set link_library 	[list {*} tcbn65gplustc.db]

set DESIGN_NAME fir_filter
set files_loc [pwd]

set file_name fir_filter_netlist
set vdd 1_0


set SUB_MODULE [list ${files_loc}/output/$file_name.v ${files_loc}/DRUM6_16_16.v]
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
set output_folder_name output

report_timing -max_paths 1 > $files_loc/$output_folder_name/sta_stp_rpt_$file_name$vdd
report_power > $files_loc/$output_folder_name/pwr_rpt_$file_name$vdd
report_area > $files_loc/$output_folder_name/area_rpt_$file_name$vdd
report_timing -delay min -max_paths 1 > $files_loc/$output_folder_name/sta_hld_rpt_$file_name$vdd
report_timing -path full_clock_expanded > $files_loc/$output_folder_name/clk_ntwk_rpt_$file_name$vdd
exit

