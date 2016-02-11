
    #######################################
    #     	    MAKEFILE				  #
    # 									  #
    # AUTHORS: Ares Aguilar Sotos		  #
    # 	       Daniel Gimenez Llorente	  #
    #######################################

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
LFLAGS		= -Wall -I$(IDIR)
# Others
rm			= rm -f

# Compiles all programs in project
compile: $(BDIR)/$(TARGET)
	@echo "INFO: All targets compiled"

$(BDIR)/$(TARGET): $(OBJECTS)
	@$(LINKER) $@ $(LFLAGS) $(OBJECTS)
	@echo "INFO: Linking completed"

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "INFO: Compiled "$<" successfully"

# P1.2 MacCulloch-Pitts Network
p1.2-macculloch-pits:
	@echo "TODO: p1.2-macculloch-pits"

# P1.3.1 Perceptron
p1.3.1-perceptron:
	@echo "TODO: p1.3.1-perceptron"

# P1.3.2 Adelaide
p1.3.2-adelaide:
	@echo "TODO: p1.3.2-adelaide"

# Flycheck (emacs) requirement
.PHONEY: check-syntax
check-syntax:
	@$(LINKER) nul $(LFLAGS) -S ${CHK_SOURCES}

# Displays the help for this makefile
help:
	@echo "TODO: help"

.PHONEY: clean
clean:
	@$(rm) $(OBJECTS)
	@echo "INFO: Cleaning completed"
