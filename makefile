#------------------------------------------------------------------------------
# folders
SOURCE_DIR  	:= source/
INCLUDE_DIR     := include/
LIB_SOURCE_DIR  := lib/source/
LIB_INCLUDE_DIR := lib/include/
OUTPUT_DIR  	:= output/
OBJECT_DIR 		:= object/

# files
SOURCE 	   := $(shell find $(SOURCE_DIR)     -name "*.c")
LIB_SOURCE := $(shell find $(LIB_SOURCE_DIR) -name "*.c")
OBJECT     := $(addprefix $(OBJECT_DIR),$(patsubst %.c,%.o,$(notdir $(SOURCE))))
LIB_OBJECT := $(addprefix $(OBJECT_DIR),$(patsubst %.c,%.o,$(notdir $(LIB_SOURCE))))
DEP    	   := $(patsubst %.o,%.o.d, $(OBJECT))

# executables
RUN_TESTS := run_tests

# compilation
CC       := gcc
FLAGS    := -Wextra -Wall -Wfloat-equal -Wundef -Wshadow -Wpointer-arith -Wcast-align -Wstrict-prototypes -Wwrite-strings -Waggregate-return -Wunreachable-code
SANITIZE := -fsanitize=address -fsanitize=undefined -fno-sanitize-recover=all -fsanitize=float-divide-by-zero -fsanitize=float-cast-overflow -fno-sanitize=null -fno-sanitize=alignment
INCLUDE  := -I$(INCLUDE_DIR) -I$(LIB_INCLUDE_DIR)
#------------------------------------------------------------------------------



#------------------------------------------------------------------------------
# compile main file
$(RUN_TESTS): $(OBJECT_DIR) $(OBJECT) $(LIB_OBJECT) 
	@$(CC) $(FLAGS) $(SANITIZE) $(INCLUDE) $(OBJECT) $(LIB_OBJECT) -o $@

# include dependencies
-include $(DEP)

# make object files
$(OBJECT_DIR)%.o: $(SOURCE_DIR)%.c
	@$(CC) $(FLAGS) $(SANITIZE) $(INCLUDE) -MMD -MF $@.d -c -o $@ $<
	
# make lib object files
$(OBJECT_DIR)%.o: $(LIB_SOURCE_DIR)%.c
	@$(CC) $(FLAGS) $(SANITIZE) $(INCLUDE) -MMD -MF $@.d -c -o $@ $<

# make object directory
$(OBJECT_DIR):
	@mkdir -p $(OBJECT_DIR)
#------------------------------------------------------------------------------



#------------------------------------------------------------------------------
MAKE_TEST_SOURCE    := generate_tests/source/array_generate.c
MAKE_ANSWER_SOURCE  := generate_tests/source/standard_qsort.c
SCRIPT 			    := generate_tests/script

VERY_SMALL_TESTS    	:= very_small_tests 1 150 1 5 2100000000
SMALL_TESTS 			:= small_tests 0 10000 100 5 2100000000
BIG_TESTS 				:= big_tests 0 1000000 10000 1 2100000000
TEST_MOST_DUBLICATES	:= test_most_dublicates 0 1000000 10000 1 10000

maketests:
	@$(CC) $(MAKE_TEST_SOURCE)   -o gen_test
	@$(CC) $(MAKE_ANSWER_SOURCE) -o gen_answer
	@$(SCRIPT) $(VERY_SMALL_TESTS)
	@$(SCRIPT) $(SMALL_TESTS)
	@$(SCRIPT) $(BIG_TESTS)
	@$(SCRIPT) $(TEST_MOST_DUBLICATES)
	@rm -rf gen_test
	@rm -rf gen_answer
#------------------------------------------------------------------------------
