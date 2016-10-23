/*
Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following
conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
*/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "globals.h"
#include "types.h"
#include "util.h"
#include "parse_making_ast.h"
#include "read_xml_config_file.h"
#include "read_xml_arch_file.h"
#include "hard_blocks.h"
#include "errors.h"


int current_parse_file;
t_arch Arch;
t_type_descriptor* type_descriptors;
int block_tag;
global_args_t global_args;

char **matlab_vars;
int matlab_vars_cnt;

void get_options(int argc, char **argv);
void do_high_level_synthesis();
void print_usage();

int main(int argc, char **argv)
{
	printf("--------------------------------------------------------------------\n\n");
	printf("Welcome to ABACUS Behavioral Compiler tool.\nThis tool is adapted from the original open-source ODIN_II synthesizer tool\n");
	printf("Email: kumud.nepal@gmail.com and hokchhay_tann@brown.edu for support issues \n\n");

    srand(time(NULL));
	/* get the command line options */
	get_options(argc, argv);

	if (!global_args.blif_file)
	{
	    /* High level synthesis tool */
		do_high_level_synthesis();
	}
	return 0;
}

/*
 * Prints usage information for Odin II. This should be kept up to date with the latest
 * features added to Odin II.
 */
void print_usage()
{
	printf
	(
			"USAGE: ABACUS.exe [-c <Configuration> | -b <BLIF> | -V <Verilog HDL>]\n"
			"  -c <XML Configuration File>\n"
			"  -V <Verilog HDL File>\n"
			"  -b <BLIF File>\n"
			" Other options:\n"
			"  -o <output_path and file name>\n"
			"  -a <architecture_file_in_VPR6.0_form>\n"
			#ifdef VPR5
			"  -B <blif_file_for_activation_estimation> \n"
			"     Use with: -N <net_file_for_activation_estimation>\n"
			#endif
            "  -C path to text file for critical path identification\n"
			"  -G Output netlist graph in graphviz .dot format. (net.dot, opens with dotty)\n"
			"  -A Output AST graph in .dot format.\n"
			"  -W Print all warnings. (Can be substantial.) \n"
			"  -h Print help\n"
			"\n"
			" SIMULATION: Always produces input_vectors, output_vectors,\n"
			"             and ModelSim test.do file.\n"
			"  Activate simulation with either: \n"
			"  -g <Number of random test vectors to generate>\n"
			"     -L <Comma-separated list of primary inputs to hold \n"
			"         high at cycle 0, and low for all subsequent cycles.>\n"
			"     -H <Comma-separated list of primary inputs to hold low at \n"
			"         cycle 0, and high for all subsequent cycles.>\n"
			"     -3 Generate three valued logic. (Default is binary.)\n"
			"  -t <input vector file>: Supply a predefined input vector file\n"

			" Other Simulation Options: \n"
			"  -T <output vector file>: Supply an output vector file to check output\n"
			"                            vectors against.\n"
			"  -E Output after both edges of the clock.\n"
			"     (Default is to output only after the falling edge.)\n"
			"  -R Output after rising edge of the clock only.\n"
			"     (Default is to output only after the falling edge.)\n"
			"  -p <Comma-separated list of additional pins/nodes to monitor\n"
			"      during simulation.>\n"
			"     Eg: \"-p input~0,input~1\" monitors pin 0 and 1 of input, \n"
			"       or \"-p input\" monitors all pins of input as a single port. \n"
			"       or \"-p input~\" monitors all pins of input as separate ports. (split) \n"
			"     - Note: Non-existent pins are ignored. \n"
			"     - Matching is done via strstr so general strings will match \n"
			"       all similar pins and nodes.\n"
			"         (Eg: FF_NODE will create a single port with all flipflops) \n"
	);
	fflush(stdout);
}

/*---------------------------------------------------------------------------------------------
 * (function: get_options)
 *-------------------------------------------------------------------------*/
void get_options(int argc, char **argv)
{
	/* Set up the global arguments to their default. */
	global_args.config_file = NULL;
	global_args.matlab_input_arg = NULL;
	global_args.verilog_file = NULL;
    global_args.ast_file = NULL;
    global_args.power_estimation = 0;
    global_args.critical_path_file = NULL;
	global_args.blif_file = NULL;
	global_args.output_file = "./default_out.blif";
	global_args.arch_file = NULL;
	global_args.activation_blif_file = NULL;
	global_args.activation_netlist_file = NULL;
	global_args.high_level_block = NULL;
	global_args.sim_vector_input_file = NULL;
	global_args.sim_vector_output_file = NULL;
	global_args.sim_additional_pins = NULL;
	global_args.sim_num_test_vectors = 0;
	global_args.sim_generate_three_valued_logic = 0;
	global_args.sim_hold_low = NULL;
	global_args.sim_hold_high = NULL;
	global_args.sim_output_both_edges = 0;
	global_args.sim_output_rising_edge = 0;
	global_args.all_warnings = 0;

	/* Set up the global configuration. */
	configuration.list_of_file_names = NULL;
	configuration.num_list_of_file_names = 0;
	configuration.output_type = "blif";
	configuration.output_ast_graphs = 0;
	configuration.output_netlist_graphs = 0;
	configuration.print_parse_tokens = 0;
	configuration.output_preproc_source = 0;
	configuration.debug_output_path = ".";
	configuration.arch_file = NULL;

	configuration.fixed_hard_multiplier = 0;
	configuration.split_hard_multiplier = 0;

	configuration.split_memory_width = 0;
	configuration.split_memory_depth = 0;

	/*
	 * Soft logic cutoffs. If a memory or a memory resulting from a split
	 * has a width AND depth below these, it will be converted to soft logic.
	 */
	configuration.soft_logic_memory_width_threshold = 0;
	configuration.soft_logic_memory_depth_threshold = 0;

	/* Parse the command line options.  */
	const char *optString = "hc:C:m:V:x:WREh:o:a:B:b:N:f:s:S:p:z:g:t:T:L:H:GAP3";
	int opt = getopt(argc, argv, optString);
	while(opt != -1)
	{
		switch(opt)
		{
			/* arch file */
			case 'a':
				global_args.arch_file = optarg;
				configuration.arch_file = optarg;
			break;
			/* config file */
			case 'c':
				global_args.config_file = optarg;
			break;
			case 'm':
				global_args.matlab_input_arg = optarg;
			break;
			case 'V':
				global_args.verilog_file = optarg;
			break;
            case 'x': 
                global_args.ast_file = optarg;
            break;
            case 'P':
                global_args.power_estimation = 1;
            break;
            case 'C':
                global_args.critical_path_file = optarg;
            break;
			case 'o':
				global_args.output_file = optarg;
			break;
			#ifdef VPR5
			case 'B':
				global_args.activation_blif_file = optarg;
			break;
			case 'N':
				global_args.activation_netlist_file = optarg;
			break;
			#endif
			case 'b':
				global_args.blif_file = optarg;
			break;
			case 'f':
				#ifdef VPR5
				global_args.high_level_block = optarg;
				#endif
				#ifdef VPR6
				warning_message(0, -1, 0, 
								"Option -f: VPR 6.0 doesn't have this feature yet.  You'll need to deal with the output_blif.c differences wrapped by \"if (global_args.high_level_block != NULL)\"\n");
				#endif
			break;
			case 'h':
				print_usage();
				exit(-1);
			break;
			case 'g':
				global_args.sim_num_test_vectors = atoi(optarg);
			break;
			case '3':
				global_args.sim_generate_three_valued_logic = 1;
			break;
			case 'L':
				global_args.sim_hold_low = optarg;
			break;
			case 'H':
				global_args.sim_hold_high = optarg;
			break;
			case 't':
				global_args.sim_vector_input_file = optarg;
			break;
			case 'T':
				global_args.sim_vector_output_file = optarg;
			break;
			case 'p':
				global_args.sim_additional_pins = optarg;
	    	break;
			case 'G':
				configuration.output_netlist_graphs = 1;
			break;
			case 'A':
				configuration.output_ast_graphs = 1;
			break;
			case 'W':
				global_args.all_warnings = 1;
			break;
			case 'E':
				global_args.sim_output_both_edges = 1;
			break;
			case 'R':
				global_args.sim_output_rising_edge = 1;
			break;
			default :
				print_usage();
				error_message(-1, 0, -1, "Invalid arguments.\n");
			break;
		}
		opt = getopt(argc, argv, optString);
	}

	if (
			   !global_args.config_file
			&& !global_args.blif_file
			&& !global_args.verilog_file
            && !global_args.ast_file 
			&& ((!global_args.activation_blif_file) || (!global_args.activation_netlist_file)))
	{
		print_usage();
		error_message(-1,0,-1,"Must include either "
				#ifdef VPR5
				"a activation blif and netlist file, "
				#endif
				"a config file, a blif netlist, a verilog file, or an ast file\n");
	}
	else if ((global_args.config_file && global_args.verilog_file) || global_args.activation_blif_file)
	{
		warning_message(-1,0,-1, "Using command line options for verilog input file!!!\n");
	}
}

/*---------------------------------------------------------------------------
 * (function: do_high_level_synthesis)
 *-------------------------------------------------------------------------*/
void do_high_level_synthesis()
{
	printf("--------------------------------------------------------------------\n");
	printf("High-level synthesis Begin\n");

	/* parse to abstract syntax tree */
	printf("Parser starting - we'll create an abstract syntax tree.  "
			"Note this tree can be viewed using GraphViz (see documentation)\n");
	global_param_table_sc = sc_new_string_cache();
	parse_to_ast();
	/* Note that the entry point for ast optimzations is done per module with the
	 * function void next_parsed_verilog_file(ast_node_t *file_items_list) */

	printf("Succesful generation of AST. Netlist creation and synthesis skipped for now");
	printf("\n");
	printf("--------------------------------------------------------------------\n");

}

