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
AVL_TEST   := avl_test
TREAP_TEST := treap_test
SPLAY_TEST := splay_test
COMMON     := tests_common.o

AVL_OBJECT 	 := $(OBJECT_DIR)/$(AVL_TEST).o   $(OBJECT_DIR)/avl_tree.o
TREAP_OBJECT := $(OBJECT_DIR)/$(TREAP_TEST).o $(OBJECT_DIR)/treap.o
SPLAY_OBJECT := $(OBJECT_DIR)/$(SPLAY_TEST).o $(OBJECT_DIR)/splay.o

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
$(AVL_TEST): $(OBJECT_DIR) $(OBJECT)
	@$(CC) $(FLAGS) $(SANITIZE) $(INCLUDE) $(AVL_OBJECT)   $(OBJECT_DIR)/$(COMMON) -o $@

$(TREAP_TEST): $(OBJECT_DIR) $(OBJECT)
	@$(CC) $(FLAGS) $(SANITIZE) $(INCLUDE) $(TREAP_OBJECT) $(OBJECT_DIR)/$(COMMON) -o $@

$(SPLAY_TEST): $(OBJECT_DIR) $(OBJECT)
	@$(CC) $(FLAGS) $(SANITIZE) $(INCLUDE) $(SPLAY_OBJECT) $(OBJECT_DIR)/$(COMMON) -o $@

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
MAKE_TEST_SOURCE := tests_gen/array_generator.c
SCRIPT 			 := tests_gen/script

FROM   := 100000
TO     := 1000000
STEP   := 100000
REPEAT := 5

INT_ARRAY := $(FROM) $(TO) $(STEP) $(REPEAT)

maketests:
	@$(CC) $(MAKE_TEST_SOURCE) -o gen_array
	@$(SCRIPT) $(INT_ARRAY)
	@rm gen_array
#------------------------------------------------------------------------------



#------------------------------------------------------------------------------
# Run tests
AVL_INSERT_OUTPUT 	:= $(OUTPUT_DIR)$(AVL_TEST)_insert.out
AVL_DELETE_OUTPUT 	:= $(OUTPUT_DIR)$(AVL_TEST)_delete.out
TREAP_INSERT_OUTPUT := $(OUTPUT_DIR)$(TREAP_TEST)_insert.out
TREAP_DELETE_OUTPUT := $(OUTPUT_DIR)$(TREAP_TEST)_delete.out
SPLAY_INSERT_OUTPUT := $(OUTPUT_DIR)$(SPLAY_TEST)_insert.out
SPLAY_DELETE_OUTPUT := $(OUTPUT_DIR)$(SPLAY_TEST)_delete.out

$(OUTPUT_DIR):
	@mkdir -p $(OUTPUT_DIR)

run_avl: $(AVL_TEST) $(OUTPUT_DIR)
	@touch $(AVL_INSERT_OUTPUT)
	@touch $(AVL_DELETE_OUTPUT)
	@> $(AVL_INSERT_OUTPUT)
	@> $(AVL_DELETE_OUTPUT)
	@size=$(FROM) ; while [[ $$size -le $(TO) ]] ; do \
		repeat=1 ; while [[ $$repeat -le $(REPEAT) ]] ; do \
			./$(AVL_TEST) "$(INPUT_DIR)$$size""_""$$repeat.in" $(AVL_INSERT_OUTPUT) $(AVL_DELETE_OUTPUT); \
			((repeat = repeat + 1))   ; \
		done ;\
		((size = size + $(STEP))) ; \
    done
	@rm $(AVL_TEST)

run_treap: $(TREAP_TEST) $(OUTPUT_DIR)
	@touch $(TREAP_INSERT_OUTPUT)
	@touch $(TREAP_DELETE_OUTPUT)
	@> $(TREAP_INSERT_OUTPUT)
	@> $(TREAP_DELETE_OUTPUT)
	@size=$(FROM) ; while [[ $$size -le $(TO) ]] ; do \
		repeat=1 ; while [[ $$repeat -le $(REPEAT) ]] ; do \
			./$(TREAP_TEST) "$(INPUT_DIR)$$size""_""$$repeat.in" $(TREAP_INSERT_OUTPUT) $(TREAP_DELETE_OUTPUT); \
			((repeat = repeat + 1))   ; \
		done ;\
		((size = size + $(STEP))) ; \
    done
	@rm $(TREAP_TEST)

run_splay: $(SPLAY_TEST) $(OUTPUT_DIR)
	@touch $(SPLAY_INSERT_OUTPUT)
	@touch $(SPLAY_DELETE_OUTPUT)
	@> $(SPLAY_INSERT_OUTPUT)
	@> $(SPLAY_DELETE_OUTPUT)
	@size=$(FROM) ; while [[ $$size -le $(TO) ]] ; do \
		repeat=1 ; while [[ $$repeat -le $(REPEAT) ]] ; do \
			./$(SPLAY_TEST) "$(INPUT_DIR)$$size""_""$$repeat.in" $(SPLAY_INSERT_OUTPUT) $(SPLAY_DELETE_OUTPUT); \
			((repeat = repeat + 1))   ; \
		done ;\
		((size = size + $(STEP))) ; \
    done
	@rm $(SPLAY_TEST)

run_all_tests:
	@make run_avl
	@make run_treap
	@make run_splay
#------------------------------------------------------------------------------



#------------------------------------------------------------------------------
PY = python
MAKE_PLOT_SOURCE = $(SOURCE_DIR)make_plot.py

makeplots:
	@$(PY) $(MAKE_PLOT_SOURCE) $(PLOTS_DIR) inserts 3 $(REPEAT) $(AVL_INSERT_OUTPUT) $(TREAP_INSERT_OUTPUT) $(SPLAY_INSERT_OUTPUT)
	@$(PY) $(MAKE_PLOT_SOURCE) $(PLOTS_DIR) deletes 3 $(REPEAT) $(AVL_DELETE_OUTPUT) $(TREAP_DELETE_OUTPUT) $(SPLAY_DELETE_OUTPUT)
#------------------------------------------------------------------------------
