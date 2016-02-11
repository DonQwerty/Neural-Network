
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
SOURCES		:= $(wildcard $(SDIR)/*.c)
INCLUDES 	:= $(wildcard $(IDIR)/*.h)
OBJECTS  	:= $(SOURCES:$(SDIR)/%.c=$(ODIR)/%.o)

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
compile:
	@echo "TODO: compile"

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
check-syntax:
	gcc -o nul -S ${CHK_SOURCES}

# Displays the help for this makefile
help:
	@echo "TODO: help"

.PHONEY: clean
clean:
	@$(rm) $(OBJECTS)
