#include "types.h"

void parse_to_ast();

/* INITIALISATIONS */
void init_parser();
void cleanup_parser();
void cleanup_hard_blocks();
void init_parser_for_file();
void clean_up_parser_for_file();

//added by Kumud
void change_mult_to_plus(ast_node_t *node);
void change_signs(ast_node_t *node, ast_node_t *from, int from_num);
void make_constant(ast_node_t *top);
void make_id_constant(ast_node_t *node, ast_node_t *from, 
				      int from_num, char **matlab_vars, 
				      int matlab_vars_cnt);
void replace_with_const(ast_node_t *node, char **matlab_vars, int where);
void factorize (ast_node_t *node, ast_node_t *from, int from_num);
void pre_add (ast_node_t *node, ast_node_t *from, int from_num);
void NodeReplaceFactorizaton(ast_node_t *nodetochange_from, ast_node_t *nodetochange_to);
void truncate_bitwidth(ast_node_t *node, ast_node_t* top_node, int num_bits);
void truncate_bitwidth_alternative(ast_node_t *node, ast_node_t* top_node, int num_bits);

ast_node_t *make_ref_range(ast_node_t *node, ast_node_t *top_node, int num_bits_truncate);
void change_id_to_range(ast_node_t *node, ast_node_t* top_node, 
				        ast_node_t *from, int from_num);
void recursive_truncate(ast_node_t *child_assign, ast_node_t *top_node, int nbits);
void recursive_truncate2(ast_node_t *bin_node, ast_node_t* top_node, 
				         ast_node_t* node, int num_bits);
void create_shift_node(ast_node_t *bin_node, ast_node_t *node, int num_bits);

void transformTree (ast_node_t *mult_node, int s1, int s2);
void changeNode (ast_node_t *mult_node);
int getRange(ast_node_t *node);
int findRange(char *id);
int getG();

void remove_equal_nodes(ast_node_t *top);
void add_equal_nodes(ast_node_t *top);
int get_range_var(ast_node_t *top, ast_node_t *node, 
				  ast_node_t *from, int from_num, 
				  int loop_cnt, char *rangetext);
void remove_node(ast_node_t *node, ast_node_t *from, int from_num);
void add_node(ast_node_t *node, ast_node_t *from, int from_num);

/* GENERAL PARSER NODES */
ast_node_t *newSymbolNode(char *id, int line_number);
ast_node_t *newNumberNode(char *num, int line_number);
ast_node_t *newList(ids type_id, ast_node_t *expression);
ast_node_t *newList_entry(ast_node_t *concat_node, ast_node_t *expression);
ast_node_t *newListReplicate(ast_node_t *exp, ast_node_t *child );
ast_node_t *markAndProcessSymbolListWith(ids id, ast_node_t *symbol_list);

/* EXPRESSIONS */
ast_node_t *newArrayRef(char *id, ast_node_t *expression, int line_number);
ast_node_t *newArrayArrayRef(char *id, ast_node_t *expression1, 
				             ast_node_t *expression2, int line_number);
ast_node_t *newRangeRef(char *id, ast_node_t *expression1, 
				        ast_node_t *expression2, int line_number);
ast_node_t *newArrayRangeRef(char *id, ast_node_t *expression1, 
				             ast_node_t *expression2, 
				             ast_node_t *expression3, int line_number);
ast_node_t *newMatrixRef(char *id, ast_node_t *expression1, 
				         ast_node_t *expression2, ast_node_t *expression3, 
				         ast_node_t *expression4, int line_number);
ast_node_t *newBinaryOperation(operation_list op_id, ast_node_t *expression1, 
				               ast_node_t *expression2, int line_number);
ast_node_t *newUnaryOperation(operation_list op_id, ast_node_t *expression, int line_number);

/* EVENT LIST AND DELAY CONTROL */
ast_node_t *newPosedgeSymbol(char *symbol, int line_number);
ast_node_t *newNegedgeSymbol(char *symbol, int line_number);

/* STATEMENTS */
ast_node_t *newCaseItem(ast_node_t *expression, ast_node_t *statement, int line_number);
ast_node_t *newDefaultCase(ast_node_t *statement, int line_number);
ast_node_t *newNonBlocking(char* sign, ast_node_t *expression1, 
				           ast_node_t *expression2, int line_number);
ast_node_t *newBlocking(ast_node_t *expression1, ast_node_t *expression2, int line_number);
ast_node_t *newFor(ast_node_t *init_statement, ast_node_t *compare_expression, 
				   ast_node_t *step_increment, ast_node_t *inside_statement, int line_number);
ast_node_t *newIf(ast_node_t *compare_expression, ast_node_t *true_expression, 
				  ast_node_t *false_expression, int line_number);
ast_node_t *newGenerate(ast_node_t *generate_statement, int line_number);
ast_node_t *newDefine(char *define_id, char *define_num, int line_number);
ast_node_t *newIfQuestion(ast_node_t *compare_expression, ast_node_t *true_expression, 
				          ast_node_t *false_expression, int line_number);
ast_node_t *newCase(ast_node_t *compare_expression, ast_node_t *case_list, int line_number);
ast_node_t *newAlways(ast_node_t *delay_control, ast_node_t *statements, int line_number);

/* MODULE INSTANCES FUNCTIONS */
ast_node_t *newModuleConnection(char* id, ast_node_t *expression, int line_number);
ast_node_t *newModuleNamedInstance(char* unique_name, ast_node_t *module_connect_list, 
				                   ast_node_t *module_parameter_list, int line_number);
ast_node_t *newModuleInstance(char* module_ref_name, ast_node_t *module_named_instance, int line_number);
ast_node_t *newModuleParameter(char* id, ast_node_t *expression, int line_number);

/* GATE INSTANCE */
ast_node_t *newGateInstance(char* gate_instance_name, ast_node_t *expression1, 
				            ast_node_t *expression2, ast_node_t *expression3, int line_number);
ast_node_t *newGate(operation_list gate_type, ast_node_t *gate_instance, int line_number);

/* MODULE ITEMS */
ast_node_t *newAssign(ast_node_t *statement, int line_number);
ast_node_t *newVarDeclare(char* symbol, ast_node_t *expression1, ast_node_t *expression2, 
						  ast_node_t *expression3, ast_node_t *expression4, ast_node_t *value, int line_number);
ast_node_t *newDoubleArray(char* symbol, ast_node_t *expression1, 
				           ast_node_t *expression2, ast_node_t *expression3, 
				           ast_node_t *expression4, ast_node_t *expression5, 
				           ast_node_t *expression6, ast_node_t *value, int line_number);

/* HIGH LEVEL ITEMS */
ast_node_t *newModule(char* module_name, ast_node_t *list_of_ports, 
				      ast_node_t *list_of_module_items, int line_number);
void next_module();
void newConstant(char *id, char *number, int line_number);
void newDefparam(char *inst, char *param, char *val, int line_number);
void next_parsed_verilog_file(ast_node_t *file_items_list);

/* VISUALIZATION */
void graphVizOutputAst(char* path, ast_node_t *top);
void graphVizOutputAst_traverse_node(FILE *fp, ast_node_t *node, ast_node_t *from, int from_num);

/*Back to Verilog from AST */ 
void VerilogOutputAst(char* path, ast_node_t *top);
void VerilogOutputAst_traverse_node(FILE *fp, ast_node_t *node, ast_node_t *from, int from_num);
void BlockingNonBlockingStatements(ast_node_t *newnode, char *assign_var, int count_op_depth);
void ConcatNodes(ast_node_t* node, char * concat_id);
void RecursiveBinaryop(ast_node_t *node, char *assign_var, char* root_bin_op, 
				       char* parent_bin_op, int cnt, int cnt0);
int TraverseTree(ast_node_t* node, int child_cntr);
void AddSameBinaryExpression(ast_node_t *node, char* statement_text);
void DeleteBinaryExpression(ast_node_t *node, char* statement_text);
void BinaryOpsGenerator(ast_node_t *node, char *bin_op);
void UnaryOpsGenerator(ast_node_t *node, char *unary_op);
void BeginBlock(ast_node_t *node, char* block, char* if_case, 
				char* bin_op, int cnt, int if_flag, 
				int begin_cnt, int block_indent, int block_cnt);
void IfElse(ast_node_t* block_node,int i, char* if_case, 
			int stat_cnt, int stat_cnt2, int cnt_block, 
			char *bin_op, int cnt, int block_cnt);
void ForLoop(ast_node_t* block_node, char* if_case, 
			 int bin_op_cnt, int stat_cnt, int stat_cnt2,  
			 int bin_op, int cnt_block, int cnt, int i);

/*-------Added by LYT-------*/
void AstOperation(ast_node_t *top);
void AstOperation_traverse(ast_node_t *node, int conditionid);
void AstOperation_swap();
void AstOperation_subtree(ast_node_t *node, int side, int conditionid);
void AstOperation_delete(ast_node_t *node);
void AstOperation_insert();
void AstOperation_factorize();
void NodeReplaceFactorization(ast_node_t *nodetochange_from, ast_node_t *nodetochange_to);
ast_node_t *AstOperation_copytree(ast_node_t *orgnode);
//int AstOperation_prob(int degree);
void AstOperation_Mutate(ast_node_t *node);
void AstOperation_unroll(ast_node_t *forparent, int forindex);
void AstOperation_unroll_replace(ast_node_t *node, char *mainvar, int value);
int AstOperation_find_paravalue(char *target_parameter, record_id *para_lib);
/*--------------------------*/

/* Added by CHH */
void write_AST_to_file(FILE* fp, ast_node_t* node, int from_node);
ast_node_t* read_AST_from_file(char* file_in);
void read_AST_connect_child(ast_node_t* top, ast_node_t* node, int from_node, int* found);
void Ast_param_substition(ast_node_t* top, record_id* para_lib);
int lookup_bitwidth(ast_node_t* top);
//void print_input_BW(ast_node_t *top, ast_node_t *node);
//char* ast_find_identifier(ast_node_t* node);

