
    #######################################
    #     	    MAKEFILE				  #
    # 									  #
    # AUTHORS: Ares Aguilar Sotos		  #
    # 	       Daniel Gimenez Llorente	  #
    #######################################

# ARGUMENTS
INPUT_FILE ?= no_input

#NEURAL FILES
PERCEPTRON_FILE = data/models/prueba1.txt
ADELAIDE_FILE    = data/models/prueba2.txt

# DIRECTORIES
IDIR		= include
SDIR		= src
ODIR		= obj
BDIR		= bin

# FILES
TARGET		= neural-network
SOURCES		:= $(wildcard $(SDIR)/*.c)
INCLUDES 	:= $(wildcard $(IDIR)/*.h)
OBJECTS  	:= $(SOURCES:$(SDIR)/%.c=$(ODIR)/%.o)
CHK_SOURCES	:= $(SOURCES) $(INCLUDES)

# VARIABLES
# Compiler
CC			= gcc -c
CFLAGS		= -Wall -I$(IDIR)
# Linker
LINKER		= gcc -o
LFLAGS		= -Wall -I$(IDIR) -lm
# Others
rm			= rm -f
ln          = ln -sf

# Compiles all programs in project
compile: $(BDIR)/$(TARGET)
	@$(ln) $(BDIR)/$(TARGET) $(TARGET)
	@echo "INFO: All targets compiled"

$(BDIR)/$(TARGET): $(OBJECTS)
	@$(LINKER) $@ $(OBJECTS) $(LFLAGS)
	@echo "INFO: Linking completed"

$(OBJECTS):$(ODIR)/%.o: $(SDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@
	@echo "INFO: Compiled "$<" successfully"

# P1.2 MacCulloch-Pitts Network
p1.2-macculloch-pits:
ifeq ($(INPUT_FILE),no_input)
	./$(TARGET) --preset macculloch
else
	./$(TARGET) --preset macculloch --input-file $(INPUT_FILE)
endif

# P1.3.1 Perceptron
p1.3.1-perceptron:
ifeq ($(INPUT_FILE),no_input)
	@echo "ERROR: INPUT_FILE was not set."
else
	./$(TARGET) --mode perceptron --neural-network $(PERCEPTRON_FILE) --input-file $(INPUT_FILE)
endif

# P1.3.2 Adelaide
p1.3.2-adelaide:
ifeq ($(INPUT_FILE),no_input)
	@echo "ERROR: INPUT_FILE was not set."
else
	./$(TARGET) --mode adelaide --neural-network $(ADELAIDE_FILE) --input-file $(INPUT_FILE)
endif

# Flycheck (emacs) requirement
.PHONEY: check-syntax
check-syntax:
	@$(LINKER) nul $(LFLAGS) -S ${CHK_SOURCES}

# Displays the help for this makefile
help:
	@echo "             Neural Network Makefile"
	@echo "Targets:"
	@echo "   - compile"
	@echo "         Compiles all sources and generates the executable."
	@echo "   - p1.2-macculloch-pitts"
	@echo "         Executes the macculloch-pitts network."
	@echo "   - p1.3.1-perceptron"
	@echo "         Executes the perceptron classifier."
	@echo "   - p1.3.2-adelaide"
	@echo "         Executes the adelaide classifier."
	@echo "   - clean"
	@echo "         Removes all objects."
	@echo "Arguments:"
	@echo "   - INPUT_FILE"
	@echo "         File to read from. Must be set for p1.3.1 and p1.3.2"

.PHONEY: clean
clean:
	@$(rm) $(OBJECTS)
	@echo "INFO: Cleaning completed"
