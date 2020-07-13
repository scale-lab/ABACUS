
BUILD = VPR6

PARSER1 = flex 
PARSER2 = bison

WARN_FLAGS = -Wall

SRC_DIR = SRC
OBJ_DIR = OBJ

EXEO = ABACUS

DEBUG_FLAGS = -g

OPT_FLAGS = -O2 
# Use this value of OPT_FLAGS to simulate in parallel (requires GCC >= 4.2)
#OPT_FLAGS = -O2 -fopenmp 

ifeq "$(BUILD)" "VPR5"
	INC_FLAGS =  -I../libvpr_5/include
	LIB = -L../libvpr_5 -lvpr_5 -lm -ldl
	C_DIRECTIVE = -DVPR5 -DUSING_BISON
else
	INC_FLAGS =  -I../libvpr_6/include 
	LIB = -L../libvpr_6 -lvpr_6 -lm -ldl
	C_DIRECTIVE = -DVPR6 -DUSING_BISON
endif

FLAGS = $(DEBUG_FLAGS) $(WARN_FLAGS) $(OPT_FLAGS) $(INC_FLAGS)
C_FLAGS = $(C_DIRECTIVE$)
# modified by sherief
#CC = gcc -m32
CC = gcc

YFILES = $(SRC_DIR)/verilog_bison.y
LFILES = $(SRC_DIR)/verilog_flex.l

SRC_PARSERS=$(YFILES:.y=.c) $(LFILES:.l=.c)

SRC = $(addprefix $(SRC_DIR)/,$(strip \
	  errors.c \
      abacus_util.c \
      string_cache.c \
      hard_blocks.c \
))

SRCO = $(addprefix $(SRC_DIR)/,$(strip \
	read_xml_config_file.c \
	parse_making_ast.c \
	ast_optimizations.c \
	ast_util.c \
	verilog_preprocessor.c \
	abacus.c \
))

SRCV = SRC/read_blif.c \
SRC/verify_blif.c 

OBJ = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o, \
       $(SRC_PARSERS) $(SRC)  $(SRCO))


DEP = $(OBJ:.o=.d)

all: CTAGS $(EXEO)

$(EXEO): $(OBJ)
	$(CC) $(C_FLAGS) $(FLAGS) $(OBJ) -o $(EXEO) $(LIB)

CTAGS :
	@ cd SRC && ctags *.[ch]

# Make sure the parsers are generated before compiling
# any c files
$(OBJ_DIR)/%.o:$(SRC_DIR)/%.c | $(OBJ_DIR) $(SRC_PARSERS)
	$(CC) $(C_FLAGS) $(FLAGS) -MD -MP -c $< -o $@ 

SRC/verilog_flex.c: %.c: %.l
	$(PARSER1) -o$@ $<

SRC/verilog_bison.c: %.c: %.y
	$(PARSER2) --verbose -d -t -o $@ $<

# Silently create the object directory as needed
$(OBJ_DIR):
	@ mkdir -p $@
	
clean :
	rm -f $(SRC_PARSERS)	
	rm -rf $(OBJ_DIR)
	rm -f SRC/verilog_bison.h
	rm -f SRC/verilog_bison.output
	rm -f SRC/tags
	rm -f *.exe
	rm -f SRC/*~
	

regression_clean :
	rm -f REGRESSION_TESTS/BASIC_TESTS/*.*

-include $(DEP)
