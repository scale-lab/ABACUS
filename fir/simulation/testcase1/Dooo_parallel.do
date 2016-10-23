if {[file exists work]} {
	vdel -lib work -all
}
vlib work
vmap work work

onerror {quit -f}

vlog -compat -work work +acc=rnbp +perceptron_test. +perceptron.svm. "fir_filter.v" "$env(SOURCE_FOLDER)/DRUM6_16_16.v" "fir_test.v"
vsim -c -t 1ps +transport_int_delays +transport_path_delays -novopt -L work fir_test

#profile on
#profile interval 1

#vcd file monitor.vcd
#vcd add *
#add wave -decimal -analog-step *
#power add *

run -all

#power report -file power.rpt

#profile report -hierarchical -file profile.rpt
#profile off

#view list
#add list -decimal /perceptron_test/data_x /perceptron_test/data_y /perceptron_test/reset /perceptron_test/start
#add list -decimal /perceptron_test/DE_in /perceptron_test/DE_out /perceptron_test/label

#write list output/careddata.txt
#write list alldata.txt
#noview list

quit -sim
quit -f
