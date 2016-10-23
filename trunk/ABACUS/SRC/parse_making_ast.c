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
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "globals.h"
#include "types.h"
#include "errors.h"
#include "ast_util.h"
#include "parse_making_ast.h"
#include "string_cache.h"
#include "ast_optimizations.h"
#include "verilog_bison_user_defined.h"
#include "verilog_preprocessor.h"
#include "util.h"
#include "hard_blocks.h"
#include <time.h>

extern int yylineno;

STRING_CACHE *defines_for_file_sc;
STRING_CACHE **defines_for_module_sc;
STRING_CACHE *modules_inputs_sc;
STRING_CACHE *modules_outputs_sc;

STRING_CACHE *module_names_to_idx;

ast_node_t **block_instantiations_instance;
int size_block_instantiations;

ast_node_t **module_instantiations_instance;
int size_module_instantiations;

int num_modules;
ast_node_t **ast_modules;

ast_node_t *TREE_ROOT;
ast_node_t *MODULE_ITEMS_ROOT;
int app_mult_cnt;
int G;

ast_node_t **all_file_items_list;
int size_all_file_items_list;

short to_view_parse;
int if_flag;
ast_node_t *parent_node;
int block_cnt=0;
char signed_unsigned[4096]="";
int range_high=0;
char rangetext[4096]="";
char *identif2[4096][4096];
char *bin_op_exception[4096][10];
int two_shift_children_flag=0;
int shift_flag;

//void graphVizOutputPreproc(FILE *yyin, char* path, char *file)
////at mult1trun = 1.7456e-02;
//// 56 float mult2trun = 3.2890e-02;
////  57 float add1trun = 0.0848e-02;
////   58 float add2trun = 0.2244e-02;
////    59 float or1trun = 0.0011e-02;
////     60 float or2trun = 0.1375e-02;
//{
//	char line[MaxLine];
//	FILE *fp;
//	char *tmp;
////
//	// strip the ".v" from file
//	tmp = strrchr(file, '.');
//	oassert(tmp);
//	oassert(*(tmp+1) == 'v');
//	*tmp = '\0';
//
//	// strip the path from file
//	tmp = strrchr(file, '/');
//	if (tmp) file = tmp;
//
//	sprintf(line, "%s/%s_preproc.v", path, file);
//	fp = fopen(line, "w");
//	oassert(fp);
//	while (fgets(line, MaxLine, yyin))
//		fprintf(fp, "%s", line);
//	fclose(fp);
//	rewind(yyin);
//}


/*---------------------------------------------------------------------------------------------
 * (function: parse_to_ast)
 *-------------------------------------------------------------------------------------------*/
void parse_to_ast()
{
    int i;
    extern FILE *yyin;
    extern int yylineno;

    /* hooks into macro at the top of verilog_flex.l that shows the tokens as they're parsed.  
     * Set to true if you want to see it go...*/
    to_view_parse = configuration.print_parse_tokens;

    /* initialize the parser */
    init_parser();

    /* open files for parsing */
    if (global_args.verilog_file != NULL)
    {
        /* make a consitant file list so we can access in compiler ... replicating what read 
         * config does for the filenames */
        configuration.list_of_file_names = (char**)malloc(sizeof(char*));
        configuration.num_list_of_file_names = 1;
        configuration.list_of_file_names[0] = global_args.verilog_file;

        yyin = fopen(global_args.verilog_file, "r");
        if (yyin == NULL)
        {
            error_message(-1, -1, -1, "cannot open file: %s", global_args.verilog_file);
        }

        /*Testing preprocessor - Paddy O'Brien*/
        init_veri_preproc();
        yyin = veri_preproc(yyin);
        cleanup_veri_preproc();

        /* write out the pre-processed file */
        if (configuration.output_preproc_source)
            //graphVizOutputPreproc(yyin, configuration.debug_output_path, 
            //                      configuration.list_of_file_names[0]) ;

            /* set the file name */
            current_parse_file = 0;

        /* setup the local parser structures for a file */
        init_parser_for_file();
        /* parse */
        yyparse();

        /* cleanup parser */
        clean_up_parser_for_file();

        fclose(yyin);
    }
    else if (global_args.config_file != NULL)
    {
        /* read all the files in the configuration file */
        for (i = 0; i < configuration.num_list_of_file_names; i++)
        {
            yyin = fopen(configuration.list_of_file_names[i], "r");
            if (yyin == NULL)
            {
                error_message(-1, -1, -1, "cannot open file: %s\n", 
                			  configuration.list_of_file_names[i]);
            }

            /*Testing preprocessor - Paddy O'Brien*/
            init_veri_preproc();
            yyin = veri_preproc(yyin);
            cleanup_veri_preproc();

            /* write out the pre-processed file */
            if (configuration.output_preproc_source)
                // graphVizOutputPreproc(yyin, configuration.debug_output_path, 
                // configuration.list_of_file_names[i]) ;

                /* set the file name */
                current_parse_file = i;

            /* reset the line count */
            yylineno = 0;

            /* setup the local parser structures for a file */
            init_parser_for_file();
            /* parse next file */
            yyparse();
            /* cleanup parser */
            clean_up_parser_for_file();

            fclose(yyin);
        }
    }
    else if (global_args.ast_file != NULL){ 
        ast_node_t* top = NULL;
        top = read_AST_from_file(global_args.ast_file);
        if (top == NULL){
            error_message(-1,-1,-1,"Unable to open input ast file.\n");
            return;
        }
        next_parsed_verilog_file(top);

    }

    /* clean up all the structures in the parser */
    cleanup_parser();

    /* for error messages - this is in case we use any of the parser functions 
     * after parsing (i.e. create_case_control_signals()) */
    current_parse_file = -1;
}

/* --------------------------------------------------------------------------------------------
   --------------------------------------------------------------------------------------------
   --------------------------------------------------------------------------------------------
   BASIC PARSING FUNCTIONS
   Assume that all `defines are constants so we can change the constant into a number 
   (see def_reduct by performing a search in this file)
   --------------------------------------------------------------------------------------------
   --------------------------------------------------------------------------------------------
   --------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 * (function: cleanup_hard_blocks)
 *   This function will correctly label nodes in the AST as being part of a
 *   hard block and not a module. This needs to be done post parsing since
 *   there are no language differences between hard blocks and modules.
 *--------------------------------------------------------------------------*/
//void cleanup_hard_blocks()
//{
//	int i, j;
//	ast_node_t *block_node, *instance_node, *connect_list_node;
//
//	for (i = 0; i < size_block_instantiations; i++)
//	{
//		block_node = block_instantiations_instance[i];
//		instance_node = block_node->children[1];
//		instance_node->type = HARD_BLOCK_NAMED_INSTANCE;
//		connect_list_node = instance_node->children[1];
//		connect_list_node->type = HARD_BLOCK_CONNECT_LIST;
//
//		for (j = 0; j < connect_list_node->num_children; j++)
//		{
//			connect_list_node->children[j]->type = HARD_BLOCK_CONNECT;
//		}
//	}
//	return;
//}

/*---------------------------------------------------------------------------------------------
 * (function: init_parser)
 *-------------------------------------------------------------------------------------------*/
void init_parser()
{
    defines_for_file_sc = sc_new_string_cache();

    defines_for_module_sc = NULL;

    /* record of each of the individual modules */
    num_modules = 0; // we're going to record all the modules in a list so we can build a tree of them later
    ast_modules = NULL;
    module_names_to_idx = sc_new_string_cache();
    module_instantiations_instance = NULL;
    size_module_instantiations = 0;
    block_instantiations_instance = NULL;
    size_block_instantiations = 0;

    /* keeps track of all the ast roots */
    all_file_items_list = NULL;
    size_all_file_items_list = 0;
}

/*---------------------------------------------------------------------------------------------
 * (function: cleanup_parser)
 *-------------------------------------------------------------------------------------------*/
void cleanup_parser()
{
    int i;

    /* frees all the defines for module string caches (used for parameters) */
    if (num_modules > 0)
    {
        for (i = 0; i < num_modules; i++)
        {
            sc_free_string_cache(defines_for_module_sc[i]);
        }

        free(defines_for_module_sc);
    }
}

/*---------------------------------------------------------------------------------------------
 * (function: init_parser_for_file)
 *-------------------------------------------------------------------------------------------*/
void init_parser_for_file()
{
    /* crrate a hash for defines so we can look them up when we find them */
    defines_for_module_sc = (STRING_CACHE**)realloc(defines_for_module_sc, sizeof(STRING_CACHE*)*(num_modules+1));
    defines_for_module_sc[num_modules] = sc_new_string_cache();

    /* create string caches to hookup PORTS with INPUT and OUTPUTs.  
     * This is made per module and will be cleaned and remade at next_module */
    modules_inputs_sc = sc_new_string_cache();
    modules_outputs_sc = sc_new_string_cache();
}

/*---------------------------------------------------------------------------------------------
 * (function: clean_up_parser_for_file)
 *-------------------------------------------------------------------------------------------*/
void clean_up_parser_for_file()
{
    /* cleanup the defines hash */
    sc_free_string_cache(defines_for_file_sc);
}

/*---added by Kumud--*/
int unique_label_count;
void change_mult_to_plus(ast_node_t *top)
{
    change_signs(top, NULL, -1);
}

void make_constant(ast_node_t *top)
{
    char **matlab_vars;
    int matlab_vars_cnt;
    char line[4096] = "";
    sprintf(line, "%s", global_args.matlab_input_arg);
    char *search1 = "+";

    //--------------------test---------------------
    //char  line[]= "weights[0]+results[0]+data[0]";
    matlab_vars  = NULL;
    char *  p    = strtok (line, search1);
    int n_tokens = 0, i;

    /* split string and append tokens to 'matlab_vars' */

    while (p) {
        matlab_vars = realloc (matlab_vars, sizeof (char*) * ++n_tokens);

        if (matlab_vars == NULL)
            exit (-1); /* memory allocation failed */

        matlab_vars[n_tokens-1] = p;

        p = strtok (NULL, search1);
    }

    /* realloc one extra element for the last NULL */

    matlab_vars = realloc (matlab_vars, sizeof (char*) * (n_tokens+1));
    matlab_vars[n_tokens] = 0;

    /* print the result */

    //for (i = 0; i < (n_tokens+1); ++i)
    // printf ("matlab_vars[%d] = %s\n", i, matlab_vars[i]);
    matlab_vars_cnt = n_tokens;
    //printf ("matlab_vars_cnt = %d\n", matlab_vars_cnt);

    /* free the memory allocated */

    //free (matlab_vars);

    //-----------------test end--------------------


    make_id_constant(top,NULL, -1, matlab_vars, matlab_vars_cnt);
}

char *ok1;
char *ok2;
char tempholder[4096]="";
int truncate_or_not=1;
int truncate_flag=0;
int truncate_both_operands=0;

int truncate_cntr=0;
int truncate_done=0;
void change_id_to_range(ast_node_t *node, ast_node_t *top_node, ast_node_t *from, int from_num)
{
    //srand(time(NULL));
    truncate_or_not=1;
    int i;


    int my_label = unique_label_count;

    /* increase the unique count for other nodes since ours is recorded */
    unique_label_count++;

    if (node == NULL)
    {
        /* print out the node and label details */

    }
    else
    {
        //switch(node->type)
        //{
        int flag_DRUM=0;   
        ast_node_t *node_DRUM;
        if(node->type==MODULE_INSTANCE)
        {
            char inst_name[5];
		    //inst_name=(char*)malloc(5);
		    strncpy(inst_name,node->children[0]->types.identifier,4);
		    inst_name[4]='\0';
		    if(strcmp(inst_name,"DRUM")==0)
			{
			    flag_DRUM=1;
                node_DRUM = node->children[1]->children[1];
			}
        }
        if (node->type==ASSIGN || node->type==BLOCK || flag_DRUM==1)
        {
            int i=0;
            //srand(time(NULL));
            int nbits= rand()%3 + 1;
            char num_bits=(char) nbits; //was num_bits[] = "3";
            for (i=0; i<node->num_children; i++)
            {
                ast_node_t* child_assign;
                if(flag_DRUM==1)
                    child_assign = node_DRUM->children[i];
                else
                    child_assign = node->children[i];

                if (child_assign==NULL){}
                else
                {
                    if (child_assign->type==BLOCKING_STATEMENT || child_assign->type==NON_BLOCKING_STATEMENT
                           || (child_assign->type==MODULE_CONNECT && i < 2)) // only truncate the inputs to DRUM
                    {
                        //srand(time(NULL));
                        int lottery = rand()%2;

                        if (child_assign->children[1]->type==IDENTIFIERS)
                        {
                            truncate_or_not=0;

                            range_high=0;
                            memset(rangetext,0, sizeof(rangetext));
                            //srand(time(NULL));
                            nbits = rand()%3 + 1;
                            truncate_bitwidth(child_assign->children[1], top_node, nbits); //was nbits hardcoded as 3

                            if (truncate_or_not==1)
                            {
                                if (child_assign->children[1]->children[0]->children[1]->type==NUMBERS)
                                {
                                    child_assign->children[1]->children[0]->children[1]->types.number.number= 
                                    											(char*)calloc(sizeof(tempholder), sizeof(char));
                                    strcpy(child_assign->children[1]->children[0]->children[1]->types.number.number, tempholder);
                                }
                                else
                                {
                                    child_assign->children[1]->children[0]->children[1]->children[0]->types.identifier = 
                                    											(char*)calloc(sizeof(tempholder), sizeof(char));
                                    strcpy(child_assign->children[1]->children[0]->children[1]->children[0]->types.identifier, tempholder);
                                }

                            }
                        }

                        if (child_assign->children[1]->type==ARRAY_REF )
                        {
                            truncate_or_not=0;
                            range_high=0;
                            memset(rangetext,0, sizeof(rangetext));
                            nbits = rand()%3 + 1;
                            truncate_bitwidth(child_assign->children[1], top_node, nbits); //nbits hardcoded as 3

                            if (truncate_or_not==1)
                            {
                                if (child_assign->children[1]->children[0]->children[2]->type==NUMBERS)
                                {
                                    child_assign->children[1]->children[0]->children[2]->types.number.number= 
                                    											 (char*)calloc(sizeof(tempholder), sizeof(char));
                                    strcpy(child_assign->children[1]->children[0]->children[2]->types.number.number, tempholder);
                                }

                                else if (child_assign->children[1]->children[0]->children[2]->children[0]->type==NUMBERS)
                                {
                                    child_assign->children[1]->children[0]->children[2]->children[0]->types.number.number = 
                                    											 (char*)calloc(sizeof(tempholder), sizeof(char));
                                    strcpy(child_assign->children[1]->children[0]->children[2]->children[0]->types.number.number, tempholder);

                                }

                                else
                                {
                                    child_assign->children[1]->children[0]->children[2]->children[0]->types.identifier = 
                                    											 (char*)calloc(sizeof(tempholder), sizeof(char));
                                    strcpy(child_assign->children[1]->children[0]->children[2]->children[0]->types.identifier, tempholder);

                                }
                            }
                        }

                        if (child_assign->children[1]->type==MATRIX_REF )
                        {
                            truncate_or_not=0;
                            range_high=0;
                            memset(rangetext,0, sizeof(rangetext));
                            //srand(time(NULL));
                            nbits = rand()%3 + 1;
                            truncate_bitwidth(child_assign->children[1], top_node, nbits); 
                            //nbits hardcoded as 3
                        }
                        if (lottery==0||lottery==1)
                        {
                            nbits = rand()%3 + 1;
                            if (child_assign->children[1]->type==BINARY_OPERATION)
                            {
                                if (child_assign->children[1]->children[0]->types.operation.op==SL 
                                	|| child_assign->children[1]->children[0]->types.operation.op==SR 
                                	|| child_assign->children[1]->children[1]->types.operation.op==SL 
                                	|| child_assign->children[1]->children[1]->types.operation.op==SR){}
                                else
                                    recursive_truncate(child_assign->children[1], top_node, nbits);
                            }
                        }
                        else if (lottery==2)
                        {

                            if (child_assign->children[1]->type==BINARY_OPERATION 
                            	&& (child_assign->children[1]->types.operation.op==ADD 
                            		|| child_assign->children[1]->types.operation.op==MINUS 
                            		|| child_assign->children[1]->types.operation.op==MULTIPLY 
                            		|| child_assign->children[1]->types.operation.op==DIVIDE))
                            {
                                if (child_assign->children[1]->children[0]->types.operation.op==SL 
                                	|| child_assign->children[1]->children[0]->types.operation.op==SR 
                                	|| child_assign->children[1]->children[1]->types.operation.op==SL 
                                	|| child_assign->children[1]->children[1]->types.operation.op==SR){}
                                else
                                {
                                    truncate_flag=0;
                                    recursive_truncate2(child_assign->children[1], top_node, node, num_bits);
                                    if (truncate_flag==1)
                                    {
                                        ast_node_t *num;
                                        ast_node_t *shiftnode;
                                        if (child_assign->children[1]->types.operation.op==MULTIPLY 
                                        	|| child_assign->children[1]->types.operation.op==DIVIDE)
                                        {
                                            char num_bits_double[10]="";
                                            int newnum = atoi(num_bits) * truncate_both_operands;
                                            ;
                                            sprintf(num_bits_double, "%d", newnum);
                                            num=newNumberNode(num_bits_double, node->line_number);
                                        }
                                        else
                                            num=newNumberNode(num_bits, node->line_number);

                                        if (child_assign->children[1]->types.operation.op!=DIVIDE)
                                            shiftnode = newBinaryOperation(SL, child_assign->children[1], num, node->line_number);
                                        else
                                            shiftnode = newBinaryOperation(SR, child_assign->children[1], num, node->line_number);

                                        child_assign->children[1] = shiftnode;
                                        truncate_flag=0;
                                    }
                                }

                            }

                        }
                        //add_child_to_node(node, newBlock);
                    }

                }
            }
        }
    }
    if (node != NULL)
    {
        /* print out the connection with the previous node */
        if (from != NULL){};
        //fprintf(fp, "\t%d -> %d;\n", from_num, my_label);

        for (i = 0; i < node->num_children; i++)
        {
            //truncate_or_not=1;
            change_id_to_range(node->children[i], top_node, node, my_label);
        }
    }
}

void recursive_truncate(ast_node_t *bin_node, ast_node_t *top_node, int nbits)
{

    if (bin_node->children[0]->type==IDENTIFIERS )
    {
        truncate_or_not=0;
        range_high=0;
        memset(rangetext,0, sizeof(rangetext));
        //srand(time(NULL));
        nbits = rand()%3 + 1;
        truncate_bitwidth(bin_node->children[0],top_node,  nbits);

        if (truncate_or_not==1)
        {

            if (bin_node->children[0]->children[0]->children[1]->type==NUMBERS)
            {
                bin_node->children[0]->children[0]->children[1]->types.number.number= 
                										 (char*)calloc(sizeof(tempholder), sizeof(char));
                strcpy(bin_node->children[0]->children[0]->children[1]->types.number.number, tempholder);
            }
            else
            {
                bin_node->children[0]->children[0]->children[1]->children[0]->types.identifier = 
                										(char*)calloc(sizeof(tempholder), sizeof(char));
                strcpy(bin_node->children[0]->children[0]->children[1]->children[0]->types.identifier, tempholder);
            }
        }
    }

    if (bin_node->children[0]->type==ARRAY_REF )
    {
        truncate_or_not=0;
        range_high=0;
        memset(rangetext,0, sizeof(rangetext));
        //srand(time(NULL));
        nbits = rand()%3 + 1;
        truncate_bitwidth(bin_node->children[0], top_node, nbits);
        if (truncate_or_not==1)
        {
            if (bin_node->children[0]->children[0]->children[2]->type==NUMBERS)
            {
                bin_node->children[0]->children[0]->children[2]->types.number.number= 
                										(char*)calloc(sizeof(tempholder), sizeof(char));
                strcpy(bin_node->children[0]->children[0]->children[2]->types.number.number, tempholder);
            }

            else if (bin_node->children[0]->children[0]->children[2]->children[0]->type==NUMBERS)
            {
                bin_node->children[0]->children[0]->children[2]->children[0]->types.number.number = 
                										(char*)calloc(sizeof(tempholder), sizeof(char));
                strcpy(bin_node->children[0]->children[0]->children[2]->children[0]->types.number.number, tempholder);
            }
            else
            {
                bin_node->children[0]->children[0]->children[2]->children[0]->types.identifier = 
                										(char*)calloc(sizeof(tempholder), sizeof(char));
                strcpy(bin_node->children[0]->children[0]->children[2]->children[0]->types.identifier, tempholder);
            }
        }
    }

    if (bin_node->children[0]->type==MATRIX_REF )
    {
        truncate_or_not=0;
        range_high=0;
        memset(rangetext,0, sizeof(rangetext));
        //srand(time(NULL));
        nbits = rand()%3 + 1;
        truncate_bitwidth(bin_node->children[0], top_node, nbits);
    }

    if (bin_node->children[1]->type==IDENTIFIERS)
    {
        truncate_or_not=0;
        range_high=0;
        memset(rangetext,0, sizeof(rangetext));
        //srand(time(NULL));
        nbits = rand()%3 + 1;
        truncate_bitwidth(bin_node->children[1], top_node, nbits);
        if (truncate_or_not==1)
        {
            if (bin_node->children[1]->children[0]->children[1]->type==NUMBERS)
            {
                bin_node->children[1]->children[1]->children[0]->types.number.number= 
                										(char*)calloc(sizeof(tempholder), sizeof(char));
                strcpy(bin_node->children[1]->children[1]->children[0]->types.number.number, tempholder);
            }

            else
            {
                bin_node->children[1]->children[0]->children[1]->children[0]->types.identifier = 
                												(char*)calloc(sizeof(tempholder), sizeof(char));
                strcpy(bin_node->children[1]->children[0]->children[1]->children[0]->types.identifier, tempholder);
            }

            int ctr;
            ast_node_t *concat_zero=newNumberNode("0", bin_node->line_number);

            for (ctr=0; ctr<bin_node->children[1]->children[1]->num_children; ctr++)
                bin_node->children[1]->children[1]->children[ctr]=concat_zero;

        }
    }

    if (bin_node->children[1]->type==ARRAY_REF)
    {

        truncate_or_not=0;
        range_high=0;
        memset(rangetext,0, sizeof(rangetext));
        //srand(time(NULL));
        nbits = rand()%3 + 1;
        truncate_bitwidth(bin_node->children[1], top_node, nbits);
        if (truncate_or_not==1)
        {
            if (bin_node->children[1]->children[0]->children[2]->type==NUMBERS)
            {
                bin_node->children[1]->children[0]->children[2]->types.number.number= 
                										(char*)calloc(sizeof(tempholder), sizeof(char));
                strcpy(bin_node->children[1]->children[0]->children[2]->types.number.number, tempholder);
            }

            else if (bin_node->children[1]->children[0]->children[2]->children[0]->type==NUMBERS)
            {
                bin_node->children[1]->children[0]->children[2]->children[0]->types.number.number = 
                												(char*)calloc(sizeof(tempholder), sizeof(char));
                strcpy(bin_node->children[1]->children[0]->children[2]->children[0]->types.number.number, tempholder);

            }
            else
            {
                bin_node->children[1]->children[0]->children[2]->children[0]->types.identifier = 
                												(char*)calloc(sizeof(tempholder), sizeof(char));
                strcpy(bin_node->children[1]->children[0]->children[2]->children[0]->types.identifier, tempholder);
            }

            int ctr;
            ast_node_t *concat_zero=newNumberNode("0", bin_node->line_number);

            for (ctr=0; ctr<bin_node->children[1]->children[1]->num_children; ctr++)
                bin_node->children[1]->children[1]->children[ctr]=concat_zero;
        }
    }

    if (bin_node->children[1]->type==MATRIX_REF )
    {
        truncate_or_not=0;
        range_high=0;
        memset(rangetext,0, sizeof(rangetext));
        //srand(time(NULL));
        nbits = rand()%3 + 1;
        truncate_bitwidth(bin_node->children[1], top_node, nbits);
    }

    if (bin_node->children[0]->type==BINARY_OPERATION)
    {
        //srand(time(NULL));
        nbits = rand()%3 + 1;
        recursive_truncate(bin_node->children[0], top_node, nbits);
    }

    if (bin_node->children[1]->type==BINARY_OPERATION)
    {
        //srand(time(NULL));
        nbits = rand()%3 + 1;
        recursive_truncate(bin_node->children[1], top_node, nbits);
    }

}

void recursive_truncate2(ast_node_t *bin_node, ast_node_t* top_node, 
						 ast_node_t* node, int num_bits)
{

    if (bin_node->children[0]->type==IDENTIFIERS )
    {
        truncate_both_operands=0;
        truncate_or_not=1;
        range_high=0;
        memset(rangetext,0, sizeof(rangetext));
        truncate_bitwidth_alternative (bin_node->children[0],top_node,  3);

        if (truncate_or_not==1)
        {
            truncate_flag=1;
            truncate_both_operands++;

            if (bin_node->children[0]->children[1]->type==NUMBERS)
            {
                bin_node->children[0]->children[1]->types.number.number= 
                							(char*)calloc(sizeof(tempholder), sizeof(char));
                strcpy(bin_node->children[0]->children[1]->types.number.number, tempholder);
            }

            else
            {
                bin_node->children[0]->children[1]->children[0]->types.identifier = 
                									(char*)calloc(sizeof(tempholder), sizeof(char));
                strcpy(bin_node->children[0]->children[1]->children[0]->types.identifier, tempholder);
            }
        }
    }

    if (bin_node->children[0]->type==ARRAY_REF )
    {
        truncate_both_operands=0;
        truncate_or_not=1;
        range_high=0;
        memset(rangetext,0, sizeof(rangetext));
        truncate_bitwidth_alternative(bin_node->children[0], top_node, 3);
        if (truncate_or_not==1)
        {
            truncate_flag=1;
            truncate_both_operands++;

            if (bin_node->children[0]->children[2]->type==NUMBERS)
            {
                bin_node->children[0]->children[2]->types.number.number= 
                							(char*)calloc(sizeof(tempholder), sizeof(char));
                strcpy(bin_node->children[0]->children[2]->types.number.number, tempholder);
            }

            else
            {
                bin_node->children[0]->children[2]->children[0]->types.identifier = 
                									(char*)calloc(sizeof(tempholder), sizeof(char));
                strcpy(bin_node->children[0]->children[2]->children[0]->types.identifier, tempholder);
            }
        }
    }

    if (bin_node->children[0]->type==MATRIX_REF )
    {
        truncate_both_operands=0;
        truncate_or_not=1;
        range_high=0;
        memset(rangetext,0, sizeof(rangetext));
        truncate_bitwidth_alternative(bin_node->children[0], top_node, 3);
        truncate_flag=1;
        truncate_both_operands++;
    }

    if (bin_node->children[1]->type==IDENTIFIERS)
    {
        truncate_or_not=1;
        range_high=0;
        memset(rangetext,0, sizeof(rangetext));
        truncate_bitwidth_alternative(bin_node->children[1], top_node, 3);
        if (truncate_or_not==1)
        {
            truncate_flag=1;
            truncate_both_operands++;


            if (bin_node->children[1]->children[1]->type==NUMBERS)
            {
                bin_node->children[1]->children[1]->types.number.number= 
                							(char*)calloc(sizeof(tempholder), sizeof(char));
                strcpy(bin_node->children[1]->children[1]->types.number.number, tempholder);
            }

            else
            {
                bin_node->children[1]->children[1]->children[0]->types.identifier = 
                									(char*)calloc(sizeof(tempholder), sizeof(char));
                strcpy(bin_node->children[1]->children[1]->children[0]->types.identifier, tempholder);
            }
        }
    }

    if (bin_node->children[1]->type==ARRAY_REF)
    {
        truncate_or_not=1;
        range_high=0;
        memset(rangetext,0, sizeof(rangetext));
        truncate_bitwidth_alternative(bin_node->children[1], top_node, 3);
        if (truncate_or_not==1)
        {
            truncate_flag=1;
            truncate_both_operands++;


            if (bin_node->children[1]->children[2]->type==NUMBERS)
            {
                bin_node->children[1]->children[2]->types.number.number= 
                							(char*)calloc(sizeof(tempholder), sizeof(char));
                strcpy(bin_node->children[1]->children[2]->types.number.number, tempholder);
            }

            else
            {
                bin_node->children[1]->children[2]->children[0]->types.identifier = 
                									(char*)calloc(sizeof(tempholder), sizeof(char));
                strcpy(bin_node->children[1]->children[2]->children[0]->types.identifier, tempholder);
            }
        }
    }

    if (bin_node->children[1]->type==MATRIX_REF )
    {
        truncate_or_not=0;
        range_high=0;
        memset(rangetext,0, sizeof(rangetext));
        truncate_bitwidth_alternative(bin_node->children[1], top_node, 3);
        truncate_flag=1;
        truncate_both_operands++;
    }

    if (bin_node->children[0]->type==BINARY_OPERATION)
    {
        recursive_truncate2(bin_node->children[0], top_node, node, num_bits);
    }

    if (bin_node->children[1]->type==BINARY_OPERATION)
    {
        recursive_truncate2(bin_node->children[1], top_node, node, num_bits);
    }
}

void create_shift_node(ast_node_t* bin_node, ast_node_t *node, int num_bits)
{
    ast_node_t *num;
    ast_node_t *shiftnode;
    if (bin_node->types.operation.op==MULTIPLY 
    	|| bin_node->types.operation.op==DIVIDE)
    {
        char num_bits_double[10]="";
        int newnum = atoi(num_bits) * 2;
        sprintf(num_bits_double, "%d", newnum);
        num=newNumberNode(num_bits_double, node->line_number);
    }
    else
        num=newNumberNode(num_bits, node->line_number);

    if (bin_node->types.operation.op!=DIVIDE)
        shiftnode = newBinaryOperation(SL, bin_node, num, node->line_number);
    else
        shiftnode = newBinaryOperation(SR, bin_node, num, node->line_number);

    bin_node = shiftnode;
}

void remove_equal_nodes(ast_node_t *top)
{
    remove_node(top, NULL, -1);
}

void add_equal_nodes(ast_node_t *top)
{
    add_node(top, NULL, -1);
}

void change_signs(ast_node_t *node, ast_node_t *from, int from_num)
{
    int i;
    int my_label = unique_label_count;

    /* increase the unique count for other nodes since ours is recorded */
    unique_label_count++;

    if (node == NULL)
    {
        /* print out the node and label details */
    }
    else
    {
        switch(node->type)
        {
            case BINARY_OPERATION:
                {
                    switch (node->types.operation.op)
                    {
                        case MULTIPLY:
                            {
                                node->types.operation.op = node->types.operation.op-16 ;
                                //change to add
                                break;
                            }
                    }
                    break;
                }
        }
    }
    if (node != NULL)
    {
        for (i = 0; i < node->num_children; i++)
        {
            change_signs(node->children[i], node, my_label);
        }
    }
    //oassert(FALSE);
}

void make_id_constant(ast_node_t *node, ast_node_t *from, 
				      int from_num, char **matlab_vars, 
				      int matlab_vars_cnt)
{

    int i;
    int my_label = unique_label_count;

    /* increase the unique count for other nodes since ours is recorded */
    unique_label_count++;

    if (node == NULL)
    {
        /* print out the node and label details */
    }
    else
    {
        if (node->type == BLOCKING_STATEMENT 
        	|| node->type==NON_BLOCKING_STATEMENT)
        {
            int i;
            for (i=0; i<node->num_children; i++)
            {
                ast_node_t* child_node = node->children[i];
                if (child_node==NULL){}
                else
                {
                    if (i==0 && child_node->type==ARRAY_REF)
                    {
                        char id_and_num[4096]="";
                        sprintf(id_and_num, "%s[", child_node->children[0]->types.identifier);
                        if (child_node->children[1]->type==IDENTIFIERS)
                            strcat(id_and_num, child_node->children[1]->types.identifier);
                        else if (child_node->children[1]->type==NUMBERS)
                            strcat(id_and_num, child_node->children[1]->types.number.number);
                        strcat(id_and_num, "]");
                        int j = 0;
                        for (j = 0; j<matlab_vars_cnt; j=j+3)
                        {
                            int comp = strcmp(id_and_num, matlab_vars[j]);
                            if (comp==0)
                                replace_with_const(node->children[i+1], matlab_vars, j);
                        }
                    }

                    else if (i>0 && child_node->type==ARRAY_REF)
                    {
                        char id_and_num[4096]="";
                        sprintf(id_and_num, "%s[", child_node->children[0]->types.identifier);
                        if (child_node->children[1]->type==IDENTIFIERS)
                            strcat(id_and_num, child_node->children[1]->types.identifier);
                        else if (child_node->children[1]->type==NUMBERS)
                            strcat(id_and_num, child_node->children[1]->types.number.number);
                        strcat(id_and_num, "]");
                        int j = 0;
                        int upto = matlab_vars_cnt;
                        for (j = 0; j<matlab_vars_cnt; j++)
                        {
                            int comp = strcmp(id_and_num, matlab_vars[j]);
                            if (comp==0)
                                replace_with_const(child_node, matlab_vars, j);
                        }
                    }
                    else if (i==0 && child_node->type==IDENTIFIERS)
                    {
                        char id_and_num[4096]="";
                        //sprintf(id_and_num, "%s", child_node->types.identifier);
                        if (child_node->type==IDENTIFIERS)
                            strcat(id_and_num, child_node->types.identifier);
                        else if (child_node->type==NUMBERS)
                            strcat(id_and_num, child_node->types.number.number);
                        //strcat(id_and_num, "]");
                        int j = 0;
                        int upto = matlab_vars_cnt;
                        for (j = 0; j<matlab_vars_cnt; j++)
                        {
                            int comp = strcmp(id_and_num, matlab_vars[j]);
                            if (comp==0)
                                replace_with_const(node->children[i+1], matlab_vars, j);
                        }
                    }
                    else if (i>0 && child_node->type==IDENTIFIERS)
                    {
                        char id_and_num[4096]="";
                        //sprintf(id_and_num, "%s", child_node->types.identifier);
                        if (child_node->type==IDENTIFIERS)
                            strcat(id_and_num, child_node->types.identifier);
                        else if (child_node->type==NUMBERS)
                            strcat(id_and_num, child_node->types.number.number);
                        //strcat(id_and_num, "]");
                        int j = 0;
                        int upto = matlab_vars_cnt;
                        for (j = 0; j<matlab_vars_cnt; j++)
                        {
                            int comp = strcmp(id_and_num, matlab_vars[j]);
                            if (comp==0)
                                replace_with_const(child_node, matlab_vars, j);
                        }
                    }

                }
            }
            //default:
            //oassert(FALSE);
        }

        //			if (node->type==BINARY_OPERATION)
        //			{
        //				int k;
        //				for (k=0;k<node->num_children;k++)
        //				{
        //					ast_node_t *child_node   = node->children[k];
        //					if (child_node==NULL){}
        //					else
        //					{
        //					if (child_node->type==ARRAY_REF)
        //						{
        //							char id_and_num[4096]="";
        //							sprintf(id_and_num, "%s[", child_node->children[0]->types.identifier);
        //							if (child_node->children[1]->type==IDENTIFIERS)
        //								strcat(id_and_num, child_node->children[1]->types.identifier);
        //							else if (child_node->children[1]->type==NUMBERS)
        //								strcat(id_and_num, child_node->children[1]->types.number.number);
        //							strcat(id_and_num, "]");
        //							int j = 0;
        //							int upto = matlab_vars_cnt;
        //							for (j = 0; j<matlab_vars_cnt; j++)
        //							{
        //								int comp = strcmp(id_and_num, matlab_vars[j]);
        //								if (comp==0)
        //									replace_with_const(child_node, matlab_vars, j);
        //
        //							}
        //
        //						}
        //						else if (child_node->type==IDENTIFIERS)
        //						{
        //							char id_and_num[4096]="";
        //							//sprintf(id_and_num, "%s", child_node->types.identifier);
        //							if (child_node->type==IDENTIFIERS)
        //								strcat(id_and_num, child_node->types.identifier);
        //							else if (child_node->type==NUMBERS)
        //								strcat(id_and_num, child_node->types.number.number);
        //							//strcat(id_and_num, "]");
        //							int j = 0;
        //							int upto = matlab_vars_cnt;
        //							for (j = 0; j<matlab_vars_cnt; j++)
        //							{
        //								int comp = strcmp(id_and_num, matlab_vars[j]);
        //								if (comp==0)
        //									replace_with_const(child_node, matlab_vars, j);
        //
        //							}
        //
        //						}
        //
        //
        //					}
        //				}
        //			}
    }
    if (node != NULL)
    {
        /* print out the connection with the previous node */
        if (from != NULL){};
        //fprintf(fp, "\t%d -> %d;\n", from_num, my_label);

        for (i = 0; i < node->num_children; i++)
        {
            make_id_constant(node->children[i], node, my_label, 
            				 matlab_vars, matlab_vars_cnt);
        }
    }

    //oassert(FALSE);
}

void replace_with_const(ast_node_t *node, char **matlab_vars, int where)
{
    ast_node_t *newnode = newNumberNode(matlab_vars[where+1], node->line_number);
    newnode->types.number.base = BIN;
    newnode->types.number.binary_size = atoi(matlab_vars[where+2]);

    *node=*newnode;
}

void pre_add (ast_node_t *node, ast_node_t *from, int from_num)
{
    int i;
    int my_label = unique_label_count;

    /* increase the unique count for other nodes since ours is recorded */
    unique_label_count++;
    int flag;
    ast_node_t *tempnode;
    ast_node_t *binarynode1;
    ast_node_t *binarynode2;

    if (node == NULL)
    {
        /* print out the node and label details */
    }
    else
    {
        switch (node->type)
        {
            case BINARY_OPERATION:
                {
                    if (node->types.operation.op==ADD && node->num_children==2)
                    {
                        if ((node->children[0]->type==BINARY_OPERATION 
                        		&& node->children[0]->types.operation.op==MULTIPLY) 
                        	 && (node->children[1]->type==BINARY_OPERATION 
                        	 	&& node->children[1]->types.operation.op==MULTIPLY))
                        {
                            binarynode1 = newNumberNode("3", node->line_number);
                            char *test;
                            test="test";

                            //srand(time(NULL));
                            int idx = rand() % 2;
                            //srand(time(NULL));
                            int idx2 = rand() % 2;

                            node->children[1]->children[0]=node->children[0]->children[idx];

                            node->children[0]=node->children[0]->children[idx2];
                            node->types.operation.op=MULTIPLY;
                            node->children[1]->types.operation.op=ADD;

                            //node = binarynode2;
                            break;
                        }
                    }
                    break;
                }
        }
    }

    if (node != NULL)
    {
        /* print out the connection with the previous node */
        if (from != NULL){};
        //fprintf(fp, "\t%d -> %d;\n", from_num, my_label);

        for (i = 0; i < node->num_children; i++)
        {
            pre_add(node->children[i], node, my_label);
        }
    }
}



//int truncate_cntr=0;

void NodeReplaceFactorizaton(ast_node_t *nodetochange_from, ast_node_t *nodetochange_to)
{
    if (nodetochange_from->type==BLOCKING_STATEMENT 
    	|| nodetochange_from->type==NON_BLOCKING_STATEMENT )
    {
        ast_node_t *node_child_from = nodetochange_from->children[1];
        ast_node_t *zeronum=newNumberNode("0", node_child_from->line_number);
        *node_child_from = *zeronum;

        ast_node_t *node_child_to = nodetochange_to->children[1];
        ast_node_t *temp = nodetochange_to->children[1]->children[1];


        node_child_to->children[1] = node_child_from;
        node_child_to->children[1]->types.operation.op = ADD;
        node_child_to->children[1]->children[0]=temp;
    }
}


void truncate_bitwidth(ast_node_t *node, ast_node_t* top_node, int num_bits)
{
    truncate_cntr++;
    //if (truncate_cntr>2)
    //truncate_cntr=1;
    ast_node_t *tempnode=node;
    char bits[15]="";
    sprintf(bits, "%d", num_bits);

    ast_node_t *num=newNumberNode(bits, node->line_number);

    //srand(time(NULL));
    int lottery = rand() % 30;

    truncate_or_not=1;
    ast_node_t *new_concat = create_node_w_type(CONCATENATE, node->line_number, current_parse_file);

    if (node->type!=MATRIX_REF)
        tempnode = make_ref_range(node, top_node, num_bits);
    else
        tempnode= newMatrixRef(node->children[0]->types.identifier, node->children[1], 
        				       node->children[2], node->children[3], num, node->line_number);

    //ast_node_t *test;
    //test = tempnode->children[1]];

    if (tempnode==NULL || lottery<26)
    {
        //truncate_cntr--;
        truncate_or_not=0;
        return;
    }
    // count number of truncation done
    truncate_done++;
    if (tempnode->type==RANGE_REF)
    {

        if (tempnode->children[1]->type==NUMBERS)
            sprintf(tempholder, "%s", tempnode->children[1]->types.number.number);
        else if (tempnode->children[1]->children[0]->type==IDENTIFIERS)
            sprintf(tempholder, "%s", tempnode->children[1]->children[0]->types.identifier);
    }
    else if (tempnode->type==ARRAY_RANGE_REF)
    {
        if (tempnode->children[2]->type==NUMBERS)
            sprintf(tempholder, "%s", tempnode->children[2]->types.number.number);
        else if (tempnode->children[2]->children[0]->type==IDENTIFIERS)
            sprintf(tempholder, "%s", tempnode->children[2]->children[0]->types.identifier);
    }

    ast_node_t *test = tempnode->children[4];
    ast_node_t *test2 = tempnode->children[4];

    ast_node_t *zeronum=newNumberNode("0", node->line_number);
    ast_node_t* new_listreplicate = newListReplicate(num, zeronum);
    allocate_children_to_node(new_concat, 2, tempnode, new_listreplicate);


    *node = *new_concat;
    //test=node->children[0]->children[1]->children[0];
}

void truncate_bitwidth_alternative(ast_node_t *node, ast_node_t* top_node, int num_bits)
{
    truncate_cntr++;
    //if (truncate_cntr>2)
    //truncate_cntr=1;

    ast_node_t *tempnode;
    char bits[15]="";
    sprintf(bits, "%d", num_bits);

    ast_node_t *num=newNumberNode(bits, node->line_number);

    //srand(time(NULL));
    int lottery = rand() % 30;
    truncate_or_not=1;

    if (node->type!=MATRIX_REF)
        tempnode = make_ref_range(node, top_node, num_bits);
    else
        tempnode= newMatrixRef(node->children[0]->types.identifier, 
        				       node->children[1], 
        				       node->children[2], 
        				       node->children[3], 
        				       num, node->line_number);

    if (tempnode==NULL || lottery <29)
    {
        //truncate_cntr--;
        truncate_or_not=0;
        //		truncate_flag=0;
        return;
    }


    if (tempnode->type==RANGE_REF)
    {

        if (tempnode->children[1]->type==NUMBERS)
            sprintf(tempholder, "%s", tempnode->children[1]->types.number.number);
        else if (tempnode->children[1]->children[0]->type==IDENTIFIERS)
            sprintf(tempholder, "%s", tempnode->children[1]->children[0]->types.identifier);
    }
    else if (tempnode->type==ARRAY_RANGE_REF)
    {

        if (tempnode->children[2]->type==NUMBERS)
            sprintf(tempholder, "%s", tempnode->children[2]->types.number.number);
        else if (tempnode->children[2]->children[0]->type==IDENTIFIERS)
            sprintf(tempholder, "%s", tempnode->children[2]->children[0]->types.identifier);
    }

    ast_node_t *shiftnode = newBinaryOperation(SL, tempnode, num, node->line_number);
    *node = *tempnode;
    //test=node->children[0]->children[1]->children[0];
}

int cnt_call=0;
char *rangetext_clone="";

ast_node_t *make_ref_range(ast_node_t *node, ast_node_t *top_node, int num_bits_truncate)
{

    cnt_call++;
    int highest_bit_int, low_bit_int;
    //memset(rangetext, 0, sizeof(rangetext));
    int loopcnt;
    if (cnt_call==1)
        loopcnt=0;

    highest_bit_int= get_range_var(top_node, node, NULL, -1, loopcnt, rangetext);
    low_bit_int = num_bits_truncate;

    if (highest_bit_int==-1 || (highest_bit_int>0 && highest_bit_int<num_bits_truncate))
        return NULL;

    char highest_bit[15]="";
    char low_bit[15]="";
    sprintf(highest_bit, "%d", highest_bit_int);
    sprintf(low_bit, "%d", low_bit_int);

    rangetext_clone="";
    rangetext_clone= rangetext;
    //sprintf(rangetext,"%s","");

    ast_node_t *tempnode;
    ast_node_t *newnum1;
    ast_node_t *symbolnode1;
    ast_node_t *binarynode1;
    ast_node_t *newnumnode;

    ast_node_t *newONEnode=newNumberNode("1", node->line_number);

    if (highest_bit_int!=0)
        newnum1=newNumberNode(highest_bit, node->line_number);
    else
    {
        symbolnode1=newSymbolNode(rangetext, node->line_number);
        binarynode1 = newBinaryOperation(MINUS, symbolnode1, newONEnode, 
        				                 node->line_number);
    }

    ast_node_t *newnum2=newNumberNode(low_bit, node->line_number);

    if (node->type==IDENTIFIERS)
    {
        if (strlen(rangetext)>0)
            tempnode=newRangeRef(node->types.identifier,binarynode1, 
            				     newnum2, node->line_number);
        else
            tempnode=newRangeRef(node->types.identifier,newnum1, 
            				     newnum2, node->line_number);

    }
    else if(node->type==ARRAY_REF)
    {

        char tempnum[4096]="";
        if (node->children[1]->type==NUMBERS)
        {
            sprintf(tempnum, "%d\0", node->children[1]->types.number.value);
            newnumnode = newNumberNode(tempnum, node->line_number);
        }
        if (node->children[1]->type==BINARY_OPERATION)
        {

            newnumnode = node->children[1];
        }


        if (strlen(rangetext)>0)
            tempnode=newArrayRangeRef(node->children[0]->types.identifier, newnumnode,
            				          binarynode1, newnum2, node->line_number);
        else
            tempnode=newArrayRangeRef(node->children[0]->types.identifier, newnumnode, 
            				          newnum1, newnum2, node->line_number);
    }

    else if(node->type==ARRAY_RANGE_REF)
    {

        char tempnum[4096]="";
        sprintf(tempnum, "%s", node->children[2]->types.number.number);
        newnumnode = newNumberNode(tempnum, node->line_number);

        if (strlen(rangetext)>0)
            tempnode=newArrayRangeRef(node->children[0]->types.identifier, newnumnode,
            				          binarynode1, newnum2, node->line_number);
        else
            tempnode=newArrayRangeRef(node->children[0]->types.identifier, newnumnode, 
            				          newnum1, newnum2, node->line_number);

    }

    return tempnode;
}

int cnt_range_var=0;

int get_range_var(ast_node_t *top, ast_node_t *node, ast_node_t *from, 
				  int from_num, int loop_cnt, char *range)
{

    ast_node_t* previous;

    int i;
    int my_label = unique_label_count;

    /* increase the unique count for other nodes since ours is recorded */
    unique_label_count++;

    if (top == NULL)
    {
        /* print out the node and label details */
    }
    else
    {
        switch (top->type)
        {
            case IDENTIFIERS:
                {
                    if (node->type==ARRAY_REF || node->type==ARRAY_RANGE_REF)
                    {
                        if (strcmp(top->types.identifier, node->children[0]->types.identifier)==0)
                        {

                            if (from->children[loop_cnt+1]->type==NUMBERS)
                            {
                                range_high = atoi(from->children[loop_cnt+1]->types.number.number);

                            }
                            else if (from->children[loop_cnt+1]->type==IDENTIFIERS)
                            {
                                range_high = 0;
                                range = from->children[loop_cnt+1]->types.identifier;
                                cnt_range_var++;

                            }
                            else if (from->children[loop_cnt+1]->type==BINARY_OPERATION)
                            {
                                range_high = 0;
                                range = from->children[loop_cnt+1]->children[0]->types.identifier;
                                sprintf(rangetext, "%s", range);
                                cnt_range_var++;
                            }
                            break;
                        }
                    }

                    else
                    {
                        if (strcmp(top->types.identifier, node->types.identifier)==0)
                        {
                            if (from->children[loop_cnt+1]==NULL)
                            {
                                range_high=-1;
                                break;
                            }

                            if (from->children[loop_cnt+1]->type==NUMBERS)
                            {
                                range_high = atoi(from->children[loop_cnt+1]->types.number.number);

                            }
                            else if (from->children[loop_cnt+1]->type==IDENTIFIERS)
                            {
                                range_high = 0;
                                range = from->children[loop_cnt+1]->types.identifier;
                                cnt_range_var++;

                            }
                            else if (from->children[loop_cnt+1]->type==BINARY_OPERATION)
                            {
                                range_high = 0;
                                range = from->children[loop_cnt+1]->children[0]->types.identifier;
                                sprintf(rangetext, "%s", range);
                                cnt_range_var++;
                            }
                            break;
                        }
                    }
                    break;
                }
        }
    }

    if (top!=NULL)
    {
        /* print out the connection with the previous node */
        if (from != NULL){};
        //fprintf(fp, "\t%d -> %d;\n", from_num, my_label);

        for (i = 0; i < top->num_children; i++)
        {

            if (strlen(rangetext)>0 || (range_high>0) ||range_high==-1)
                return range_high;
            get_range_var(top->children[i],node, top, my_label, i, range);

        }
        return range_high;

    }
    else
        return range_high;
    //oassert(FALSE);
}


void remove_node(ast_node_t *node, ast_node_t *from, int from_num)
{
    int i;
    int my_label = unique_label_count;

    /* increase the unique count for other nodes since ours is recorded */
    unique_label_count++;

    if (node == NULL)
    {
        /* print out the node and label details */
    }
    else
    {
        switch(node->type)
        {
            case ASSIGN:
                {
                    int i=0;
                    for (i=0; i<node->num_children; i++)
                    {
                        ast_node_t* child_assign = node->children[i];
                        if (child_assign==NULL){}
                        else
                        {
                            if (child_assign->type==BLOCKING_STATEMENT 
                            	|| child_assign->type==NON_BLOCKING_STATEMENT)
                            {
                                if (child_assign->children[1]->children[0]->type==IDENTIFIERS 
                                	&& child_assign->children[1]->children[0]->type==IDENTIFIERS)
                                {
                                    if(strcmp(child_assign->children[1]->children[0]->types.identifier,
                                    		  child_assign->children[1]->children[1]->types.identifier) == 0)
                                    {

                                        *(child_assign->children[1])=*child_assign->children[1]->children[0];
                                    }
                                }
                            }
                        }
                    }
                    break;
                }
                //default:
                //oassert(FALSE);
        }
    }
    if (node != NULL)
    {
        /* print out the connection with the previous node */
        if (from != NULL){};
        //fprintf(fp, "\t%d -> %d;\n", from_num, my_label);

        for (i = 0; i < node->num_children; i++)
        {
            remove_node(node->children[i], node, my_label);
        }
    }
}


void add_node(ast_node_t *node, ast_node_t *from, int from_num)
{

    int i;
    int my_label = unique_label_count;

    /* increase the unique count for other nodes since ours is recorded */
    unique_label_count++;

    if (node == NULL)
    {
        /* print out the node and label details */
    }
    else
    {
        switch(node->type)
        {
            case ASSIGN:
                {
                    int i=0;
                    for (i=0; i<node->num_children; i++)
                    {
                        ast_node_t* child_assign = node->children[i];
                        if (child_assign==NULL){}
                        else
                        {
                            if (child_assign->type==BLOCKING_STATEMENT 
                            	|| child_assign->type==NON_BLOCKING_STATEMENT)
                            {
                                if (child_assign->children[1]->children[0]->type==IDENTIFIERS 
                                	&& child_assign->children[1]->children[0]->type==IDENTIFIERS)
                                {
                                    if(strcmp(child_assign->children[1]->children[0]->types.identifier,
                                    		  child_assign->children[1]->children[1]->types.identifier) == 0)
                                    {

                                        ast_node_t *newop = newBinaryOperation(
                                        						child_assign->children[1]->types.operation.op, 
                                        				        child_assign->children[1]->children[0], 
                                        				        child_assign->children[1], node->line_number
                                        				        );
                                        *(child_assign->children[1]->children[0])=*newop;
                                    }
                                }
                            }
                        }
                    }

                    break;
                }
        }
    }
    if (node != NULL)
    {
        /* print out the connection with the previous node */
        if (from != NULL){};
        //fprintf(fp, "\t%d -> %d;\n", from_num, my_label);

        for (i = 0; i < node->num_children; i++)
        {
            add_node(node->children[i], node, my_label);
        }
    }

    //oassert(FALSE);
}


/*---------------------------------------------------------------------------------------------
 * (function: next_parsed_verilog_file)
 *-------------------------------------------------------------------------------------------*/
void next_parsed_verilog_file(ast_node_t *file_items_list)
{
    int i;
    G=getG();
    G++;
    FILE* ast_txt;
    /* optimization entry point */
    printf("Optimizing module by AST based optimizations\n");
    optimizations_on_AST(file_items_list);

    if (configuration.output_ast_graphs == 1)
    {
        /* IF - we want outputs for the graphViz files of each module */
        for (i = 0; i < file_items_list->num_children; i++)
        {
            assert(file_items_list->children[i]);
//#ifndef DEBUG_C
            if (i==0)
                change_id_to_range(file_items_list, file_items_list, NULL, -1);
//#endif
	    	app_mult_cnt=0;
	    	graphVizOutputAst(configuration.debug_output_path, file_items_list->children[i]);
            VerilogOutputAst(configuration.debug_output_path, file_items_list->children[i]);
        }
    }

    printf("Number of possible truncations: %d\n",truncate_cntr);
    printf("Number of truncations that happen: %d\n",truncate_done);

    i = -1;
    /* store the root of this files ast */
    all_file_items_list = (ast_node_t**)realloc(all_file_items_list,
                            sizeof(ast_node_t*)*(size_all_file_items_list + 1));
    all_file_items_list[size_all_file_items_list] = file_items_list;
    size_all_file_items_list ++;
}

/*---------------------------------------------------------------------------------------------
 * (function: newSymbolNode)
 *-------------------------------------------------------------------------------------------*/
ast_node_t *newSymbolNode(char *id, int line_number)
{
    long sc_spot;
    ast_node_t *current_node;

    if (id[0] == '`')
    {
        /* IF - this is a define replace with number constant */
        /* def_reduct */

        /* get the define symbol from the string cache */
        if ((sc_spot = sc_lookup_string(defines_for_file_sc, (id+1))) == -1)
        {
            error_message(PARSE_ERROR, line_number, current_parse_file, "Define \"%s\" used but not declared\n", id);
        }

        /* return the number node */
        return (ast_node_t*)defines_for_file_sc->data[sc_spot];
    }
    else
    {
        /* create node */
        current_node = create_tree_node_id(id, line_number, current_parse_file);

    }

    return current_node;
}

/*---------------------------------------------------------------------------------------------
 * (function: newNumberNode)
 *-------------------------------------------------------------------------------------------*/
ast_node_t *newNumberNode(char *num, int line_number)
{
    ast_node_t *current_node = create_tree_node_number(num, line_number, current_parse_file);
    return current_node;
}

/*---------------------------------------------------------------------------------------------
 * (function: newList)
 *-------------------------------------------------------------------------------------------*/
ast_node_t *newList(ids node_type, ast_node_t *child)
{
    /* create a node for this array reference */
    ast_node_t* new_node = create_node_w_type(node_type, yylineno, current_parse_file);
    /* allocate child nodes to this node */
    allocate_children_to_node(new_node, 1, child);

    return new_node;
}

/*---------------------------------------------------------------------------------------------
 * (function: newList_entry)
 *-------------------------------------------------------------------------------------------*/
ast_node_t *newList_entry(ast_node_t *list, ast_node_t *child)
{
    /* allocate child nodes to this node */
    add_child_to_node(list, child);

    return list;
}

/*---------------------------------------------------------------------------------------------
 * (function: newListReplicate)
 * Basically this functions emulates verilog replication: {5{1'b0}} by concatenating that many
 * children together -- certainly not the most elegant solution, but was the easiest
 *-------------------------------------------------------------------------------------------*/
ast_node_t *newListReplicate(ast_node_t *exp, ast_node_t *child )
{
    /* create a node for this array reference */
    ast_node_t* new_node = create_node_w_type(CONCATENATE, yylineno, current_parse_file);

    new_node->types.concat.num_bit_strings = -1;

    /* allocate child nodes to this node */
    allocate_children_to_node(new_node, 1, child);

    int i;
    for (i = 1; i < exp->types.number.value; i++)
    {
        add_child_to_node(new_node, child);
    }

    return new_node;
}

/*---------------------------------------------------------------------------------------------
 * (function: markAndProcessSymbolListWith)
 *-------------------------------------------------------------------------------------------*/
ast_node_t *markAndProcessSymbolListWith(ids id, ast_node_t *symbol_list)
{
    int i;
    long sc_spot;
    ast_node_t *range_min = 0;
    ast_node_t *range_max = 0;

    for (i = 0; i < symbol_list->num_children; i++)
    {
        /* checks range is legal */
        get_range(symbol_list->children[i]);

        if ((i == 0) && (symbol_list->children[0]->children[1] != NULL)
        			 && (symbol_list->children[0]->children[2] != NULL) 
        			 && (symbol_list->children[0]->children[1]->type == NUMBERS) 
        			 && (symbol_list->children[0]->children[2]->type == NUMBERS))
        {
            range_max = symbol_list->children[0]->children[1];
            range_min = symbol_list->children[0]->children[2];
        }

        if ((symbol_list->children[i]->children[1] == NULL) 
        	&& (symbol_list->children[i]->children[2] == NULL))
        {
            symbol_list->children[i]->children[1] = range_max;
            symbol_list->children[i]->children[2] = range_min;
        }

        switch(id)
        {
            case PORT:
                {
                    short found_match = FALSE;

                    symbol_list->children[i]->types.variable.is_port = TRUE;

                    /* find the related INPUT or OUTPUT definition and store that instead */
                    if ((sc_spot = sc_lookup_string(modules_inputs_sc, 
                    								symbol_list->children[i]->children[0]->types.identifier)) != -1)
                    {
                        symbol_list->children[i]->types.variable.is_input = TRUE;
                        symbol_list->children[i]->children[0] = (ast_node_t*)modules_inputs_sc->data[sc_spot];
                        found_match = TRUE;
                    }
                    if ((found_match == FALSE) 
                    	&& ((sc_spot = sc_lookup_string(modules_outputs_sc, 
                    		           symbol_list->children[i]->children[0]->types.identifier)) != -1))
                    {
                        symbol_list->children[i]->types.variable.is_output = TRUE;
                        symbol_list->children[i]->children[0] = (ast_node_t*)modules_outputs_sc->data[sc_spot];
                        found_match = TRUE;
                    }

                    if (found_match == FALSE)
                    {
                        error_message(PARSE_ERROR, symbol_list->children[i]->line_number,
                        			  current_parse_file, "No matching input declaration for port %s\n",
                        			  symbol_list->children[i]->children[0]->types.identifier);
                    }
                    break;
                }
            case PARAMETER:
                {
                    int binary_range = -1;
                    if (i == 0)
                    {
                        binary_range = get_range(symbol_list->children[i]);
                    }

                    /* fifth spot in the children list holds a parameter value */
                    if (binary_range != -1)
                    {
                        /* check that the parameter size matches the number included */
                        if((symbol_list->children[i]->children[5]->types.number.size != 0)
                                && (symbol_list->children[i]->children[5]->types.number.base == BIN 
                                	|| symbol_list->children[i]->children[5]->types.number.base == SBIN)
                                && (symbol_list->children[i]->children[5]->types.number.size != binary_range))
                        {
                            error_message(PARSE_ERROR, symbol_list->children[i]->children[5]->line_number, 
                            				current_parse_file, "parameter %s and range %d don't match\n", 
                            				symbol_list->children[i]->children[0]->types.identifier, binary_range);
                        }
                        else
                        {
                            symbol_list->children[i]->children[5]->types.number.size = binary_range; // assign the binary range
                        }
                    }

                    /* create an entry in the symbol table for this parameter */
                    if ((sc_spot = sc_add_string(defines_for_module_sc[num_modules], 
                    							symbol_list->children[i]->children[0]->types.identifier)) == -1)
                    {
                        error_message(PARSE_ERROR, symbol_list->children[i]->children[5]->line_number, 
                        				current_parse_file, 
                        				"define has same name (%s).  Other define migh be in another file.  Odin considers a define as global.\n",
                                		symbol_list->children[i]->children[0]->types.identifier,
                                		((ast_node_t*)(defines_for_module_sc[num_modules]->data[sc_spot]))->line_number);
                    }

                    defines_for_module_sc[num_modules]->data[sc_spot] = (void*)symbol_list->children[i]->children[5];
                    /* mark the node as shared so we don't delete it */
                    symbol_list->children[i]->children[5]->shared_node = TRUE;
                    /* now do the mark */
                    symbol_list->children[i]->types.variable.is_parameter = TRUE;
                    break;
                }
            case INPUT:
                symbol_list->children[i]->types.variable.is_input = TRUE;
                /* add this input to the modules string cache */
                if ((sc_spot = sc_add_string(modules_inputs_sc, 
                					symbol_list->children[i]->children[0]->types.identifier)) == -1)
                {
                    error_message(PARSE_ERROR, symbol_list->children[i]->children[0]->line_number, 
                    			  current_parse_file, "Module already has input with this name %s\n", 
                    			  symbol_list->children[i]->children[0]->types.identifier);
                }
                /* store the data which is an idx here */
                modules_inputs_sc->data[sc_spot] = (void*)symbol_list->children[i];
                break;
            case INPUT_SIGNED:
                symbol_list->children[i]->types.variable.is_input_signed = TRUE;
                /* add this input to the modules string cache */
                if ((sc_spot = sc_add_string(modules_inputs_sc, 
                					symbol_list->children[i]->children[0]->types.identifier)) == -1)
                {
                    error_message(PARSE_ERROR, symbol_list->children[i]->children[0]->line_number,
                    				current_parse_file, "Module already has input with this name %s\n",
                    				symbol_list->children[i]->children[0]->types.identifier);
                }
                /* store the data which is an idx here */
                modules_inputs_sc->data[sc_spot] = (void*)symbol_list->children[i];
                break;

            case OUTPUT:
                symbol_list->children[i]->types.variable.is_output = TRUE;
                /* add this output to the modules string cache */
                if ((sc_spot = sc_add_string(modules_outputs_sc, 
                					symbol_list->children[i]->children[0]->types.identifier)) == -1)
                {
                    error_message(PARSE_ERROR, symbol_list->children[i]->children[0]->line_number,
                    				current_parse_file, "Module already has input with this name %s\n", 
                    				symbol_list->children[i]->children[0]->types.identifier);
                }
                /* store the data which is an idx here */
                modules_outputs_sc->data[sc_spot] = (void*)symbol_list->children[i];
                break;
            case OUTPUT_REG:
                symbol_list->children[i]->types.variable.is_output_reg = TRUE;
                /* add this output to the modules string cache */
                if ((sc_spot = sc_add_string(modules_outputs_sc, 
                					symbol_list->children[i]->children[0]->types.identifier)) == -1)
                {
                    error_message(PARSE_ERROR, symbol_list->children[i]->children[0]->line_number,
                    				current_parse_file, "Module already has input with this name %s\n",
                    				symbol_list->children[i]->children[0]->types.identifier);
                }
                /* store the data which is an idx here */
                modules_outputs_sc->data[sc_spot] = (void*)symbol_list->children[i];
                break;
            case INOUT:
                symbol_list->children[i]->types.variable.is_inout = TRUE;
                error_message(PARSE_ERROR, symbol_list->children[i]->children[0]->line_number,
                				current_parse_file, "Odin does not handle inouts (%s)\n", 
                				symbol_list->children[i]->children[0]->types.identifier);
                break;
            case WIRE:
                symbol_list->children[i]->types.variable.is_wire = TRUE;
                break;
            case REG:
                symbol_list->children[i]->types.variable.is_reg = TRUE;
                break;
            case INTEGER:
                symbol_list->children[i]->types.variable.is_integer = TRUE; 
                break;
            case GENVAR:
                symbol_list->children[i]->types.variable.is_genvar = TRUE;
                break;
            case REG_SIGNED:
                symbol_list->children[i]->types.variable.is_reg_signed = TRUE;
                break;
            case WIRE_SIGNED:
                symbol_list->children[i]->types.variable.is_wire_signed = TRUE;
                break;
            default:
                oassert(FALSE);
        }
    }

    return symbol_list;
}
/*---------------------------------------------------------------------------------------------
 * (function: newArrayRef)
 *-------------------------------------------------------------------------------------------*/
ast_node_t *newArrayRef(char *id, ast_node_t *expression, int line_number)
{
    /* allocate or check if there's a node for this */
    ast_node_t *symbol_node = newSymbolNode(id, line_number);
    /* create a node for this array reference */
    ast_node_t* new_node = create_node_w_type(ARRAY_REF, line_number, current_parse_file);
    /* allocate child nodes to this node */
    allocate_children_to_node(new_node, 2, symbol_node, expression);

    return new_node;
}

/*---------------------------------------------------------------------------------------------
 * (function: newArrayArrayRef) -- added by Kumud
 *-------------------------------------------------------------------------------------------*/

ast_node_t *newArrayArrayRef(char *id, ast_node_t *expression1, ast_node_t *expression2, int line_number)
{
    /* allocate or check if there's a node for this */
    ast_node_t *symbol_node = newSymbolNode(id, line_number);
    /* create a node for this array reference */
    ast_node_t* new_node = create_node_w_type(ARRAY_ARRAY_REF, line_number, current_parse_file);
    /* allocate child nodes to this node */
    allocate_children_to_node(new_node, 3, symbol_node, expression1, expression2);
    /* swap the direction so in form [MSB:LSB] */
    //get_range(new_node);

    return new_node;
}

/*---------------------------------------------------------------------------------------------
 * (function: newRangeRef)
 *-------------------------------------------------------------------------------------------*/
ast_node_t *newRangeRef(char *id, ast_node_t *expression1, ast_node_t *expression2, int line_number)
{
    /* allocate or check if there's a node for this */
    ast_node_t *symbol_node = newSymbolNode(id, line_number);
    /* create a node for this array reference */
    ast_node_t* new_node = create_node_w_type(RANGE_REF, line_number, current_parse_file);
    /* allocate child nodes to this node */
    allocate_children_to_node(new_node, 3, symbol_node, expression1, expression2);
    /* swap the direction so in form [MSB:LSB] */
    get_range(new_node);

    return new_node;
}



/*---------------------------------------------------------------------------------------------
 * (function: newArrayRangeRef) -- added by Kumud
 *-------------------------------------------------------------------------------------------*/

ast_node_t *newArrayRangeRef(char *id, ast_node_t *expression1, 
				ast_node_t *expression2, ast_node_t *expression3, int line_number)
{
    /* allocate or check if there's a node for this */
    ast_node_t *symbol_node = newSymbolNode(id, line_number);
    /* create a node for this array reference */
    ast_node_t* new_node = create_node_w_type(ARRAY_RANGE_REF, line_number, current_parse_file);
    /* allocate child nodes to this node */
    allocate_children_to_node(new_node, 4, symbol_node, expression1, expression2, expression3);
    /* swap the direction so in form [MSB:LSB] */
    //get_range(new_node);

    return new_node;
}

/*---------------------------------------------------------------------------------------------
 * (function: newMatrixRef) -- added by Kumud
 *-------------------------------------------------------------------------------------------*/
ast_node_t *newMatrixRef(char *id, ast_node_t *expression1, 
				ast_node_t *expression2, 
				ast_node_t *expression3, 
				ast_node_t *expression4, int line_number)
{
    /* allocate or check if there's a node for this */
    ast_node_t *symbol_node = newSymbolNode(id, line_number);
    /* create a node for this array reference */
    ast_node_t* new_node = create_node_w_type(MATRIX_REF, line_number, current_parse_file);
    /* allocate child nodes to this node */
    allocate_children_to_node(new_node, 5, symbol_node, expression1, expression2, expression3, expression4);
    /* swap the direction so in form [MSB:LSB] */ //dont think this is necessary (Kumud)
    //get_range(new_node);

    return new_node;
}

/*---------------------------------------------------------------------------------------------
 * (function: newBinaryOperation)
 *-------------------------------------------------------------------------------------------*/
ast_node_t *newBinaryOperation(operation_list op_id, ast_node_t *expression1, 
				ast_node_t *expression2, int line_number)
{
    info_ast_visit_t *node_details = NULL;
    /* create a node for this array reference */
    ast_node_t* new_node = create_node_w_type(BINARY_OPERATION, line_number, current_parse_file);
    /* store the operation type */
    new_node->types.operation.op = op_id;
    /* allocate child nodes to this node */
    allocate_children_to_node(new_node, 2, expression1, expression2);

    /* see if this binary expression can have some constant folding */
    node_details = constantFold(new_node);
    if ((node_details != NULL) && (node_details->is_constant_folded == TRUE))
    {
        new_node = node_details->from;
        free(node_details);
    }

    return new_node;
}

/*---------------------------------------------------------------------------------------------
 * (function: newUnaryOperation)
 *-------------------------------------------------------------------------------------------*/
ast_node_t *newUnaryOperation(operation_list op_id, ast_node_t *expression, int line_number)
{
    info_ast_visit_t *node_details = NULL;
    /* create a node for this array reference */
    ast_node_t* new_node = create_node_w_type(UNARY_OPERATION, line_number, current_parse_file);
    /* store the operation type */
    new_node->types.operation.op = op_id;
    /* allocate child nodes to this node */
    allocate_children_to_node(new_node, 1, expression);

    /* see if this binary expression can have some constant folding */
    node_details = constantFold(new_node);
    if ((node_details != NULL) && (node_details->is_constant_folded == TRUE))
    {
        new_node = node_details->from;
        free(node_details);
    }

    return new_node;
}

/*---------------------------------------------------------------------------------------------
 * (function: newNegedgeSymbol)
 *-------------------------------------------------------------------------------------------*/
ast_node_t *newNegedgeSymbol(char *symbol, int line_number)
{
    /* get the symbol node */
    ast_node_t *symbol_node = newSymbolNode(symbol, line_number);
    /* create a node for this array reference */
    ast_node_t* new_node = create_node_w_type(NEGEDGE, line_number, current_parse_file);
    /* allocate child nodes to this node */
    allocate_children_to_node(new_node, 1, symbol_node);

    return new_node;
}

/*---------------------------------------------------------------------------------------------
 * (function: newPosedgeSymbol)
 *-------------------------------------------------------------------------------------------*/
ast_node_t *newPosedgeSymbol(char *symbol, int line_number)
{
    /* get the symbol node */
    ast_node_t *symbol_node = newSymbolNode(symbol, line_number);
    /* create a node for this array reference */
    ast_node_t* new_node = create_node_w_type(POSEDGE, line_number, current_parse_file);
    /* allocate child nodes to this node */
    allocate_children_to_node(new_node, 1, symbol_node);

    return new_node;
}

/*---------------------------------------------------------------------------------------------
 * (function: newCaseItem)
 *-------------------------------------------------------------------------------------------*/
ast_node_t *newCaseItem(ast_node_t *expression, ast_node_t *statement, int line_number)
{
    /* create a node for this array reference */
    ast_node_t* new_node = create_node_w_type(CASE_ITEM, line_number, current_parse_file);
    /* allocate child nodes to this node */
    allocate_children_to_node(new_node, 2, expression, statement);

    return new_node;
}

/*---------------------------------------------------------------------------------------------
 * (function: newDefaultCase)
 *-------------------------------------------------------------------------------------------*/
ast_node_t *newDefaultCase(ast_node_t *statement, int line_number)
{
    /* create a node for this array reference */
    ast_node_t* new_node = create_node_w_type(CASE_DEFAULT, line_number, current_parse_file);
    /* allocate child nodes to this node */
    allocate_children_to_node(new_node, 1, statement);

    return new_node;
}

/*---------------------------------------------------------------------------------------------
 * (function: newNonBlocking)
 *-------------------------------------------------------------------------------------------*/
ast_node_t *newNonBlocking(char * sign, ast_node_t *expression1, ast_node_t *expression2, int line_number)
{
    /* create a node for this array reference */
    ast_node_t* new_node;

    if (strlen(sign)>1)
    {
        strcat(signed_unsigned, "$signed");
        new_node = create_node_w_type(NON_BLOCKING_STATEMENT_SIGNED, line_number, current_parse_file);

    }
    else
        new_node = create_node_w_type(NON_BLOCKING_STATEMENT, line_number, current_parse_file);
    /* allocate child nodes to this node */
    allocate_children_to_node(new_node, 2, expression1, expression2);


    return new_node;
}

/*---------------------------------------------------------------------------------------------
 * (function: newBlocking)
 *-------------------------------------------------------------------------------------------*/
ast_node_t *newBlocking(ast_node_t *expression1, ast_node_t *expression2, int line_number)
{
    /* create a node for this array reference */
    ast_node_t* new_node = create_node_w_type(BLOCKING_STATEMENT, line_number, current_parse_file);
    /* allocate child nodes to this node */
    allocate_children_to_node(new_node, 2, expression1, expression2);

    return new_node;
}

/*---------------------------------------------------------------------------------------------
 * (function: newFor) added by Kumud
 *-------------------------------------------------------------------------------------------*/
ast_node_t *newFor(ast_node_t *init_statement, ast_node_t *compare_expression, 
				ast_node_t *step_increment, ast_node_t *inside_statement, int line_number)
{
    /* create a node for this array reference */
    ast_node_t* new_node = create_node_w_type(FOR, line_number, current_parse_file);
    /* allocate child nodes to this node */
    allocate_children_to_node(new_node, 4, init_statement, 
    				compare_expression, step_increment, inside_statement);

    return new_node;
}

/*---------------------------------------------------------------------------------------------
 * (function: newIf)
 *-------------------------------------------------------------------------------------------*/
ast_node_t *newIf(ast_node_t *compare_expression, ast_node_t *true_expression, 
				ast_node_t *false_expression, int line_number)
{
    /* create a node for this array reference */
    ast_node_t* new_node = create_node_w_type(IF, line_number, current_parse_file);
    /* allocate child nodes to this node */
    allocate_children_to_node(new_node, 3, compare_expression, true_expression, false_expression);

    return new_node;
}

/*---------------------------------------------------------------------------------------------
 * (function: newGenerate) //added by Kumud
 *-------------------------------------------------------------------------------------------*/

ast_node_t *newGenerate(ast_node_t *generate_statement, int line_number)
{
    /* create a node for this array reference */
    ast_node_t* new_node = create_node_w_type(GENERATE, line_number, current_parse_file);
    /* allocate child nodes to this node */
    allocate_children_to_node(new_node, 1, generate_statement);

    return new_node;
}


/*---------------------------------------------------------------------------------------------
 * (function: newDefine) //added by Kumud
 *-------------------------------------------------------------------------------------------*/

ast_node_t *newDefine(char *define_id, char *define_num, int line_number)
{
    ast_node_t *symbol_node = newSymbolNode(define_id, line_number);
    ast_node_t *number_node = newNumberNode(define_num, line_number);
    /* create a node for this array reference */
    ast_node_t* new_node = create_node_w_type(DEFINE, line_number, current_parse_file);
    /* allocate child nodes to this node */
    allocate_children_to_node(new_node, 2, symbol_node, number_node);

    return new_node;
}


/*---------------------------------------------------------------------------------------------
 * (function: newIfQuestion) for f = a ? b : c;
 *-------------------------------------------------------------------------------------------*/
ast_node_t *newIfQuestion(ast_node_t *compare_expression, 
				ast_node_t *true_expression, ast_node_t *false_expression, int line_number)
{
    /* create a node for this array reference */
    ast_node_t* new_node = create_node_w_type(IF_Q, line_number, current_parse_file);
    /* allocate child nodes to this node */
    allocate_children_to_node(new_node, 3, compare_expression, true_expression, false_expression);

    return new_node;
}
/*---------------------------------------------------------------------------------------------
 * (function: newCase)
 *-------------------------------------------------------------------------------------------*/
ast_node_t *newCase(ast_node_t *compare_expression, ast_node_t *case_list, int line_number)
{
    /* create a node for this array reference */
    ast_node_t* new_node = create_node_w_type(CASE, line_number, current_parse_file);
    /* allocate child nodes to this node */
    allocate_children_to_node(new_node, 2, compare_expression, case_list);

    return new_node;
}

/*---------------------------------------------------------------------------------------------
 * (function: newAlways)
 *-------------------------------------------------------------------------------------------*/
ast_node_t *newAlways(ast_node_t *delay_control, ast_node_t *statement, int line_number)
{
    /* create a node for this array reference */
    ast_node_t* new_node = create_node_w_type(ALWAYS, line_number, current_parse_file);
    /* allocate child nodes to this node */
    allocate_children_to_node(new_node, 2, delay_control, statement);

    return new_node;
}

/*---------------------------------------------------------------------------------------------
 * (function: newModuleConnection)
 *-------------------------------------------------------------------------------------------*/
ast_node_t *newModuleConnection(char* id, ast_node_t *expression, int line_number)
{
    ast_node_t *symbol_node;
    /* create a node for this array reference */
    ast_node_t* new_node = create_node_w_type(MODULE_CONNECT, line_number, current_parse_file);
    if (id != NULL)
    {
        symbol_node = newSymbolNode(id, line_number);
    }
    else
    {
        symbol_node = NULL;
    }

    /* allocate child nodes to this node */
    allocate_children_to_node(new_node, 2, symbol_node, expression);

    return new_node;
}

/*---------------------------------------------------------------------------------------------
 * (function: newModuleParameter)
 *-------------------------------------------------------------------------------------------*/
ast_node_t *newModuleParameter(char* id, ast_node_t *expression, int line_number)
{
    ast_node_t *symbol_node;
    /* create a node for this array reference */
    ast_node_t* new_node = create_node_w_type(MODULE_PARAMETER, line_number, current_parse_file);
    if (id != NULL)
    {
        error_message(PARSE_ERROR, line_number, current_parse_file, "Specifying parameters by name not currently supported!\n");
        symbol_node = newSymbolNode(id, line_number);
    }
    else
    {
        symbol_node = NULL;
    }

    if (expression->type != NUMBERS)
    {
        error_message(PARSE_ERROR, line_number, current_parse_file, "Parameter value must be of type NUMBERS!\n");
    }

    /* allocate child nodes to this node */
    // leave 4 blank nodes so that expression is the 6th node to behave just like
    // a default var_declare parameter (see create_symbol_table_for_module)
    allocate_children_to_node(new_node, 6, symbol_node, NULL, NULL, NULL, NULL, expression);

    // set is_parameter for the same reason
    new_node->types.variable.is_parameter = TRUE;

    return new_node;
}

/*---------------------------------------------------------------------------------------------
 * (function: newModuleNamedInstance)
 *-------------------------------------------------------------------------------------------*/
ast_node_t *newModuleNamedInstance(char* unique_name, ast_node_t *module_connect_list, 
				ast_node_t *module_parameter_list, int line_number)
{
    ast_node_t *symbol_node = newSymbolNode(unique_name, line_number);

    /* create a node for this array reference */
    ast_node_t* new_node = create_node_w_type(MODULE_NAMED_INSTANCE, 
    				line_number, current_parse_file);
    /* allocate child nodes to this node */
    allocate_children_to_node(new_node, 3, symbol_node, module_connect_list, 
    				module_parameter_list);

    return new_node;
}

/*-------------------------------------------------------------------------
 * (function: newHardBlockInstance)
 *-----------------------------------------------------------------------*/
ast_node_t *newHardBlockInstance(char* module_ref_name, 
				ast_node_t *module_named_instance, int line_number)
{
    ast_node_t *symbol_node = newSymbolNode(module_ref_name, line_number);

    // create a node for this array reference
    ast_node_t* new_node = create_node_w_type(HARD_BLOCK, line_number, current_parse_file);
    // allocate child nodes to this node
    allocate_children_to_node(new_node, 2, symbol_node, module_named_instance);

    // store the hard block symbol name that this calls in 
    // a list that will at the end be asociated with the hard block node
    block_instantiations_instance = (ast_node_t **)realloc(block_instantiations_instance, 
    				sizeof(ast_node_t*)*(size_block_instantiations+1));
    block_instantiations_instance[size_block_instantiations] = new_node;
    size_block_instantiations++;

    return new_node;
}

/*-------------------------------------------------------------------------
 * (function: newModuleInstance)
 *-----------------------------------------------------------------------*/
ast_node_t *newModuleInstance(char* module_ref_name, ast_node_t *module_named_instance, int line_number)
{
#ifdef VPR6
    /*   if
         (
         sc_lookup_string(hard_block_names, module_ref_name) != -1
         || !strcmp(module_ref_name, "single_port_ram")
         || !strcmp(module_ref_name, "dual_port_ram")
         )
         {
         return newHardBlockInstance(module_ref_name, module_named_instance, line_number);
         }*/
#endif

    // make a unique module name based on its parameter list
    ast_node_t *module_param_list = module_named_instance->children[2];
    char *module_param_name = make_module_param_name(module_param_list, module_ref_name);
    ast_node_t *symbol_node = newSymbolNode(module_param_name, line_number);

    // if this is a parameterised instantiation
    if (module_param_list)
    {
        // which doesn't exist in ast_modules yet
        long sc_spot;
        if ((sc_spot = sc_lookup_string(module_names_to_idx, module_param_name)) == -1)
        {
            // then add it, but set it to the symbol_node, because the
            // module in question may not have been parsed yet
            // later, we convert this symbol node back into a module node
            ast_modules = (ast_node_t **)realloc(ast_modules, sizeof(ast_node_t*)*(num_modules+1));
            ast_modules[num_modules] = symbol_node;
            num_modules++;
            sc_spot = sc_add_string(module_names_to_idx, module_param_name);
            module_names_to_idx->data[sc_spot] = symbol_node;
            defines_for_module_sc = (STRING_CACHE**)realloc(defines_for_module_sc, 
            				sizeof(STRING_CACHE*)*(num_modules+1));
            defines_for_module_sc[num_modules] = NULL;
        }
    }

    /* create a node for this array reference */
    ast_node_t* new_node = create_node_w_type(MODULE_INSTANCE, line_number, current_parse_file);
    /* allocate child nodes to this node */
    allocate_children_to_node(new_node, 2, symbol_node, module_named_instance);

    /* store the module symbol name that this calls in a list that 
     * will at the end be asociated with the module node */
    module_instantiations_instance = (ast_node_t **)realloc(module_instantiations_instance, 
    				sizeof(ast_node_t*)*(size_module_instantiations+1));
    module_instantiations_instance[size_module_instantiations] = new_node;
    size_module_instantiations++;

    return new_node;
}

/*---------------------------------------------------------------------------------------------
 * (function: newGateInstance)
 *-------------------------------------------------------------------------------------------*/
ast_node_t *newGateInstance(char* gate_instance_name, ast_node_t *expression1, 
				ast_node_t *expression2, ast_node_t *expression3, int line_number)
{
    /* create a node for this array reference */
    ast_node_t* new_node = create_node_w_type(GATE_INSTANCE, line_number, current_parse_file);
    ast_node_t *symbol_node = NULL;

    if (gate_instance_name != NULL)
    {
        symbol_node = newSymbolNode(gate_instance_name, line_number);
    }

    /* allocate child nodes to this node */
    allocate_children_to_node(new_node, 4, symbol_node, expression1, expression2, expression3);

    return new_node;
}

/*---------------------------------------------------------------------------------------------
 * (function: newGate)
 *-------------------------------------------------------------------------------------------*/
ast_node_t *newGate(operation_list op_id, ast_node_t *gate_instance, int line_number)
{
    /* create a node for this array reference */
    ast_node_t* new_node = create_node_w_type(GATE, line_number, current_parse_file);
    /* store the operation type */
    new_node->types.operation.op = op_id;
    /* allocate child nodes to this node */
    allocate_children_to_node(new_node, 1, gate_instance);

    return new_node;
}

/*---------------------------------------------------------------------------------------------
 * (function: newAssign)
 *-------------------------------------------------------------------------------------------*/
ast_node_t *newAssign(ast_node_t *statement, int line_number)
{
    /* create a node for this array reference */
    ast_node_t* new_node = create_node_w_type(ASSIGN, line_number, current_parse_file);
    /* allocate child nodes to this node */
    allocate_children_to_node(new_node, 1, statement);

    return new_node;
}

/*---------------------------------------------------------------------------------------------
 * (function: newVarDeclare)
 *-------------------------------------------------------------------------------------------*/
ast_node_t *newVarDeclare(char* symbol, ast_node_t *expression1, 
				ast_node_t *expression2, ast_node_t *expression3, 
				ast_node_t *expression4, ast_node_t *value, int line_number)
{
    ast_node_t *symbol_node = newSymbolNode(symbol, line_number);

    /* create a node for this array reference */
    ast_node_t* new_node = create_node_w_type(VAR_DECLARE, line_number, current_parse_file);

    /* allocate child nodes to this node */
    allocate_children_to_node(new_node, 6, symbol_node, expression1, expression2, expression3, expression4, value);

    return new_node;
}

ast_node_t *newDoubleArray(char* symbol, ast_node_t *expression1, 
				ast_node_t *expression2, ast_node_t *expression3, 
				ast_node_t *expression4, ast_node_t *expression5, 
				ast_node_t *expression6, ast_node_t *value, int line_number)
{
    ast_node_t *symbol_node = newSymbolNode(symbol, line_number);

    /* create a node for this array reference */
    ast_node_t* new_node = create_node_w_type(DOUBLE_ARRAY_DECLARE, 
    				line_number, current_parse_file);

    /* allocate child nodes to this node */
    allocate_children_to_node(new_node, 8, symbol_node, expression1, 
    				expression2, expression3, expression4, expression5, 
    				expression6, value);

    return new_node;
}

/*-----------------------------------------
 * ----------------------------------------------------
 * (function: newModule)
 *-------------------------------------------------------------------------------------------*/
ast_node_t *newModule(char* module_name, ast_node_t *list_of_ports, 
				ast_node_t *list_of_module_items, int line_number)
{
    long sc_spot;
    ast_node_t *symbol_node = newSymbolNode(module_name, line_number);

    /* create a node for this array reference */
    ast_node_t* new_node = create_node_w_type(MODULE, line_number, current_parse_file);
    /* mark all the ports symbols as ports */
    markAndProcessSymbolListWith(PORT, list_of_ports);
    /* allocate child nodes to this node */
    allocate_children_to_node(new_node, 3, symbol_node, list_of_ports, list_of_module_items);

    /* store the list of modules this module instantiates */
    new_node->types.module.module_instantiations_instance = module_instantiations_instance;
    new_node->types.module.size_module_instantiations = size_module_instantiations;
    new_node->types.module.is_instantiated = FALSE;
    new_node->types.module.index = num_modules;

    /* record this module in the list of modules (for evaluation later in terms of just nodes) */
    ast_modules = (ast_node_t **)realloc(ast_modules, sizeof(ast_node_t*)*(num_modules+1));
    ast_modules[num_modules] = new_node;

    if ((sc_spot = sc_add_string(module_names_to_idx, module_name)) == -1)
    {
        error_message(PARSE_ERROR, line_number, current_parse_file, 
        				"module names with the same name -> %s\n", module_name);
    }
    /* store the data which is an idx here */
    module_names_to_idx->data[sc_spot] = (void*)new_node;

    /* now that we've bottom up built the parse tree for this module, go to the next module */
    next_module();

    TREE_ROOT=new_node;
    MODULE_ITEMS_ROOT=new_node->children[2];

    return new_node;
}

/*---------------------------------------------------------------------------------------------
 * (function: next_module)
 *-------------------------------------------------------------------------------------------*/
void next_module()
{
    num_modules ++;

    /* define the string cache for the next module */
    defines_for_module_sc = (STRING_CACHE**)realloc(defines_for_module_sc, 
    				sizeof(STRING_CACHE*)*(num_modules+1));
    defines_for_module_sc[num_modules] = sc_new_string_cache();

    /* create a new list for the instantiations list */
    module_instantiations_instance = NULL;
    size_module_instantiations = 0;

    /* old ones are done so clean */
    sc_free_string_cache(modules_inputs_sc);
    sc_free_string_cache(modules_outputs_sc);
    /* make for next module */
    modules_inputs_sc = sc_new_string_cache();
    modules_outputs_sc = sc_new_string_cache();
}

/*--------------------------------------------------------------------------
 * (function: newDefparam)
 *------------------------------------------------------------------------*/
void newDefparam(char *inst, char *param, char *val, int line_number)
{
    ast_node_t *sym_node = newSymbolNode(val, line_number);
    sym_node->shared_node = TRUE;
}

/*--------------------------------------------------------------------------
 * (function: newConstant)
 *------------------------------------------------------------------------*/
void newConstant(char *id, char *number, int line_number)
{
    long sc_spot;
    ast_node_t *number_node = newNumberNode(number, line_number);

    /* add the define character string to the parser and maintain the number around */
    /* def_reduct */
    if ((sc_spot = sc_add_string(defines_for_file_sc, id)) == -1)
    {
        error_message(PARSE_ERROR, current_parse_file, line_number, 
        				"define with same name (%s) on line %d\n", id, 
        				((ast_node_t*)(defines_for_file_sc->data[sc_spot]))->line_number);
    }
    /* store the data */
    defines_for_file_sc->data[sc_spot] = (void*)number_node;
    /* mark node as shared */
    number_node->shared_node = TRUE;
}

/* --------------------------------------------------------------------------------------------
   --------------------------------------------------------------------------------------------
   --------------------------------------------------------------------------------------------
   AST VIEWING FUNCTIONS
   --------------------------------------------------------------------------------------------
   --------------------------------------------------------------------------------------------
   --------------------------------------------------------------------------------------------*/
int unique_label_count;

ast_node_t *startnode, *nodetochange[2];
ast_node_t *module_items; 
ast_node_t *var_declare_list;
int module_items_num, var_declare_list_num;
#define ROUND 2
#ifdef DEBUG_C
#define MUL2ADD_PRCTG 0
#define ADD2OR_PRCTG 1
#define BOOSTER 0
#define BOOSTER_SWAP 0
#else
#define MUL2ADD_PRCTG 0.1
#define ADD2OR_PRCTG 0.08
#define BOOSTER 4
#define BOOSTER_SWAP 5
#endif

#define SWAP 1
#define INSERT 2
#define FACTORIZE 3
#define MUTATE 4
#define DELETE 5

int side, repitition, probability, nodedepth;
int *probm, *probm_index;
//int prob[20], prob_index[20];
int num_mults = 0, count_mult;
int *proba, *proba_index;
int num_adds = 0, count_add;
int num_ids = 0;
record_id *id_lib = NULL, *new_id, *lookup_id;
int num_paras;
record_id *para_lib = NULL, *new_para, *lookup_para;
int num_always = 0, count_always = 1;

ast_node_t *top_global;


/*---------------------------------------------------------------------------
 * (function: graphVizOutputAst)
 *-------------------------------------------------------------------------*/
void graphVizOutputAst(char* path, ast_node_t *top)
{
    char path_and_file[4096];
    FILE *fp;
    static int file_num = 0;
    /* open the file */
    if( top->children[0]->types.identifier != NULL)
        sprintf(path_and_file, "%s/%s_ast.dot", path, top->children[0]->types.identifier);
    else
        sprintf(path_and_file, "%s/tmp_ast.dot", path);
    fp = fopen(path_and_file, "w");
    file_num++;

    /* open graph */
    fprintf(fp, "digraph G {\t\nranksep=.25;\n");

    AstOperation(top);

    unique_label_count = 0;
    graphVizOutputAst_traverse_node(fp, top, NULL, -1);

    /* close graph */
    fprintf(fp, "}\n");
    fclose(fp);
}


/*-----------Coded by LYT------------*/
void AstOperation(ast_node_t *top)
{
    num_ids = 0;
    num_mults = 0;
    num_paras = 0;
    for (repitition = 0; repitition < ROUND; repitition++ )
    {
        //srand(time(NULL));
        int i;

        if (repitition != 0)
        {
            for (i=0; i<BOOSTER; i++)
            {
                nodetochange[0] = NULL;
                nodetochange[1] = NULL;
                startnode = NULL;
                AstOperation_traverse(top, FACTORIZE);
                if (startnode != NULL)
                {
                    if (count_always == 1)
                    {
                        AstOperation_factorize();
                        count_always = 0;
                    }
                }
            }
        }

        if (repitition == 0)
        {
            for (i=0; i<BOOSTER_SWAP; i++)
            {
                nodetochange[0] = NULL;
                nodetochange[1] = NULL;
                startnode = NULL;
                AstOperation_traverse(top, SWAP);
                if (startnode != NULL)
                {
                    nodedepth = 0;
                    AstOperation_swap();
                }
            }
        }

        /*********** Generate an array to implement random changing of multipliers **********/
        if (repitition != 0)
        {
            int i,j;
            int num_change;
            num_change = (int)(num_mults*MUL2ADD_PRCTG + 0.5);	// Round the number of multipliers need to change
            probm = (int *)calloc(num_mults, sizeof(int));
            for (i=0; i<num_mults; i++)
            {
                probm[i] = 0;
            }
#ifndef DEBUG_C
            probm_index = (int *)calloc(num_change, sizeof(int));
            for (i=0; i<num_change; i++)
            {
                //srand(time(NULL));
                probm_index[i] = rand()%num_mults;
                probm[probm_index[i]] = 1;
                for (j=0; j<i; j++)
                {
                    if (probm_index[i] == probm_index[j])
                    {
                        i--;
                        break;
                    }
                }
            }
            free(probm_index);
#endif
            printf("**********\nlyt message\ntotal MULT#:\t%d\npotential change MULT#:\t%d\n",
            				num_mults,num_change);	
            for(i=0; i<num_mults; i++)
                printf("%d\t",probm[i]);
            free(probm);
            printf("\n**********\n");
        }


        /*********** Generate an array to implement random changing of adders **********/
        if (repitition != 0)
        {
            int i,j;
            int num_change;
            // Round the number of multipliers need to change
            num_change = (int)(num_adds*ADD2OR_PRCTG + 0.5);
            proba = (int *)calloc(num_adds, sizeof(int));
            for (i=0; i<num_adds; i++)
            {
                proba[i] = 0;
            }

#ifndef DEBUG_C
            proba_index = (int *)calloc(num_change, sizeof(int));
            for (i=0; i<num_change; i++)
            {
                //srand(time(NULL));
                proba_index[i] = rand()%num_adds;
                proba[proba_index[i]] = 1;
                for (j=0; j<i; j++)
                {
                    if (proba_index[i] == proba_index[j])
                    {
                        i--;
                        break;
                    }
                }
            }
            free(proba_index);
#endif

            printf("**********\ntotal ADDER#:\t%d\nchange ADDER#:\t%d\n",num_adds,num_change);
            for(i=0; i<num_adds; i++)	printf("%d\t",proba[i]);
            printf("\n**********\n");
            printf("Power Estimation Info:\n");

        }

        count_mult = 0;
        count_add = 0;
        AstOperation_Mutate(top);
        printf("********* # of always: %d **********\n", num_always);

/*        if (global_args.power_estimation == 1 && repitition == 0 && num_paras > 0){
            printf("Replacing parameters with their values");
            Ast_param_substition(top,para_lib);
            // replace all parameter with number
        }
*/
#ifndef DEBUG_C
        if (repitition != 0)
        {
        // replace all binary operation whose operants are constants
        // with constant
        //  optimizations_on_AST(top);
        /* randomly delete some nodes */
            AstOperation_traverse(top, DELETE);
            nodedepth=0;;
            if (startnode!=NULL && startnode->num_children>=2)
                AstOperation_delete(startnode->children[1]);
        }
#endif
    }
}

void AstOperation_Mutate(ast_node_t *node)
{
    int i,j, index;

    if (node != NULL)
    {
        switch(node->type)
        {
            case ALWAYS:
                {
                    if (repitition == 0)
                    {
                        num_always++;
                    }
                    break;
                }

            case BINARY_OPERATION:
                {
                    switch(node->types.operation.op)
                    {
                        case MULTIPLY:
                            {
                                if (repitition == 0)
                                {
                                    num_mults++;
                                }
                                else
                                {
                                    count_mult++;
                                    //srand(time(NULL));
                                    index = rand()%2;
                                    int flag_CP=0;

                                    if(global_args.critical_path_file != NULL)
                                    {
                                        char CP_id[150][150];
                                        char buffer[150];
                                        int i=0;
                                        FILE *file_in;

                                        file_in=fopen(global_args.critical_path_file, "r");

                                        if (file_in==NULL){
                                            printf("Something wrong with critical path file to read for addition. Check the path\n");
                                            exit(0);
                                        }
                                        /*stores and prints the data from the string*/
                                        while (fgets(buffer,150,file_in)) {
                                            strcpy(CP_id[i],buffer);
                                            i++;
                                        }
                                        fclose(file_in);


                                        if(node->children[0]->type==IDENTIFIERS) {
                                            int filectr;
                                            for (filectr=0; filectr<150; filectr++) {

                                                CP_id[filectr][strlen(CP_id[filectr])-1]=0;
                                                if (strcmp(CP_id[filectr],node->children[0]->types.identifier) == 0)
                                                {
                                                    printf ("Critical path component found!\n");
                                                    flag_CP=1;
                                                    break;
                                                }
                                            }
                                        }


                                        if(node->children[1]->type==IDENTIFIERS) {
                                            int filectr;
                                            for (filectr=0; filectr<150; filectr++) {

                                                CP_id[filectr][strlen(CP_id[filectr])-1]=0;
                                                if (strcmp(CP_id[filectr],node->children[1]->types.identifier) == 0)
                                                {
                                                    printf ("Critical path component found!\n");
                                                    flag_CP=1;
                                                    break;
                                                }
                                            }
                                        }
                                    }

                                    if (probm[count_mult-1]==0 && flag_CP==1)	
                                    {
                                        //srand(time(NULL));
                                        int high_prob=rand()%5;
                                        if (high_prob==0 || high_prob==1 || high_prob==2 || high_prob==3 || high_prob==4) {
				                    	    changeNode(node);				
				                    	    flag_CP=0;
                                            break;
                                        }

                                    }
                                    else if (probm[count_mult-1] == 1)
                                    {
                                        switch (index)
                                        {
                                            case 0:
                                                {
                                                    changeNode(node);
                                                    break;
                                                }
                                            default:
                                                break;
                                        }
                                    }
                                }
                                break;
                            }

                        case ADD:
                            { // first round, just counting # adders
                                if (repitition == 0)
                                {
                                    num_adds++;
                                }
                                else
                                {
                                    count_add++;
                                    index = rand()%2; 

                                    char CP_id[150][150];
                                    char buffer[150];
                                    int i=0;
                                    FILE *file_in;
                                    int flag_CP=0;
                                    if(global_args.critical_path_file != NULL)
                                    {
                                        file_in=fopen(global_args.critical_path_file, "r");

                                        if (file_in==NULL){
                                            printf("Something wrong with critical path file to read for multiply. Check directory path.\n");
                                            exit(0);
                                        }
                                        /*stores and prints the data from the string*/
                                        while (fgets(buffer,150,file_in)) {
                                            strcpy(CP_id[i],buffer);
                                            i++;
                                        }
                                        fclose(file_in);

                                        if(node->children[0]->type==IDENTIFIERS) {
                                            int filectr;
                                            for (filectr=0; filectr<150; filectr++) {

                                                CP_id[filectr][strlen(CP_id[filectr])-1]=0;
                                                if (strcmp(CP_id[filectr],node->children[0]->types.identifier) == 0)
                                                {
                                                    printf ("Critical path component found!\n");
                                                    flag_CP=1;
                                                    break;
                                                }

                                            }
                                        }


                                        if(node->children[1]->type==IDENTIFIERS) {
                                            int filectr;
                                            for (filectr=0; filectr<150; filectr++) {
                                                CP_id[filectr][strlen(CP_id[filectr])-1]=0;
                                                if (strcmp(CP_id[filectr],node->children[1]->types.identifier) == 0)
                                                {
                                                    printf ("Critical path component found!\n");
                                                    flag_CP=1;
                                                    break;
                                                }
                                            }
                                        }
                                    }

                                    if (proba[count_add-1]==0 && flag_CP==1)
                                    {
                                        //srand(time(NULL));
                                        int high_prob=rand()%5;
                                        if (high_prob==0 || high_prob==1 || high_prob==2 || high_prob==3 || high_prob==4) {
                                            // print_input_BW(top_global, node);
                                            node->types.operation.op = BITWISE_OR;
                                            flag_CP=0;
                                            break;
                                        }
                                    }

                                    else if (proba[count_add-1] ==1)
                                    {
                                        if (flag_CP==1)
                                            node->types.operation.op = BITWISE_OR;
                                        switch (index)
                                        {
                                            case 0:
                                                {
                                                    //printf("%s\n", node->types.identifier);
                                                    //       print_input_BW(top_global, node);
                                                    node->types.operation.op = BITWISE_OR;
                                                    break;
                                                }
                                                //		case 1:
                                                //		{
                                                //			node->types.operation.op = BITWISE_AND;
                                                //			break;
                                                //		}
                                                //		case 2:
                                                //		{
                                                //			node->types.operation.op = BITWISE_NAND;
                                                //		break;
                                                //		}
                                                //		case 3:
                                                //		{
                                                //			node->types.operation.op = BITWISE_NOR;
                                                //			break;
                                                //		}
                                            default:
                                                break;
                                        }
                                    }
                                }
                                break;
                            }
                        default:
                            break;
                    }
                    break;
                }

                //		case UNARY_OPERATION:
                //		{
                //			probability = rand()%20 ;
                //			index = rand()%4;
                //			if (probability == 1)
                //			{
                //				switch (index)
                //				{
                //				case 0:
                //				{
                //					node->types.operation.op = BITWISE_NOT;
                //					break;
                //				}
                //				case 1:
                //				{
                //					node->types.operation.op = LOGICAL_NOT;
                //					break;
                //				}
                //				case 2:
                //				{
                //					node->types.operation.op = ADD;
                //					break;
                //				}
                //				case 3:
                //				{
                //					node->types.operation.op = MINUS;
                //					break;
                //				}
                //				default:
                //					break;
                //				}
                //			}
                //			break;
                //		}

                //		case IDENTIFIERS:
                //		{
                //			int j, check;
                //			char *id1, *id2;
                //
                //			if (repitition == 0)	//First time traversing the tree, memorize all the identifiers.
                //			{
                //				if (num_ids == 0)
                //					{
                //						new_id = (record_id*)calloc(1, sizeof(record_id));
                //						new_id->content = (ast_node_t*)calloc(1, sizeof(ast_node_t));
                //						*(new_id->content) = *node;
                //						new_id->next = NULL;
                //						id_lib = new_id;
                //						num_ids++;
                //					}
                //					else
                //					{
                //						lookup_id = id_lib;
                //						check = 0;
                //						for (j=0; j<num_ids; j++)
                //						{
                //							id1 = node->types.identifier;
                //							id2 = lookup_id->content->types.identifier;
                //							if ( !strcmp(id1, id2) )
                //							{
                //								check = 1;
                //								break;
                //							}
                //							lookup_id = lookup_id->next;
                //						}
                //						if (check == 0)
                //						{
                //							new_id->next = (record_id*)calloc(1, sizeof(record_id));
                //							new_id->next->content = (ast_node_t*)calloc(1, sizeof(ast_node_t));
                //							*(new_id->next->content) = *node;
                //							new_id = new_id->next;
                //							num_ids++;
                //						}
                //					}
                //			}
                //			else
                //			{
                //				if (id_lib != NULL)
                //				{
                //					lookup_id = id_lib;
                //					for (j=0; j<num_ids; j++)
                //					{
                //						probability = rand() % (30 + num_ids);
                //						if (probability == 1)
                //						{
                //							*node = *(lookup_id->content);
                //							break;
                //						}
                //						else	lookup_id = lookup_id->next;
                //					}
                //				}
                //			}
                //			break;
                //		}

            case BLOCK:
                {
                    if(repitition == 0)
                        for (i=0; i<node->num_children; i++)
                        {
                            if (node->children[i]->type == FOR)
                            {
                                AstOperation_unroll(node, i);
                                //break;
                            }
                        }
                    break;
                }
            case MODULE_ITEMS:
                {
                    // module_items points to MODULE_ITEMS so that we can delete its
                    // children that are parameter
                    module_items = node;
                    break;
                }
            case VAR_DECLARE_LIST:
                {
                    int k;
                    var_declare_list = node;
                    for(j=0;j<var_declare_list->num_children; j++){
                        if(var_declare_list->children[j] == NULL){
                            for(k=j; k < var_declare_list->num_children - 1; k++){
                                var_declare_list->children[k] = var_declare_list->children[k+1];
                            }
                            var_declare_list->num_children--;
                            var_declare_list->children[var_declare_list->num_children] = NULL;
                        }
                    }
                    break;
                }
            case VAR_DECLARE:
                {
                    int check;
                    char *id1, *id2;
                    ast_node_t *is_target_parameter = node->children[0];

                    //First time traversing the tree, memorize all the identifiers.
                    if ((repitition == 0) && (node->types.variable.is_parameter)){
                        if (num_paras == 0)
                        {
                            new_para = (record_id*)calloc(1, sizeof(record_id));
                            new_para->content = (ast_node_t*)calloc(1, sizeof(ast_node_t));
                            *(new_para->content) = *is_target_parameter;
                            // TODO: check the behavior here
                            if (node->num_children >= 6)
                                new_para->value = node->children[5]->types.number.value;
                            else
                                new_para->value = node->children[1]->types.number.value;
                            new_para->next = NULL;
                            para_lib = new_para;
                            num_paras = 1;
                        }
                        else
                        {
                            lookup_para = para_lib;
                            check = 0;
                            for (j=0; j<num_paras; j++)
                            {
                                id1 = is_target_parameter->types.identifier;
                                id2 = lookup_para->content->types.identifier;
                                if ( !strcmp(id1, id2) )
                                {
                                    check = 1;
                                    break;
                                }
                                lookup_para = lookup_para->next;
                            }
                            if (check == 0)
                            {
                                new_para->next = (record_id*)calloc(1, sizeof(record_id));
                                new_para->next->content = (ast_node_t*)calloc(1, sizeof(ast_node_t));
                                *(new_para->next->content) = *is_target_parameter;
                                //new_para->next->value = node->children[5]->types.number.value;
                                if (node->num_children >= 6)
                                    new_para->next->value = node->children[5]->types.number.value;
                                else
                                    new_para->next->value = node->children[1]->types.number.value;
                                new_para = new_para->next;
                                num_paras++;
                            }
                        }

                        // Delete the node from MODULE_ITEMS because we will substitute all its value:
                        //free_child_in_tree(module_items, module_items_num);
                        if(global_args.power_estimation == 1){
                            ast_node_t* change_node;
                            int num;
                            if(var_declare_list->num_children > 1){
                                change_node = var_declare_list;
                                num = var_declare_list_num;
                            }else{
                                change_node = module_items;
                                num = module_items_num;
                            }

                            for(j=num; j < change_node->num_children - 1; j++){
                                change_node->children[j] = change_node->children[j+1];
                            }
                            change_node->num_children--;
                            change_node->children[change_node->num_children] = NULL;
                            AstOperation_Mutate(change_node->children[num]);
                        }
                    }
                    break;
                }
            default:
                break;
        }
        for (i = 0; i < node->num_children; i++)
        {
            if(node->type == MODULE_ITEMS){
                module_items_num = i;
            }else if(node->type == VAR_DECLARE_LIST){
                var_declare_list_num = i;
            }
            AstOperation_Mutate(node->children[i]);
        }
    }
}

void AstOperation_unroll(ast_node_t *forparent, int forindex)
{
    char *mainvar, *limit_id=NULL;
    int start_value=0, limit_value=0, operand=0, rounds=0;
    operation_list conditionop, op;
    ast_node_t *node;
    int i, k, total;

    node = forparent->children[forindex];
    total = forparent->num_children;

    mainvar = node->children[0]->children[0]->types.identifier;
    conditionop = node->children[1]->types.operation.op;
    switch(conditionop)
    {
        case LT:
            {
                start_value = node->children[0]->children[1]->types.number.value;
                //			printf("*******start value is: %d\n",start_value);
                switch(node->children[1]->children[1]->type)
                {
                    case IDENTIFIERS:
                        {
                            limit_id = node->children[1]->children[1]->types.identifier;
                            //					printf("*******parameter is: %s\n",limit_id);
                            limit_value = AstOperation_find_paravalue(limit_id, para_lib);
                            //					printf("*******value is: %d\n",limit_value);
                            break;
                        }
                    case NUMBERS:
                        {
                            limit_value = node->children[1]->children[1]->types.number.value;
                            break;
                        }
                    default:
                        break;
                }
                break;
            }
        case GT:
            {
                start_value = node->children[1]->children[1]->types.number.value;
                switch(node->children[0]->children[1]->type)
                {
                    case IDENTIFIERS:
                        {
                            limit_id = node->children[0]->children[1]->types.identifier;
                            //					printf("*******parameter is: %s\n",limit_id);
                            limit_value = AstOperation_find_paravalue(limit_id, para_lib);
                            //					printf("*******value is: %d\n",limit_value);
                            break;
                        }
                    case NUMBERS:
                        {
                            limit_value = (int)node->children[0]->children[1]->types.number.value;
                            break;
                        }
                    default:
                        break;
                }
                break;
            }
        default:
            break;
    }

    op = node->children[2]->children[1]->types.operation.op;
    operand = (int)node->children[2]->children[1]->children[1]->types.number.value;

    if (operand >0) {for (i=start_value; i<limit_value; i=i+operand)	rounds++;}

    ast_node_t *tempnode;
    tempnode = (ast_node_t*)calloc(1,sizeof(ast_node_t));
    *tempnode = *(node->children[3]->children[0]);

    forparent->num_children = total + rounds - 1;
    forparent->children = (ast_node_t**)realloc(forparent->children, 
    						sizeof(ast_node_t*)*(forparent->num_children));
    for (i=forparent->num_children-1; i>forindex+rounds-1 ; i--)
    {
        forparent->children[i] = (ast_node_t*)calloc(1, sizeof(ast_node_t));
        *(forparent->children[i]) = *(forparent->children[--total]);
    }
    for (i=forindex, k=start_value; i<rounds+forindex; i++, k=k+operand)
    {
        forparent->children[i] = AstOperation_copytree(tempnode);
        AstOperation_unroll_replace(forparent->children[i], mainvar, k);
    }
    free (tempnode);
}

int AstOperation_find_paravalue(char *target_parameter, record_id *para_lib)
{
    lookup_para = para_lib;

    while ((lookup_para != NULL) && 
    		(*(lookup_para->content->types.identifier) != *target_parameter))
    {
        lookup_para = lookup_para->next;
    }

    return (int)lookup_para->value;
}

void AstOperation_unroll_replace(ast_node_t *node, char *mainvar, int value)
{
    int i;

    if (node != NULL)
    {
        if (node->type == IDENTIFIERS)
        {
            int check = 1;
            check = strcmp(node->types.identifier, mainvar);
            if (check == 0)
            {
                int number_size = 0;
                char temp[32];
                char *floating_pointer = temp;
                sprintf(temp, "%d\0", value);
                if (*floating_pointer != '\0')
                {
                    floating_pointer++;
                    number_size ++;
                }
                node->types.number.number = (char*)calloc(number_size + 1, sizeof(char));
                strncpy(node->types.number.number, temp, number_size + 1);
                node->type = NUMBERS;
                node->types.number.base = DEC;
                node->types.number.size = number_size + 1;
                node->types.number.value = (long long)value;// Fail to write
            }
        }
        for (i = 0, nodedepth++; i < node->num_children; i++)
        {
            AstOperation_unroll_replace(node->children[i], mainvar, value);
        }
    }
}

void AstOperation_swap()
{
    int x[2];

    if (startnode->children[1]->num_children > 1)
    {
        //srand(time(NULL));
        x[1] = rand()%startnode->children[1]->num_children;
        do
        {
            //srand(time(NULL));
            x[2] = rand()%startnode->children[1]->num_children;
        }while(x[1] ==  x[2]);

        /* find the subtree we want to manipulate */
        ast_node_t *tempnode;
        nodedepth = 0;
        side = 0;
        AstOperation_subtree(startnode->children[1]->children[x[1]], side, SWAP);
        nodedepth = 0;
        side = 1;
        AstOperation_subtree(startnode->children[1]->children[x[2]], side, SWAP);

        if (nodetochange[0] != NULL && nodetochange[1] != NULL)
        {
            printf("********** SWAP check **********\n");
            /* exchange the two subtrees from left and right sides */
            tempnode = (ast_node_t*)calloc(1, sizeof(ast_node_t));	/*****Important usage!*****/
            *tempnode = *nodetochange[0];
            *nodetochange[0]= *nodetochange[1];
            *nodetochange[1] = *tempnode;
            free(tempnode);
        }
    }
}

void AstOperation_insert()
{
    int x[2];

    if (startnode->children[1]->num_children > 1)
    {
        //srand(time(NULL));
        x[1] = rand()%startnode->children[1]->num_children;
        do
        {
            //srand(time(NULL));
            x[2] = rand()%startnode->children[1]->num_children;
        }while(x[1] ==  x[2]);

        /* find the subtree we want to manipulate */
        ast_node_t *tempnode;
        nodedepth = 0;
        side = 0;
        AstOperation_subtree(startnode->children[1]->children[x[1]], side, INSERT);
        nodedepth = 0;
        side = 1;
        AstOperation_subtree(startnode->children[1]->children[x[2]], side, INSERT);

        if (nodetochange[0] != NULL && nodetochange[1] != NULL)
        {
            tempnode = (ast_node_t*)calloc(1, sizeof(ast_node_t));
            *tempnode = *nodetochange[0];
            *nodetochange[0] = *nodetochange[1];
            if (nodetochange[0]->num_children != 0)
            {
                nodetochange[0]->children= &tempnode;
                nodetochange[0]->children[1] = AstOperation_copytree(nodetochange[1]->children[0]);
            }
        }
    }
}

void AstOperation_factorize()
{
    /* find the subtree we want to manipulate */
    ast_node_t *tempnode;
    nodedepth = 0;
    side = 0;
    AstOperation_subtree(startnode->children[1], side, FACTORIZE);
    nodedepth = 0;
    side = 1;
    AstOperation_subtree(startnode->children[1], side, FACTORIZE);

    printf("********** FACTORIZE [ ");
    printf("total: %d\t", num_mults);

    if ((nodetochange[0]!=NULL) && (nodetochange[1]!=NULL))
    {
        if (nodetochange[0]->unique_count != nodetochange[1]->unique_count)
        {
            printf("Congrats! You got a hit!\t");
            NodeReplaceFactorization(nodetochange[0], nodetochange[1]); // Call functions to do factorization.
            num_mults--;
            printf("No.1: %s\tNo.2: %s", nodetochange[0]->children[0]->children[0]->types.identifier, nodetochange[1]->children[0]->children[0]->types.identifier);
        }
        else
            printf("Oops! Same multiplier...\t");
    }
    printf(" ] FACTORIZE **********\n");
}

// Function for factorization.
void NodeReplaceFactorization(ast_node_t *nodetochange_from, ast_node_t *nodetochange_to)
{
    if ((nodetochange_from!=NULL) && (nodetochange_to!=NULL))
    {
        int c[2];
        //srand(time(NULL));
        c[0] = rand()%2;
        //srand(time(NULL));
        c[1] = rand()%2;
        ast_node_t *node_child_from = nodetochange_from->children[1];
        ast_node_t *zeronum=newNumberNode("0", node_child_from->line_number);

        ast_node_t *node_child_to = nodetochange_to->children[1];
        ast_node_t *temp = nodetochange_to->children[1]->children[c[0]];

        node_child_to->children[c[0]] = node_child_from;
        node_child_to->children[c[0]]->types.operation.op = ADD;
        node_child_to->children[c[0]]->children[c[1]]=temp;

        nodetochange_from->children[1]= zeronum;
    }
}

ast_node_t *AstOperation_copytree(ast_node_t *orgnode)
{
    int i;
    ast_node_t *copynode;

    if (orgnode != NULL)
    {
        copynode = (ast_node_t*)calloc(1, sizeof(ast_node_t));
        *copynode = *orgnode;
        // New memory space for the children!!!!!!
        copynode->children = (ast_node_t**)calloc(orgnode->num_children, sizeof(ast_node_t*));
        for (i = 0; i <orgnode->num_children; i++)
        {
            copynode->children[i] = AstOperation_copytree(orgnode->children[i]);
        }
    }

    return copynode;
}

void AstOperation_traverse(ast_node_t *node, int conditionid)
{
    int i, condition=0;

    if (node != NULL)
    {
        //		probability = AstOperation_prob(nodedepth);
        switch (conditionid)
        {
            case SWAP:
            case INSERT:
                {
                    condition = (node->type == ALWAYS);
                    //srand(time(NULL));
                    probability = rand()%2;
                    break;
                }
            case FACTORIZE:
                {
                    condition = (node->type == ALWAYS);
                    if(num_always==0)
                    		probability = 0;
		            else
			        {
                        //srand(time(NULL));
                        probability = rand()%num_always;
                    }
					// Indicates that found one always block to change.
                    if (probability == 1)	count_always = 1;	
                    break;
                }
            case MUTATE:
                {
                    condition = (node->type == BLOCK);
                    probability = 1;
                    break;
                }
            case DELETE:
                {
                    condition = (node->type == ALWAYS);
                    if(num_always==0)
			            probability = 0;
		            else
                    {
                        //srand(time(NULL));
			            probability = rand()%num_always;
                    }
                    break;
                }
            default:
                break;
        }
        if (condition && (probability == 1))
        {
            //			if (count_always != 7)
            //				count_always++;
            //			else
            //			{
            //				count_always = 1;
            startnode = node;/*If find the first binary operation that we want, memorize this node*/
            //			}
        }
        else
        {
            for (i = 0; i < node->num_children; i++)
            {
                AstOperation_traverse(node->children[i], conditionid);
            }
        }
    }
}

void AstOperation_subtree(ast_node_t *node, int side, int conditionid)
{
    int i, condition=0;

    if (node != NULL)
    {
        //		probability = AstOperation_prob(nodedepth);
        switch (conditionid)
        {
            case SWAP:
                {
                    condition = ((node->type == BINARY_OPERATION)
                    				||(node->type == MATRIX_REF)
                    				||(node->type == CONCATENATE));
                    //srand(time(NULL));
                    probability = rand()%10; //((10>nodedepth)?(10-nodedepth):1);
                    break;
                }
            case INSERT:
                {
                    condition = ((node->type == BINARY_OPERATION));
                    //srand(time(NULL));
                    probability = rand()%10; //((10>nodedepth)?(10-nodedepth):1);
                    break;
                }
            case FACTORIZE:
                {
                    condition = ((node->type == NON_BLOCKING_STATEMENT) 
                    			&& (node->children[1]->type == BINARY_OPERATION) 
                    			&& (node->children[1]->types.operation.op == MULTIPLY));
                    //srand(time(NULL));
                    probability = (num_mults==0 ? 0 : rand()%num_mults);
                    break;
                }
            default:
                break;
        }
        if (condition && (probability == 1))
        {
            nodetochange[side] = node;
        }
        else
        {
            for (i = 0, nodedepth++; i < node->num_children; i++)
            {
                AstOperation_subtree(node->children[i], side, conditionid);
            }
        }
    }
}

void AstOperation_delete(ast_node_t *node)
{
    int i;
    ast_node_t *tempnode;

    if (node != NULL)
    {
        //srand(time(NULL));
        probability = rand()%((20>nodedepth)?(20-nodedepth):3);
        //		probability = AstOperation_prob(nodedepth);
        if ( (node->type==BINARY_OPERATION)
                && ( (node->children[0]->type==IDENTIFIERS)
                     ||(node->children[0]->type==CONCATENATE)
                     ||(node->children[0]->type==MATRIX_REF) )
                && (probability == 3) )
        {
            switch(node->types.operation.op)
            {
                case MULTIPLY:
                    {
                        num_mults--;
                        break;
                    }
                case ADD:
                    {
                        num_adds--;
                        break;
                    }
            }
            if (node->num_children != 0)
            {
                tempnode = (ast_node_t*)calloc(1, sizeof(ast_node_t));
                *tempnode = *(node->children[1]);
                printf("********** DELETE: check **********\n");
                free(node->children);	//The space need to be freed.
                *node = *tempnode;
                free(tempnode);
            }
            else
            {
                node = NULL;
            }
        }
        else
        {
            for (i = 0, nodedepth++; i < node->num_children; i++)
            {
                AstOperation_delete(node->children[i]);
            }
            nodedepth--;
        }
    }
}
/*---------End of Codes by lyt-------*/


/*---------------------------------------------------------------------------------------------
 * (function: graphVizOutputAst_traverse_node)
 *-------------------------------------------------------------------------------------------*/
void graphVizOutputAst_traverse_node(FILE *fp, ast_node_t *node, ast_node_t *from, int from_num)
{
    int i;
    int my_label = unique_label_count;

    /* increase the unique count for other nodes since ours is recorded */
    unique_label_count++;

    if (node == NULL)
    {
        /* print out the node and label details */
    }
    else
    {
        switch(node->type)
        {
            case FILE_ITEMS:
                fprintf(fp, "\t%d [label=\"FILE_ITEMS\"];\n", my_label);
                break;
            case MODULE:
                fprintf(fp, "\t%d [label=\"MODULE\"];\n", my_label);
                break;
            case MODULE_ITEMS:
                fprintf(fp, "\t%d [label=\"MODULE_ITEMS\"];\n", my_label);
                break;
            case VAR_DECLARE:
                {
                    char temp[4096] = "";
                    if(node->types.variable.is_input)
                    {
                        sprintf(temp, "%s INPUT", temp);
                    }
                    if(node->types.variable.is_input_signed)
                    {
                        sprintf(temp, "%s INPUT SIGNED", temp);
                    }
                    if(node->types.variable.is_output)
                    {
                        sprintf(temp, "%s OUTPUT", temp);
                    }
                    if(node->types.variable.is_output_reg)
                    {
                        sprintf(temp, "%s OUTPUT REG", temp);
                    }
                    if(node->types.variable.is_inout)
                    {
                        sprintf(temp, "%s INOUT", temp);
                    }
                    if(node->types.variable.is_port)
                    {
                        sprintf(temp, "%s PORT", temp);
                    }
                    if(node->types.variable.is_parameter)
                    {
                        sprintf(temp, "%s PARAMETER", temp);
                    }
                    if(node->types.variable.is_wire)
                    {
                        sprintf(temp, "%s WIRE", temp);
                    }
                    if(node->types.variable.is_reg)
                    {
                        sprintf(temp, "%s REG", temp);
                    }
                    if(node->types.variable.is_reg)
                    {
                        sprintf(temp, "%s OUTPUT REG", temp);
                    }
                    if(node->types.variable.is_integer)
                    {
                        sprintf(temp, "%s INTEGER", temp);
                    }
                    if(node->types.variable.is_genvar)
                    {
                        sprintf(temp, "%s GENVAR", temp);
                    }
                    if(node->types.variable.is_reg_signed)
                    {
                        sprintf(temp, "%s REG SIGNED", temp);
                    }
                    if(node->types.variable.is_wire_signed)
                    {
                        sprintf(temp, "%s WIRE SIGNED", temp);
                    }



                    fprintf(fp, "\t%d [label=\"VAR_DECLARE %s\"];\n", my_label, temp);
                }
                break;
            case DOUBLE_ARRAY_DECLARE:
                {
                    char temp[4096] = "";
                    if(node->types.variable.is_reg)
                    {
                        sprintf(temp, "%s DOUBLE_ARRAY_REG ", temp); 
                    }

                    if(node->types.variable.is_reg_signed)
                    {
                        sprintf(temp, "%s DOUBLE_REG SIGNED", temp);
                    }
                    fprintf(fp, "\t%d [label=\"VAR_DECLARE %s\"];\n", my_label, temp);
                    break;
                }

            case VAR_DECLARE_LIST:
                fprintf(fp, "\t%d [label=\"VAR_DECLARE_LIST\"];\n", my_label);
                break;
            case ASSIGN:
                fprintf(fp, "\t%d [label=\"ASSIGN\"];\n", my_label);
                break;
            case GATE:
                fprintf(fp, "\t%d [label=\"GATE\"];\n", my_label);
                break;
            case GATE_INSTANCE:
                fprintf(fp, "\t%d [label=\"GATE_INSTANCE\"];\n", my_label);
                break;
            case MODULE_CONNECT_LIST:
                fprintf(fp, "\t%d [label=\"MODULE_CONNECT_LIST\"];\n", my_label);
                break;
            case MODULE_CONNECT:
                fprintf(fp, "\t%d [label=\"MODULE_CONNECT\"];\n", my_label);
                break;
            case MODULE_PARAMETER_LIST:
                fprintf(fp, "\t%d [label=\"MODULE_PARAMETER_LIST\"];\n", my_label);
                break;
            case MODULE_PARAMETER:
                fprintf(fp, "\t%d [label=\"MODULE_PARAMETER\"];\n", my_label);
                break;
            case MODULE_NAMED_INSTANCE:
                fprintf(fp, "\t%d [label=\"MODULE_NAMED_INSTANCE\"];\n", my_label);
                break;
            case MODULE_INSTANCE:
                fprintf(fp, "\t%d [label=\"MODULE_INSTANCE\"];\n", my_label);
                break;
            case HARD_BLOCK:
                fprintf(fp, "\t%d [label=\"HARD_BLOCK\"];\n", my_label);
                break;
            case HARD_BLOCK_NAMED_INSTANCE:
                fprintf(fp, "\t%d [label=\"HARD_BLOCK_NAMED_INSTANCE\"];\n", my_label);
                break;
            case HARD_BLOCK_CONNECT:
                fprintf(fp, "\t%d [label=\"HARD_BLOCK_CONNECT\"];\n", my_label);
                break;
            case HARD_BLOCK_CONNECT_LIST:
                fprintf(fp, "\t%d [label=\"HARD_BLOCK_CONNECT_LIST\"];\n", my_label);
                break;
            case BLOCK:
                fprintf(fp, "\t%d [label=\"BLOCK\"];\n", my_label);
                break;
            case NON_BLOCKING_STATEMENT_SIGNED:
                {
                    fprintf(fp, "\t%d [label=\"NON_BLOCKING_STATEMENT %s\"];\n", my_label, signed_unsigned);
                    memset(signed_unsigned, 0, 4096);
                    break;
                }
            case NON_BLOCKING_STATEMENT:
                fprintf(fp, "\t%d [label=\"NON_BLOCKING_STATEMENT\"];\n", my_label);
                break;
            case BLOCKING_STATEMENT:
                fprintf(fp, "\t%d [label=\"BLOCKING_STATEMENT\"];\n", my_label);
                break;
            case CASE:
                fprintf(fp, "\t%d [label=\"CASE\"];\n", my_label);
                break;
            case CASE_LIST:
                fprintf(fp, "\t%d [label=\"CASE_LIST\"];\n", my_label);
                break;
            case CASE_ITEM:
                fprintf(fp, "\t%d [label=\"CASE_ITEM\"];\n", my_label);
                break;
            case CASE_DEFAULT:
                fprintf(fp, "\t%d [label=\"CASE_DEFAULT\"];\n", my_label);
                break;
            case ALWAYS:
                fprintf(fp, "\t%d [label=\"ALWAYS\"];\n", my_label);
                break;
            case DELAY_CONTROL:
                fprintf(fp, "\t%d [label=\"DELAY_CONTROL\"];\n", my_label);
                break;
            case POSEDGE:
                fprintf(fp, "\t%d [label=\"POSEDGE\"];\n", my_label);
                break;
            case NEGEDGE:
                fprintf(fp, "\t%d [label=\"NEGEDGE\"];\n", my_label);
                break;
            case IF:
                fprintf(fp, "\t%d [label=\"IF\"];\n", my_label);
                break;
            case GENERATE:
                fprintf(fp, "\t%d [label=\"GENERATE\"];\n", my_label);
                break;
            case DEFINE:
                fprintf(fp, "\t%d [label=\"DEFINE\"];\n", my_label);
                break;
            case FOR:
                fprintf(fp, "\t%d [label=\"FOR\"];\n", my_label);
                break;
            case IF_Q:
                fprintf(fp, "\t%d [label=\"IF_Q\"];\n", my_label);
                break;
            case BINARY_OPERATION:
                switch (node->types.operation.op)
                {
                    case ADD:
                        fprintf(fp, "\t%d [label=\"BINARY_OPERATION ADD\"];\n", my_label);
                        break;
                    case MINUS:
                        fprintf(fp, "\t%d [label=\"BINARY_OPERATION MINUS\"];\n", my_label);
                        break;
                    case BITWISE_NOT:
                        fprintf(fp, "\t%d [label=\"BINARY_OPERATION BITWISE_NOT\"];\n", my_label);
                        break;
                    case BITWISE_AND:
                        fprintf(fp, "\t%d [label=\"BINARY_OPERATION BITWISE_AND\"];\n", my_label);
                        break;
                    case BITWISE_OR:
                        fprintf(fp, "\t%d [label=\"BINARY_OPERATION BITWISE_OR\"];\n", my_label);
                        break;
                    case BITWISE_NAND:
                        fprintf(fp, "\t%d [label=\"BINARY_OPERATION BITWISE_NAND\"];\n", my_label);
                        break;
                    case BITWISE_NOR:
                        fprintf(fp, "\t%d [label=\"BINARY_OPERATION BITWISE_NOR\"];\n", my_label);
                        break;
                    case BITWISE_XNOR:
                        fprintf(fp, "\t%d [label=\"BINARY_OPERATION BITWISE_XNOR\"];\n", my_label);
                        break;
                    case BITWISE_XOR:
                        fprintf(fp, "\t%d [label=\"BINARY_OPERATION BITWISE_XOR\"];\n", my_label);
                        break;
                    case LOGICAL_NOT:
                        fprintf(fp, "\t%d [label=\"BINARY_OPERATION LOGICAL_NOT\"];\n", my_label);
                        break;
                    case LOGICAL_OR:
                        fprintf(fp, "\t%d [label=\"BINARY_OPERATION LOGICAL_OR\"];\n", my_label);
                        break;
                    case LOGICAL_AND:
                        fprintf(fp, "\t%d [label=\"BINARY_OPERATION LOGICAL_AND\"];\n", my_label);
                        break;
                    case MULTIPLY:
                        fprintf(fp, "\t%d [label=\"BINARY_OPERATION MULTIPLY\"];\n", my_label);
                        break;
                    case DIVIDE:
                        fprintf(fp, "\t%d [label=\"BINARY_OPERATION DIVIDE\"];\n", my_label);
                        break;
                    case MODULO:
                        fprintf(fp, "\t%d [label=\"BINARY_OPERATION MODULO\"];\n", my_label);
                        break;
                    case LT:
                        fprintf(fp, "\t%d [label=\"BINARY_OPERATION LT\"];\n", my_label);
                        break;
                    case GT:
                        fprintf(fp, "\t%d [label=\"BINARY_OPERATION GT\"];\n", my_label);
                        break;
                    case LOGICAL_EQUAL:
                        fprintf(fp, "\t%d [label=\"BINARY_OPERATION LOGICAL_EQUAL\"];\n", my_label);
                        break;
                    case NOT_EQUAL:
                        fprintf(fp, "\t%d [label=\"BINARY_OPERATION NOT_EQUAL\"];\n", my_label);
                        break;
                    case LTE:
                        fprintf(fp, "\t%d [label=\"BINARY_OPERATION LTE\"];\n", my_label);
                        break;
                    case GTE:
                        fprintf(fp, "\t%d [label=\"BINARY_OPERATION GTE\"];\n", my_label);
                        break;
                    case SR:
                        fprintf(fp, "\t%d [label=\"BINARY_OPERATION SR\"];\n", my_label);
                        break;
                    case SL:
                        fprintf(fp, "\t%d [label=\"BINARY_OPERATION SL\"];\n", my_label);
                        break;
                    case CASE_EQUAL:
                        fprintf(fp, "\t%d [label=\"BINARY_OPERATION CASE_EQUAL\"];\n", my_label);
                        break;
                    case CASE_NOT_EQUAL:
                        fprintf(fp, "\t%d [label=\"BINARY_OPERATION\"];\n", my_label);
                        break;
                    default:
                        break;
                }
                break;
            case UNARY_OPERATION:
                switch (node->types.operation.op)
                {
                    case ADD:
                        fprintf(fp, "\t%d [label=\"UNARY_OPERATION ADD\"];\n", my_label);
                        break;
                    case MINUS:
                        fprintf(fp, "\t%d [label=\"UNARY_OPERATION MINUS\"];\n", my_label);
                        break;
                    case BITWISE_NOT:
                        fprintf(fp, "\t%d [label=\"UNARY_OPERATION BITWISE_NOT\"];\n", my_label);
                        break;
                    case BITWISE_AND:
                        fprintf(fp, "\t%d [label=\"UNARY_OPERATION BITWISE_AND\"];\n", my_label);
                        break;
                    case BITWISE_OR:
                        fprintf(fp, "\t%d [label=\"UNARY_OPERATION BITWISE_OR\"];\n", my_label);
                        break;
                    case BITWISE_NAND:
                        fprintf(fp, "\t%d [label=\"UNARY_OPERATION BITWISE_NAND\"];\n", my_label);
                        break;
                    case BITWISE_NOR:
                        fprintf(fp, "\t%d [label=\"UNARY_OPERATION BITWISE_NOR\"];\n", my_label);
                        break;
                    case BITWISE_XNOR:
                        fprintf(fp, "\t%d [label=\"UNARY_OPERATION BITWISE_XNOR\"];\n", my_label);
                        break;
                    case BITWISE_XOR:
                        fprintf(fp, "\t%d [label=\"UNARY_OPERATION BITWISE_XOR\"];\n", my_label);
                        break;
                    case LOGICAL_NOT:
                        fprintf(fp, "\t%d [label=\"UNARY_OPERATION LOGICAL_NOT\"];\n", my_label);
                        break;
                    default:
                        break;
                }
                break;
            case ARRAY_REF:
                fprintf(fp, "\t%d [label=\"ARRAY_REF\"];\n", my_label);
                break;
            case ARRAY_ARRAY_REF:
                fprintf(fp, "\t%d [label=\"ARRAY_ARRAY_REF\"];\n", my_label);
                break;
            case ARRAY_RANGE_REF:
                fprintf(fp, "\t%d [label=\"ARRAY_RANGE_REF\"];\n", my_label);
                break;
            case RANGE_REF:
                fprintf(fp, "\t%d [label=\"RANGE_REF\"];\n", my_label);
                break;
            case MATRIX_REF:
                fprintf(fp, "\t%d [label=\"MATRIX_REF\"];\n", my_label);
                break;
            case CONCATENATE:
                fprintf(fp, "\t%d [label=\"CONCATENATE\"];\n", my_label);
                break;
            case IDENTIFIERS:
                fprintf(fp, "\t%d  [label=\"IDENTIFIERS:%s\"];\n", my_label, node->types.identifier);
                break;
            case NUMBERS:
                switch (node->types.number.base)
                {
                    case(DEC):
                        fprintf(fp, "\t%d [label=\"NUMBERS DEC:%s\"];\n", my_label, node->types.number.number);
                        break;
                    case(HEX):
                        fprintf(fp, "\t%d [label=\"NUMBERS HEX:%s\"];\n", my_label, node->types.number.number);
                        break;
                    case(OCT):
                        fprintf(fp, "\t%d [label=\"NUMBERS OCT:%s\"];\n", my_label, node->types.number.number);
                        break;
                    case(BIN):
                        fprintf(fp, "\t%d [label=\"NUMBERS BIN:%s\"];\n", my_label, node->types.number.number);
                        break;
                    case(LONG_LONG):
                        fprintf(fp, "\t%d [label=\"NUMBERS LONG_LONG:%lld\"];\n", my_label, node->types.number.value);
                        break;
                    case(SDEC):
                        fprintf(fp, "\t%d [label=\"NUMBERS SDEC:%s\"];\n", my_label, node->types.number.number);  
                        break;
                    case(SHEX):
                        fprintf(fp, "\t%d [label=\"NUMBERS SHEX:%s\"];\n", my_label, node->types.number.number);
                        break;
                    case(SOCT):
                        fprintf(fp, "\t%d [label=\"NUMBERS SOCT:%s\"];\n", my_label, node->types.number.number);
                        break;
                    case(SBIN):
                        fprintf(fp, "\t%d [label=\"NUMBERS SBIN:%s\"];\n", my_label, node->types.number.number);
                        break;
                    case(SLONG_LONG):
                        fprintf(fp, "\t%d [label=\"NUMBERS SLONG_LONG:%lld\"];\n", my_label, node->types.number.value);
                        break;
                }
                break;
            default:
                oassert(FALSE);
        }
    }

    if (node != NULL)
    {
        /* print out the connection with the previous node */
        if (from != NULL)
            fprintf(fp, "\t%d -> %d;\n", from_num, my_label);

        for (i = 0; i < node->num_children; i++)
        {
            graphVizOutputAst_traverse_node(fp, node->children[i], node, my_label);
        }
    }
}

void VerilogOutputAst(char* path, ast_node_t *top)
{
    char path_and_file[4096];
    FILE *fp;
    static int file_num = 0;

    // open the file
    sprintf(path_and_file, "%s/%s_fromAST.v", path, top->children[0]->types.identifier);
    fp = fopen(path_and_file, "w");
    file_num++;

    //fprintf(fp, "digraph G {\t\nranksep=.25;\n");
    unique_label_count = 0;

    VerilogOutputAst_traverse_node(fp, top, NULL, -1);

    fprintf(fp, "\nendmodule\n");
    fclose(fp);
}

/*---------------------------------------------------------------------------------------------
 * (function: VerilogOutputAst_traverse_node)
 *-------------------------------------------------------------------------------------------*/
void VerilogOutputAst_traverse_node(FILE *fp, ast_node_t *node, ast_node_t *from, int from_num)
{
    int main_loop;
    int i;
    int my_label = unique_label_count;
    char full_verilog="";

    int inpt_cnt = 0;
    int outpt_cnt = 0;
    int reg_cnt = 0;
    int wire_cnt = 0;
    int wire_signed_cnt=0;
    int param_cnt = 0;
    int integer_cnt =0;
    int reg_signed_cnt=0;
    int count_op_depth = 0;
    int begin_cnt;
    int block_indent;

    //increase the unique count for other nodes since ours is recorded
    unique_label_count++;

    if (node == NULL)
    {
        // print out the node and label details
    }
    else
    {
        switch(node->type)
        {
            case FILE_ITEMS:
                //fprintf(fp, "\t%d [label=\"FILE_ITEMS\"];\n", my_label);
                break;
            case MODULE:
                {
                    char module_name_list[4096]="module ";

                    for (i = 0; i < node->num_children; i++)
                    {
                        ast_node_t *tempnode = node->children[i];
                        switch (tempnode->type)
                        {
                            case IDENTIFIERS:
                                {
                                    char temp[4096]="";
                                    sprintf(temp,"%s", tempnode->types.identifier);
                                    strcat(module_name_list,temp);
                                    strcat(module_name_list," (");
                                    break;
                                }
                                //--
                            case VAR_DECLARE_LIST:
                                {
                                    int j;
                                    for (j = 0; j < tempnode->num_children; j++)
                                    {
                                        ast_node_t *tempnode2 = tempnode->children[j];
                                        switch(tempnode2->type)
                                        {
                                            case VAR_DECLARE:
                                                {
                                                    int m;
                                                    for (m=0; m <tempnode2->num_children; m++)
                                                    {
                                                        ast_node_t *tempnode3 = tempnode2->children[m];
                                                        if (tempnode3==NULL){}
                                                        else {
                                                            switch(tempnode3->type)
                                                            {
                                                                case VAR_DECLARE:
                                                                    {
                                                                        if(tempnode3->types.variable.is_input || tempnode3->types.variable.is_input_signed)
                                                                        {
                                                                            int n;
                                                                            for (n=0; n<tempnode3->num_children; n++)
                                                                            {
                                                                                ast_node_t *tempnode4 = tempnode3->children[n];
                                                                                if (tempnode4==NULL){}
                                                                                else
                                                                                {

                                                                                    switch(tempnode4->type)
                                                                                    {
                                                                                        case IDENTIFIERS:
                                                                                            {
                                                                                                char temp2[4096]="";
                                                                                                sprintf(temp2,"%s, ", tempnode4->types.identifier);
                                                                                                strcat(module_name_list, temp2);
                                                                                                break;
                                                                                            }
                                                                                        case NUMBERS:
                                                                                            {
                                                                                                break;
                                                                                            }
                                                                                    }
                                                                                }
                                                                            }
                                                                        }
                                                                        else if (tempnode3->types.variable.is_output || tempnode3->types.variable.is_output_reg)
                                                                        {
                                                                            int n;
                                                                            for (n=0; n<tempnode3->num_children; n++)
                                                                            {
                                                                                ast_node_t *tempnode4 = tempnode3->children[n];
                                                                                if (tempnode4==NULL){}
                                                                                else
                                                                                {
                                                                                    switch(tempnode4->type)
                                                                                    {
                                                                                        case IDENTIFIERS:
                                                                                            {
                                                                                                char temp2[4096]="";
                                                                                                sprintf(temp2,"%s, ", tempnode4->types.identifier);
                                                                                                strcat(module_name_list, temp2);
                                                                                                break;
                                                                                            }
                                                                                        case NUMBERS:
                                                                                            {
                                                                                                break;
                                                                                            }
                                                                                    }
                                                                                }
                                                                            }
                                                                        }
                                                                        break;
                                                                    }
                                                            }
                                                        }
                                                    }
                                                    break;
                                                }
                                        }
                                    }
                                    break;
                                }
                                //--
                        }
                    }
					//removing the last comma from the module declaration list
                    module_name_list[strlen(module_name_list)-2]= 0; 
                    fprintf(fp,"%s);\n", module_name_list);
                    break;
                }
            case MODULE_ITEMS:
                {
                    char module_items[4096]="\n";
                    int i;
                    for (i = 0; i < node->num_children; i++)
                    {
                        if(node->children[i] == NULL){
                            printf("here\n");
                            continue;
                        }
                        ast_node_t *tempnode_mi = node->children[i];
                        switch (tempnode_mi->type)
                        {
                            case IDENTIFIERS:
                                {
                                    break;
                                }
                            case VAR_DECLARE_LIST:
                                {
                                    int j;
                                    for (j = 0; j < tempnode_mi->num_children; j++)
                                    {

                                        ast_node_t *tempnode3_mi = tempnode_mi->children[j];

                                        if (tempnode3_mi==NULL){}

                                        else if (tempnode3_mi->type==VAR_DECLARE || tempnode3_mi->type==DOUBLE_ARRAY_DECLARE)
                                            //case VAR_DECLARE:
                                        {
                                            char input_declaration[4096]="";
                                            char input_num_range[4096]="";
                                            char input_num_range2[4096]="";
                                            char input_num_range3[4096]="";
                                            char input_num_range4[4096]="";
                                            char temp_input[4096]="";
                                            char temp_number[4096]="";
                                            char temp_number2[4096]="";
                                            char temp_number_val[4096]="";
                                            char temp_num_param[4096]="";

                                            int n;
                                            int num_bits=0;
                                            int bit_high=0;
                                            int bit_low=0;
                                            int bit_cnt=0;
                                            int bin_op_param_occurance=0;
                                            int num_cnt=0;

                                            for (n=0; n<tempnode3_mi->num_children; n++)
                                            {

                                                ast_node_t *tempnode4_mi = tempnode3_mi->children[n];
                                                if (tempnode4_mi==NULL){}
                                                else
                                                {

                                                    switch(tempnode4_mi->type)
                                                    {
                                                        case IDENTIFIERS:
                                                            {
                                                                sprintf(temp_input,"%s", tempnode4_mi->types.identifier);
                                                                //strcat(module_inout_list, temp_input);
                                                                break;
                                                            }
                                                        case NUMBERS:
                                                            {
                                                                num_cnt++;

                                                                if (strlen(input_num_range )<=3 || (n <3))
                                                                {
                                                                    bit_cnt++;

                                                                    if (tempnode4_mi->types.number.number!=NULL)
                                                                    {
                                                                        if (bit_cnt<=1)
                                                                            bit_high = atoi(tempnode4_mi->types.number.number);
                                                                        else
                                                                            bit_low = atoi(tempnode4_mi->types.number.number);
                                                                        num_bits = (bit_high - bit_low)+1;
                                                                        sprintf(temp_number,"%s", tempnode4_mi->types.number.number);
                                                                    }

                                                                    else if (tempnode4_mi->types.number.number==NULL && tempnode4_mi->types.number.value>=0)
                                                                        sprintf(temp_number,"%d", tempnode4_mi->types.number.value);


                                                                    strcat(input_num_range,temp_number);
                                                                    strcat(input_num_range,":");

                                                                }

                                                                else if((n%2)!=0 && (tempnode3_mi->children[n+1]==NULL))
                                                                {
                                                                    if (tempnode4_mi->types.number.base == HEX)
                                                                        sprintf(temp_number_val, "%d'h%s", num_bits, tempnode4_mi->types.number.number);
                                                                    else if(tempnode4_mi->types.number.base == DEC)
                                                                        sprintf(temp_number_val, "%d'd%s", num_bits, tempnode4_mi->types.number.number);
                                                                    else if (tempnode4_mi->types.number.base == OCT)
                                                                        sprintf(temp_number_val, "%d'o%s", num_bits, tempnode4_mi->types.number.number);
                                                                    else if (tempnode4_mi->types.number.base == BIN)
                                                                        sprintf(temp_number_val, "%d'b%s",  num_bits, tempnode4_mi->types.number.number);
                                                                    else if (tempnode4_mi->types.number.base == SHEX) 
                                                                        sprintf(temp_number_val, "%d'sh%s", num_bits, tempnode4_mi->types.number.number);
                                                                    else if(tempnode4_mi->types.number.base == SDEC)
                                                                        sprintf(temp_number_val, "%d'sd%s", num_bits, tempnode4_mi->types.number.number);
                                                                    else if (tempnode4_mi->types.number.base == SOCT)
                                                                        sprintf(temp_number_val, "%d'so%s", num_bits, tempnode4_mi->types.number.number);
                                                                    else if (tempnode4_mi->types.number.base == SBIN)
                                                                        sprintf(temp_number_val, "%d'sb%s",  num_bits, tempnode4_mi->types.number.number);
                                                                    else
                                                                        sprintf(temp_number_val, "%s", num_bits, tempnode4_mi->types.number.number);

                                                                    strcat(input_num_range3, "= ");
                                                                    strcat(input_num_range3, temp_number_val);
                                                                }
                                                                else if (n<=4)
                                                                {
                                                                    sprintf(temp_number2,"%s", tempnode4_mi->types.number.number);
                                                                    strcat(input_num_range2,temp_number2);
                                                                    strcat(input_num_range2,":");
                                                                }
                                                                else
                                                                {
                                                                    sprintf(temp_number2,"%s", tempnode4_mi->types.number.number);
                                                                    strcat(input_num_range4,temp_number2);
                                                                    strcat(input_num_range4,":");
                                                                }


                                                                break;
                                                            }
                                                        case BINARY_OPERATION: //TODO: UNFINISHED
                                                            {
                                                                char temp_num_param_op[4096]="";
                                                                char temp_num_param_text[4096]="";
                                                                int param_bin_cnt;
                                                                bin_op_param_occurance++;
                                                                BinaryOpsGenerator(tempnode4_mi, temp_num_param_op);
                                                                RecursiveBinaryop(tempnode4_mi, temp_num_param_text, temp_num_param_op, temp_num_param_op, param_bin_cnt, 0);
                                                                strcat(temp_num_param, temp_num_param_op);
                                                                temp_num_param_text[strlen(temp_num_param_text)-2]=0;
                                                                if (bin_op_param_occurance<=1 && num_cnt<=1)
                                                                {
                                                                    memmove(temp_num_param_text, temp_num_param_text+1, 4096-1);
                                                                    strcat(input_num_range, temp_num_param_text);
                                                                    strcat(input_num_range, ": ");
                                                                    input_num_range[strlen(input_num_range)-1]=0;
                                                                }

                                                                else if (bin_op_param_occurance<=1 && num_cnt>1)
                                                                {
                                                                    memmove(temp_num_param_text, temp_num_param_text+1, 4096-1);
                                                                    strcat(input_num_range2, temp_num_param_text);
                                                                    strcat(input_num_range2, ": ");
                                                                    input_num_range2[strlen(input_num_range2)-1]=0;
                                                                }

                                                                else if(bin_op_param_occurance==2 && num_cnt>1)
                                                                {

                                                                    memmove(temp_num_param_text, temp_num_param_text+1, 4096-1);
                                                                    strcat(input_num_range2, temp_num_param_text);
                                                                    strcat(input_num_range2, ": ");
                                                                    input_num_range2[strlen(input_num_range2)-1]=0;
                                                                }

                                                                else if(bin_op_param_occurance==2 && num_cnt<=1)
                                                                {

                                                                    memmove(temp_num_param_text, temp_num_param_text+1, 4096-1);
                                                                    strcat(input_num_range, temp_num_param_text);
                                                                    strcat(input_num_range, ": ");
                                                                    input_num_range[strlen(input_num_range)-1]=0;
                                                                }
                                                                else if(bin_op_param_occurance>2)
                                                                {
                                                                    memmove(temp_num_param_text, temp_num_param_text+1, 4096-1);
                                                                    strcat(input_num_range4, temp_num_param_text);
                                                                    strcat(input_num_range4, ": ");
                                                                    input_num_range4[strlen(input_num_range4)-1]=0;
                                                                }

                                                            }
                                                    }
                                                }
                                            }
                                            //input_num_range[strlen(input_num_range)-1]= 0;

                                            if(strlen(input_num_range)>=1)
                                            {
                                                memmove(input_num_range+1, input_num_range, 4096-1);
                                                input_num_range[0]='[';
                                                input_num_range[strlen(input_num_range)-1]=0;
                                                strcat(input_num_range, "]");
                                            }

                                            if(strlen(input_num_range2)>=1)
                                            {
                                                memmove(input_num_range2+1, input_num_range2, 4096-1);
                                                input_num_range2[0]='[';
                                                input_num_range2[strlen(input_num_range2)-1]=0;
                                                strcat(input_num_range2, "]");
                                            }

                                            if(strlen(input_num_range4)>=1)
                                            {
                                                memmove(input_num_range4+1, input_num_range4, 4096-1);
                                                input_num_range4[0]='[';
                                                input_num_range4[strlen(input_num_range4)-1]=0;
                                                strcat(input_num_range4, "]");
                                            }


                                            if (tempnode3_mi->types.variable.is_input_signed)
                                            {
                                                inpt_cnt++;
                                                sprintf(input_declaration, "input signed %s %s;\n", input_num_range, temp_input);
                                            }
                                            else if (tempnode3_mi->types.variable.is_input)
                                            {
                                                inpt_cnt++;
                                                sprintf(input_declaration, "input %s %s;\n", input_num_range, temp_input);
                                            }

                                            else if(tempnode3_mi->types.variable.is_output)
                                            {
                                                outpt_cnt++;
                                                sprintf(input_declaration, "output %s %s;\n", input_num_range, temp_input);
                                            }

                                            else if(tempnode3_mi->types.variable.is_output_reg)
                                            {
                                                outpt_cnt++;
                                                sprintf(input_declaration, "output reg %s %s;\n", input_num_range, temp_input);
                                            }

                                            else if(tempnode3_mi->types.variable.is_reg)
                                            {
                                                reg_cnt++;
                                                sprintf(input_declaration, "reg %s %s %s%s%s", input_num_range, 
                                                		temp_input, input_num_range2, input_num_range4, input_num_range3);
                                                if (strlen(input_num_range3)==0 && strlen(input_num_range2)==0)
                                                    input_declaration[strlen(input_declaration)-1]=0;

                                                strcat(input_declaration, ";\n");
                                            }

                                            else if(tempnode3_mi->types.variable.is_wire)
                                            {
                                                wire_cnt++;
                                                sprintf(input_declaration, "wire %s %s %s%s%s", input_num_range, temp_input,
                                                        input_num_range2, input_num_range4, input_num_range3);
                                                if (strlen(input_num_range3)==0 && strlen(input_num_range2)==0)
                                                    input_declaration[strlen(input_declaration)-1]=0;

                                                strcat(input_declaration, ";\n");
                                            }

                                            else if(tempnode3_mi->types.variable.is_parameter)
                                            {
                                                param_cnt++;
                                                input_num_range[strlen(input_num_range)-1]=0;
                                                memmove(input_num_range, input_num_range+1, 4096-1);
                                                sprintf(input_declaration, "parameter %s = %s;\n", temp_input, input_num_range);
                                            }

                                            else if (tempnode3_mi->types.variable.is_integer)
                                            {
                                                integer_cnt++;
                                                sprintf(input_declaration, "integer %s;\n", temp_input);
                                            }
                                            else if (tempnode3_mi->types.variable.is_genvar)
                                            {
                                                integer_cnt++;
                                                sprintf(input_declaration, "genvar %s;\n", temp_input);
                                            }

                                            else if (tempnode3_mi->types.variable.is_reg_signed)
                                            {
                                                reg_signed_cnt++;
                                                sprintf(input_declaration, "reg signed %s %s %s%s%s", 
                                                		input_num_range, temp_input, input_num_range2,
                                                        input_num_range4, input_num_range3);
                                                if (strlen(input_num_range3)==0 && strlen(input_num_range2)==0)
                                                    input_declaration[strlen(input_declaration)-1]=0;

                                                strcat(input_declaration, ";\n");
                                            }
                                            else if (tempnode3_mi->types.variable.is_wire_signed)
                                            {
                                                wire_signed_cnt++;
                                                sprintf(input_declaration, "wire signed %s %s %s%s%s", 
                                                		input_num_range, temp_input, input_num_range2,
                                                        input_num_range4, input_num_range3);
                                                if (strlen(input_num_range3)==0 && strlen(input_num_range2)==0)
                                                    input_declaration[strlen(input_declaration)-1]=0;

                                                strcat(input_declaration, ";\n");
                                            }

                                            if (inpt_cnt==1 || outpt_cnt==1 || reg_cnt==1 
                                            		|| wire_cnt==1 || reg_signed_cnt==1 
                                            		|| integer_cnt==1 || wire_signed_cnt==1)
                                                strcat(module_items,"\n");
                                            strcat(module_items, input_declaration);
                                        }

                                    }

                                    break;
                                }

                        }

                    }

                    fprintf(fp,"%s", module_items);

                    break;
                }
            case ASSIGN:
                {
                    char assign_var[4096]="";
                    //char temp[4096]="\n";
                    int i;
                    for (i = 0; i < node->num_children; i++)
                    {
                        ast_node_t *tempnode = node->children[i];
                        if (tempnode==NULL){}
                        else
                        {
                            //strcat(temp, "assign ");
                            BlockingNonBlockingStatements(tempnode, assign_var, count_op_depth);
                        }
                    }

                    //strcat(assign_var, ";");
                    fprintf(fp, "\nassign %s", assign_var);
                    break;
                }
            case GENERATE:
                {
                    fprintf(fp, "\ngenerate");
                    break;
                }

            case DEFINE:
                {
                    fprintf(fp, "\ndefine");
                    break;
                }

            case ALWAYS:
                {
                    char sensitivity[4096]="";
                    char always_statement[4096]="\nalways @ (";
                    char block[4096]="";
                    char if_case[4096]="if ";
                    char bin_if_else[4096]="";
                    int cnt_dummy=0;
                    block_cnt=0;


                    int j;
                    for (j = 0; j < node->num_children-1; j++)
                    {
                        ast_node_t *tempnode3_mi = node->children[j];

                        if (tempnode3_mi==NULL){
                            strcat(sensitivity, "*    ");
                        }
                        else if (tempnode3_mi->type==DELAY_CONTROL)
                        {
                            int m;
                            for (m=0; m<tempnode3_mi->num_children; m++)
                            {
                                ast_node_t *tempnode3_mi2 = tempnode3_mi->children[m];
                                if (tempnode3_mi2==NULL){}
                                else if(tempnode3_mi2->type==IDENTIFIERS)
                                {
                                    strcat(sensitivity, tempnode3_mi2->types.identifier);
                                    strcat(sensitivity, " or ");
                                }
                                else if (tempnode3_mi2->type==NEGEDGE)
                                {
                                    strcat(sensitivity, "negedge ");
                                    strcat(sensitivity, tempnode3_mi2->children[0]->types.identifier);
                                    strcat(sensitivity, " or ");
                                }

                                else if (tempnode3_mi2->type==POSEDGE)
                                {
                                    strcat(sensitivity, "posedge ");
                                    strcat(sensitivity, tempnode3_mi2->children[0]->types.identifier);
                                    strcat(sensitivity, " or ");
                                }
                            }
                        }
                        else if (tempnode3_mi->type==BLOCK)
                        {
                            //if (tempnode3_mi->type==BLOCK)
                            strcat(sensitivity, "*    ");
                        }
                    }
                    strcat(always_statement, sensitivity);
                    always_statement[strlen(always_statement)-4]= 0;
                    strcat(always_statement, ")\n");
                    fprintf(fp, "%s", always_statement);

                    break;
                }

            case CASE:
                {
                    char case_var[4096]="";
                    char case_number[4096]="";
                    sprintf(case_var,"\n\tcase (%s)", node->children[0]->types.identifier);
                    fprintf(fp, case_var);

                    int j;
                    for (j=0; j<node->num_children; j++)
                    {
                        ast_node_t* case_item = node->children[j];
                        if (case_item==NULL){}
                        else
                        {
                            if (case_item->type==CASE_ITEM)
                            {
                                int m;
                                for (m=0; m<case_item->num_children;m++)
                                {
                                    char case_statement[4096]="";
                                    ast_node_t* case_item_child = case_item->children[m];
                                    if (case_item_child==NULL){}
                                    else
                                    {
                                        if (case_item_child->type==NUMBERS)
                                            strcat(case_number, case_item_child->types.number.number);
                                        else if (case_item_child->type==NON_BLOCKING_STATEMENT 
                                        		|| case_item_child->type==NON_BLOCKING_STATEMENT_SIGNED 
                                        		|| case_item_child->type==BLOCKING_STATEMENT)
                                        {
                                            BlockingNonBlockingStatements(case_item_child, case_statement,0);								 }
                                            strcat(case_number, case_statement);

                                    }
                                }

                            }



                        }

                    }

                    fprintf(fp, case_number);
                    break;

                }

            case BLOCK:
                {
                    char whole_block[16384]="";
                    char if_case[4096]="";
                    char bin_if_else[4096]="";
                    int cnt_dummy;
                    block_cnt++;

                    BeginBlock(node, whole_block, if_case, bin_if_else, if_flag, 
                    				cnt_dummy, begin_cnt, block_indent, block_cnt);
                    begin_cnt++;

                    //fprintf(fp, "begin\n");
                    if (fp != NULL && (block_cnt<=1 || parent_node->type==ALWAYS))
                    {
                        fprintf(fp, "begin\n");
                        fprintf(fp, "%s\n", whole_block);
                        fprintf(fp, "end\n");
                    }
                    //fprintf(fp, "%s", if_case);

                    //fprintf(fp, "end\n");
                    else if (block_cnt<=1 || parent_node->type==GENERATE)
                    {
                        fprintf(fp, "%s\n", whole_block);
                        fprintf(fp, "endgenerate\n");
                    }
                    block_indent++;
                    begin_cnt++;
                    break;
                }

            case MODULE_INSTANCE:
                {
                    char module_instance[4096]="";
                    int global_id_1or2;
                    int i;
                    int array_ref_flag=0;
                    int array_array_flag=0;
                    int range_ref_flag=0;
                    int matrix_ref_flag=0;
                    int array_range_ref_flag=0;
                    int format_1_or_2=1;
                    for (i=0; i<node->num_children; i++)
                    {
                        ast_node_t *module_inst_child = node->children[i];
                        if (module_inst_child==NULL){}
                        else
                        {
                            switch(module_inst_child->type)
                            {
                                case IDENTIFIERS:
                                    {
                                        sprintf(module_instance, "\n%s ", module_inst_child->types.identifier);
                                        break;
                                    }
                                case MODULE_NAMED_INSTANCE:
                                    {
                                        strcat(module_instance, module_inst_child->children[0]->types.identifier);
                                        strcat(module_instance, "(");


                                        int j;
                                        for (j=0;j<module_inst_child->num_children; j++)
                                        {
                                            ast_node_t* module_connect_list_child = module_inst_child->children[j];
                                            if (module_connect_list_child==NULL){}
                                            else
                                            {
                                                if(module_connect_list_child->type==MODULE_CONNECT_LIST)
                                                {
                                                    int k;
                                                    for (k=0;k<module_connect_list_child->num_children; k++)
                                                    {
                                                        ast_node_t* module_connect_items = module_connect_list_child->children[k];
                                                        if (module_connect_items==NULL){}
                                                        else
                                                        {
                                                            if (module_connect_items->type==MODULE_CONNECT)
                                                            {
                                                                int cnt_id_1or2=0;
                                                                int l;
                                                                for (l=0;l<module_connect_items->num_children; l++)
                                                                {
                                                                    ast_node_t* module_ids = module_connect_items->children[l];
                                                                    if (module_ids==NULL){}
                                                                    else
                                                                    {
                                                                        array_ref_flag=0;
                                                                        array_array_flag=0;
                                                                        range_ref_flag=0;
                                                                        matrix_ref_flag=0;
                                                                        array_range_ref_flag=0;

                                                                        if (module_ids->type==IDENTIFIERS)
                                                                        {
                                                                            cnt_id_1or2++;

                                                                            if (l>0 && cnt_id_1or2==1 && module_connect_items->children[l-1]==NULL)
                                                                            {
                                                                                strcat(module_instance, module_ids->types.identifier);
                                                                                strcat(module_instance, ", ");
                                                                            }
                                                                            else
                                                                            {
                                                                                if (cnt_id_1or2==1)
                                                                                {
                                                                                    strcat(module_instance,"\t.");
                                                                                    strcat(module_instance, module_ids->types.identifier);
                                                                                    strcat(module_instance, "(");
                                                                                    format_1_or_2=2;
                                                                                }
                                                                                else
                                                                                {
                                                                                    strcat(module_instance, module_ids->types.identifier);
                                                                                    if(k!=(module_connect_list_child->num_children-1))	
                                                                                    		strcat(module_instance, "), \n \t");
										                                            else
								                                            			strcat(module_instance, ") \n \t");
                                                                                }
                                                                            }

                                                                        }


                                                                        else if (module_ids->type==ARRAY_REF)
                                                                        {

                                                                            cnt_id_1or2=0;
                                                                            cnt_id_1or2++;
                                                                            array_ref_flag=1;
                                                                            strcat(module_instance, module_ids->children[0]->types.identifier);
                                                                            char array_name[4096]="";

                                                                            if (module_ids->children[1]->type!=BINARY_OPERATION)
                                                                            {
                                                                                if (cnt_id_1or2==1 && module_connect_items->children[l-1]==NULL)
                                                                                    sprintf(array_name, "[%d], ", module_ids->children[1]->types.number.value);

                                                                                else if (cnt_id_1or2==1)
                                                                                {
                                                                                    sprintf(array_name, "[%d]), \n\t ", module_ids->children[1]->types.number.value);
                                                                                    format_1_or_2=2;
                                                                                }
                                                                            }
                                                                            else if (module_ids->children[1]->type==BINARY_OPERATION)
                                                                            {
                                                                                char module_inst_array_bin[4096]="";
                                                                                char module_inst_array_bin_name[4096]="";
                                                                                BinaryOpsGenerator(module_ids->children[1], module_inst_array_bin);
                                                                                RecursiveBinaryop(module_ids->children[1], module_inst_array_bin_name,
                                                                                				module_inst_array_bin, module_inst_array_bin, 0, 0);
                                                                                sprintf(array_name,"[%s]", module_inst_array_bin_name);
                                                                            }


                                                                            strcat(module_instance, array_name);
                                                                        }

                                                                        else if (module_ids->type==ARRAY_ARRAY_REF)
                                                                        {

                                                                            cnt_id_1or2=0;
                                                                            cnt_id_1or2++;
                                                                            array_ref_flag=1;
                                                                            array_array_flag=1;
                                                                            strcat(module_instance, module_ids->children[0]->types.identifier);
                                                                            char array_name[4096]="";

                                                                            if (module_ids->children[1]->type!=BINARY_OPERATION)
                                                                            {
                                                                                if (module_ids->children[1]->type==NUMBERS 
                                                                                	&& module_ids->children[2]->type==NUMBERS)
                                                                                {
                                                                                    char test1[4096]="";
                                                                                    char test2[4096]="";

                                                                                    sprintf(test1, "%d", module_ids->children[1]->types.number.value);
                                                                                    sprintf(test2, "%d", module_ids->children[2]->types.number.value);

                                                                                    module_ids->children[1]->types.number.number = test1;
                                                                                    module_ids->children[2]->types.number.number = test2;



                                                                                    if (cnt_id_1or2==1 && module_connect_items->children[l-1]==NULL)

                                                                                        sprintf(array_name, "[%s][%s], ", 
                                                                                        		module_ids->children[1]->types.number.number, 
                                                                                        		module_ids->children[2]->types.number.number);
                                                                                    else if (cnt_id_1or2==1)
                                                                                    {
                                                                                        sprintf(array_name, "[%s][%s]), \n\t ", 
                                                                                        		module_ids->children[1]->types.number.number, 
                                                                                        		module_ids->children[2]->types.number.number);
                                                                                        format_1_or_2=2;
                                                                                    }
                                                                                }
                                                                            }
                                                                            else if (module_ids->children[1]->type==BINARY_OPERATION)
                                                                            {
                                                                                char module_inst_array_bin[4096]="";
                                                                                char module_inst_array_bin_name[4096]="";
                                                                                BinaryOpsGenerator(module_ids->children[1], 
                                                                                				   module_inst_array_bin);
                                                                                RecursiveBinaryop(module_ids->children[1], 
                                                                                				  module_inst_array_bin_name, 
                                                                                				  module_inst_array_bin, 
                                                                                				  module_inst_array_bin, 0, 0);
                                                                                sprintf(array_name,"[%s]", module_inst_array_bin_name);
                                                                            }


                                                                            strcat(module_instance, array_name);

                                                                        }
                                                                        else if (module_ids->type==NUMBERS)   
                                                                        {
                                                                            char assign_in[4096]="";
                                                                            if (module_ids->types.number.base==BIN)
                                                                                sprintf(assign_in, " %d'b%s", 
                                                                                		module_ids->types.number.binary_size, 
                                                                                		module_ids->types.number.number);
                                                                            else if (module_ids->types.number.base==SBIN)
                                                                                sprintf(assign_in, " %d'sb%s", 
                                                                                		module_ids->types.number.binary_size, 
                                                                                		module_ids->types.number.number);
                                                                            else
                                                                                sprintf(assign_in," (%d", module_ids->types.number.value);

                                                                            strcat(module_instance, assign_in);
                                                                            if(k!=(module_connect_list_child->num_children-1))
								                                            	strcat(module_instance, "), \n \t");
									                                        else
										                                        strcat(module_instance, ") \n \t");

                                                                        }
                                    									else if (module_ids->type==CONCATENATE)	
							                                    		{
                                							    		    char concat_string[4096]="";
							                                    		    ConcatNodes(module_ids,concat_string);
								                                     	    strcat(module_instance,concat_string);
                                                                        	if(k!=(module_connect_list_child->num_children-1))
								                                            	strcat(module_instance, "), \n \t");
									                                    	else
										                                    	strcat(module_instance, ") \n \t");
									                                    }
									                                    else if (module_ids->type==ARRAY_RANGE_REF)	
									                                    {
                                                                            array_range_ref_flag=1;
                                                                            strcat(module_instance, module_ids->children[0]->types.identifier);
                                                                            char array_name[4096]="";

                                                                            if (module_ids->children[1]->type==NUMBERS || module_ids->children[1]->type==IDENTIFIERS)
                                                                            {
                                                                                if (module_ids->children[1]->type==IDENTIFIERS)
                                                                                {
                                                                                    strcat(array_name,"[");
                                                                                    strcat(array_name, module_ids->children[1]->types.identifier);
                                                                                    strcat(array_name,"]");
                                                                                }
                                                                                else if (module_ids->children[1]->type==NUMBERS)
                                                                                {
                                                                                    strcat(array_name,"[");
                                                                                    strcat(array_name, module_ids->children[1]->types.number.number);
                                                                                    strcat(array_name,"]");
                                                                                }

                                                                            }
									                                        strcat(module_instance, array_name);
									                                        if (module_ids->children[2]->type==BINARY_OPERATION)
                                                                            {
                                                                                char module_inst_range_bin[4096]="";
                                                                                char module_inst_range_bin_name[4096]="";
                                                                                BinaryOpsGenerator(module_ids->children[2], module_inst_range_bin);
                                                                                RecursiveBinaryop(module_ids->children[2], module_inst_range_bin_name, module_inst_range_bin, module_inst_range_bin, 0, 0);
                                                                                module_inst_range_bin_name[strlen(module_inst_range_bin_name)-1]=0;

                                                                                sprintf(array_name,"[%s", module_inst_range_bin_name);
                                                                                if (module_ids->children[3]->type==IDENTIFIERS)
                                                                                {
                                                                                    strcat(array_name,":" );
                                                                                    strcat(array_name, module_ids->children[3]->types.identifier);
                                                                                    strcat(array_name, "],  \n, ");
                                                                                }

                                                                                else if (module_ids->children[3]->type==NUMBERS)
                                                                                {
                                                                                    strcat(array_name,":" );
                                                                                    strcat(array_name, module_ids->children[3]->types.number.number);
                                                                                    strcat(array_name, "], ");
                                                                                }

                                                                            }
                                                                            else if (module_ids->children[3]->type==BINARY_OPERATION)
                                                                            {
                                                                                if (module_ids->children[2]->type==IDENTIFIERS)
                                                                                {
                                                                                    strcat(array_name,"[" );
                                                                                    strcat(array_name, module_ids->children[2]->types.identifier);
                                                                                    strcat(array_name, ": ");
                                                                                }
                                                                                else if (module_ids->children[2]->type==NUMBERS)
                                                                                {
                                                                                    strcat(array_name,"[" );
                                                                                    strcat(array_name, module_ids->children[2]->types.number.number);
                                                                                    strcat(array_name, ": ");
                                                                                }

                                                                                char module_inst_range_bin[4096]="";
                                                                                char module_inst_range_bin_name[4096]="";
                                                                                BinaryOpsGenerator(module_ids->children[3], module_inst_range_bin);
                                                                                RecursiveBinaryop(module_ids->children[3], module_inst_range_bin_name, 
                                                                                				  module_inst_range_bin, module_inst_range_bin, 0, 0);
                                                                                module_inst_range_bin_name[strlen(module_inst_range_bin_name)-1]=0;
                                                                                strcat(array_name, module_inst_range_bin_name);
                                                                                strcat(array_name, "], \n");
                                                                            }

                                                                            else
                                                                            {
                                                                                if (module_ids->children[2]->type==NUMBERS 
                                                                                	&& module_ids->children[3]->type==NUMBERS)
                                                                                {

                                                                                    char test1[4096]="";
                                                                                    char test2[4096]="";
                                                                                    sprintf(test1, "%d", module_ids->children[2]->types.number.value);
                                                                                    sprintf(test2, "%d", module_ids->children[3]->types.number.value);

                                                                                    module_ids->children[2]->types.number.number = test1;
                                                                                    module_ids->children[3]->types.number.number = test2;


                                                                                    if (cnt_id_1or2==1)
                                                                                        sprintf(array_name, "[%s:%s]),\n\t ", 
                                                                                        		module_ids->children[2]->types.number.number, 
                                                                                        		module_ids->children[3]->types.number.number);
                                                                                    else
                                                                                        sprintf(array_name, "[%d:%s], ", 
                                                                                        		module_ids->children[2]->types.number.number, 
                                                                                        		module_ids->children[3]->types.number.number);
                                                                                }

                                                                                else if (module_ids->children[2]->type==NUMBERS 
                                                                                		 && module_ids->children[3]->type==IDENTIFIERS)
                                                                                {

                                                                                    char test2[4096]="";
                                                                                    sprintf(test2, "%d", module_ids->children[2]->types.number.value);
                                                                                    module_ids->children[2]->types.number.number = test2;


                                                                                    if (cnt_id_1or2==1)
                                                                                        sprintf(array_name, "[%s:%s]),\n\t ", 
                                                                                        		module_ids->children[2]->types.number.number, 
                                                                                        		module_ids->children[3]->types.identifier);
                                                                                    else
                                                                                        sprintf(array_name, "[%s:%s], ", 
                                                                                        		module_ids->children[2]->types.number.number, 
                                                                                        		module_ids->children[3]->types.identifier);
                                                                                }

                                                                                else if (module_ids->children[2]->type==IDENTIFIERS 
                                                                                		 && module_ids->children[3]->type==IDENTIFIERS)
                                                                                {
                                                                                    if (cnt_id_1or2==1)
                                                                                        sprintf(array_name, "[%s:%s]),\n\t ", 
                                                                                        		module_ids->children[2]->types.identifier, 
                                                                                        		module_ids->children[3]->types.identifier);
                                                                                    else
                                                                                        sprintf(array_name, "[%s:%s], ", 
                                                                                        		module_ids->children[2]->types.identifier, 
                                                                                        		module_ids->children[3]->types.identifier);
                                                                                }

                                                                                else if (module_ids->children[2]->type==IDENTIFIERS 
                                                                                		 && module_ids->children[3]->type==NUMBERS)
                                                                                {
                                                                                    char test2[4096]="";
                                                                                    sprintf(test2, "%d", module_ids->children[3]->types.number.value);
                                                                                    module_ids->children[3]->types.number.number = test2;

                                                                                    if (cnt_id_1or2==1)
                                                                                        sprintf(array_name, "[%s:%s]),\n\t ", 
                                                                                        		module_ids->children[2]->types.identifier, 
                                                                                        		module_ids->children[3]->types.number.value);
                                                                                    else
                                                                                        sprintf(array_name, "[%s:%s], ", 
                                                                                        		module_ids->children[2]->types.identifier, 
                                                                                        		module_ids->children[3]->types.number.number);
                                                                                }
                                                                            }

                                                                            strcat(module_instance, array_name);   
																		}
                                                                        else if (module_ids->type==RANGE_REF)
                                                                        {
                                                                            array_ref_flag=1;
                                                                            range_ref_flag=1;
                                                                            strcat(module_instance, module_ids->children[0]->types.identifier);
                                                                            char array_name[4096]="";


                                                                            if (module_ids->children[1]->type==BINARY_OPERATION)
                                                                            {
                                                                                char module_inst_range_bin[4096]="";
                                                                                char module_inst_range_bin_name[4096]="";
                                                                                BinaryOpsGenerator(module_ids->children[1], module_inst_range_bin);
                                                                                RecursiveBinaryop(module_ids->children[1], module_inst_range_bin_name, 
                                                                                				  module_inst_range_bin, module_inst_range_bin, 0, 0);
                                                                                module_inst_range_bin_name[strlen(module_inst_range_bin_name)-1]=0;

                                                                                sprintf(array_name,"[%s", module_inst_range_bin_name);
                                                                                if (module_ids->children[2]->type==IDENTIFIERS)
                                                                                {
                                                                                    strcat(array_name,":" );
                                                                                    strcat(array_name, module_ids->children[2]->types.identifier);
                                                                                    strcat(array_name, "],  \n, ");
                                                                                }

                                                                                else if (module_ids->children[2]->type==NUMBERS)
                                                                                {
                                                                                    strcat(array_name,":" );
                                                                                    strcat(array_name, module_ids->children[2]->types.number.number);
                                                                                    strcat(array_name, "], ");
                                                                                }

                                                                            }

                                                                            else if (module_ids->children[2]->type==BINARY_OPERATION)
                                                                            {

                                                                                if (module_ids->children[1]->type==IDENTIFIERS)
                                                                                {
                                                                                    strcat(array_name,"[" );
                                                                                    strcat(array_name, module_ids->children[1]->types.identifier);
                                                                                    strcat(array_name, ": ");
                                                                                }

                                                                                else if (module_ids->children[1]->type==NUMBERS)
                                                                                {
                                                                                    strcat(array_name,"[" );
                                                                                    strcat(array_name, module_ids->children[1]->types.number.number);
                                                                                    strcat(array_name, ": ");
                                                                                }


                                                                                char module_inst_range_bin[4096]="";
                                                                                char module_inst_range_bin_name[4096]="";
                                                                                BinaryOpsGenerator(module_ids->children[2], module_inst_range_bin);
                                                                                RecursiveBinaryop(module_ids->children[2], module_inst_range_bin_name,
                                                                                				  module_inst_range_bin, module_inst_range_bin, 0, 0);
                                                                                module_inst_range_bin_name[strlen(module_inst_range_bin_name)-1]=0;
                                                                                //strcat(array_name, "[");
                                                                                strcat(array_name, module_inst_range_bin_name);
                                                                                strcat(array_name, "], \n");
                                                                            }

                                                                            else
                                                                            {
                                                                                if (module_ids->children[1]->type==NUMBERS 
                                                                                	&& module_ids->children[2]->type==NUMBERS)
                                                                                {

                                                                                    char test1[4096]="";
                                                                                    char test2[4096]="";
                                                                                    sprintf(test1, "%d", module_ids->children[1]->types.number.value);
                                                                                    sprintf(test2, "%d", module_ids->children[2]->types.number.value);

                                                                                    module_ids->children[1]->types.number.number = test1;
                                                                                    module_ids->children[2]->types.number.number = test2;


                                                                                    if (cnt_id_1or2==1)
                                                                                        sprintf(array_name, "[%s:%s]),\n\t ",
                                                                                        	    module_ids->children[1]->types.number.number, 
                                                                                        	    module_ids->children[2]->types.number.number);
                                                                                    else
                                                                                        sprintf(array_name, "[%d:%s], ", 
                                                                                        		module_ids->children[1]->types.number.number, 
                                                                                        		module_ids->children[2]->types.number.number);
                                                                                }

                                                                                else if (module_ids->children[1]->type==NUMBERS 
                                                                                		 && module_ids->children[2]->type==IDENTIFIERS)
                                                                                {

                                                                                    char test2[4096]="";
                                                                                    sprintf(test2, "%d", module_ids->children[1]->types.number.value);
                                                                                    module_ids->children[1]->types.number.number = test2;


                                                                                    if (cnt_id_1or2==1)
                                                                                        sprintf(array_name, "[%s:%s]),\n\t ", 
                                                                                        		module_ids->children[1]->types.number.number, 
                                                                                        		module_ids->children[2]->types.identifier);
                                                                                    else
                                                                                        sprintf(array_name, "[%s:%s], ", 
                                                                                        		module_ids->children[1]->types.number.number, 
                                                                                        		module_ids->children[2]->types.identifier);
                                                                                }

                                                                                else if (module_ids->children[1]->type==IDENTIFIERS 
                                                                                		 && module_ids->children[2]->type==IDENTIFIERS)
                                                                                {
                                                                                    if (cnt_id_1or2==1)
                                                                                        sprintf(array_name, "[%s:%s]),\n\t ",
                                                                                        		module_ids->children[1]->types.identifier, 
                                                                                        		module_ids->children[2]->types.identifier);
                                                                                    else
                                                                                        sprintf(array_name, "[%s:%s], ", 
                                                                                        		module_ids->children[1]->types.identifier, 
                                                                                        		module_ids->children[2]->types.identifier);
                                                                                }

                                                                                else if (module_ids->children[1]->type==IDENTIFIERS 
                                                                                		 && module_ids->children[2]->type==NUMBERS)
                                                                                {
                                                                                    char test2[4096]="";
                                                                                    sprintf(test2, "%d", module_ids->children[2]->types.number.value);
                                                                                    module_ids->children[2]->types.number.number = test2;

                                                                                    if (cnt_id_1or2==1)
                                                                                        sprintf(array_name, "[%s:%s]),\n\t ", 
                                                                                        		module_ids->children[1]->types.identifier, 
                                                                                        		module_ids->children[2]->types.number.value);
                                                                                    else
                                                                                        sprintf(array_name, "[%s:%s], ", 
                                                                                        		module_ids->children[1]->types.identifier, 
                                                                                        		module_ids->children[2]->types.number.number);
                                                                                }
                                                                            }

                                                                            strcat(module_instance, array_name);
                                                                        }

                                                                        else if (module_ids->type==MATRIX_REF)
                                                                        {
                                                                            array_ref_flag=1;
                                                                            range_ref_flag=1;
                                                                            matrix_ref_flag=1;
                                                                            strcat(module_instance, module_ids->children[0]->types.identifier);
                                                                            char array_name[4096]="";
                                                                            if (module_ids->children[1]->type!=BINARY_OPERATION)
                                                                            {
                                                                                if (cnt_id_1or2==1)
                                                                                    sprintf(array_name, "[%d:%d] [%d:%d]),\n\t ", 
                                                                                    		module_ids->children[1]->types.number.value, 
                                                                                    		module_ids->children[2]->types.number.value, 
                                                                                    		module_ids->children[3]->types.number.value, 
                                                                                    		module_ids->children[4]->types.number.value);
                                                                                else
                                                                                    sprintf(array_name, "[%d:%d] [%d:%d], ", 
                                                                                    		module_ids->children[1]->types.number.value, 
                                                                                    		module_ids->children[2]->types.number.value, 
                                                                                    		module_ids->children[3]->types.number.value, 
                                                                                    		module_ids->children[4]->types.number.value);
                                                                            }

                                                                            else if (module_ids->children[1]->type==BINARY_OPERATION)
                                                                            {
                                                                                char module_inst_matrix_bin[4096]="";
                                                                                char module_inst_matrix_bin_name[4096]="";
                                                                                BinaryOpsGenerator(module_ids->children[1], 
                                                                                				   module_inst_matrix_bin);
                                                                                RecursiveBinaryop(module_ids->children[1], 
                                                                                				  module_inst_matrix_bin_name, 
                                                                                				  module_inst_matrix_bin, 
                                                                                				  module_inst_matrix_bin, 0, 0);
                                                                                module_inst_matrix_bin_name[strlen(module_inst_matrix_bin_name)-1]=0;
                                                                                sprintf(array_name,"[%s]", module_inst_matrix_bin_name);

                                                                                if (module_ids->children[2]->type==IDENTIFIERS)
                                                                                {
                                                                                    strcat(array_name,":" );
                                                                                    strcat(array_name, module_ids->children[2]->types.identifier);
                                                                                    strcat(array_name, "], ");
                                                                                }

                                                                                else if (module_ids->children[2]->type==NUMBERS)
                                                                                {
                                                                                    strcat(array_name,":" );
                                                                                    strcat(array_name, module_ids->children[2]->types.number.number);
                                                                                    strcat(array_name, "], ");
                                                                                }

                                                                            }

                                                                            strcat(module_instance, array_name);
                                                                        }

                                                                    }
                                                                }
                                                                global_id_1or2=cnt_id_1or2;
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }

                                        break;
                                    }
                            }
                        }
                    }
                    if (global_id_1or2!=1 && array_ref_flag==0)
                        module_instance[strlen(module_instance)-3]=0;
                    else if(global_id_1or2==1 && array_ref_flag==1 && format_1_or_2==2)
                    {
                        if (range_ref_flag==1)
                            module_instance[strlen(module_instance)-4]=0;
                        else
                            module_instance[strlen(module_instance)-5]=0;

                    }
                    else
                        module_instance[strlen(module_instance)-2]=0;
                    strcat(module_instance, ");");
                    fprintf(fp, "%s\n", module_instance);

                    break;
                }

            default:
                //oassert(FALSE);
                break;
        }


        if (node != NULL)
        {

            while (main_loop < node->num_children)
                //for (i = 0; i < node->num_children; i++)
            {
                parent_node = node;
                VerilogOutputAst_traverse_node(fp, node->children[main_loop], node, my_label);
                main_loop++;
            }
        }
        //fprintf(fp, "endmodule\n");
    }
}

void BeginBlock(ast_node_t *node, char* block, char* if_case, 
				char* bin_op, int cnt, int if_flag, int begin_cnt, 
				int block_indent, int block_cnt)
{

    int r=0;
    int i;
    int bin_op_cnt;
    int stat_cnt=0;
    int stat_cnt2=0;

    begin_cnt=0;
    block_cnt++;
    //int if_alert = if_flag;

    //for (r=0; r<node->num_children; r++)
    while (r<node->num_children)
    {
        ast_node_t *block_node = node->children[r];
        //call function here
        int cnt_block=0;
        //ast_node_t *recursive_bin_node;

        if (block_node==NULL) {}
        else if (block_node->type==IF)
        {
            //strcat(block, "\tbegin");
            //if_case="";
            //if_flag=1;
            if (parent_node->type!=IF)
            {
                IfElse(block_node,i, if_case, stat_cnt, 
                	   stat_cnt2, cnt_block, bin_op, 
                	   cnt, block_cnt);
                strcat(block, if_case);
                memset(if_case, 0, 4096);
                r=r+1;
            }
            else
                r=r+1;
        }

        else if (block_node->type==NON_BLOCKING_STATEMENT 
        		 || block_node->type==NON_BLOCKING_STATEMENT_SIGNED 
        		 || block_node->type==BLOCKING_STATEMENT)
        {

            if (parent_node->type!=IF)
            {
                BlockingNonBlockingStatements(block_node, block,cnt_block);
                strcat(block, "\n");
                r=r+1;
            }

            else
                r=r+1;
        }
        else if (block_node->type==FOR) //TODO: FIX THIS 
        {
            //			//memset(if_case, 0, 4096);
            //			char for_statement[4096]="";
            //			strcat(if_case, "\n\tfor (");
            //			int k;
            //			for (k=0; k<block_node->num_children; k++)
            //			{
            //				ast_node_t *recursive_for_node = block_node->children[k];
            //				if (recursive_for_node==NULL){}
            //				else if (recursive_for_node->type==BLOCKING_STATEMENT)
            //				{
            //					if (recursive_for_node->children[0]->type==IDENTIFIERS)
            //					{
            //						strcat(if_case,recursive_for_node->children[0]->types.identifier);
            //						strcat(if_case, "=");
            //					}
            //					if (recursive_for_node->children[1]->type==NUMBERS)
            //					{
            //						strcat(if_case, recursive_for_node->children[1]->types.number.number);
            //						strcat(if_case, "; ");
            //					}
            //					if (recursive_for_node->children[1]->type==BINARY_OPERATION)
            //					{
            //
            //						char block_bin_op[1]="";
            //						char block_bin_statement[4096]="";
            //						int dummy_block_count;
            //						BinaryOpsGenerator(recursive_for_node->children[1], block_bin_op);
            //						//RecursiveBinaryop(recursive_for_node->children[1], 
            //						//				block_bin_statement, block_bin_op, 
            //						//				block_bin_op, dummy_block_count);
            //						strcat(if_case, recursive_for_node->children[1]->children[0]->types.identifier);
            //						strcat(if_case, block_bin_op);
            //						strcat(if_case, recursive_for_node->children[1]->children[1]->types.number.number);
            //						strcat(if_case, ")");
            //
            //					}
            //
            //				}
            //
            //				else if (recursive_for_node->type==BINARY_OPERATION)
            //				{
            //					bin_op_cnt++;
            //					char bin_op[1]="";
            //					char for_bin_op[4096]="";
            //					int dummy_for_count;
            //					char if_case2[4096]="";
            //					BinaryOpsGenerator(recursive_for_node, for_bin_op);
            //					RecursiveBinaryop(recursive_for_node, if_case2, 
            //									  for_bin_op, for_bin_op, dummy_for_count);
            //					if (bin_op_cnt<=2)
            //					{
            //						memmove(if_case2, if_case2+2, 4096-2);
            //						if_case2[strlen(if_case2)-3]=0;
            //					}
            //						strcat(if_case, if_case2);
            //						strcat(if_case, "; ");
            //
            //				}
            //				else if (recursive_for_node->type==UNARY_OPERATION)
            //				{
            //					//UnaryOpsGenerator(block, bin_op);
            //					//RecursiveUnaryOp
            //				}
            //
            //				else if (recursive_for_node->type==BLOCK)
            //				{
            //					block_cnt++;
            //					int block_child_cnt;
            //					for (block_child_cnt=0; block_child_cnt<recursive_for_node->num_children; block_child_cnt++)
            //					{
            //						ast_node_t* block_child = recursive_for_node->children[block_child_cnt];
            //						if (block_child==NULL) {}
            //						else if (block_child->type==BLOCKING_STATEMENT 
            //								|| block_child->type==NON_BLOCKING_STATEMENT)
            //						{
            //							stat_cnt++;
            //							//if_flag=1;
            //							if (stat_cnt<=1)
            //								strcat(if_case, "\n\tbegin\n\t");
            //							BlockingNonBlockingStatements(block_child, if_case,cnt_block);
            //							//if (stat_cnt==recursive_bin_node->num_children -1)
            //							if (stat_cnt==recursive_for_node->num_children)
            //								strcat(if_case,"\n\tend\n");
            //							else
            //								strcat(if_case, "\n\t");
            //						}
            //						else if (block_child->type==IF)
            //						{
            //							if (k>1)
            //								strcat(if_case, "\n\telse");
            //							strcat(if_case, "\n\tbegin\n\t");
            //							IfElse(block_child, i, if_case, stat_cnt, 
            //								   cnt_block,  bin_op, cnt, block_cnt);
            //							strcat(if_case, "\n\tend\n\t");
            //						}
            //					}
            //				}
            //
            //			}
            ForLoop(block_node, if_case,  bin_op_cnt,  stat_cnt, stat_cnt2, bin_op,  cnt_block,  cnt,  i);
            strcat(block, if_case);
            memset(if_case, 0, 4096);
            r=r+1;
        }


        //else if (block_node->type!=IF)
        //{
        //strcat(if_case,"");
        //}
        //strcat(block, "\n");
    }
    //probably needs fixing. most certainly does!
    //if (strlen(if_case)==0)
    //	strcat(block, "end");

    //if ((begin_cnt!=1) && (r==node->num_children))
    //if (r==node->num_children)
    //strcat(block, "\nend\n");
}

void ForLoop(ast_node_t* block_node, char* if_case, 
			 int bin_op_cnt, int stat_cnt, int stat_cnt2,
			 int bin_op, int cnt_block, int cnt, int i)
{
    //memset(if_case, 0, 4096);
    char for_statement[4096]="";
    strcat(if_case, "\n\tfor (");
    int k;
    int expression_cnt=0;
    int bin_loc_cnt=0;
    stat_cnt=0;
    stat_cnt2=0;
    for (k=0; k<block_node->num_children; k++)
    {
        ast_node_t *recursive_for_node = block_node->children[k];
        if (recursive_for_node==NULL){}
        else if (recursive_for_node->type==BLOCKING_STATEMENT)
        {
            expression_cnt++;
            if (recursive_for_node->children[0]->type==IDENTIFIERS)
            {
                strcat(if_case,recursive_for_node->children[0]->types.identifier);
                strcat(if_case, "=");
            }
            if (recursive_for_node->children[1]->type==NUMBERS)
            {
                strcat(if_case, recursive_for_node->children[1]->types.number.number);
                strcat(if_case, "; ");
            }
            if (recursive_for_node->children[1]->type==BINARY_OPERATION)
            {
                bin_loc_cnt++;
                char block_bin_op[1]="";
                char block_bin_statement[4096]="";
                int dummy_block_count;
                BinaryOpsGenerator(recursive_for_node->children[1], block_bin_op);
                strcat(if_case, recursive_for_node->children[1]->children[0]->types.identifier);
                strcat(if_case, block_bin_op);
                strcat(if_case, recursive_for_node->children[1]->children[1]->types.number.number);
                strcat(if_case, "; ");
            }
        }

        else if (recursive_for_node->type==BINARY_OPERATION)
        {
            expression_cnt++;
            bin_op_cnt++;
            char bin_op[1]="";
            char for_bin_op[4096]="";
            int dummy_for_count;
            char if_case2[4096]="";
            BinaryOpsGenerator(recursive_for_node, for_bin_op);
            RecursiveBinaryop(recursive_for_node, if_case2, 
            				  for_bin_op, for_bin_op, dummy_for_count, 0);
            if (bin_op_cnt<=2)
            {
                memmove(if_case2, if_case2+2, 4096-2);
                if_case2[strlen(if_case2)-3]=0;
            }
            strcat(if_case, if_case2);
            strcat(if_case, "; ");

        }
        else if (recursive_for_node->type==UNARY_OPERATION)
        {
            //UnaryOpsGenerator(block, bin_op);
            //RecursiveUnaryOp
            expression_cnt++;
        }

        else if (recursive_for_node->type==BLOCK)
        {
            expression_cnt++;
            block_cnt++;
            int block_child_cnt;
            for (block_child_cnt=0; block_child_cnt<recursive_for_node->num_children; block_child_cnt++)
            {
                ast_node_t* block_child = recursive_for_node->children[block_child_cnt];
                if (block_child==NULL) {}
                else if (block_child->type==BLOCKING_STATEMENT 
                		 || block_child->type==NON_BLOCKING_STATEMENT 
                		 || block_child->type==NON_BLOCKING_STATEMENT_SIGNED)
                {
                    stat_cnt++;
                    //if_flag=1;
                    if (stat_cnt<=1)
                        strcat(if_case, "\n\tbegin\n\t");
                    BlockingNonBlockingStatements(block_child, if_case,cnt_block);
                    if (stat_cnt==recursive_for_node->num_children)
                        strcat(if_case,"\n\tend\n");
                    else
                        strcat(if_case, "\n\t");
                }
                else if (block_child->type==IF)
                {
                    if (k>1)
                        strcat(if_case, "\n\telse");
                    strcat(if_case, "\n\tbegin\n\t");
                    IfElse(block_child, i, if_case, stat_cnt, 
                    	   stat_cnt2, cnt_block,  bin_op, cnt, 
                    	   block_cnt);
                    strcat(if_case, "\n\tend\n\t");
                }
            }
        }
        if (expression_cnt==3)
        {
            if_case[strlen(if_case)-2]=0;
            strcat(if_case, ")");
        }
    }

}

void IfElse(ast_node_t* block_node,int i, char* if_case, 
			int stat_cnt, int stat_cnt2, int cnt_block, 
			char* bin_op, int cnt, int block_cnt)
{

    int k=0, t;
    char block[4096]="";
    int block_indent;
    int block_child_cnt;
    int check=0;
    int un_op_check;
    //stat_cnt=0;
    //stat_cnt2=0;
    //if(k>0 && block_node->children[k-1]->type!=UNARY_OPERATION)
    strcat(if_case, "\n\tif ");
    for (k=0; k<block_node->num_children; k++)
    {
        ast_node_t *recursive_bin_node = block_node->children[k];
        if (recursive_bin_node==NULL){}
        else if (recursive_bin_node->type==BINARY_OPERATION)
        {
            if (k>1)
                strcat(if_case, "\n\telse");
            BinaryOpsGenerator(recursive_bin_node, bin_op);
            RecursiveBinaryop(recursive_bin_node, if_case, bin_op, bin_op, cnt, 0);
            if_case[strlen(if_case)-2]=0;
            //strcat(if_case, "\n\tbegin");
        }
        else if (recursive_bin_node->type==UNARY_OPERATION)
        {
            if (k>1)
                strcat(if_case, "\n\telse");

            UnaryOpsGenerator(recursive_bin_node, bin_op);
        }

        else if (recursive_bin_node->type==IDENTIFIERS && k<=1)
        {
            //strcat(if_case, "\n\tif "); //fix this for UNARY OPERATOR within IF
            strcat(if_case, " (");
            strcat(if_case, recursive_bin_node->types.identifier);
            strcat(if_case, ")");
            un_op_check=0;
            //strcat(if_case, "\n\tbegin");
        }

        else if (recursive_bin_node->type==BLOCKING_STATEMENT 
        		 || recursive_bin_node->type==NON_BLOCKING_STATEMENT 
        		 || recursive_bin_node->type==NON_BLOCKING_STATEMENT_SIGNED)
        {
            if (k>1)
                strcat(if_case,"\n\telse");
            BlockingNonBlockingStatements(recursive_bin_node, if_case, cnt_block);
            //strcat()
        }

        else if(recursive_bin_node->type==IF)
        {
            strcat(if_case, "\n\telse ");
            IfElse(recursive_bin_node, i, if_case, stat_cnt, 
            	   stat_cnt2, cnt_block,  bin_op, cnt, block_cnt);
        }

        else if (recursive_bin_node->type==BLOCK)
        {
            block_cnt++;
            if (k>1)
                strcat(if_case, "\n\telse\n\tbegin");
            else
                strcat(if_case, "\n\tbegin");


            for (block_child_cnt=0; block_child_cnt<recursive_bin_node->num_children; block_child_cnt++)
            {
                //stat_cnt2=0;
                ast_node_t* block_child = recursive_bin_node->children[block_child_cnt];
                if (block_child==NULL) {}
                else if (block_child->type==BLOCKING_STATEMENT 
                		 || block_child->type==NON_BLOCKING_STATEMENT 
                		 || block_child->type==NON_BLOCKING_STATEMENT_SIGNED)
                {
                    stat_cnt++;
                    strcat(if_case, "\n\t");
                    BlockingNonBlockingStatements(block_child, if_case,cnt_block);
                }
                else if (block_child->type==IF)
                {
                    //if (k>1)
                    //strcat(if_case, "\n\telse");
                    stat_cnt++;
                    //strcat(if_case, "\n\tbegin\n\t");
                    IfElse(block_child, i, if_case, stat_cnt, stat_cnt2, 
                    	   cnt_block,  bin_op, cnt, block_cnt);
                }

                else if (block_child->type==FOR)
                {
                    stat_cnt++;
                    int bin_op_cnt;

                    //if (stat_cnt2<=1)
                    //	strcat(if_case, "\n\tbegin\n\t");
                    ForLoop(block_child, if_case,  bin_op_cnt,  stat_cnt, 
                    		stat_cnt2,  bin_op,  cnt_block,  cnt,  i);
                }
            }
            check = TraverseTree(recursive_bin_node, 0);
            if (check)
                strcat(if_case, "\n\tend\n\t");
        }
    }

}

void ConcatNodes(ast_node_t* node, char * concat_id)
{
    int mc;
    for (mc=0; mc<node->num_children;mc++)
    {
        ast_node_t* concat_node = node->children[mc];
        if (concat_node==NULL){}
        else
        {
            if(concat_node->type==NUMBERS)
            {
                if (mc==0)
                    strcat(concat_id, "{");
                if(concat_node->types.number.base==3 
                   || concat_node->types.number.base==0 
                   || concat_node->types.number.base==5 
                   || concat_node->types.number.base==8)
                {
                    char concat_constant_size[4096]="";
                    sprintf(concat_constant_size, "%d'b", 
                    		concat_node->types.number.binary_size);
                    strcat(concat_id, concat_constant_size );
                }
                strcat(concat_id, concat_node->types.number.number);
                strcat(concat_id, ",");


            }
            else if(concat_node->type==IDENTIFIERS)
            {
                if (mc==0)
                    strcat(concat_id, "{");
                strcat(concat_id, concat_node->types.identifier);
                strcat(concat_id, ",");
            }

            else if (concat_node->type==ARRAY_REF)
            {
                int i;
                for(i=0; i<concat_node->num_children; i++)
                {
                    ast_node_t* concat_node_child = concat_node->children[i];
                    if (concat_node_child==NULL){}
                    else if (concat_node_child->type==IDENTIFIERS)
                    {
                        //if (i==1)
                        strcat(concat_id,"{");
                        strcat(concat_id, concat_node->children[i]->types.identifier);
                        strcat(concat_id, "[");
                    }
                    else if (concat_node_child->type==NUMBERS)
                    {
                        strcat(concat_id, concat_node->children[i]->types.number.number);
                        strcat(concat_id, "], ");
                    }
                }
            }

            else if (concat_node->type==MATRIX_REF)
            {
                int i;
                for(i=0; i<concat_node->num_children; i++)
                {
                    ast_node_t* concat_node_child = concat_node->children[i];
                    if (concat_node_child==NULL){}
                    else if (concat_node_child->type==IDENTIFIERS)
                    {
                        if (i==0)
                            strcat(concat_id,"{");
                        strcat(concat_id, concat_node_child->types.identifier);
                        strcat(concat_id, "[");
                    }
                    else if (concat_node_child->type==NUMBERS)
                    {
                        if (i==2 || i==3)
                            strcat(concat_id, "[");
                        strcat(concat_id, concat_node_child->types.number.number);
                        if (i==1 || i==2)
                            strcat(concat_id, "]");
                        if (i==3)
                            strcat(concat_id, ":");
                        if  (i==4)
                            strcat(concat_id, "], ");
                    }
                }
            }

            else if (concat_node->type==RANGE_REF)
            {
                int i;
                for(i=0; i<concat_node->num_children; i++)
                {
                    ast_node_t* concat_node_child = concat_node->children[i];
                    if (concat_node_child==NULL){}
                    else if (concat_node_child->type==IDENTIFIERS)
                    {
                        if (i==0)
                            strcat(concat_id,"{");
                        strcat(concat_id, concat_node->children[i]->types.identifier);
                        strcat(concat_id, "[");
                    }
                    else if (concat_node_child->type==NUMBERS)
                    {
                        strcat(concat_id, concat_node->children[i]->types.number.number);
                        if (i==1)
                            strcat(concat_id, ":");
                        else if (i==2)
                            strcat(concat_id, "], ");

                    }

                    else if (concat_node_child->type==BINARY_OPERATION)
                    {

                        if (i==0)
                            strcat(concat_id, "{");
                        char bin_op_here[4096]="";
                        char concat_bin_op[4096]="";
                        BinaryOpsGenerator(concat_node_child, bin_op_here);
                        sprintf(concat_bin_op, "(%s %s %s):", 
                        		concat_node_child->children[0]->types.identifier, 
                        		bin_op_here, concat_node_child->children[1]->types.number.number);
                        //concat_bin_op[strlen(concat_bin_op)-2] = 0;
                        strcat(concat_id, concat_bin_op);
                    }
                }
            }

            else if (concat_node->type==ARRAY_RANGE_REF)
            {
                int i;
                for(i=0; i<concat_node->num_children; i++)
                {
                    ast_node_t* concat_node_child = concat_node->children[i];
                    if (concat_node_child==NULL){}
                    else if (concat_node_child->type==IDENTIFIERS)
                    {

                        if (i==0)
                            strcat(concat_id,"{");
                        strcat(concat_id, concat_node->children[i]->types.identifier);
                        strcat(concat_id, "[");
                    }
                    else if (concat_node_child->type==NUMBERS)
                    {
                        strcat(concat_id, concat_node->children[i]->types.number.number);
                        if (i==1)
                            strcat(concat_id, "] [");
                        else if (i==2)
                            strcat(concat_id, ":");
                        else if (i==3)
                            strcat(concat_id, "], ");

                    }

                    else if (concat_node_child->type==BINARY_OPERATION)
                    {
                        if (i==0)
                            strcat(concat_id, "{");
                        char bin_op_here[4096]="";
                        char concat_bin_op[4096]="";
                        BinaryOpsGenerator(concat_node_child, bin_op_here);
                        if (i==1)
                            sprintf(concat_bin_op, "(%s %s %s)] [", 
                            		concat_node_child->children[0]->types.identifier, 
                            		bin_op_here, 
                            		concat_node_child->children[1]->types.number.number);
                        else
                            sprintf(concat_bin_op, "(%s %s %s):", 
                            		concat_node_child->children[0]->types.identifier, 
                            		bin_op_here, 
                            		concat_node_child->children[1]->types.number.number);

                        //concat_bin_op[strlen(concat_bin_op)-2] = 0;
                        strcat(concat_id, concat_bin_op);

                    }
                }
            }

            else if (concat_node->type==CONCATENATE)
            {
                ConcatNodes(concat_node, concat_id);
            }
        }
        //break;
    }
    concat_id[strlen(concat_id)-1]=0;
    strcat(concat_id, "} ");
}

void BlockingNonBlockingStatements(ast_node_t *newnode, char *assign_var, int count_op_depth)
{
    //int string_size = 4096;
    //char *tmp;
    memset(bin_op_exception, 0, sizeof(bin_op_exception));
    memset(identif2, 0, sizeof(identif2));
    shift_flag=0;

    if (newnode->type==ASSIGN || newnode->type==BLOCKING_STATEMENT
            || newnode->type==NON_BLOCKING_STATEMENT 
            || newnode->type==NON_BLOCKING_STATEMENT_SIGNED
            || newnode->type==UNARY_OPERATION)
    {
        int j;int id_cnt=0;
        char assign_num[4096]="";
        for (j=0; j<newnode->num_children; j++)
        {
            ast_node_t *tempnode = newnode->children[j];
            int unary_flag;
            if (tempnode==NULL){}
            else
            {

                switch(tempnode->type)
                {
                    case CONCATENATE:
                        {
                            char concat_id[4096]="";
                            ConcatNodes(tempnode, concat_id);
                            strcat(assign_var, "(");
                            strcat(assign_var, concat_id);
                            assign_var[strlen(assign_var)-1]=0;
                            strcat(assign_var, ")");

                            if (newnode->type==UNARY_OPERATION)
                                strcat(assign_var, " )");
                            strcat(assign_var, ";\n");
                            break;
                        }
                    case IDENTIFIERS:
                        {
                            id_cnt++;

                            char assign_id[4096]="";

                            ast_node_t* temp_array_check=newnode->children[j];
                            if (j>0)
                                temp_array_check=newnode->children[j-1];

                            if (id_cnt<=1 && temp_array_check->type!=ARRAY_REF 
                            			  && newnode->type!=UNARY_OPERATION) 
                            {
                                if (newnode->type==BLOCKING_STATEMENT)
                                    sprintf(assign_id,"\t%s = ", tempnode->types.identifier);
								//need to fix this tab situation
                                else if(newnode->type==NON_BLOCKING_STATEMENT)
                                    sprintf(assign_id, "\t%s <= ", tempnode->types.identifier); 
								//need to fix this tab situation
                                else if (newnode->type==NON_BLOCKING_STATEMENT_SIGNED)
                                    sprintf(assign_id, "\t%s <= $signed ", tempnode->types.identifier); 

                                strcat(assign_var, assign_id);
                            }

                            else
                            {
                                sprintf(assign_num, "(%s)", tempnode->types.identifier);
                                strcat(assign_var, assign_num);
                                if (tempnode->num_children==0)
                                {
                                    if (newnode->type==UNARY_OPERATION)
                                        strcat(assign_var, " )");
                                    strcat(assign_var, "; ");
                                }
                            }
                            break;
                        }

                case NUMBERS:
                        {
                            if (tempnode->types.number.base ==BIN 
                            	|| tempnode->types.number.base ==SBIN)
                                sprintf(assign_num,"%d'b%s", 
                                		tempnode->types.number.binary_size, 
                                		tempnode->types.number.number);
                            else
                                sprintf(assign_num, "%d", tempnode->types.number.value);
                            strcat(assign_var, assign_num);
                            if(tempnode->num_children==0)
                            {
                                if (newnode->type==UNARY_OPERATION)
                                    strcat(assign_var, " )");
                                strcat(assign_var, ";");

                            }
                            break;
                        }

                case UNARY_OPERATION:
                        {
                            char unary_op_here[4096]="";
                            UnaryOpsGenerator(tempnode, unary_op_here);
                            strcat(assign_var, unary_op_here);
                            strcat(assign_var, " (");
                            BlockingNonBlockingStatements(tempnode, assign_var, count_op_depth);
                            break;
                        }

                case BINARY_OPERATION:
                        {
                            int cnt_bin;
                            count_op_depth++;
                            char bin_op_here[4096]="";
                            BinaryOpsGenerator(tempnode, bin_op_here);
                            RecursiveBinaryop(tempnode, assign_var, 
                            				  bin_op_here, 
                            				  bin_op_here,cnt_bin,0);
                            assign_var[strlen(assign_var)-2] = 0;

                            //srand(time(NULL));
                            //int lottery = rand() % 3;
                            //if (lottery==0)
                            //AddSameBinaryExpression(tempnode, assign_var);
                            //else
                            //DeleteBinaryExpression(tempnode, assign_var);

                            if (newnode->type==NON_BLOCKING_STATEMENT 
                            	|| newnode->type==NON_BLOCKING_STATEMENT_SIGNED 
                            	|| newnode->type==BLOCKING_STATEMENT)
                                strcat(assign_var, ";\n");
                            break;
                        }

                case ARRAY_REF:
                        {
                            char array_ref_id[4096]="";
                            char array_ref_val[4096]="";
                            int i; int num_count; int cnt_bin; int cnt_identifiers=0;
                            for (i=0; i<tempnode->num_children;i++)
                            {
                                ast_node_t* array_ref_child=tempnode->children[i];
                                if (array_ref_child==NULL){}
                                else
                                {
                                    if (array_ref_child->type==IDENTIFIERS)
                                    {
                                        cnt_identifiers++;
                                        if (cnt_identifiers<=1)
                                            sprintf(array_ref_id," %s", array_ref_child->types.identifier);
                                        else if(cnt_identifiers==2)
                                        {
                                            strcat(array_ref_id, " [");
                                            strcat(array_ref_id, array_ref_child->types.identifier);
                                            strcat(array_ref_id, "]");
                                        }

                                    }
                                    else if(array_ref_child->type==NUMBERS 
                                    		|| array_ref_child->type==PARAMETER)
                                    {
                                        if (array_ref_child->types.number.base==BIN 
                                        	|| array_ref_child->types.number.base==SBIN)
                                            sprintf(array_ref_val, "[%d'b%s]", 
                                            		array_ref_child->types.number.binary_size, 
                                            		array_ref_child->types.number.number);
                                        else
                                            sprintf(array_ref_val, "[%d]", 
                                            		array_ref_child->types.number.value);

                                    }
                                    else if(array_ref_child->type==BINARY_OPERATION)
                                    {
                                        char bin_op_arrayref[4096]="";
                                        strcat(array_ref_val, " [");
                                        BinaryOpsGenerator(array_ref_child, bin_op_arrayref);
                                        RecursiveBinaryop(array_ref_child, 
                                        				  array_ref_val, bin_op_arrayref, 
                                        				  bin_op_arrayref, cnt_bin, 0);
                                        array_ref_val[strlen(array_ref_val)-1]=0;
                                        strcat(array_ref_val,"]");
                                    }
                                }
                            }
                            strcat(array_ref_id, array_ref_val);
                            if (j!=newnode->num_children-1 
                            	&& (newnode->children[j+1]->type==IDENTIFIERS 
                            		 || newnode->children[j+1]->type==ARRAY_REF 
                            		 || newnode->children[j+1]->type==ARRAY_REF 
                            		 || newnode->children[j+1]->type==BINARY_OPERATION 
                            		 || newnode->children[j+1]->type==UNARY_OPERATION 
                            		 || newnode->children[j+1]->type==NUMBERS 
                            		 || newnode->children[j+1]->type==CONCATENATE))
                            {
                                if (newnode->type==NON_BLOCKING_STATEMENT)
                                    strcat(array_ref_id, " <= ");
                                else if (newnode->type==NON_BLOCKING_STATEMENT_SIGNED)
                                    strcat(array_ref_id, " <= $signed ");

                                else if (newnode->type==BLOCKING_STATEMENT)
                                    strcat(array_ref_id, " = ");
                            }
                            else
                            {
                                if (newnode->type==UNARY_OPERATION)
                                    strcat(array_ref_id, " )");
                                strcat(array_ref_id, ";\n");
                            }

                            strcat(assign_var, array_ref_id);
                            break;
                        }

                case RANGE_REF:
                        {
                            char array_ref_id[4096]="";
                            char array_ref_val[4096]="";
                            int i; int num_count; int cnt_bin=0; int cnt_identifiers=0; int cnt_numbers=0;
                            for (i=0; i<tempnode->num_children;i++)
                            {
                                ast_node_t* array_ref_child=tempnode->children[i];
                                if (array_ref_child==NULL){}
                                else
                                {
                                    if (array_ref_child->type==IDENTIFIERS)
                                    {
                                        cnt_identifiers++;
                                        if (cnt_identifiers<=1)
                                            sprintf(array_ref_id," %s", array_ref_child->types.identifier);
                                        else if(cnt_identifiers==2)
                                        {
                                            strcat(array_ref_id, " [");
                                            strcat(array_ref_id, array_ref_child->types.identifier);
                                            strcat(array_ref_id, ":");
                                        }
                                        else if (cnt_identifiers==3)
                                        {
                                            strcat(array_ref_id, array_ref_child->types.identifier);
                                            strcat(array_ref_id, "]");
                                        }

                                    }
                                    else if(array_ref_child->type==NUMBERS 
                                    		|| array_ref_child->type==PARAMETER)
                                    {
                                        cnt_numbers++;
                                        if (cnt_numbers<=1)
                                            sprintf(array_ref_val, "[%d:", array_ref_child->types.number.value);
                                        else
                                        {
                                            if (i>0 && tempnode->children[i-1]->type!=NUMBERS)
                                                strcat(array_ref_val, ":");
                                            strcat(array_ref_val, array_ref_child->types.number.number);
                                            strcat(array_ref_val, "]");
                                        }
                                    }
                                    else if(array_ref_child->type==BINARY_OPERATION)
                                    {
                                        cnt_numbers++;
                                        char bin_op_arrayref[4096]="";
                                        if (cnt_numbers%2==1)
                                            strcat(array_ref_val, "[");
                                        BinaryOpsGenerator(array_ref_child, bin_op_arrayref);
                                        RecursiveBinaryop(array_ref_child, array_ref_val, 
                                        				  bin_op_arrayref, bin_op_arrayref, cnt_bin, 0);
                                        array_ref_val[strlen(array_ref_val)-1]=0;
                                        if (cnt_numbers%2==0)
                                            strcat(array_ref_val,"]");
                                    }
                                }
                            }
                            strcat(array_ref_id, array_ref_val);
                            if (j!=newnode->num_children-1 
                            	&& (newnode->children[j+1]->type==IDENTIFIERS 
                            		|| newnode->children[j+1]->type==ARRAY_REF  
                            		|| newnode->children[j+1]->type==RANGE_REF
                            		|| newnode->children[j+1]->type==BINARY_OPERATION 
                            		|| newnode->children[j+1]->type==UNARY_OPERATION 
                            		|| newnode->children[j+1]->type==NUMBERS 
                            		|| newnode->children[j+1]->type==CONCATENATE))
                            {
                                if (newnode->type==NON_BLOCKING_STATEMENT)
                                    strcat(array_ref_id, " <= ");
                                else if (newnode->type==NON_BLOCKING_STATEMENT_SIGNED)
                                    strcat(array_ref_id, " <= $signed ");

                                else if (newnode->type==BLOCKING_STATEMENT)
                                    strcat(array_ref_id, " = ");
                            }
                            else
                            {
                                if (newnode->type==UNARY_OPERATION)
                                    strcat(array_ref_id, " )");
                                strcat(array_ref_id, ";\n");
                            }
                            strcat(assign_var, array_ref_id);
                            break;
                        }

                case ARRAY_RANGE_REF:
                        {
                            char array_ref_id[4096]="";
                            char array_ref_val[4096]="";
                            int i; int num_count; int cnt_bin=0; 
                            int cnt_identifiers=0; int cnt_numbers=0;
                            for (i=0; i<tempnode->num_children;i++)
                            {
                                ast_node_t* array_ref_child=tempnode->children[i];
                                if (array_ref_child==NULL){}
                                else
                                {
                                    if (array_ref_child->type==IDENTIFIERS)
                                    {
                                        cnt_identifiers++;
                                        if (cnt_identifiers<=1)
                                            sprintf(array_ref_id," %s", array_ref_child->types.identifier);
                                        else if(cnt_identifiers==2)
                                        {
                                            strcat(array_ref_id, " [");
                                            strcat(array_ref_id, array_ref_child->types.identifier);
                                            strcat(array_ref_id, "]");
                                        }

                                        else if(cnt_identifiers==3)
                                        {
                                            strcat(array_ref_id, " [");
                                            strcat(array_ref_id, array_ref_child->types.identifier);
                                            strcat(array_ref_id, ":");
                                        }
                                        else if (cnt_identifiers==4)
                                        {
                                            strcat(array_ref_id, array_ref_child->types.identifier);
                                            strcat(array_ref_id, "]");
                                        }

                                    }
                                    else if(array_ref_child->type==NUMBERS 
                                    		|| array_ref_child->type==PARAMETER)
                                    {
                                        cnt_numbers++;
                                        if (cnt_numbers<=1)
                                            sprintf(array_ref_val, "[%d]", array_ref_child->types.number.value);
                                        else if (cnt_numbers==2)
                                        {
                                            strcat(array_ref_val, "[");
                                            strcat(array_ref_val, array_ref_child->types.number.number);
                                            strcat(array_ref_val, ":");
                                        }
                                        else
                                        {
                                            if (i>0 && tempnode->children[i-1]->type!=NUMBERS)
                                                strcat(array_ref_val, ":");
                                            strcat(array_ref_val, array_ref_child->types.number.number);
                                            strcat(array_ref_val, "]");
                                        }
                                    }
                                    else if(array_ref_child->type==BINARY_OPERATION)
                                    {
                                        cnt_numbers++;
                                        char bin_op_arrayref[4096]="";
                                        if (cnt_numbers%2==1 || cnt_numbers%2==0)
                                            strcat(array_ref_val, "[");
                                        BinaryOpsGenerator(array_ref_child, bin_op_arrayref);
                                        RecursiveBinaryop(array_ref_child, array_ref_val, bin_op_arrayref, bin_op_arrayref, cnt_bin, 0);
                                        array_ref_val[strlen(array_ref_val)-1]=0;
                                        if (cnt_numbers%2==0 || cnt_numbers%2==1)
                                            strcat(array_ref_val,"]");
                                    }
                                }
                            }
                            strcat(array_ref_id, array_ref_val);
                            if (j!=newnode->num_children-1 
                            	&& (newnode->children[j+1]->type==IDENTIFIERS 
                            		|| newnode->children[j+1]->type==ARRAY_REF  
                            		|| newnode->children[j+1]->type==ARRAY_RANGE_REF  
                            		|| newnode->children[j+1]->type==MATRIX_REF  
                            		|| newnode->children[j+1]->type==RANGE_REF
                            		|| newnode->children[j+1]->type==BINARY_OPERATION 
                            		|| newnode->children[j+1]->type==UNARY_OPERATION 
                            		|| newnode->children[j+1]->type==NUMBERS 
                            		|| newnode->children[j+1]->type==CONCATENATE))
                            {
                                if (newnode->type==NON_BLOCKING_STATEMENT)
                                    strcat(array_ref_id, " <= ");
                                else if (newnode->type==NON_BLOCKING_STATEMENT_SIGNED)
                                    strcat(array_ref_id, " <= $signed ");

                                else if (newnode->type==BLOCKING_STATEMENT)
                                    strcat(array_ref_id, " = ");
                            }
                            else
                            {
                                if (newnode->type==UNARY_OPERATION)
                                    strcat(array_ref_id, " )");
                                strcat(array_ref_id, ";\n");
                            }

                            strcat(assign_var, array_ref_id);

                            break;
                        }


                case IF_Q:
                        {
                            char if_q_statement[4096]="";
                            sprintf(if_q_statement, "%s ? %s : %s;\n", 
                            		tempnode->children[0]->types.identifier, 
                            		tempnode->children[1]->types.identifier, 
                            		tempnode->children[2]->types.identifier);
                            strcat(assign_var, if_q_statement);

                            break;
                        }
                        //strcat(assign_var, "\n");
                }
            }
        }
    }
}

void UnaryOpsGenerator(ast_node_t *node, char *unary_op)
{
    switch (node->types.operation.op)
    {
        case ADD:
            sprintf(unary_op,"+");
            break;
        case MINUS:
            sprintf(unary_op, "-");
            break;
        case BITWISE_NOT:
            sprintf(unary_op, "~");
            break;
        case BITWISE_AND:
            sprintf(unary_op, "&");
            break;
        case BITWISE_OR:
            sprintf(unary_op, "|");
            break;
        case BITWISE_NAND:
            sprintf(unary_op, "~&");
            break;
        case BITWISE_NOR:
            sprintf(unary_op, "~|");
            break;
        case BITWISE_XNOR:
            sprintf(unary_op, "~^");
            break;
        case BITWISE_XOR:
            sprintf(unary_op, "^");
            break;
        case LOGICAL_NOT:
            sprintf(unary_op, "!");
            break;
        default:
            break;
    }
}


void AddSameBinaryExpression(ast_node_t *node, char* statement_text)
{
    //srand(time(NULL));
    int ran;
    ran = rand() % (node->num_children);
    char bin_op_to_add[4096]="";
    BinaryOpsGenerator(node, bin_op_to_add);
    strcat(statement_text, bin_op_to_add);
    strcat(statement_text, " ");
    strcat(statement_text, node->children[1]->types.identifier);
}

void DeleteBinaryExpression(ast_node_t *node, char* statement_text)
{
    //srand(time(NULL));
    int ran = rand() % (node->num_children);
    char *ptr = strchr(statement_text, '=');
    int index1=0;
    index1 =  ptr - statement_text;
    statement_text[index1+1] = 0;
    strcat(statement_text, " ");
    strcat(statement_text, node->children[ran]->types.identifier);
}

void BinaryOpsGenerator(ast_node_t *node, char *bin_op)
{
    switch(node->types.operation.op)
    {
        case ADD:
            {
                sprintf(bin_op, " +");
                //strcat(assign_var,bin_op);
                break;
            }

        case MINUS:
            {
                sprintf(bin_op, " -");
                //strcat(assign_var,bin_op);
                break;
            }

        case BITWISE_NOT:
            {
                sprintf(bin_op, " ~");
                //strcat(assign_var,bin_op);
                break;
            }

        case BITWISE_AND:
            {
                sprintf(bin_op, " &");
                //strcat(assign_var,bin_op);
                break;
            }

        case BITWISE_OR:
            {
                sprintf(bin_op, " |");
                //strcat(assign_var,bin_op);
                break;
            }

        case BITWISE_NAND:
            {
                sprintf(bin_op, " ~&");
                //strcat(assign_var,bin_op);
                break;
            }

        case BITWISE_NOR:
            {
                sprintf(bin_op, " ~|");
                //strcat(assign_var,bin_op);
                break;
            }

        case BITWISE_XNOR:
            {
                sprintf(bin_op, " ~^");
                //strcat(assign_var,bin_op);
                break;
            }

        case BITWISE_XOR:
            {
                sprintf(bin_op, " ^");
                //strcat(assign_var,bin_op);
                break;
            }

        case LOGICAL_NOT:
            {
                sprintf(bin_op, " !");
                //strcat(assign_var,bin_op);
                break;
            }

        case LOGICAL_OR:
            {
                sprintf(bin_op, " ||");
                //strcat(assign_var,bin_op);
                break;
            }

        case LOGICAL_AND:
            {
                sprintf(bin_op, " &&");
                //strcat(assign_var,bin_op);
                break;
            }

        case MULTIPLY:
            {
                sprintf(bin_op, " *");
                //strcat(assign_var,bin_op);
                break;
            }

        case DIVIDE:
            {
                sprintf(bin_op, " /");
                //strcat(assign_var,bin_op);
                break;
            }

        case MODULO:
            {
                sprintf(bin_op, " %");
                //strcat(assign_var,bin_op);
                break;
            }

        case LT:
            {
                sprintf(bin_op, " <");
                break;
            }

        case GT:
            {
                sprintf(bin_op, " >");
                break;
            }

        case LOGICAL_EQUAL:
            {
                sprintf(bin_op, " ==");
                break;
            }

        case NOT_EQUAL:
            {
                sprintf(bin_op, " !=");
                break;
            }

        case LTE:
            {
                sprintf(bin_op, " <=");
                break;
            }

        case GTE:
            {
                sprintf(bin_op, " >=");
                break;
            }

        case SR:
            {
                sprintf(bin_op, " >>");
                break;
            }

        case SL:
            {
                sprintf(bin_op, " <<");
                break;
            }

        case CASE_EQUAL:
            {
                sprintf(bin_op, " ===");
                break;
            }

        case CASE_NOT_EQUAL:
            {
                sprintf(bin_op, " !==");
                break;
            }
    }
}

/*NEW FUNCTION*/

void RecursiveBinaryop(ast_node_t *node, char *assign_var, 
				       char* root_bin_op, char*parent_bin_op, 
				       int cnt, int cnt0)
{
    char bin_op[4096]="";
    //int cnt0;
    int cnt_temp=0;
    int cnt_temp0=0;
    int cnt_bin;

    char array_ref_identifier[4096]="";
    int cnt_array_ref_num_nodes;
    char array_ref_range[4096]="";
    int array_ref_flag=0;
    int i;
    int bin_array_ref_flag;

    char range_ref_range[16384]="";
    char range_ref_identifier[4096]="";
    int bin_range_ref_flag;
    int cnt_range_ref_num_nodes;

    char matrix_ref_range[8193]="";
    char matrix_ref_identifier[4096]="";
    int bin_matrix_ref_flag=0;
    int cnt_matrix_ref_num_nodes;
    int id_cnt;

    ast_node_t * recursive_bin_parent_node=node;

    BinaryOpsGenerator(node, bin_op);

    int m=0;
    for (m=0; m<node->num_children; m++)
    {
        ast_node_t *node_child = node->children[m];
        if (node_child==NULL){}
        else
        {
            if (node->num_children==2 
            	&& node->children[0]->type==BINARY_OPERATION 
            	&& node->children[1]->type==BINARY_OPERATION)
            {
                if ((node->children[0]->types.operation.op==SL 
                	  && node->children[1]->types.operation.op==SL) 
                	 || (node->children[0]->types.operation.op==SR 
                	  && node->children[1]->types.operation.op==SR))
                {
                    two_shift_children_flag=1;
                }
            }

            switch(node_child->type)
            {
                case ARRAY_REF:
                    {
                        if (recursive_bin_parent_node->type==CONCATENATE)
                        {

                            break;
                        }

                        cnt++;
                        cnt_temp++;

                        memset(array_ref_range, 0, 4096);	//SEG FAULT was 8193
                        int m_sub; int cnt_identifier2=0;
                        for (m_sub=0; m_sub<node_child->num_children; m_sub++)
                        {
                            ast_node_t *array_ref_node = node_child->children[m_sub];
                            if (array_ref_node==NULL){}
                            else
                            {
                                ast_node_t *array_ref_node2;
                                if (m_sub!=node_child->num_children-1)

                                {
                                    array_ref_node2 = node_child->children[m_sub+1];									}

                                    if (array_ref_node->type==IDENTIFIERS)
                                    {

                                        if (m_sub>0)
                                        {
                                            strcat(array_ref_identifier, "[");
                                            strcat(array_ref_identifier, array_ref_node->types.identifier);
                                            strcat(array_ref_identifier, "]");
                                        }
                                        else if  (bin_array_ref_flag!=1 
                                        		  && array_ref_node2->type!=NUMBERS 
                                        		  && array_ref_node2->type!=IDENTIFIERS 
                                        		  && array_ref_node2->type!=BINARY_OPERATION)
                                        {
                                            sprintf(array_ref_identifier, "(%s", 
                                            		array_ref_node->types.identifier);
                                            bin_array_ref_flag=0;
                                        }
                                        else{
                                            sprintf(array_ref_identifier, "%s", 
                                            		array_ref_node->types.identifier);
                                        }
                                    }
                                    else if(array_ref_node->type==NUMBERS)
                                    {

                                        cnt_array_ref_num_nodes++;
                                        if ((cnt_array_ref_num_nodes==1) || (cnt_array_ref_num_nodes%2)==0)
                                            sprintf(array_ref_range,"[%d]",
                                            		array_ref_node->types.number.value);
                                        else
                                        {
                                            strcat(array_ref_range,":");
                                            strcat(array_ref_range, array_ref_node->types.number.value);
                                            strcat(array_ref_range, "]");
                                        }

                                    }
                                    //else if add bin_op case
                                    else if (array_ref_node->type==BINARY_OPERATION)
                                    {
                                        char parent_bin_op_arrayref[4096]="";
                                        int dummy_cnt_bin_op;
                                        strcat(array_ref_range, "[");
                                        bin_array_ref_flag=1;
                                        BinaryOpsGenerator(array_ref_node, parent_bin_op_arrayref);
                                        RecursiveBinaryop(array_ref_node, array_ref_range, 
                                        				  parent_bin_op_arrayref, 
                                        				  parent_bin_op_arrayref, 
                                        				  dummy_cnt_bin_op, 0);
                                        array_ref_range[strlen(array_ref_range)-1]=0;
                                        strcat(array_ref_range, "]");
                                    }
                                    cnt_array_ref_num_nodes=0;


                            }
                        }

                        strcat(array_ref_identifier, array_ref_range);

                        //this is where you need to take care of parenthesis as with the identifiers

                        char assign_in[4096]="";
                        char identif[8193]="";

                        if ((m==node->num_children-1) || m>=1)
                            //if (node_child->children[m+1]==NULL)
                        {

                            strcat(identif, array_ref_identifier);

                            if (node->types.operation.op == SR 
                            	|| node->types.operation.op == SL 
                            	|| node->types.operation.op==MINUS 
                            	|| node->types.operation.op==DIVIDE)
                            {
                                //sprintf(identif2, "%s", node_child->types.identifier);
                                identif2[cnt][0]=array_ref_identifier;
                                char temp[10]="";
                                BinaryOpsGenerator(node, temp);
                                bin_op_exception[cnt][0]=temp;
                                if (node->types.operation.op!=MINUS 
                                	&& node->types.operation.op!=DIVIDE)
                                    strcat(assign_var, "(");
                                shift_flag=1;

                            }

                            //								if (node->types.operation.op==MINUS)
                            //								{
                            //									memmove(identif+1, identif, 4096-1);
                            //									identif[0]='-';
                            //									strcat(identif, " +");
                            //								}
                            //
                            //								else if (node->types.operation.op==DIVIDE)
                            //								{
                            //									memmove(identif+3, identif, 4096-3);
                            //									identif[0]='(';
                            //									identif[1]='1';
                            //									identif[2]='/';
                            //									strcat(identif, ") *");
                            //								}

                            //strcat(identif, range_ref_identifier);

                            if (cnt_temp>1 && cnt_bin==0)
                                sprintf(assign_in," %s", identif);
                            else if (cnt_temp<=1 && cnt_bin==0)
                                sprintf(assign_in," (%s", identif);
                            else if (cnt_temp>1 && cnt_bin>0)
                                sprintf(assign_in," %s)", identif);
                            else if (cnt_temp<=1 && cnt_bin>0)
                                sprintf(assign_in," %s)", identif);

                            strcat(assign_var, assign_in);

                            int r;
                            if (cnt_temp>1)
                                //{if (node->type==ARRAY_REF)
                            {
                                for (r=0; r<cnt-1; r++)
                                {
                                    if (node->types.operation.op!=SL 
                                    	&& node->types.operation.op!=SR 
                                    	&& identif2[cnt-1-r][0]!=NULL)
                                    {
                                        strcat(assign_var, bin_op_exception[cnt-1-r][0]);
                                        strcat(assign_var, " ");
                                        strcat(assign_var, identif2[cnt-1-r][0]);
                                    } //wow this works!!

                                    strcat(assign_var,")");
                                }

                            }


                            //}

                            if (node->types.operation.op!=SR || node->types.operation.op!=SL)
                                strcat(assign_var, parent_bin_op);


                            if (r==(cnt-1) && r!=0 
                            	&& (strcmp(root_bin_op, "<<")==1) 
                            	&& (strcmp(root_bin_op,">>")==1))
                            {
                                assign_var[strlen(assign_var)-2]=0;
                                strcat(assign_var, root_bin_op);
                            }

                            if (r==(cnt-1) && r!=0 && identif2[1][0]!=NULL) 
                                strcat(assign_var, "(");

                        }
                        else
                        {
                            switch(node_child->children[m+1]->type)
                            {
                                case IDENTIFIERS:
                                    {
                                        if (shift_flag==1)
                                            sprintf(assign_in,"(%s", array_ref_identifier);
                                        else
                                            sprintf(assign_in," (%s", array_ref_identifier);
                                        strcat(assign_var, assign_in);
                                        strcat(assign_var, bin_op);

                                        break;
                                    }

                                case NUMBERS:
                                    {

                                        if (shift_flag==1)
                                            sprintf(assign_in,"(%s", array_ref_identifier);
                                        else
                                            sprintf(assign_in," (%s", array_ref_identifier);
                                        strcat(assign_var, assign_in);
                                        strcat(assign_var, bin_op);

                                        break;
                                    }

                                case BINARY_OPERATION:
                                    {

                                        if (shift_flag==1)
                                            sprintf(assign_in,"(%s", array_ref_identifier);
                                        else
                                            sprintf(assign_in," (%s", array_ref_identifier);
                                        strcat(assign_var, assign_in);
                                        strcat(assign_var, bin_op);

                                        break;
                                    }

                                case UNARY_OPERATION:
                                    {

                                        if (shift_flag==1)
                                            sprintf(assign_in,"(%s", array_ref_identifier);
                                        else
                                            sprintf(assign_in," (%s", array_ref_identifier);
                                        strcat(assign_var, assign_in);
                                        strcat(assign_var, bin_op);

                                        break;
                                    }

                                default:
                                    {
                                        if (shift_flag==1)
                                            sprintf(assign_in,"%s", array_ref_identifier);
                                        else
                                            sprintf(assign_in," %s", array_ref_identifier);
                                        strcat(assign_var, assign_in);
                                        strcat(assign_var, bin_op);

                                        break;
                                    }

                            }

                        }
                        //
                        //strcat(assign_var, array_ref_identifier);
                        //strcat(assign_var, bin_op);

                        //i=i+node_child->num_children;

                        break;
                    }

                    //						case RANGE_REF:
                    //						{
                    //							cnt++;
                    //							cnt_temp++;
                    //							memset(range_ref_range, 0, 4096);
                    //							int m_sub2;
                    //							for (m_sub2=0; m_sub2<node_child->num_children; m_sub2++)
                    //							{
                    //								ast_node_t *range_ref_node = node_child->children[m_sub2];
                    //								if (range_ref_node==NULL){}
                    //								else
                    //								{
                    //									if (range_ref_node->type==IDENTIFIERS)
                    //									{
                    //
                    //										if (m_sub2>0 && m_sub2<2)
                    //										{
                    //											strcat(array_ref_identifier, "[");
                    //											strcat(array_ref_identifier, range_ref_node->types.identifier);
                    //											strcat(array_ref_identifier, ":");
                    //										}
                    //										else if (m_sub2>1 && m_sub2<3)
                    //										{
                    //											strcat(array_ref_identifier, range_ref_node->types.identifier);
                    //											strcat(array_ref_identifier, "]");
                    //										}
                    //
                    //										else if(bin_range_ref_flag!=1 
                    //												&& node_child->children[m_sub2+1]->type!=NUMBERS 
                    //												&& node_child->children[m_sub2+1]->type!=IDENTIFIERS 
                    //												&& node_child->children[m_sub2+1]->type!=BINARY_OPERATION )
                    //										{
                    //											sprintf(array_ref_identifier, "(%s", range_ref_node->types.identifier);
                    //											bin_range_ref_flag=0;
                    //										}
                    //										else
                    //											sprintf(array_ref_identifier, "%s", range_ref_node->types.identifier);
                    //									}
                    //								}
                    //							}
                    //
                    //
                    //						break;
                    //						}


                case RANGE_REF:
                    {
                        if (recursive_bin_parent_node->type==CONCATENATE)
                        {

                            break;
                        }

                        cnt++;
                        cnt_temp++;
                        ast_node_t *range_ref_node;

                        memset(range_ref_range, 0, 16384);
                        int m_sub; int cnt_identifier2=0;
                        for (m_sub=0; m_sub<node_child->num_children; m_sub++)
                        {
                            range_ref_node = node_child->children[m_sub];
                            if (range_ref_node==NULL){}
                            else
                            {

                                if (range_ref_node->type==IDENTIFIERS)
                                {

                                    if (m_sub>0)
                                    {
                                        if (cnt_range_ref_num_nodes<=1)
                                        {

                                            cnt_range_ref_num_nodes++;
                                            strcat(range_ref_identifier, "[");
                                            strcat(range_ref_identifier,range_ref_node->types.identifier);
                                        }

                                        else
                                        {
                                            strcat(range_ref_identifier, ":");
                                            strcat(range_ref_identifier, range_ref_node->types.identifier);
                                            strcat(range_ref_identifier, "]");
                                        }
                                    }
                                    //else if (m_sub==2)
                                    //{
                                    //strcat(array_ref_identifier, "[");
                                    //	strcat(range_ref_identifier, range_ref_node->types.identifier);
                                    //	strcat(range_ref_identifier, "]");
                                    //}

                                    else if(bin_range_ref_flag!=1 
                                    		&& node_child->children[m_sub+1]->type!=NUMBERS 
                                    		&& node_child->children[m_sub+1]->type!=IDENTIFIERS 
                                    		&& node_child->children[m_sub+1]->type!=BINARY_OPERATION )
                                    {

                                        cnt_range_ref_num_nodes++;
                                        sprintf(range_ref_identifier, "(%s", range_ref_node->types.identifier);
                                        bin_range_ref_flag=0;
                                    }
                                    else
                                    {
                                        cnt_range_ref_num_nodes++;
                                        sprintf(range_ref_identifier, "%s", range_ref_node->types.identifier);
                                    }
                                }
                                else if(range_ref_node->type==NUMBERS)
                                {

                                    cnt_range_ref_num_nodes++;
                                    if (cnt_range_ref_num_nodes==2 || cnt_range_ref_num_nodes%2==0)
                                        sprintf(range_ref_range,"[%d:",range_ref_node->types.number.value);
                                    else
                                    {
                                        char range_num2[4096]="";
                                        if (m_sub>0 && node_child->children[m_sub-1]->type!=NUMBERS)
                                            strcat(range_ref_range,":");
                                        sprintf(range_num2, "%d]", range_ref_node->types.number.value);
                                        strcat(range_ref_range, range_num2);
                                    }
                                }
                                //else if add bin_op case
                                else if (range_ref_node->type==BINARY_OPERATION)
                                {
                                    cnt_range_ref_num_nodes++;

                                    char parent_bin_op_rangeref[8193]="";
                                    int dummy_cnt_bin_op;
                                    if (m_sub%2==1)
                                        strcat(range_ref_range, "[");
                                    bin_range_ref_flag=1;
                                    BinaryOpsGenerator(range_ref_node, parent_bin_op_rangeref);
                                    RecursiveBinaryop(range_ref_node, range_ref_range, 
                                    				  parent_bin_op_rangeref, parent_bin_op_rangeref, 
                                    				  dummy_cnt_bin_op, 0);
                                    range_ref_range[strlen(range_ref_range)-2]=0;
                                    if (m_sub%2==0)
                                        strcat(range_ref_range, "]");
                                }
                                //cnt_range_ref_num_nodes=0;
                            }
                        }

                        strcat(range_ref_identifier, range_ref_range);

                        cnt_range_ref_num_nodes=0;

                        //this is where you need to take care of parenthesis as with the identifiers


                        char assign_in[4096]="";
                        char identif[8193]="";


                        if (node_child->children[m+1]!=NULL)
                        {

                            strcat(identif, range_ref_identifier);
                            if (node->types.operation.op == SR || node->types.operation.op == SL)
                            {
                                //sprintf(identif2, "%s", node_child->types.identifier);
                                identif2[cnt][0]=range_ref_identifier;
                                char temp[10]="";
                                BinaryOpsGenerator(node, temp);
                                bin_op_exception[cnt][0]=temp;
                                strcat(assign_var, "(");
                                shift_flag=1;

                            }

                            if (node->types.operation.op==MINUS)
                            {
                                memmove(identif+1, identif, 8193-1);
                                identif[0]='-';
                                strcat(identif, " +");
                            }

                            else if (node->types.operation.op==DIVIDE)
                            {
                                memmove(identif+3, identif, 8193-3);
                                identif[0]='(';
                                identif[1]='1';
                                identif[2]='/';
                                strcat(identif, ") *");
                            }

                            //strcat(identif, range_ref_identifier);


                            if (cnt_temp>1 && cnt_bin==0)
                                sprintf(assign_in," %s", identif);
                            else if (cnt_temp<=1 && cnt_bin==0)
                            {
                                if (shift_flag==1)
                                    sprintf(assign_in,"(%s", identif);
                                else
                                    sprintf(assign_in," (%s", identif);
                            }
                            else if (cnt_temp>1 && cnt_bin>0)
                                sprintf(assign_in," %s)", identif);
                            else if (cnt_temp<=1 && cnt_bin>0)
                                sprintf(assign_in," %s)", identif);
                            strcat(assign_var, assign_in);
                            int r;
                            if (cnt_temp>1)
                                //{if (node->type==ARRAY_REF)
                            {
                                for (r=0; r<cnt-1; r++)
                                {
                                    if (node->types.operation.op!=SL 
                                    	&& node->types.operation.op!=SR 
                                    	&& identif2[cnt-1-r][0]!=NULL)
                                    {
                                        strcat(assign_var, bin_op_exception[cnt-1-r][0]);
                                        strcat(assign_var, " ");
                                        strcat(assign_var, identif2[cnt-1-r][0]);
                                    } //wow this works!!
                                    strcat(assign_var,")");
                                }

                            }

                            //}
                            if (node->types.operation.op!=MINUS 
                            	&& node->types.operation.op!=DIVIDE 
                            	&& node->types.operation.op!=SR 
                            	&& node->types.operation.op!=SL)
                                strcat(assign_var, parent_bin_op);

                            //strcat(assign_var, parent_bin_op);
                            if (r==(cnt-1) && r!=0 
                            	&& (strcmp(root_bin_op, "<<")==1) 
                            	&& (strcmp(root_bin_op,">>")==1))
                            {
                                assign_var[strlen(assign_var)-2]=0;
                                strcat(assign_var, root_bin_op);
                            }

                            if (r==(cnt-1) && r!=0 && identif2[1][0]!=NULL 
                            			   && two_shift_children_flag==0)
                                strcat(assign_var, "(");

                        }
                        else
                        {
                            switch(node_child->children[m+1]->type)
                            {
                                case IDENTIFIERS:
                                    {
                                        if (shift_flag==1)
                                            sprintf(assign_in,"(%s", range_ref_identifier);
                                        else
                                            sprintf(assign_in," (%s", range_ref_identifier);

                                        strcat(assign_var, assign_in);
                                        strcat(assign_var, bin_op);

                                        break;
                                    }

                                case NUMBERS:
                                    {
                                        if (shift_flag==1)
                                            sprintf(assign_in,"(%s", range_ref_identifier);
                                        else
                                            sprintf(assign_in," (%s", range_ref_identifier);
                                        strcat(assign_var, assign_in);
                                        strcat(assign_var, bin_op);

                                        break;
                                    }

                                case BINARY_OPERATION:
                                    {
                                        if (shift_flag==1)
                                            sprintf(assign_in,"(%s", range_ref_identifier);
                                        else
                                            sprintf(assign_in," (%s", range_ref_identifier);
                                        strcat(assign_var, assign_in);
                                        strcat(assign_var, bin_op);

                                        break;
                                    }

                                case UNARY_OPERATION:
                                    {
                                        if (shift_flag==1)
                                            sprintf(assign_in,"(%s", range_ref_identifier);
                                        else
                                            sprintf(assign_in," (%s", range_ref_identifier);
                                        strcat(assign_var, assign_in);
                                        strcat(assign_var, bin_op);

                                        break;
                                    }

                                default:
                                    {
                                        if (shift_flag==1)
                                            sprintf(assign_in,"%s", range_ref_identifier);
                                        else
                                            sprintf(assign_in," %s", range_ref_identifier);
                                        strcat(assign_var, assign_in);
                                        strcat(assign_var, bin_op);
                                        break;
                                    }
                            }
                        }
                        break;
                    }

                case ARRAY_RANGE_REF:
                    {
                        //range_ref_range
                        //range_ref_identifier
                        //bin_range_ref_flag
                        //cnt_range_ref_num_nodes

                        if (recursive_bin_parent_node->type==CONCATENATE)
                        {
                            break;
                        }

                        cnt++;
                        cnt_temp++;
                        ast_node_t *range_ref_node;

                        memset(range_ref_range, 0, 16384);
                        int m_sub; int cnt_identifier2=0;
                        for (m_sub=0; m_sub<node_child->num_children; m_sub++)
                        {
                            range_ref_node = node_child->children[m_sub];
                            if (range_ref_node==NULL){}
                            else
                            {
                                if (range_ref_node->type==IDENTIFIERS)
                                {

                                    if (m_sub>0)
                                    {
                                        if (cnt_range_ref_num_nodes<=2)
                                        {

                                            cnt_range_ref_num_nodes++;
                                            strcat(range_ref_identifier, "[");
                                            strcat(range_ref_identifier,
                                            	   range_ref_node->types.identifier);

                                            if (cnt_range_ref_num_nodes==2)
                                                strcat(range_ref_identifier, "]");
                                        }
                                        else
                                        {
                                            strcat(range_ref_identifier, ":");
                                            strcat(range_ref_identifier, 
                                            	   range_ref_node->types.identifier);
                                            strcat(range_ref_identifier, "]");
                                        }
                                    }

                                    else if(bin_range_ref_flag!=1 
                                    		&& node_child->children[m_sub+1]->type!=NUMBERS 
                                    		&& node_child->children[m_sub+1]->type!=IDENTIFIERS 
                                    		&& node_child->children[m_sub+1]->type!=BINARY_OPERATION)
                                    {

                                        cnt_range_ref_num_nodes++;
                                        sprintf(range_ref_identifier, "(%s", range_ref_node->types.identifier);
                                        bin_range_ref_flag=0;
                                    }
                                    else
                                    {
                                        cnt_range_ref_num_nodes++;
                                        sprintf(range_ref_identifier, "%s", range_ref_node->types.identifier);
                                    }
                                }
                                else if(range_ref_node->type==NUMBERS)
                                {

                                    cnt_range_ref_num_nodes++;
                                    if (cnt_range_ref_num_nodes==2)
                                        sprintf(range_ref_range,"[%s]",range_ref_node->types.number.number);

                                    else if (cnt_range_ref_num_nodes==3 || cnt_range_ref_num_nodes%3==0){
                                        strcat(range_ref_range, "[");
                                        strcat(range_ref_range, range_ref_node->types.number.number);
                                        strcat(range_ref_range, ":");
                                    }
                                    else
                                    {
                                        char range_num2[4096]="";

                                        if (m_sub>0 && node_child->children[m_sub-1]->type!=NUMBERS)
                                            strcat(range_ref_range,":");

                                        sprintf(range_num2, "%d]", range_ref_node->types.number.value);

                                        strcat(range_ref_range, range_num2);

                                    }
                                }
                                //else if add bin_op case
                                else if (range_ref_node->type==BINARY_OPERATION)
                                {
                                    cnt_range_ref_num_nodes++;

                                    char parent_bin_op_rangeref[8193]="";
                                    int dummy_cnt_bin_op;
                                    if (m_sub%3==2 || m_sub%3==1)
                                        strcat(range_ref_range, "[");
                                    bin_range_ref_flag=1;
                                    BinaryOpsGenerator(range_ref_node, parent_bin_op_rangeref);
                                    RecursiveBinaryop(range_ref_node, range_ref_range, 
                                    				  parent_bin_op_rangeref, 
                                    				  parent_bin_op_rangeref, 
                                    				  dummy_cnt_bin_op, 0);
                                    range_ref_range[strlen(range_ref_range)-2]=0;
                                    if (m_sub%3==0|| m_sub%3==1)
                                        strcat(range_ref_range, "]");
                                }
                                //cnt_range_ref_num_nodes=0;


                            }
                        }

                        strcat(range_ref_identifier, range_ref_range);
                        cnt_range_ref_num_nodes=0;

                        //this is where you need to take care of parenthesis as with the identifiers
                        char assign_in[4096]="";
                        char identif[8193]="";


                        if (node_child->children[m+1]!=NULL)
                        {

                            strcat(identif, range_ref_identifier);
                            if (node->types.operation.op == SR || node->types.operation.op == SL)
                            {
                                //sprintf(identif2, "%s", node_child->types.identifier);
                                identif2[cnt][0]=range_ref_identifier;
                                char temp[10]="";
                                BinaryOpsGenerator(node, temp);
                                bin_op_exception[cnt][0]=temp;
                                strcat(assign_var, "(");
                                shift_flag=1;

                            }

                            if (node->types.operation.op==MINUS)
                            {
                                memmove(identif+1, identif, 8193-1);
                                identif[0]='-';
                                strcat(identif, " +");
                            }

                            else if (node->types.operation.op==DIVIDE)
                            {
                                memmove(identif+3, identif, 8193-3);
                                identif[0]='(';
                                identif[1]='1';
                                identif[2]='/';
                                strcat(identif, ") *");
                            }

                            //strcat(identif, range_ref_identifier);


                            if (cnt_temp>1 && cnt_bin==0)
                                sprintf(assign_in," %s", identif);
                            else if (cnt_temp<=1 && cnt_bin==0)
                            {
                                if (shift_flag==1)
                                    sprintf(assign_in,"(%s", identif);
                                else
                                    sprintf(assign_in," (%s", identif);
                            }
                            else if (cnt_temp>1 && cnt_bin>0)
                                sprintf(assign_in," %s)", identif);
                            else if (cnt_temp<=1 && cnt_bin>0)
                                sprintf(assign_in," %s)", identif);
                            strcat(assign_var, assign_in);
                            int r;
                            if (cnt_temp>1)
                                //{if (node->type==ARRAY_REF)
                            {
                                for (r=0; r<cnt-1; r++)
                                {
                                    if (node->types.operation.op!=SL 
                                    	&& node->types.operation.op!=SR 
                                    	&& identif2[cnt-1-r][0]!=NULL)
                                    {
                                        strcat(assign_var, bin_op_exception[cnt-1-r][0]);
                                        strcat(assign_var, " ");
                                        strcat(assign_var, identif2[cnt-1-r][0]);
                                    } //wow this works!!
                                    strcat(assign_var,")");
                                }

                            }

                            //}
                            if (node->types.operation.op!=MINUS 
                            	&& node->types.operation.op!=DIVIDE 
                            	&& node->types.operation.op!=SR 
                            	&& node->types.operation.op!=SL)
                                strcat(assign_var, parent_bin_op);

                            //strcat(assign_var, parent_bin_op);
                            if (r==(cnt-1) && r!=0)
                            {
                                assign_var[strlen(assign_var)-2]=0;
                                strcat(assign_var, root_bin_op);
                            }
                        }
                        else
                        {
                            switch(node_child->children[m+1]->type)
                            {
                                case IDENTIFIERS:
                                    {
                                        if (shift_flag==1)
                                            sprintf(assign_in,"(%s", range_ref_identifier);
                                        else
                                            sprintf(assign_in," (%s", range_ref_identifier);
                                        strcat(assign_var, assign_in);
                                        strcat(assign_var, bin_op);
                                        break;
                                    }
                                case NUMBERS:
                                    {
                                        if (shift_flag==1)
                                            sprintf(assign_in,"(%s", range_ref_identifier);
                                        else
                                            sprintf(assign_in," (%s", range_ref_identifier);
                                        strcat(assign_var, assign_in);
                                        strcat(assign_var, bin_op);
                                        break;
                                    }
                                case BINARY_OPERATION:
                                    {

                                        if (shift_flag==1)
                                            sprintf(assign_in,"(%s", range_ref_identifier);
                                        else
                                            sprintf(assign_in," (%s", range_ref_identifier);
                                        strcat(assign_var, assign_in);
                                        strcat(assign_var, bin_op);
                                        break;
                                    }
                                case UNARY_OPERATION:
                                    {
                                        if (shift_flag==1)
                                            sprintf(assign_in,"(%s", range_ref_identifier);
                                        else
                                            sprintf(assign_in," (%s", range_ref_identifier);
                                        strcat(assign_var, assign_in);
                                        strcat(assign_var, bin_op);
                                        break;
                                    }
                                default:
                                    {
                                        if (shift_flag==1)
                                            sprintf(assign_in,"%s", range_ref_identifier);
                                        else
                                            sprintf(assign_in," %s", range_ref_identifier);
                                        strcat(assign_var, assign_in);
                                        strcat(assign_var, bin_op);
                                        break;
                                    }
                            }
                        }
                        break;
                    }
                case MATRIX_REF:
                    {
                        //matrix_ref_range
                        //matrix_ref_identifier
                        //bin_matrix_ref_flag
                        //cnt_matrix_ref_num_nodes

                        if (recursive_bin_parent_node->type==CONCATENATE)
                        {
                            break;
                        }

                        cnt++;
                        cnt_temp++;
                        ast_node_t *matrix_ref_node;

                        memset(matrix_ref_range, 0, 8193);
                        int m_sub; int cnt_identifier2=0;
                        for (m_sub=0; m_sub<node_child->num_children; m_sub++)
                        {
                            matrix_ref_node = node_child->children[m_sub];
                            if (matrix_ref_node==NULL){}
                            else
                            {

                                if (matrix_ref_node->type==IDENTIFIERS)
                                {
                                    ast_node_t *array_ref_node3;

                                    if (m_sub!=node_child->num_children-1)
                                    {
                                        array_ref_node3 = node_child->children[m_sub+1];
                                    }

                                        if (m_sub>0)
                                        {
                                            cnt_matrix_ref_num_nodes++;
                                            strcat(matrix_ref_identifier, "[");
                                            strcat(matrix_ref_identifier,
                                            	   matrix_ref_node->types.identifier);
                                            strcat(matrix_ref_identifier, ":");
                                        }

                                        else if(bin_matrix_ref_flag!=1 
                                        		&& node_child->children[m_sub+1]->type!=NUMBERS
                                                && node_child->children[m_sub+1]->type!=IDENTIFIERS
                                                && node_child->children[m_sub+1]->type!=BINARY_OPERATION )
                                        {
                                            sprintf(matrix_ref_identifier, "(%s", 
                                            	    matrix_ref_node->types.identifier);
                                            bin_matrix_ref_flag=0;
                                        }
                                        else
                                            sprintf(matrix_ref_identifier, "%s", 
                                            		matrix_ref_node->types.identifier);
                                }

                                else if(matrix_ref_node->type==NUMBERS)
                                {
                                    char temp_matrix_id[4096]="";
                                    cnt_matrix_ref_num_nodes++;
                                    if (cnt_matrix_ref_num_nodes==1 || cnt_matrix_ref_num_nodes%5==0)
                                        sprintf(matrix_ref_range,"[%s]", matrix_ref_node->types.number.number);
                                    else if (cnt_matrix_ref_num_nodes==2 || cnt_matrix_ref_num_nodes%5==0)
                                    {
                                        strcat(matrix_ref_range,"[");
                                        strcat(matrix_ref_range,matrix_ref_node->types.number.number);
                                        strcat(matrix_ref_range,"]");
                                    }
                                    else if (cnt_matrix_ref_num_nodes==3 || cnt_matrix_ref_num_nodes%5==0)
                                    {
                                        strcat(matrix_ref_range,"[");
                                        sprintf(temp_matrix_id, "%d", matrix_ref_node->types.number.value);
                                        strcat(matrix_ref_range,temp_matrix_id);
                                        strcat(matrix_ref_range,":");
                                    }
                                    else if (cnt_matrix_ref_num_nodes==4 || cnt_matrix_ref_num_nodes%5==0)
                                    {
                                        //strcat(matrix_ref_range,matrix_ref_node->types.number.number);
                                        sprintf(temp_matrix_id, "%d", matrix_ref_node->types.number.value);
                                        strcat(matrix_ref_range,temp_matrix_id);
                                        strcat(matrix_ref_range,"]");
                                    }

                                    else
                                    {
                                        strcat(matrix_ref_range,":");
                                        strcat(matrix_ref_range, matrix_ref_node->types.number.value);
                                        strcat(matrix_ref_range, "]");
                                    }

                                }
                                //else if add bin_op case
                                else if (matrix_ref_node->type==BINARY_OPERATION)
                                {

                                    cnt_matrix_ref_num_nodes++;

                                    char parent_bin_op_matrixref[4096]="";
                                    int dummy_cnt_bin_op;
                                    if (m_sub==1 || m_sub==3 )
                                        strcat(matrix_ref_range, "[");
                                    bin_matrix_ref_flag=1;
                                    BinaryOpsGenerator(matrix_ref_node, parent_bin_op_matrixref);
                                    RecursiveBinaryop(matrix_ref_node, matrix_ref_range, 
                                    				  parent_bin_op_matrixref, 
                                    				  parent_bin_op_matrixref, 
                                    				  dummy_cnt_bin_op, 0);
                                    matrix_ref_range[strlen(matrix_ref_range)-1]=0;
                                    strcat(matrix_ref_range, ":");
                                    if (m_sub==2 || m_sub==4)
                                    {
                                        matrix_ref_range[strlen(matrix_ref_range)-1]=0;
                                        strcat(matrix_ref_range, "]");
                                    }
                                }
                            }
                        }

                        strcat(matrix_ref_identifier, matrix_ref_range);
                        cnt_matrix_ref_num_nodes=0;
                        //this is where you need to take care of parenthesis as with the identifiers

                        char assign_in[4096]="";
                        char identif[8193]="";

                        if (node_child->children[m+1]!=NULL)
                        {

                            strcat(identif, matrix_ref_identifier);
                            if ((node->types.operation.op == SR 
                            	 || node->types.operation.op == SL) 
                            	&& node->children[0]->type==BINARY_OPERATION)
                            {
                                //sprintf(identif2, "%s", node_child->types.identifier);
                                identif2[cnt][0]=matrix_ref_identifier;
                                char temp[10]="";
                                BinaryOpsGenerator(node, temp);
                                bin_op_exception[cnt][0]=temp;
                                strcat(assign_var, "(");
                                shift_flag=1;
                            }

                            if (node->types.operation.op==MINUS 
                            	&& node->children[0]->type==BINARY_OPERATION)
                            {
                                memmove(identif+1, identif, 8193-1);
                                identif[0]='-';
                                strcat(identif, " +");
                            }

                            else if (node->types.operation.op==DIVIDE 
                            		 && node->children[0]->type==BINARY_OPERATION)
                            {
                                memmove(identif+3, identif, 8193-3);
                                identif[0]='(';
                                identif[1]='1';
                                identif[2]='/';
                                strcat(identif, ") *");
                            }

                            //strcat(identif, matrix_ref_identifier);

                            if (cnt_temp>1 && cnt_bin==0)
                                sprintf(assign_in," %s", identif);
                            else if (cnt_temp<=1 && cnt_bin==0)
                                sprintf(assign_in," (%s", identif);
                            else if (cnt_temp>1 && cnt_bin>0)
                                sprintf(assign_in," %s)", identif);
                            else if (cnt_temp<=1 && cnt_bin>0)
                                sprintf(assign_in," %s)", identif);
                            strcat(assign_var, assign_in);
                            int r=0;
                            if (cnt_temp>1)
                                //{if (node->type==ARRAY_REF)
                            {
                                for (r=0; r<cnt-1; r++)
                                {
                                    if (node->types.operation.op!=SL 
                                        && node->types.operation.op!=SR 
                                        && identif2[cnt-1-r][0]!=NULL)
                                    {
                                        strcat(assign_var, bin_op_exception[cnt-1-r][0]);
                                        strcat(assign_var, " ");
                                        strcat(assign_var, identif2[cnt-1-r][0]);
                                    } //wow this works!!
                                    strcat(assign_var,")");
                                }
                            }


                            //}
                            if (node->children[0]->type==BINARY_OPERATION 
                            	|| (node->types.operation.op!=MINUS
                                    && node->types.operation.op!=DIVIDE 
                                    && node->types.operation.op!=SR
                                    && node->types.operation.op!=SL))
                                strcat(assign_var, parent_bin_op);
                            //else if (node->types.operation.op==MINUS && node_child->num_children==0)
                            //	strcat(assign_var, parent_bin_op);
                            //else if (node->types.operation.op==DIVIDE && node_child->num_children==0)
                            //strcat(assign_var, parent_bin_op);

                            //strcat(assign_var, parent_bin_op);
                            if (r==(cnt-1) && r!=0 
                            	&& (strcmp(root_bin_op, "<<")==1) 
                            	&& (strcmp(root_bin_op,">>")==1))
                            {
                                assign_var[strlen(assign_var)-2]=0;
                                strcat(assign_var, root_bin_op);
                            }

                            if (r==(cnt-1) && r!=0 && identif2[1][0]!=NULL)
                                strcat(assign_var, "(");
                        }
                        else
                        {
                            switch(node_child->children[m+1]->type)
                            {
                                case IDENTIFIERS:
                                    {

                                        if (shift_flag==1)
                                            sprintf(assign_in,"(%s", matrix_ref_identifier);
                                        else
                                            sprintf(assign_in," (%s", matrix_ref_identifier);
                                        strcat(assign_var, assign_in);
                                        strcat(assign_var, bin_op);
                                        break;
                                    }

                                case NUMBERS:
                                    {
                                        if (shift_flag==1)
                                            sprintf(assign_in,"(%s", matrix_ref_identifier);
                                        else
                                            sprintf(assign_in," (%s", matrix_ref_identifier);
                                        strcat(assign_var, assign_in);
                                        strcat(assign_var, bin_op);
                                        break;
                                    }
                                case BINARY_OPERATION:
                                    {
                                        if (shift_flag==1)
                                            sprintf(assign_in,"(%s", matrix_ref_identifier);
                                        else
                                            sprintf(assign_in," (%s", matrix_ref_identifier);
                                        strcat(assign_var, assign_in);
                                        strcat(assign_var, bin_op);
                                        break;
                                    }

                                case UNARY_OPERATION:
                                    {

                                        if (shift_flag==1)
                                            sprintf(assign_in,"(%s", matrix_ref_identifier);
                                        else
                                            sprintf(assign_in," (%s", matrix_ref_identifier);
                                        strcat(assign_var, assign_in);
                                        strcat(assign_var, bin_op);
                                        break;
                                    }

                                default:
                                    {
                                        if (shift_flag==1)
                                            sprintf(assign_in,"%s", matrix_ref_identifier);
                                        else
                                            sprintf(assign_in," %s", matrix_ref_identifier);
                                        strcat(assign_var, assign_in);
                                        strcat(assign_var, bin_op);
                                        break;
                                    }
                            }

                        }

                        break;
                    }

                case IDENTIFIERS:
                    {
                        if (recursive_bin_parent_node->type==ARRAY_REF 
                        	|| recursive_bin_parent_node->type==RANGE_REF 
                        	|| recursive_bin_parent_node->type==ARRAY_RANGE_REF 
                        	|| recursive_bin_parent_node->type==CONCATENATE 
                        	|| recursive_bin_parent_node->type==MATRIX_REF)
                        {
                            break;
                        }

                        cnt++;
                        cnt_temp++;

                        char assign_in[4096]="";
                        char identif[8193]="";

                        if ((m==node->num_children-1) || m>=1)
                        {

                            sprintf(identif, "%s", node_child->types.identifier);
                            if ((node->types.operation.op == SR || node->types.operation.op == SL) && node->children[0]->type==BINARY_OPERATION)
                            {
                                //sprintf(identif2, "%s", node_child->types.identifier);
                                identif2[cnt][0]=node_child->types.identifier;
                                char temp[10]="";
                                BinaryOpsGenerator(node, temp);
                                bin_op_exception[cnt][0]=temp;
                                strcat(assign_var, "(");
                                shift_flag=1;
                            }

                            if (node->types.operation.op==MINUS && node->children[0]->type==BINARY_OPERATION)
                            {
                                memmove(identif+1, identif, 8193-1);
                                identif[0]='-';
                                strcat(identif, " +");
                            }

                            else if (node->types.operation.op==DIVIDE && node->children[0]->type==BINARY_OPERATION)
                            {
                                memmove(identif+3, identif, 8193-3);
                                identif[0]='(';
                                identif[1]='1';
                                identif[2]='/';
                                strcat(identif, ") *");
                            }


                            if(cnt_temp>1 && cnt_bin==0)
                                sprintf(assign_in," %s", identif);
                            else
                                if (cnt_temp<=1 && cnt_bin==0)
                                    sprintf(assign_in," (%s", identif);
                                else if (cnt_temp>1 && cnt_bin>0)
                                    sprintf(assign_in," %s", identif);
                                else if (cnt_temp<=1 && cnt_bin>0){
                                    sprintf(assign_in," %s)", identif);
                                }

                            if (node->types.operation.op!=SL && node->types.operation.op!=SR)
                                strcat(assign_var, assign_in);

                            int r;
                            if (cnt_temp>1)
                            {
                                for (r=0; r<cnt-1; r++)
                                {
                                    if (node->types.operation.op!=SL && node->types.operation.op!=SR && identif2[cnt-1-r][0]!=NULL && node->types.operation.op!=MINUS)
                                    {
                                        strcat(assign_var, bin_op_exception[cnt-1-r][0]);
                                        strcat(assign_var, " ");
                                        strcat(assign_var, identif2[cnt-1-r][0]);
                                    } 
                                    strcat(assign_var,")");
                                }
                            }

                            if (node->children[0]->type!=BINARY_OPERATION 
                            	|| (node->types.operation.op!=MINUS 
                            		&& node->types.operation.op!=DIVIDE 
                            		&& node->types.operation.op!=SL 
                            		&& node->types.operation.op!=SR))
                                strcat(assign_var, parent_bin_op);

                            if (r==(cnt-1) && r!=0 
                            	&& (strcmp(root_bin_op, "<<")==1) 
                            	&& (strcmp(root_bin_op, ">>")==1))
                            {
                                assign_var[strlen(assign_var)-2]=0;
                                strcat(assign_var, root_bin_op);

                            }

                            if (r==(cnt-1) && r!=0 
                            	&& identif2[1][0]!=NULL 
                            	&& (strcmp(parent_bin_op, "<<")==1) 
                            	&& (strcmp(parent_bin_op, ">>")==1))
                                strcat(assign_var, "(");

                        }
                        else
                        {
                            switch(node->children[m+1]->type)
                            {

                                case IDENTIFIERS:
                                    {
                                        if (shift_flag==1)
                                            sprintf(assign_in,"(%s", node_child->types.identifier);
                                        else
                                            sprintf(assign_in," (%s", node_child->types.identifier);

                                        strcat(assign_var, assign_in);
                                        strcat(assign_var, bin_op);


                                        break;
                                    }
                                case NUMBERS:
                                    {

                                        if (shift_flag==1)
                                            sprintf(assign_in,"(%s", node_child->types.identifier);
                                        else
                                            sprintf(assign_in," (%s", node_child->types.identifier);
                                        strcat(assign_var, assign_in);
                                        strcat(assign_var, bin_op);

                                        break;
                                    }

                                case ARRAY_REF:
                                    {
                                        if (shift_flag==1)
                                            sprintf(assign_in,"(%s", node_child->types.identifier);
                                        else
                                            sprintf(assign_in," (%s", node_child->types.identifier);
                                        strcat(assign_var, assign_in);
                                        strcat(assign_var, bin_op);

                                        break;
                                    }

                                case RANGE_REF:
                                    {
                                        if (shift_flag==1)
                                            sprintf(assign_in,"(%s", node_child->types.identifier);
                                        else
                                            sprintf(assign_in," (%s", node_child->types.identifier);
                                        strcat(assign_var, assign_in);
                                        strcat(assign_var, bin_op);

                                        break;
                                    }

                                case BINARY_OPERATION:
                                    {

                                        if (shift_flag==1)
                                            sprintf(assign_in,"(%s", node_child->types.identifier);
                                        else
                                            sprintf(assign_in," (%s", node_child->types.identifier);
                                        strcat(assign_var, assign_in);
                                        strcat(assign_var, bin_op);

                                        break;
                                    }

                                case UNARY_OPERATION:
                                    {

                                        if (shift_flag==1)
                                            sprintf(assign_in,"(%s", node_child->types.identifier);
                                        else
                                            sprintf(assign_in," (%s", node_child->types.identifier);
                                        strcat(assign_var, assign_in);
                                        strcat(assign_var, bin_op);

                                        break;
                                    }


                                default:
                                    {
                                        sprintf(identif, "%s", node_child->types.identifier);
                                        if ((node->types.operation.op == SR 
                                        	 || node->types.operation.op == SL) 
                                        	&& node->children[0]->type==BINARY_OPERATION)
                                        {
                                            //sprintf(identif2, "%s", node_child->types.identifier);
                                            identif2[cnt][0]=node_child->types.identifier;
                                            char temp[10]="";
                                            BinaryOpsGenerator(node, temp);
                                            bin_op_exception[cnt][0]=temp;
                                            strcat(assign_var, "(");
                                            shift_flag=1;

                                        }

                                        if (node->types.operation.op==MINUS 
                                        	&& node->children[0]->type==BINARY_OPERATION)
                                        {
                                            memmove(identif+1, identif, 8193-1);
                                            identif[0]='-';
                                            strcat(identif, " +");
                                        }

                                        else if (node->types.operation.op==DIVIDE 
                                        		 && node->children[0]->type==BINARY_OPERATION)
                                        {
                                            memmove(identif+3, identif, 8193-3);
                                            identif[0]='(';
                                            identif[1]='1';
                                            identif[2]='/';
                                            strcat(identif, ") *");
                                        }


                                        if(cnt_temp>1 && cnt_bin==0)
                                            sprintf(assign_in," %s", identif);
                                        else if (cnt_temp<=1 && cnt_bin==0)
                                            sprintf(assign_in," (%s", identif);
                                        else if (cnt_temp>1 && cnt_bin>0)
                                            sprintf(assign_in," %s", identif);
                                        else if (cnt_temp<=1 && cnt_bin>0)
                                            sprintf(assign_in," %s)", identif);
                                        //

                                        if (node->types.operation.op!=SL 
                                        	&& node->types.operation.op!=SR)
                                            strcat(assign_var, assign_in);

                                        int r;
                                        if (cnt_temp>1)
                                        {

                                            for (r=0; r<cnt-1; r++)
                                            {


                                                if (node->types.operation.op!=SL 
                                                	&& node->types.operation.op!=SR 
                                                	&& identif2[cnt-1-r][0]!=NULL)
                                                {
                                                    strcat(assign_var, bin_op_exception[cnt-1-r][0]);
                                                    strcat(assign_var, " ");
                                                    strcat(assign_var, identif2[cnt-1-r][0]);
                                                } //wow this works!!
                                                strcat(assign_var,")");
                                            }
                                        }

                                        //
                                        if (node->children[0]->type!=BINARY_OPERATION 
                                        	|| (node->types.operation.op!=MINUS 
                                        		&& node->types.operation.op!=DIVIDE 
                                        		&& node->types.operation.op!=SL 
                                        		&& node->types.operation.op!=SR))
                                            strcat(assign_var, parent_bin_op);
                                        //else if (node->types.operation.op==MINUS)
                                        //strcat(assign_var, parent_bin_op);
                                        //else if (node->types.operation.op==DIVIDE)
                                        //	strcat(assign_var, parent_bin_op);

                                        if (r==(cnt-1) && r!=0 
                                        	&& (strcmp(root_bin_op, "<<")==1) 
                                        	&& (strcmp(root_bin_op,">>")==1))										
                                        {
                                            assign_var[strlen(assign_var)-2]=0;
                                            strcat(assign_var, root_bin_op);
                                        }
                                    }
                            }
                        }
                        break;
            }
            case CONCATENATE:
            {
                if (recursive_bin_parent_node->type==CONCATENATE){
                    //i=i+recursive_bin_parent_node->num_children;
                    break;
                }

                cnt++;
                cnt_temp++;

                char concat_id[4096]="";
                char assign_in[8196]="";
                char identif[8196]="";

                ConcatNodes(node_child, concat_id);

                if ((m==node_child->num_children-1) || m>=1)
                {
                    strcat(identif, concat_id);
                    if (node->types.operation.op == SR 
                    	|| node->types.operation.op == SL)
                    {
                        //sprintf(identif2, "%s", node_child->types.identifier);
                        identif2[cnt][0]=node_child->types.identifier;
                        char temp[10]="";
                        BinaryOpsGenerator(node, temp);
                        bin_op_exception[cnt][0]=temp;
                        strcat(assign_var, "(");
                        shift_flag=1;
                    }

                    if (node->types.operation.op==MINUS 
                    	&& node->children[0]->type==BINARY_OPERATION)
                    {
                        memmove(identif+1, identif, 8193-1);
                        identif[0]='-';
                        strcat(identif, " +");
                    }

                    else if (node->types.operation.op==DIVIDE 
                    		 && node->children[0]->type==BINARY_OPERATION)
                    {
                        memmove(identif+3, identif, 8193-3);
                        identif[0]='(';
                        identif[1]='1';
                        identif[2]='/';
                        strcat(identif, ") *");
                    }

                    if(cnt_temp>1 && cnt_bin==0)
                        sprintf(assign_in," %s", identif);
                    else if (cnt_temp<=1 && cnt_bin==0)
                        sprintf(assign_in," (%s", identif);
                    else if (cnt_temp>1 && cnt_bin>0)
                        sprintf(assign_in," %s", identif);
                    else if (cnt_temp<=1 && cnt_bin>0)
                        sprintf(assign_in," %s)", identif);

                    if (node->types.operation.op!=SL 
                    	&& node->types.operation.op!=SR)
                        strcat(assign_var, assign_in);

                    int r=0;
                    if (cnt_temp>1)
                    {
                        for (r=0; r<cnt-1; r++){ 
                            if (node->types.operation.op!=SL 
                            	&& node->types.operation.op!=SR 
                            	&& identif2[cnt-1-r][0]!=NULL)
                            {
                                strcat(assign_var, bin_op_exception[cnt-1-r][0]);
                                strcat(assign_var, " ");
                                strcat(assign_var, identif2[cnt-1-r][0]);
                            } //wow this works!!
                            strcat(assign_var,")");
                        }
                    }

                    if (node->children[0]->type!=BINARY_OPERATION 
                    	|| (node->types.operation.op!=MINUS 
                    		&& node->types.operation.op!=DIVIDE 
                    		&& node->types.operation.op!=SL 
                    		&& node->types.operation.op!=SR))
                        strcat(assign_var, parent_bin_op);

                    if (r==(cnt-1) && r!=0)
                    {
                        assign_var[strlen(assign_var)-2]=0;
                        strcat(assign_var, root_bin_op);
                    }
                }
                else if(node_child->num_children > m+1)
                {
                    switch(node_child->children[m+1]->type)
                    {
                        case IDENTIFIERS:
                            {
                                if (shift_flag==1)
                                    sprintf(assign_in,"(%s", concat_id);
                                else
                                    sprintf(assign_in," (%s", concat_id);
                                strcat(assign_var, assign_in);
                                strcat(assign_var, bin_op);
                                break;
                            }
                        case NUMBERS:
                            {
                                if (shift_flag==1)
                                    sprintf(assign_in,"(%s", concat_id);
                                else
                                    sprintf(assign_in," (%s", concat_id);
                                strcat(assign_var, assign_in);
                                strcat(assign_var, bin_op);
                                break;
                            }
                        case ARRAY_REF:
                            {
                                if (shift_flag==1)
                                    sprintf(assign_in,"(%s", concat_id);
                                else
                                    sprintf(assign_in," (%s", concat_id);
                                strcat(assign_var, assign_in);
                                strcat(assign_var, bin_op);
                                break;
                            }
                        case RANGE_REF:
                            {
                                if (shift_flag==1)
                                    sprintf(assign_in,"(%s", concat_id);
                                else
                                    sprintf(assign_in," (%s", concat_id);
                                strcat(assign_var, assign_in);
                                strcat(assign_var, bin_op);
                                break;
                            }
                        case ARRAY_RANGE_REF:
                            {
                                if (shift_flag==1)
                                    sprintf(assign_in,"(%s", concat_id);
                                else
                                    sprintf(assign_in," (%s", concat_id);
                                strcat(assign_var, assign_in);
                                strcat(assign_var, bin_op);
                                break;
                            }
                        case MATRIX_REF:
                            {
                                if (shift_flag==1)
                                    sprintf(assign_in,"(%s", concat_id);
                                else
                                    sprintf(assign_in," (%s", concat_id);
                                strcat(assign_var, assign_in);
                                strcat(assign_var, bin_op);
                                break;
                            }
                        case BINARY_OPERATION:
                            {

                                if (shift_flag==1)
                                    sprintf(assign_in,"(%s", concat_id);
                                else
                                    sprintf(assign_in," (%s", concat_id);
                                strcat(assign_var, assign_in);
                                strcat(assign_var, bin_op);
                                break;
                            }
                        case UNARY_OPERATION:
                            {

                                if (shift_flag==1)
                                    sprintf(assign_in,"(%s", concat_id);
                                else
                                    sprintf(assign_in," (%s", concat_id);
                                strcat(assign_var, assign_in);
                                strcat(assign_var, bin_op);
                                break;
                            }
                        case CONCATENATE:
                            {

                                if (shift_flag==1)
                                    sprintf(assign_in,"(%s", concat_id);
                                else
                                    sprintf(assign_in," (%s", concat_id);
                                strcat(assign_var, assign_in);
                                strcat(assign_var, bin_op);
                                break;
                            }
                        default:
                            {
                                if (shift_flag==1)
                                    sprintf(assign_in,"%s", concat_id);
                                else
                                    sprintf(assign_in," %s", concat_id);
                                strcat(assign_var, assign_in);
                                strcat(assign_var, bin_op);
                            }
                    }
                }
                break;
            }
            case NUMBERS:
            {

                if (recursive_bin_parent_node->type==ARRAY_REF 
                	|| recursive_bin_parent_node->type==RANGE_REF
                    || recursive_bin_parent_node->type==ARRAY_RANGE_REF 
                    || recursive_bin_parent_node->type==CONCATENATE
                    || recursive_bin_parent_node->type==MATRIX_REF)
                {
                    break;
                }

                cnt++;
                cnt_temp++;

                char assign_in[4096]="";
                char identif[8193]="";

                if (node_child->types.number.base == DEC)
                    sprintf(identif," %d", node_child->types.number.value);
                else if (node_child->types.number.base == HEX)
                    sprintf(identif," %d'h%s", node_child->types.number.size, 
                    		node_child->types.number.number); //need fixing
                else if (node_child->types.number.base == BIN)
                    sprintf(identif," %d'b%s", node_child->types.number.binary_size, 
                    		node_child->types.number.binary_string); // Changed 2015
                else if (node_child->types.number.base == OCT)
                    sprintf(identif," %d'o%s", strlen(node_child->types.number.value), 
                    		node_child->types.number.number);
                else if (node_child->types.number.base == SDEC)    
                	sprintf(identif," %d", node_child->types.number.value);
                else if (node_child->types.number.base == SHEX)
                    sprintf(identif," %d'sh%s", node_child->types.number.size, 
                    		node_child->types.number.number); //need fixing
                else if (node_child->types.number.base == SBIN)
                    sprintf(identif," %d'sb%s", node_child->types.number.binary_size, 
                    		node_child->types.number.binary_string); // Changed 2015
                else if (node_child->types.number.base == SOCT)
                    sprintf(identif," %d'so%s", strlen(node_child->types.number.value), 
                    		node_child->types.number.number);
                else
                    sprintf(identif, "%lld", node_child->types.number.value);

                if ((m==node->num_children-1) || m>=1)
                    //if (node->children[m+1]==NULL || m>=1)
                {
                    if ((node->types.operation.op == SR 
                    	 || node->types.operation.op == SL) 
                    	&& node->children[0]->type==BINARY_OPERATION)
                    {
                        //sprintf(identif2, "%s", node_child->types.identifier);
                        identif2[cnt][0]=node_child->types.number.number;
                        char temp[10]="";
                        BinaryOpsGenerator(node, temp);
                        bin_op_exception[cnt][0]=temp;
                        strcat(assign_var, "(");
                        shift_flag=1;
                    }
                    if (node->types.operation.op==MINUS 
                    	&& node->children[0]->type==BINARY_OPERATION)
                    {
                        memmove(identif+1, identif, 8193-1);
                        identif[0]='-';
                        strcat(identif, " +");
                    }

                    else if (node->types.operation.op==DIVIDE 
                    		 && node->children[0]->type==BINARY_OPERATION)
                    {
                        memmove(identif+3, identif, 8193-3);
                        identif[0]='(';
                        identif[1]='1';
                        identif[2]='/';
                        strcat(identif, ") *");
                    }

                    if(cnt_temp>1 && cnt_bin==0)
                        sprintf(assign_in," %s", identif);
                    else if (cnt_temp<=1 && cnt_bin==0)
                        sprintf(assign_in," (%s", identif);
                    else if (cnt_temp>1 && cnt_bin>0)
                        sprintf(assign_in," %s", identif);
                    else if (cnt_temp<=1 && cnt_bin>0)
                        sprintf(assign_in," %s)", identif);
                    //

                    //if (node->children[0]->type!=BINARY_OPERATION)
                    //strcat(assign_var, assign_in);
                    if ((node->types.operation.op!=SL 
                    	 && node->types.operation.op!=SR) 
                    	|| (node->children[0]->type!=BINARY_OPERATION))
                        strcat(assign_var, assign_in);

                    int r;
                    if (cnt_temp>1)
                    {
                        for (r=0; r<cnt-1; r++)
                        {
                            if (node->types.operation.op!=SL 
                            	&& node->types.operation.op!=SR 
                            	&& identif2[cnt-1-r][0]!=NULL 
                            	&& node->types.operation.op!=MINUS)
                            {
                                strcat(assign_var, bin_op_exception[cnt-1-r][0]);
                                strcat(assign_var, " ");
                                strcat(assign_var, identif2[cnt-1-r][0]);
                            }
                            strcat(assign_var,")");
                        }
                    }

                    //
                    if (node->children[0]->type!=BINARY_OPERATION 
                    	|| (node->types.operation.op!=MINUS 
                    		&& node->types.operation.op!=DIVIDE 
                    		&& node->types.operation.op!=SR 
                    		&& node->types.operation.op!=SL))
                        strcat(assign_var, parent_bin_op);

                    if (r==(cnt-1) && r!=0)
                    { assign_var[strlen(assign_var)-2]=0;
                        strcat(assign_var, root_bin_op);
                    }

                    if (r==(cnt-1) && r!=0 && identif2[1][0]!=NULL 
                    	&& (strcmp(parent_bin_op, "<<")==1) 
                    	&& (strcmp(parent_bin_op, ">>")==1))
                        strcat(assign_var, "(");
                }
                    else
                    {
                        {
                            switch(node->children[m+1]->type)
                            {
                                case IDENTIFIERS:
                                    {
                                        if (shift_flag==1)
                                        {
                                            if (node->children[m]->types.number.base==BIN)
                                                sprintf(assign_in, "(%d'b%s", 
                                                		node_child->types.number.binary_size, 
                                                		node_child->types.number.number);
                                            else if (node->children[m]->types.number.base==SBIN)
                                                sprintf(assign_in, "(%d'sb%s", 
                                                	    node_child->types.number.binary_size, 
                                                	    node_child->types.number.number);
                                            else
                                                sprintf(assign_in,"(%d", node_child->types.number.value);
                                        }
                                        else
                                        {
                                            if (node->children[m]->types.number.base==BIN)
                                                sprintf(assign_in, " (%d'b%s", 
                                                		node_child->types.number.binary_size, 
                                                		node_child->types.number.number);
                                            else if (node->children[m]->types.number.base==SBIN)
                                                sprintf(assign_in, " (%d'sb%s", 
                                                		node_child->types.number.binary_size, 
                                                		node_child->types.number.number);
                                            else
                                                sprintf(assign_in," (%d", node_child->types.number.value);
                                        }

                                        strcat(assign_var, assign_in);
                                        strcat(assign_var, bin_op);


                                        break;
                                    }
                                case NUMBERS:
                                    {

                                        if (shift_flag==1)
                                        {
                                            if (node->children[m]->types.number.base==BIN)
                                                sprintf(assign_in, "(%d'b%s", 
                                                		node_child->types.number.binary_size, 
                                                		node_child->types.number.number);
                                            else if (node->children[m]->types.number.base==SBIN)
                                                sprintf(assign_in, "(%d'sb%s", 
                                                		node_child->types.number.binary_size, 
                                                		node_child->types.number.number);
                                            else
                                                sprintf(assign_in,"(%d", node_child->types.number.value);
                                        }
                                        else
                                        {
                                            if (node->children[m]->types.number.base==BIN)
                                                sprintf(assign_in, " (%d'b%s", 
                                                		node_child->types.number.binary_size, 
                                                		node_child->types.number.number);
                                            else if (node->children[m]->types.number.base==SBIN)
                                                sprintf(assign_in, " (%d'sb%s", 
                                                		node_child->types.number.binary_size, 
                                                		node_child->types.number.number);
                                            else
                                                sprintf(assign_in," (%d", node_child->types.number.value);
                                        }


                                        strcat(assign_var, assign_in);
                                        strcat(assign_var, bin_op);

                                        break;
                                    }

                                case ARRAY_REF:
                                    {
                                        if (shift_flag==1)
                                        {
                                            if (node->children[m]->types.number.base==BIN)
                                                sprintf(assign_in, "(%d'b%s", 
                                                		node_child->types.number.binary_size, 
                                                		node_child->types.number.number);
                                            else if (node->children[m]->types.number.base==SBIN)
                                                sprintf(assign_in, "(%d'sb%s", 
                                                		node_child->types.number.binary_size, 
                                                		node_child->types.number.number);
                                            else
                                                sprintf(assign_in,"(%d", node_child->types.number.value);
                                        }
                                        else
                                        {
                                            if (node->children[m]->types.number.base==BIN)
                                                sprintf(assign_in, " (%d'b%s", 
                                                		node_child->types.number.binary_size, 
                                                		node_child->types.number.number);
                                            else if (node->children[m]->types.number.base==SBIN)
                                                sprintf(assign_in, " (%d'sb%s", 
                                                		node_child->types.number.binary_size, 
                                                		node_child->types.number.number);
                                            else
                                                sprintf(assign_in," (%d", node_child->types.number.value);
                                        }
                                        strcat(assign_var, assign_in);
                                        strcat(assign_var, bin_op);

                                        break;
                                    }

                                case RANGE_REF:
                                    {
                                        if (shift_flag==1)
                                        {
                                            if (node->children[m]->types.number.base==BIN)
                                                sprintf(assign_in, "(%d'b%s", 
                                                		node_child->types.number.binary_size, 
                                                		node_child->types.number.number);
                                            else if (node->children[m]->types.number.base==SBIN)
                                                sprintf(assign_in, "(%d'sb%s", 
                                                		node_child->types.number.binary_size, 
                                                		node_child->types.number.number);
                                            else
                                                sprintf(assign_in,"(%d", node_child->types.number.value);
                                        }
                                        else
                                        {
                                            if (node->children[m]->types.number.base==BIN)
                                                sprintf(assign_in, " (%d'b%s", 
                                                		node_child->types.number.binary_size, 
                                                		node_child->types.number.number);
                                            else if (node->children[m]->types.number.base==SBIN)
                                                sprintf(assign_in, " (%d'sb%s", 
                                                		node_child->types.number.binary_size, 
                                                		node_child->types.number.number);
                                            else
                                                sprintf(assign_in," (%d", node_child->types.number.value);
                                        }
                                        strcat(assign_var, assign_in);
                                        strcat(assign_var, bin_op);

                                        break;
                                    }

                                case BINARY_OPERATION:
                                    {
                                        if (shift_flag==1)
                                        {
                                            if (node->children[m]->types.number.base==BIN)
                                                sprintf(assign_in, "(%d'b%s", 
                                                		node_child->types.number.binary_size, 
                                                		node_child->types.number.number);
                                            else if (node->children[m]->types.number.base==SBIN)
                                                sprintf(assign_in, "(%d'sb%s", 
                                                		node_child->types.number.binary_size, 
                                                		node_child->types.number.number);
                                            else
                                                sprintf(assign_in,"(%d", node_child->types.number.value);
                                        }
                                        else
                                        {
                                            if (node->children[m]->types.number.base==BIN)
                                                sprintf(assign_in, " (%d'b%s", 
                                                		node_child->types.number.binary_size, 
                                                		node_child->types.number.number);
                                            else if (node->children[m]->types.number.base==SBIN)
                                                sprintf(assign_in, " (%d'sb%s", 
                                                		node_child->types.number.binary_size, 
                                                		node_child->types.number.number);
                                            else
                                                sprintf(assign_in," (%d", node_child->types.number.value);
                                        }
                                        strcat(assign_var, assign_in);
                                        strcat(assign_var, bin_op);

                                        break;
                                    }

                                case UNARY_OPERATION:
                                    {

                                        if (shift_flag==1)
                                        {
                                            if (node->children[m]->types.number.base==BIN)
                                                sprintf(assign_in, "(%d'b%s", 
                                                		node_child->types.number.binary_size, 
                                                		node_child->types.number.number);
                                            else if (node->children[m]->types.number.base==SBIN)
                                                sprintf(assign_in, "(%d'sb%s", 
                                                		node_child->types.number.binary_size, 
                                                		node_child->types.number.number);
                                            else
                                                sprintf(assign_in,"(%d", node_child->types.number.value);
                                        }
                                        else
                                        {
                                            if (node->children[m]->types.number.base==BIN)
                                                sprintf(assign_in, " (%d'b%s", 
                                                		node_child->types.number.binary_size, 
                                                		node_child->types.number.number);
                                            else if (node->children[m]->types.number.base==SBIN)
                                                sprintf(assign_in, " (%d'sb%s", 
                                                		node_child->types.number.binary_size, 
                                                		node_child->types.number.number);
                                            else
                                                sprintf(assign_in," (%d", node_child->types.number.value);
                                        }
                                        strcat(assign_var, assign_in);
                                        strcat(assign_var, bin_op);

                                        break;
                                    }


                                default:
                                    {
                                        if (shift_flag==1)
                                        {
                                            if (node->children[m]->types.number.base==BIN)
                                                sprintf(assign_in, "(%d'b%s", 
                                                		node_child->types.number.binary_size, 
                                                		node_child->types.number.number);
                                            else if (node->children[m]->types.number.base==SBIN)
                                                sprintf(assign_in, "(%d'sb%s", 
                                                		node_child->types.number.binary_size, 
                                                		node_child->types.number.number);

                                            else
                                                sprintf(assign_in,"(%d", node_child->types.number.value);
                                        }
                                        else
                                        {
                                            if (node->children[m]->types.number.base==BIN)
                                                sprintf(assign_in, " (%d'b%s", 
                                                		node_child->types.number.binary_size, 
                                                		node_child->types.number.number);
                                            else if (node->children[m]->types.number.base==SBIN)
                                                sprintf(assign_in, " (%d'sb%s", 
                                                		node_child->types.number.binary_size, 
                                                		node_child->types.number.number);

                                            else
                                                sprintf(assign_in," (%d", node_child->types.number.value);
                                        }



                                        if (node->types.operation.op==MINUS && node_child->num_children!=0)
                                        {
                                            memmove(identif+1, identif, 8193-1);
                                            identif[0]='-';
                                            strcat(identif, " +");
                                        }

                                        else if (node->types.operation.op==DIVIDE)
                                        {
                                            memmove(identif+3, identif, 8193-3);
                                            identif[0]='(';
                                            identif[1]='1';
                                            identif[2]='/';
                                            strcat(identif, ") *");
                                        }

                                        if(cnt_temp>1 && cnt_bin==0)
                                            sprintf(assign_in," %s", identif);
                                        else if (cnt_temp<=1 && cnt_bin==0)
                                            sprintf(assign_in," (%s", identif);
                                        else if (cnt_temp>1 && cnt_bin>0)
                                            sprintf(assign_in," %s", identif);
                                        else if (cnt_temp<=1 && cnt_bin>0)
                                            sprintf(assign_in," %s)", identif);
                                        //

                                        if (node->types.operation.op!=SL && node->types.operation.op!=SR)
                                            strcat(assign_var, assign_in);

                                        int r;
                                        if (cnt_temp>1)
                                        {

                                            for (r=0; r<cnt-1; r++)
                                            {
                                                if (node->types.operation.op!=SL 
                                                	&& node->types.operation.op!=SR 
                                                	&& identif2[cnt-1-r][0]!=NULL)
                                                {
                                                    strcat(assign_var, bin_op_exception[cnt-1-r][0]);
                                                    strcat(assign_var, " ");
                                                    strcat(assign_var, identif2[cnt-1-r][0]);
                                                } //wow this works!!
                                                strcat(assign_var,")");
                                            }
                                        }


                                        //
                                        if (node->types.operation.op!=MINUS && node->types.operation.op!=DIVIDE)
                                            strcat(assign_var, parent_bin_op);
                                        else if (node->types.operation.op==MINUS && node_child->num_children==0)
                                            strcat(assign_var, parent_bin_op);
                                        else if (node->types.operation.op==DIVIDE && node_child->num_children==0)
                                            strcat(assign_var, parent_bin_op);

                                        if (r==(cnt-1) && r!=0 
                                        	&& (strcmp(root_bin_op, "<<")==1) 
                                        	&& (strcmp(root_bin_op,">>")==1))
                                        {
                                            assign_var[strlen(assign_var)-2]=0;
                                            strcat(assign_var, root_bin_op);
                                        }
                                    }

                            }

                        }


                        break;
                    }
                }

                //						case NUMBERS:
                //						{
                //
                //							if (recursive_bin_parent_node->type==ARRAY_REF 
                //								|| recursive_bin_parent_node->type==RANGE_REF 
                //								|| recursive_bin_parent_node->type==CONCATENATE 
                //								|| recursive_bin_parent_node->type==MATRIX_REF)
                //								break;
                //
                //							cnt++;
                //							cnt_temp++;
                //
                //							char assign_in[4096]="";
                //							if (node->children[m+1]==NULL)
                //							{
                //
                //								if(cnt_temp>1)
                //								{
                //									if (node_child->types.number.base == DEC)
                //										sprintf(assign_in," %s", node_child->types.number.number);
                //									else if (node_child->types.number.base == HEX)
                //										sprintf(assign_in," %d'h%s", strlen(node_child->types.number.number)*4, node_child->types.number.number); //need fixing
                //									else if (node_child->types.number.base == BIN)
                //										sprintf(assign_in," %d'b%s", strlen(node_child->types.number.number), node_child->types.number.number);
                //									else if (node_child->types.number.base == OCT)
                //										sprintf(assign_in," %d'o%s", strlen(node_child->types.number.number), node_child->types.number.number);
                //								}
                //								else
                //									sprintf(assign_in," (%s", node_child->types.number.number);
                //								strcat(assign_var, assign_in);
                //								int r;
                //								if (cnt_temp>=1)
                //								{
                //
                //									for (r=0; r<cnt -1; r++)
                //											strcat(assign_var,")"); //wow this works!!
                //								}
                //
                //
                //								strcat(assign_var, parent_bin_op);
                //								if (r==(cnt-1) && r!=0)
                //								{
                //									assign_var[strlen(assign_var)-2]=0;
                //									strcat(assign_var, root_bin_op);
                //								}
                //
                //
                //							}
                //							else
                //							{
                //								switch(node->children[m+1]->type)
                //								{
                //									case IDENTIFIERS:
                //										{
                //
                //											sprintf(assign_in," (%s", node_child->types.identifier);
                //											strcat(assign_var, assign_in);
                //											strcat(assign_var, bin_op);
                //
                //											break;
                //										}
                //									case NUMBERS:
                //										{
                //
                //											sprintf(assign_in," (%s", node_child->types.number.number);
                //											strcat(assign_var, assign_in);
                //											strcat(assign_var, bin_op);
                //
                //											break;
                //										}
                //
                //									default:
                //									{
                //										sprintf(assign_in," %s", node_child->types.number.number); //BUG here. need FIXING
                //										strcat(assign_var, assign_in);
                //										strcat(assign_var, bin_op);
                //
                //										break;
                //									}
                //
                //								}
                //
                //							}
                //
                //
                //						break;
                //						}

            }
        }
    }

    while (i<node->num_children)
    {
        if (node->children[i]==NULL){}
        else
        {
            recursive_bin_parent_node = node->children[i];
            if (node->type==ARRAY_REF && node->children[i]->type==BINARY_OPERATION)
                break;
            if (node->type==RANGE_REF && node->children[i]->type==BINARY_OPERATION)
                break;
            if (node->type==MATRIX_REF && node->children[i]->type==BINARY_OPERATION)
                break;
            if (node->type==ARRAY_RANGE_REF && node->children[i]->type==BINARY_OPERATION)
                break;
            BinaryOpsGenerator(node->children[i], bin_op);

            RecursiveBinaryop(node->children[i], assign_var, root_bin_op, bin_op,cnt, cnt0);
            i++;
        }
    }
}

int TraverseTree(ast_node_t* node, int child_cntr)
{
    int i=0;
    int done=0;
    while (i<node->num_children)
    {
        if (node->children[i]==NULL){}
        else
        {
            TraverseTree(node->children[i], child_cntr);
            i++;
        }
    }
    child_cntr =i;
    done = 1;
    return done;

}
//finish addition by Kumud

// Dump ast to a text file
void write_AST_to_file(FILE* fp, ast_node_t* node, int from_node)
{
    int i, num;
    int my_label=unique_label_count;
    char *ptr;
    unique_label_count++;
    fprintf(fp,"%d ---> %d\n", from_node, my_label);

    fprintf(fp,"%d %d %d %d %d %d %hd\n", node->type, node->num_children,
            node->line_number, node->file_number, node->far_tag,
            node->high_number, node->shared_node);
    switch(node->type)
    {
        case FILE_ITEMS:
        case MODULE:
        case MODULE_ITEMS:
        case VAR_DECLARE_LIST:
        case ASSIGN:
        case GATE:
        case GATE_INSTANCE:
        case MODULE_CONNECT_LIST:
        case MODULE_CONNECT:
        case MODULE_PARAMETER_LIST:
        case MODULE_PARAMETER:
        case MODULE_NAMED_INSTANCE:
        case MODULE_INSTANCE:
        case HARD_BLOCK:
        case HARD_BLOCK_NAMED_INSTANCE:
        case HARD_BLOCK_CONNECT:
        case HARD_BLOCK_CONNECT_LIST:
        case BLOCK:
        case NON_BLOCKING_STATEMENT_SIGNED:
        case NON_BLOCKING_STATEMENT:
        case BLOCKING_STATEMENT:
        case CASE:
        case CASE_LIST:
        case CASE_ITEM:
        case CASE_DEFAULT:
        case ALWAYS:
        case DELAY_CONTROL:
        case POSEDGE:
        case NEGEDGE:
        case IF:
        case GENERATE:
        case DEFINE:
        case FOR:
        case IF_Q:
            fprintf(fp,"--\n");
            break;
        case DOUBLE_ARRAY_DECLARE:
        case VAR_DECLARE:
            fprintf(fp,"%hd %hd %hd %hd %hd %hd %hd %hd %hd %hd %hd %hd %hd\n",
                    node->types.variable.is_input, node->types.variable.is_input_signed,
                    node->types.variable.is_output, node->types.variable.is_output_reg,
                    node->types.variable.is_inout, node->types.variable.is_port,
                    node->types.variable.is_parameter, node->types.variable.is_wire,
                    node->types.variable.is_reg, node->types.variable.is_integer,
                    node->types.variable.is_genvar, node->types.variable.is_reg_signed,
                    node->types.variable.is_wire_signed);
            break;
        case BINARY_OPERATION:
        case UNARY_OPERATION:
            fprintf(fp,"%d\n", node->types.operation.op);
            break;
        case NUMBERS:

            fprintf(fp, "%hd %d %d ", node->types.number.base,
                    node->types.number.size, node->types.number.binary_size);
            if(node->types.number.binary_string != NULL){
                fprintf(fp,"%s ", node->types.number.binary_string);
            }
            else{
                fprintf(fp,"NULL ");
            }
            ptr = node->types.number.number;
            num = 1;
            if(ptr != NULL){
                while(*ptr){
                    if(*ptr > 57 || *ptr < 48){
                        num = 0;
                        break;
                    }
                    ptr++;
                }
                if(num) fprintf(fp, "%s %lld\n",node->types.number.number, node->types.number.value);
                else fprintf(fp, "NULL %lld\n", node->types.number.value);
            }
            else{
                fprintf(fp, "NULL %lld\n", node->types.number.value);
            }
            break;
        case IDENTIFIERS:
            fprintf(fp,"%s\n", node->types.identifier);
            break;
        default:
            fprintf(fp,"--\n");
            break;
    }

    for(i=0; i < node->num_children; i++){
        if(node->children[i] != NULL)
            write_AST_to_file(fp, node->children[i], my_label);//node->unique_count);
    }
}
// Read input file and recreate the AST
ast_node_t* read_AST_from_file(char* file_in)
{
    FILE* fp;
    int i, found, from_node, to_node;
    int line_num = 0;
    ast_node_t tmp;
    ast_node_t* top = NULL;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    char temp[1024]="";
    char temp1[1024]="";

    fp = fopen(file_in, "r");
    if(fp==NULL){
        error_message(-1,-1,-1, "Cannot open AST input file for read.\n");
        return NULL;
    }

    while ((read = getline(&line, &len, fp)) != -1)
    {
        ast_node_t* node;
        line_num++;
        sscanf(line, "%d %s %d", &from_node, temp, &to_node);
        if((read = getline(&line, &len, fp)) == -1){
            error_message(-1,-1,-1, "Incomplete Node found! (-1) %d\n", line_num);
            break;
        }
        line_num++;
        sscanf(line,"%d %d %d %d %d %d %hd\n", (int*)&tmp.type, (int*)&tmp.num_children,
                (int*)&tmp.line_number, (int*)&tmp.file_number, (int*)&tmp.far_tag,
                (int*)&tmp.high_number, (int*)&tmp.shared_node);
        if(strcmp("--->",temp)==0){
            node = create_node_w_type(tmp.type, tmp.line_number, tmp.file_number);
            node->unique_count = to_node;
        }else{
            error_message(-1,-1,-1, "AST input file contain unrecognized format at line %d\n", line_num);
        }

        switch(tmp.type)
        {
            case VAR_DECLARE:
            case DOUBLE_ARRAY_DECLARE:
                if((read = getline(&line, &len, fp)) == -1){
                    error_message(-1,-1,-1, "Incomplete Node found! (-2) %d\n", line_num);
                    break;
                }
                line_num++;
                sscanf(line,"%hd %hd %hd %hd %hd %hd %hd %hd %hd %hd %hd %hd %hd\n",
                        &node->types.variable.is_input, &node->types.variable.is_input_signed,
                        &node->types.variable.is_output, &node->types.variable.is_output_reg,
                        &node->types.variable.is_inout, &node->types.variable.is_port,
                        &node->types.variable.is_parameter, &node->types.variable.is_wire,
                        &node->types.variable.is_reg, &node->types.variable.is_integer,
                        &node->types.variable.is_genvar, &node->types.variable.is_reg_signed,
                        &node->types.variable.is_wire_signed);

                break;
            case BINARY_OPERATION:
            case UNARY_OPERATION:
                if((read = getline(&line, &len, fp)) == -1){
                    error_message(-1,-1,-1, "Incomplete Node found! (-3) %d\n", line_num);
                    break;
                }
                line_num++;
                sscanf(line,"%d\n", &node->types.operation.op);
                break;
            case NUMBERS:
                if((read = getline(&line, &len, fp)) == -1){
                    error_message(-1,-1,-1, "Incomplete Node found! (-4) %d\n", line_num);
                    break;
                }
                line_num++;
                sscanf(line, "%hd %d %d %s %s %lld", &node->types.number.base,
                        &node->types.number.size, &node->types.number.binary_size,
                        temp, temp1, &node->types.number.value);
                if(strcmp("NULL", temp) != 0){
                    i = strlen(temp);
                    node->types.number.binary_string = malloc(sizeof(char)*(i+1));
                    if(node->types.number.binary_string == NULL){
                        error_message(-1,-1,-1,"Error, cannot allocate number string for node. %d\n", line_num);
                    }
                    strcpy(node->types.number.binary_string,temp);

                }
                if(strcmp("NULL", temp1) != 0){
                    i = strlen(temp1);
                    node->types.number.number = malloc(sizeof(char)*(i+1));
                    if(node->types.number.number == NULL){
                        error_message(-1,-1,-1,"Error, cannot allocate number string for node. %d\n", line_num);
                    }
                    strcpy(node->types.number.number,temp1);
                }
                break;
            case IDENTIFIERS:
                if((read = getline(&line, &len, fp)) == -1){
                    error_message(-1,-1,-1, "Incomplete Node found! (-5) %d\n", line_num);
                    break;
                }
                line_num++;
                sscanf(line,"%s", temp);
                i = strlen(temp);
                node->types.identifier = malloc(sizeof(char)*(i+1));
                if(node->types.identifier == NULL){
                    error_message(-1,-1,-1,"Error, cannot allocate string for identifier\n");
                    break;
                }
                strcpy(node->types.identifier,temp);
                break;
            default:
                if((read = getline(&line, &len, fp)) == -1){
                    error_message(-1,-1,-1, "Incomplete Node found! (-6) %d\n", line_num);
                }
                line_num++;
                break;
        }
        if(from_node == -1){
            // top node
            top = node;
        }else{
            // if not top, connect the node to its parent
            found = 0;
            read_AST_connect_child(top, node, from_node, &found);
            if(found != 1){
                error_message(-1,-1,-1,"Node parent not found. Freeing node. %d\n", line_num);
                free_ast_node_only(node);
            }
        }
    }
    if (line)
        free(line);
    fclose(fp);
    return top;
}

void read_AST_connect_child(ast_node_t* top, ast_node_t* node, int from_node, int* found)
{
    int i;
    if (*found != 1){
        if(top->unique_count == from_node){
            add_child_to_node(top, node);
            *found = 1;
        }else{
            for (i=0; i<top->num_children; i++){
                if(top->children[i] != NULL)
                    read_AST_connect_child(top->children[i], node, from_node, found);
            }
        }
    }
}
void Ast_param_substition(ast_node_t* top, record_id* para_lib)
{
    int i, check=0;
    switch(top->type){
        case IDENTIFIERS:
            check = 0;
            lookup_para = para_lib;
            for (i=0; i<num_paras; i++){
                top->types.identifier;
                lookup_para->content->types.identifier;
                if ( !strcmp(top->types.identifier, 
                			 lookup_para->content->types.identifier) )
                {
                    check = 1;
                    break;
                }
                lookup_para = lookup_para->next;
            }
            if(check == 1){
                free(top->types.identifier);
                top->types.number.base = LONG_LONG;
                top->types.number.value = lookup_para->value;
                top->type = NUMBERS;
            }
            break;
        default:
            break;
    }
    for(i=0; i<top->num_children; i++){
        if (top->children[i] != NULL)
            Ast_param_substition(top->children[i], para_lib);
    }

}
int lookup_bitwidth(ast_node_t* top){
    /*    int i=0;
          record_identifier(  ){

          }
          for(i=0; i<top->number_children){

          }*/
    return 0;
}

/* Added to change an approximate multiplier into a MODULE_INSTANCE */
void transformTree (ast_node_t* mult_node,int s1, int s2)
{
	char tempholder[8196]="";
        //Making the new nodes necessary for making the approx mult a new submodule
        ast_node_t* module_instance = create_node_w_type(MODULE_INSTANCE,999,current_parse_file);
        ast_node_t* my_mult = create_node_w_type(IDENTIFIERS,999,current_parse_file);
	//my_mult->types.identifier = "DRUM6";
	sprintf(tempholder, "DRUM6_%d_%d", s1, s2);
	my_mult->types.identifier = (char*)calloc(sizeof(tempholder), sizeof(char));
	strcpy(my_mult->types.identifier, tempholder);
        ast_node_t* module_named_instance = create_node_w_type(MODULE_NAMED_INSTANCE,999,current_parse_file);
        ast_node_t* U1 = create_node_w_type(IDENTIFIERS,999,current_parse_file);
        //U1->types.identifier = "automated_drum_instance";
	sprintf(tempholder, "automated_drum_instance_G%d_%d", G, app_mult_cnt);
	U1->types.identifier = (char*)calloc(sizeof(tempholder), sizeof(char));
        strcpy(U1->types.identifier, tempholder);
        ast_node_t* module_connect_list = create_node_w_type(MODULE_CONNECT_LIST,999,current_parse_file);
        ast_node_t* module_connect_a = create_node_w_type(MODULE_CONNECT,999,current_parse_file);
        ast_node_t* module_connect_b = create_node_w_type(MODULE_CONNECT,999,current_parse_file);
        ast_node_t* module_connect_r = create_node_w_type(MODULE_CONNECT,999,current_parse_file);
        ast_node_t* a = create_node_w_type(IDENTIFIERS,999,current_parse_file);
        a->types.identifier = "a";
        ast_node_t* b = create_node_w_type(IDENTIFIERS,999,current_parse_file);
        b->types.identifier = "b";
        ast_node_t* r = create_node_w_type(IDENTIFIERS,999,current_parse_file);
        r->types.identifier = "r";
	ast_node_t* TMP = create_node_w_type(IDENTIFIERS,999,current_parse_file);
	//TMP->types.identifier = "automated_drum_output";
	sprintf(tempholder, "automated_drum_output_G%d_%d", G, app_mult_cnt);
	TMP->types.identifier = (char*)calloc(sizeof(tempholder), sizeof(char));
        strcpy(TMP->types.identifier, tempholder);
	
	char *t2;
	t2 = (char*)malloc(sizeof(char)*4096);
	//Connecting the nodes
        add_child_to_node(module_instance,my_mult);
        add_child_to_node(module_instance,module_named_instance);
        add_child_to_node(module_named_instance,U1);
        add_child_to_node(module_named_instance,module_connect_list);
        add_child_to_node(module_connect_list,module_connect_a);
        add_child_to_node(module_connect_list,module_connect_b);
        add_child_to_node(module_connect_list,module_connect_r);
        add_child_to_node(module_connect_a,a);
        add_child_to_node(module_connect_b,b);
        add_child_to_node(module_connect_r,r);
        add_child_to_node(module_connect_a,mult_node->children[0]);
        add_child_to_node(module_connect_b,mult_node->children[1]);
        add_child_to_node(module_connect_r,TMP);
        add_child_to_node(MODULE_ITEMS_ROOT,module_instance);
	//Make the var list for Wire decleration
	char rh[4096]="";
	sprintf(rh, "%d", s1+s2-1);
	ast_node_t* range_high = newNumberNode(rh,1);
	ast_node_t* range_low  = newNumberNode("0",1);
	sprintf(tempholder, "automated_drum_output_G%d_%d", G, app_mult_cnt);
	strcpy(t2, tempholder);
	ast_node_t* temp_var  = newVarDeclare(t2,range_high,range_low,NULL,NULL,NULL,1);
	ast_node_t* temp_var_list = newList(VAR_DECLARE_LIST,temp_var);
	markAndProcessSymbolListWith(WIRE,temp_var_list);
	add_child_to_node(MODULE_ITEMS_ROOT,temp_var_list);
}

/* Changes the multiplication node to identifier */
void changeNode (ast_node_t* tempnode)
{
	char tempholder[200]="";
	int s1,s2;
	s1=getRange(tempnode->children[0]);
	s2=getRange(tempnode->children[1]);
	transformTree(tempnode,s1,s2);
	tempnode->num_children=0;
	tempnode->children=NULL;
	tempnode->type=IDENTIFIERS;
	sprintf(tempholder, "automated_drum_output_G%d_%d", G, app_mult_cnt);
	tempnode->types.identifier = (char*)calloc(sizeof(tempholder), sizeof(char));
	strcpy(tempnode->types.identifier, tempholder);
	app_mult_cnt++;
}
/* Finds the range of a nodes children */
int getRange (ast_node_t* child_node)
{
	int wd=0;
	int wd_child=0;
	int i;
	switch (child_node->type)
	{
		case RANGE_REF:
		{
			if(child_node->children[0]->type==IDENTIFIERS 
				&& child_node->children[1]->type==NUMBERS 
				&& child_node->children[2]->type==NUMBERS)
			{
				wd=child_node->children[1]->types.number.value 
					- child_node->children[2]->types.number.value + 1;
			}
			else if(child_node->children[0]->type==IDENTIFIERS 
					&& child_node->children[1]->type==BINARY_OPERATION 
					&& child_node->children[2]->type==NUMBERS)
			{
				if(child_node->children[1]->children[0]->type==IDENTIFIERS 
				   && child_node->children[1]->children[1]->type==NUMBERS)
				{
					int r1=findRange(child_node->children[1]->children[0]->types.identifier);
					int r2=child_node->children[1]->children[1]->types.number.value;
					int r3=0;
					int r4=0;
					switch(child_node->children[1]->types.operation.op)
					{
						case MINUS: {r3=r1-r2; break;}
						case ADD: {r3=r1+r2; break;}
						case MULTIPLY: {r3=r1*r2; break;}
						default: {
							error_message(PARSE_ERROR, 
										current_parse_file, 999, 
										"OPERATION IN ARGUMENT 1 NOT SUPPORTED");
								 }
					}
					r4=child_node->children[2]->types.number.value;
					wd=r3-r4+1;
					break;
				}
				if(child_node->children[1]->children[0]->type==NUMBERS 
				   && child_node->children[1]->children[1]->type==IDENTIFIERS)
				{
					int r1=child_node->children[1]->children[0]->types.number.value;
					int r2=findRange(child_node->children[1]->children[1]->types.identifier);
					int r3=0;
					int r4=0;
					switch(child_node->children[1]->types.operation.op)
					{
						case MINUS: {r3=r1-r2; break;}
						case ADD: {r3=r1+r2; break;}
						case MULTIPLY: {r3=r1*r2; break;}
						default: {
							error_message(PARSE_ERROR, 
										  current_parse_file, 999, 
										  "OPERATION IN ARGUMENT 2 NOT SUPPORTED");
								 }
					}
					r4=child_node->children[2]->types.number.value;
					//printf("r1 = %d , r2 = %d , r3 = %d , r4 = %d",r1,r2,r3,r4);
					wd=r3-r4+1;
					break;
				}
				else if(child_node->children[1]->children[0]->type==IDENTIFIERS 
						&& child_node->children[1]->children[1]->type==IDENTIFIERS)
				{
					int r1=findRange(child_node->children[1]->children[0]->types.identifier);
					int r2=findRange(child_node->children[1]->children[1]->types.identifier);
					int r3=0;
					int r4=0;
					switch(child_node->children[1]->types.operation.op)
					{
						case MINUS: {r3=r1-r2; break;}
						case ADD: {r3=r1+r2; break;}
						case MULTIPLY: {r3=r1*r2; break;}
						default: {
							error_message(PARSE_ERROR, 
										  current_parse_file, 999, 
										  "OPERATION IN ARGUMENT 1&2 NOT SUPPORTED");
								 }
					}
					r4=child_node->children[2]->types.number.value;
					//printf("r1 = %d , r2 = %d , r3 = %d , r4 = %d",r1,r2,r3,r4);
					wd=r3-r4+1;
					break;
				}
				else
					error_message(PARSE_ERROR, current_parse_file, 
								  999, "BINARY OPERATION IN RANGE_REF_ERROR");
			}
			else
			{
				error_message(PARSE_ERROR, current_parse_file, 
							  999, "Consider this case. ERROR ID:RANGE_REF");
			}
			break;
		}
		case ARRAY_RANGE_REF:
		{
			if(child_node->children[0]->type==IDENTIFIERS 
			   && child_node->children[2]->type==NUMBERS 
			   && child_node->children[3]->type==NUMBERS)
			{
				wd=child_node->children[2]->types.number.value 
					- child_node->children[3]->types.number.value + 1;
			}
			else if(child_node->children[0]->type==IDENTIFIERS 
					&& child_node->children[2]->type==BINARY_OPERATION 
					&& child_node->children[3]->type==NUMBERS)
			{
				if(child_node->children[2]->children[0]->type==IDENTIFIERS 
				   && child_node->children[2]->children[1]->type==NUMBERS)
				{
					int r1=findRange(child_node->children[2]->children[0]->types.identifier);
					int r2=child_node->children[2]->children[1]->types.number.value;
					int r3=0;
					int r4=0;
					switch(child_node->children[2]->types.operation.op)
					{
						case MINUS: {r3=r1-r2; break;}
						case ADD: {r3=r1+r2; break;}
						case MULTIPLY: {r3=r1*r2; break;}
						default: {
							error_message(PARSE_ERROR, 
										  current_parse_file, 999, 
										  "OPERATION IN ARGUMENT 1 NOT SUPPORTED");
								 }
					}
					r4=child_node->children[3]->types.number.value;
					//printf("r1 = %d , r2 = %d , r3 = %d , r4 = %d",r1,r2,r3,r4);
					wd=r3-r4+1;
					break;
				}
				if(child_node->children[2]->children[0]->type==NUMBERS 
				   && child_node->children[2]->children[1]->type==IDENTIFIERS)
				{
					int r1=child_node->children[2]->children[0]->types.number.value;
					int r2=findRange(child_node->children[2]->children[1]->types.identifier);
					int r3=0;
					int r4=0;
					switch(child_node->children[2]->types.operation.op)
					{
						case MINUS: {r3=r1-r2; break;}
						case ADD: {r3=r1+r2; break;}
						case MULTIPLY: {r3=r1*r2; break;}
						default: {
							error_message(PARSE_ERROR, 
										  current_parse_file, 999, 
										  "OPERATION IN ARGUMENT 2 NOT SUPPORTED");
								 }
					}
					r4=child_node->children[3]->types.number.value;
					//printf("r1 = %d , r2 = %d , r3 = %d , r4 = %d",r1,r2,r3,r4);
					wd=r3-r4+1;
					break;
				}
				else if(child_node->children[2]->children[0]->type==IDENTIFIERS 
						&& child_node->children[2]->children[1]->type==IDENTIFIERS)
				{
					int r1=findRange(child_node->children[2]->children[0]->types.identifier);
					int r2=findRange(child_node->children[2]->children[1]->types.identifier);
					int r3=0;
					int r4=0;
					switch(child_node->children[2]->types.operation.op)
					{
						case MINUS: {r3=r1-r2; break;}
						case ADD: {r3=r1+r2; break;}
						case MULTIPLY: {r3=r1*r2; break;}
						default: {
							error_message(PARSE_ERROR, 
										  current_parse_file, 999, 
										  "OPERATION IN ARGUMENT 1&2 NOT SUPPORTED");
								 }
					}
					r4=child_node->children[3]->types.number.value;
					//printf("r1 = %d , r2 = %d , r3 = %d , r4 = %d",r1,r2,r3,r4);
					wd=r3-r4+1;
					break;
				}
				else
					error_message(PARSE_ERROR, current_parse_file, 
								  999, "BINARY OPERATION IN ARRAY_RANGE_REF_ERROR");

			}
			else
			{
				error_message(PARSE_ERROR, current_parse_file, 
							  999, "Consider this case. ERROR ID:ARRAY_RANGE_REF");
			}
			break;
		}
		case ARRAY_REF:
		{
			char *id=child_node->children[0]->types.identifier;
			wd=findRange(id);
			break;
		}	
		case IDENTIFIERS:
		{
			char *id=child_node->types.identifier;
			wd=findRange(id);
			break;
		}
		case CONCATENATE:
		{
			for(i=0;i<child_node->num_children;i++)
			{
				ast_node_t *grandchild=child_node->children[i];
				if(grandchild->type==CONCATENATE)
					wd=grandchild->num_children;	
				else if(grandchild->type==NUMBERS 
						&& (grandchild->types.number.base==BIN 
							|| grandchild->types.number.base==SBIN))
					wd=grandchild->types.number.binary_size;
				else
					wd_child=getRange(grandchild);
				wd=wd+wd_child;
			}
			break;
		}
        case NUMBERS:
        {
            wd=child_node->types.number.binary_size;
            break;
        }
		default:
		{
			for(i=0;i<child_node->num_children;i++)
			{
				ast_node_t *grandchild=child_node->children[i];
				wd_child=getRange(grandchild);
				wd=wd+wd_child;
				//char *id=child_node->types.identifier;
				//wd[i]=findRange(id);
			}
			break;
		}
	}
	return wd;
}
/* FindRange will traverse the tree as necessary and get the width of an input identifier */
int findRange (char *id)
{
	int i,j,k;
	ast_node_t *inout_list=TREE_ROOT->children[1];
	ast_node_t *module_items_list=TREE_ROOT->children[2];
	
	//NOW Check the Wire Reg definitions
	for(i=0; i<module_items_list->num_children;i++)
	{
		ast_node_t *temp_node=module_items_list->children[i];
		switch(temp_node->type)
		{	
			case VAR_DECLARE_LIST:
			{
				for(k=0;k<temp_node->num_children;k++)
				{
					ast_node_t *temp_node2=temp_node->children[k];
					if(temp_node2->children[0]->type==IDENTIFIERS 
					   && strcmp(temp_node2->children[0]->types.identifier,id)==0)
					{
						if(temp_node2->num_children==6)
						{
							if(temp_node2->children[5]!=NULL)
							{
								if(temp_node2->children[1]==NULL)
								{
									return temp_node2->children[5]->types.number.value;
								}
								else
								{
									return temp_node2->children[5]->types.number.value;
								}
							}
							else if(temp_node2->children[1]==NULL)
							{
								return 1;
							}
							else
							{
								if(temp_node2->children[1]->type==NUMBERS 
								   && temp_node2->children[2]->type==NUMBERS)
								{
									return (temp_node2->children[1]->types.number.value 
											- temp_node2->children[2]->types.number.value + 1);
								}
								else if(temp_node2->children[1]->type==BINARY_OPERATION 
										&& temp_node2->children[2]->type==NUMBERS)
								{
									int r1=findRange(temp_node2->children[1]->children[0]->types.identifier);
									int r2=temp_node2->children[1]->children[1]->types.number.value;
									int r3=0;
									int r4=0;
									switch(temp_node2->children[1]->types.operation.op)
									{
									case MINUS: {r3=r1-r2; break;}
									case ADD: {r3=r1+r2; break;}
									case MULTIPLY: {r3=r1*r2; break;}
									default: {
										error_message(PARSE_ERROR, 
													  current_parse_file, 999, 
													  "OPERATION IN ARGUMENT 1 NOT SUPPORTED");
											 }
									}
									r4=temp_node2->children[2]->types.number.value;
									return (r3 - r4 + 1);
								}
								else if(temp_node2->children[1]->type==NUMBERS 
										&& temp_node2->children[2]->type==BINARY_OPERATION)
								{
									int r1=temp_node2->children[2]->children[2]->types.number.value;
									int r2=findRange(temp_node2->children[2]->children[0]->types.identifier);
									int r3=0;
									int r4=0;
									switch(temp_node2->children[2]->types.operation.op)
									{
									case MINUS: {r3=r1-r2; break;}
									case ADD: {r3=r1+r2; break;}
									case MULTIPLY: {r3=r1*r2; break;}
									default: {
										error_message(PARSE_ERROR, 
													  current_parse_file, 999, 
													  "OPERATION IN ARGUMENT 2 NOT SUPPORTED");
											 }
									}
									r4=temp_node2->children[1]->types.number.value;
									return (r3 - r4 + 1);
								}
								else if(temp_node2->children[1]->type==BINARY_OPERATION 
										&& temp_node2->children[2]->type==BINARY_OPERATION)
								{
									error_message(PARSE_ERROR, current_parse_file, 
												  999, "OPERATION IN RANGE 1&2  FOR %s", id);
								}
								else
								{
									error_message(PARSE_ERROR, current_parse_file, 
												  999, "ERROR ID: 5 Child new Case... Uhhh..");
								}
							}
						}
						else
						{printf("%d",temp_node2->num_children);
						error_message(PARSE_ERROR, current_parse_file, 
									  999, "NUMBER OF CHILDREN NOT 6 , %s", id);
						}
					}
				}
				break;
			}
			default:
			{
				break;
			}
				
		}
	}
	error_message(PARSE_ERROR, current_parse_file, 
				  999, "The Variable %s is not declared in the graph.",id);
	return -1;

}

/*DOES A GLOBAL SEARCH TO GET THE NUMBER OF GENERATIONS PASSED BY*/
int getG()
{
	int i;
	int j;
	for(i=0;i<TREE_ROOT->num_children;i++)
	{
		ast_node_t *root_child=TREE_ROOT->children[i];
		if(root_child->type==MODULE_ITEMS)
		{
			for(j=root_child->num_children-1;j>=0;j--)
			{
				ast_node_t *level2=root_child->children[j];
				if(level2->type==MODULE_INSTANCE)
				{
					char *inst_name;
					inst_name=(char*)malloc(5);
					strncpy(inst_name,level2->children[0]->types.identifier,4);
					inst_name[4]='\0';
					if(strcmp(inst_name,"DRUM")==0)
					{
						char value=level2->children[1]->children[0]->types.identifier[25];
						return (value- '0');
					}
				}
			}
		}
	}
	return 0;
}

