
if {[file exists work]} {
	vdel -lib work -all
}
vlib work
vmap work work

onerror {quit -f}

vlog -compat -work work +acc=rnbp "[pwd]/me_bram_search_2column.v" "[pwd]/me_bram_search_3column.v" "[pwd]/me_comparator.v" "[pwd]/me_control.v" "[pwd]/me_datapath.v" "[pwd]/me_PE.v" "$env(SOURCE_FOLDER)/me_sad_calculation.v" "[pwd]/me_sad_reuse.v" "[pwd]/me_topmodule.v" "[pwd]/test256psnr.v"

vsim -c -t 1ps +transport_int_delays +transport_path_delays -novopt -L work test256psnr

profile on
profile interval 1

#vcd file output/monitor.vcd
#vcd add *
#vcd add /perceptron_test/data_x /perceptron_test/data_y /perceptron_test/DE_in /perceptron_test/DE_out
#vcd add /perceptron_test/svm/current_state /perceptron_test/svm/next_state

add wave -decimal -analog-step *

run -all

profile report -hierarchical -file output1/profile.rpt
profile off

#view list
#add list -decimal /perceptron_test/data_x /perceptron_test/data_y /perceptron_test/reset /perceptron_test/start
#add list -decimal /perceptron_test/DE_in /perceptron_test/DE_out /perceptron_test/label

#write list output/careddata.txt
#write list alldata.txt
#noview list

quit -sim
quit -f
