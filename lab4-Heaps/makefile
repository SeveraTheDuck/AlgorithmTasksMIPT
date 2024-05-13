#------------------------------------------------------------------------------
# folders
SOURCE_DIR  	:= source/
INCLUDE_DIR     := include/
INPUT_DIR 		:= input/
OUTPUT_DIR  	:= output/
OBJECT_DIR 		:= object/
PLOTS_DIR 		:= plots/

# files
SOURCE 	   := $(shell find $(SOURCE_DIR) -name "*.c")
OBJECT     := $(addprefix $(OBJECT_DIR),$(patsubst %.c,%.o,$(notdir $(SOURCE))))
DEP    	   := $(patsubst %.o,%.o.d, $(OBJECT))

# executables
BINARY_LINEAR := binary_linear_test
BINARY_NLOGN  := binary_nlogn_test
BINOMIAL      := binomial_test

LINEAR_OBJECT   := $(OBJECT_DIR)/common.o $(OBJECT_DIR)/d_array.o $(OBJECT_DIR)/k_heap.o
BINOMIAL_OBJECT := $(OBJECT_DIR)/common.o $(OBJECT_DIR)/binomial_heap.o $(OBJECT_DIR)/$(BINOMIAL).o

# compilation
CC       := gcc
FLAGS    := -Wextra -Wall -Wfloat-equal -Wundef -Wshadow -Wpointer-arith -Wcast-align -Wstrict-prototypes -Wwrite-strings -Waggregate-return -Wunreachable-code
SANITIZE := -fsanitize=address -fsanitize=undefined -fno-sanitize-recover=all -fsanitize=float-divide-by-zero -fsanitize=float-cast-overflow -fno-sanitize=null -fno-sanitize=alignment
INCLUDE  := -I$(INCLUDE_DIR)
#------------------------------------------------------------------------------


#------------------------------------------------------------------------------
.PHONY: run_all_tests makeplots



#------------------------------------------------------------------------------
# compile main file
$(BINARY_LINEAR): $(OBJECT_DIR) $(OBJECT)
	@$(CC) $(FLAGS) $(SANITIZE) $(INCLUDE) $(LINEAR_OBJECT) $(OBJECT_DIR)/$(BINARY_LINEAR).o -o $@

$(BINARY_NLOGN): $(OBJECT_DIR) $(OBJECT)
	@$(CC) $(FLAGS) $(SANITIZE) $(INCLUDE) $(LINEAR_OBJECT) $(OBJECT_DIR)/$(BINARY_NLOGN).o -o $@

$(BINOMIAL): $(OBJECT_DIR) $(OBJECT)
	@$(CC) $(FLAGS) $(SANITIZE) $(INCLUDE) $(BINOMIAL_OBJECT) -o $@

# include dependencies
-include $(DEP)

# make object files
$(OBJECT_DIR)%.o: $(SOURCE_DIR)%.c
	@$(CC) $(FLAGS) $(SANITIZE) $(INCLUDE) -MMD -MF $@.d -c -o $@ $<

# make object directory
$(OBJECT_DIR):
	@mkdir -p $(OBJECT_DIR)
	@mkdir -p $(OUTPUT_DIR)
	@mkdir -p $(PLOTS_DIR)
#------------------------------------------------------------------------------



#------------------------------------------------------------------------------
MAKE_TEST_SOURCE    := tests/generate_array.c
SCRIPT 			    := tests/script

FROM := 100000
TO   := 1000000
STEP := 100000

INT_ARRAY := $(FROM) $(TO) $(STEP)

maketests:
	@$(CC) $(MAKE_TEST_SOURCE) -o gen_array
	@$(SCRIPT) $(INT_ARRAY)
	@rm gen_array
#------------------------------------------------------------------------------



#------------------------------------------------------------------------------
# Run tests
BINARY_LINEAR_OUTPUT 	:= $(OUTPUT_DIR)$(BINARY_LINEAR).txt
BINARY_NLOGN_OUTPUT 	:= $(OUTPUT_DIR)$(BINARY_NLOGN).txt
BINOMIAL_OUTPUT 		:= $(OUTPUT_DIR)$(BINOMIAL).txt

$(OUTPUT_DIR):
	@mkdir -p $(OUTPUT_DIR)

run_binary_linear: $(BINARY_LINEAR) $(OUTPUT_DIR)
	@touch $(BINARY_LINEAR_OUTPUT)
	@> $(BINARY_LINEAR_OUTPUT)
	@number=$(FROM) ; while [[ $$number -le $(TO) ]] ; do \
        ./$(BINARY_LINEAR) < "$(INPUT_DIR)$$number.in" >> $(BINARY_LINEAR_OUTPUT); \
        ((number = number + $(STEP))) ; \
    done
	@rm $(BINARY_LINEAR)

run_binary_nlogn: $(BINARY_NLOGN) $(OUTPUT_DIR)
	@touch $(BINARY_NLOGN_OUTPUT)
	@> $(BINARY_NLOGN_OUTPUT)
	@number=$(FROM) ; while [[ $$number -le $(TO) ]] ; do \
        ./$(BINARY_NLOGN) < "$(INPUT_DIR)$$number.in" >> $(BINARY_NLOGN_OUTPUT) ; \
        ((number = number + $(STEP))) ; \
    done
	@rm $(BINARY_NLOGN)

run_binomial: $(BINOMIAL) $(OUTPUT_DIR)
	@touch $(BINOMIAL_OUTPUT)
	@> $(BINOMIAL_OUTPUT)
	@number=$(FROM) ; while [[ $$number -le $(TO) ]] ; do \
        ./$(BINOMIAL) < "$(INPUT_DIR)$$number.in" >> $(BINOMIAL_OUTPUT) ; \
        ((number = number + $(STEP))) ; \
    done
	@rm $(BINOMIAL)

run_all_tests:
	@make run_binary_linear
	@make run_binary_nlogn
	@make run_binomial
#------------------------------------------------------------------------------



#------------------------------------------------------------------------------
PY = python
MAKE_PLOT_SOURCE = $(SOURCE_DIR)make_plot.py

makeplots:
	@$(PY) $(MAKE_PLOT_SOURCE) $(PLOTS_DIR) k_heap 2 $(BINARY_LINEAR_OUTPUT) $(BINARY_NLOGN_OUTPUT)
	@$(PY) $(MAKE_PLOT_SOURCE) $(PLOTS_DIR) binomial_heap 2 $(BINARY_LINEAR_OUTPUT) $(BINOMIAL_OUTPUT)
#------------------------------------------------------------------------------
