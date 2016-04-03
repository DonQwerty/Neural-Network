###########################################
#				MAKEFILE				  #
#										  #
#  AUTHORS: Ares Aguilar Sotos			  #
#			Daniel Gimenez Llorente		  #
###########################################

# ARGUMENTS
INPUT_FILE	?= no_input
OUTPUT_FILE ?= out/out.txt

# INPUT FILES
P1_FILE		 = data/databases/problema_real1.txt
P2_FILE		 = data/databases/problema_real2.txt
P2_NE_FILE	 = data/databases/problema_real2_no_etiquetados.txt
P3_FILE		 = data/databases/problema-real-3clases.txt
P4_FILE		 = data/databases/problema-real4.txt
NAND_FILE	 = data/databases/nand.txt
NOR_FILE	 = data/databases/nor.txt
XOR_FILE	 = data/databases/xor.txt
PERCEPTRON_FILE = data/models/perceptron.txt
ADALINE_FILE	= data/models/adaline.txt
MULTILAYER_FILE = data/models/multilayer.txt

# DIRECTORIES
IDIR		= include
SDIR		= src
ODIR		= obj
BDIR		= bin

# FILES
TARGET		= neural-network
SOURCES		:= $(wildcard $(SDIR)/*.c)
INCLUDES	:= $(wildcard $(IDIR)/*.h)
OBJECTS		:= $(SOURCES:$(SDIR)/%.c=$(ODIR)/%.o)
CHK_SOURCES := $(SOURCES) $(INCLUDES)

# VARIABLES
# Compiler
CC			= gcc -c
CFLAGS		= -Wall -I$(IDIR)
# Linker
LINKER		= gcc -o
LFLAGS		= -Wall -I$(IDIR) -lm
# Others
rm			= rm -f
ln			= ln -sf

# Compiles all programs in project
compile: $(BDIR)/$(TARGET)
	@$(ln) $(BDIR)/$(TARGET) $(TARGET)
	@echo "INFO: All targets compiled"

$(BDIR)/$(TARGET): $(OBJECTS)
	@$(LINKER) $@ $(OBJECTS) $(LFLAGS)
	@echo "INFO: Linking completed"

$(OBJECTS):$(ODIR)/%.o: $(SDIR)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "INFO: Compiled "$<" successfully"

# P1
# P1.2 McCulloch-Pitts Network
# p1.2-mcculloch-pits: compile
# ifeq ($(INPUT_FILE),no_input)
# 	./$(TARGET) --preset mcculloch --output-file $(OUTPUT_FILE)
# else
# 	./$(TARGET) --preset mcculloch --input-file $(INPUT_FILE) --output-file $(OUTPUT_FILE)
# endif

# # P1.3.1 Perceptron
# p1.3.1-perceptron: compile
# ifeq ($(INPUT_FILE),no_input)
# 	./$(TARGET) --mode perceptron --input-file $(P1_FILE) --output-file $(OUTPUT_FILE)
# else
# 	./$(TARGET) --mode perceptron --input-file $(INPUT_FILE) --output-file $(OUTPUT_FILE)
# endif

# # P1.3.1 Adelaide
# p1.3.1-adaline: compile
# ifeq ($(INPUT_FILE),no_input)
# 	./$(TARGET) --mode adaline --input-file $(P1_FILE) --output-file $(OUTPUT_FILE)
# else
# 	./$(TARGET) --mode adaline --input-file $(INPUT_FILE) --output-file $(OUTPUT_FILE)
# endif

# # P1.3.2 NAND
# p1.3.2-nand: compile
# ifeq ($(INPUT_FILE),no_input)
# 	./$(TARGET) --mode perceptron --input-file $(NAND_FILE) --output-file $(OUTPUT_FILE) -t 100
# else
# 	./$(TARGET) --mode perceptron --input-file $(INPUT_FILE) --output-file $(OUTPUT_FILE) -t 100
# endif

# # P1.3.2 NOR
# p1.3.2-nor: compile
# ifeq ($(INPUT_FILE),no_input)
# 	./$(TARGET) --mode perceptron --input-file $(NOR_FILE) --output-file $(OUTPUT_FILE) -t 100
# else
# 	./$(TARGET) --mode perceptron --input-file $(INPUT_FILE) --output-file $(OUTPUT_FILE) -t 100
# endif

# # P1.3.2 XOR
# p1.3.2-xor: compile
# ifeq ($(INPUT_FILE),no_input)
# 	./$(TARGET) --mode perceptron --input-file $(XOR_FILE) --output-file $(OUTPUT_FILE) -t 100
# else
# 	./$(TARGET) --mode perceptron --input-file $(INPUT_FILE) --output-file $(OUTPUT_FILE) -t 100
# endif

# # P1.3.3 Perceptron
# p1.3.3-perceptron: compile
# ifeq ($(INPUT_FILE),no_input)
# 	./$(TARGET) --mode perceptron --input-file $(P2_FILE) --output-file $(OUTPUT_FILE)
# else
# 	./$(TARGET) --mode perceptron --input-file $(INPUT_FILE) --output-file $(OUTPUT_FILE)
# endif

# # P1.3.3 Adelaide
# p1.3.3-adaline: compile
# ifeq ($(INPUT_FILE),no_input)
# 	./$(TARGET) --mode adaline --input-file $(P2_FILE) --output-file $(OUTPUT_FILE)
# else
# 	./$(TARGET) --mode adaline --input-file $(INPUT_FILE) --output-file $(OUTPUT_FILE)
# endif

# # P1.3.3.1 Predicciones Preceptron
# p1.3.3.1-perceptron-predict: compile
# ifeq ($(INPUT_FILE),no_input)
# 	./$(TARGET) --mode perceptron --input-file $(P2_FILE) -s -n $(PERCEPTRON_FILE)
# 	./$(TARGET) --mode perceptron --input-file $(P2_NE_FILE) -n $(PERCEPTRON_FILE) -o $(OUTPUT_FILE) -f
# else
# 	./$(TARGET) --mode perceptron --input-file $(P2_FILE) -s -n $(PERCEPTRON_FILE)
# 	./$(TARGET) --mode perceptron --input-file $(INPUT_FILE) -n $(PERCEPTRON_FILE) -o $(OUTPUT_FILE) -f
# endif

# # P1.3.3.1 Predicciones Adaline
# p1.3.3.1-adaline-predict: compile
# ifeq ($(INPUT_FILE),no_input)
# 	./$(TARGET) --mode adaline --input-file $(P2_FILE) -s -n $(ADALINE_FILE)
# 	./$(TARGET) --mode adaline --input-file $(P2_NE_FILE) -n $(ADALINE_FILE) -o $(OUTPUT_FILE) -f
# else
# 	./$(TARGET) --mode adaline --input-file $(P2_FILE) -s -n $(ADALINE_FILE)
# 	./$(TARGET) --mode adaline --input-file $(INPUT_FILE) -n $(ADALINE_FILE) -o $(OUTPUT_FILE) -f
# endif


# P2
p2: p2.2.1-multilayer4 p2.2.1-multilayer9 p2.2.2-xor p2.2.3-problema2 p2.3-problema3 p2.4-problema4 p2.5-normalizado p2.6-predicciones

p2.2.1-multilayer4: compile
ifeq ($(INPUT_FILE),no_input)
	./$(TARGET) --mode multilayer --input-file $(P2_FILE) -l 0.1 -h 4
else
	./$(TARGET) --mode multilayer --input-file $(INPUT_FILE) -l 0.1 -h 4
endif

p2.2.1-multilayer9: compile
ifeq ($(INPUT_FILE),no_input)
	./$(TARGET) --mode multilayer --input-file $(P2_FILE) -l 0.1 -h 9
else
	./$(TARGET) --mode multilayer --input-file $(INPUT_FILE) -l 0.1 -h 9
endif

p2.2.2-xor: compile
ifeq ($(INPUT_FILE),no_input)
	./$(TARGET) --mode multilayer --input-file $(XOR_FILE) -l 0.02 -h 9 -t 100 -e 6000
else
	./$(TARGET) --mode multilayer --input-file $(INPUT_FILE)  -l 0.02 -h 9 -t 100 -e 6000
endif

p2.2.3-problema2: compile
ifeq ($(INPUT_FILE),no_input)
	./$(TARGET) --mode multilayer --input-file $(P2_FILE) -l 0.02 -h 4
else
	./$(TARGET) --mode multilayer --input-file $(INPUT_FILE)  -l 0.02 -h 4
endif

p2.3-problema3: compile
ifeq ($(INPUT_FILE),no_input)
	./$(TARGET) --mode multilayer --input-file $(P3_FILE) -l 0.02 -h 9
else
	./$(TARGET) --mode multilayer --input-file $(INPUT_FILE)  -l 0.02 -h 9
endif

p2.4-problema4: compile
ifeq ($(INPUT_FILE),no_input)
	./$(TARGET) --mode multilayer --input-file $(P4_FILE) -l 0.02 -h 4
else
	./$(TARGET) --mode multilayer --input-file $(INPUT_FILE)  -l 0.02 -h 4
endif

p2.5-normalizado: compile
ifeq ($(INPUT_FILE),no_input)
	./$(TARGET) --mode multilayer --input-file $(P4_FILE) -l 0.02 -h 9 -z
else
	./$(TARGET) --mode multilayer --input-file $(INPUT_FILE)  -l 0.02 -h 9 -z
endif

p2.6-predicciones: compile
ifeq ($(INPUT_FILE),no_input)
	./$(TARGET) --mode multilayer --input-file $(P2_FILE) -h 8 -l 0.1 -t 70 -s -n $(MULTILAYER_FILE)
	./$(TARGET) --mode multilayer --input-file $(P2_NE_FILE) -n $(MULTILAYER_FILE) -o $(OUTPUT_FILE) -f
else
	./$(TARGET) --mode multilayer --input-file $(P2_FILE) -h 8 -l 0.1 -t 70 -s -n $(MULTILAYER_FILE)
	./$(TARGET) --mode multilayer --input-file $(INPUT_FILE) -n $(MULTILAYER_FILE) -o $(OUTPUT_FILE) -f
endif


# Flycheck (emacs) requirement
.PHONEY: check-syntax
check-syntax:
	@$(LINKER) nul $(LFLAGS) -S ${CHK_SOURCES}

# Displays the help for this makefile
help:
	@echo "NEURAL NETWORK MAKEFILE"
	@echo "Targets:"
	@echo "	  - compile"
	@echo "			Compiles all sources and generates the executable."
	# @echo "	  - p1.2-mcculloch-pitts"
	# @echo "			Executes the McCulloch-Pitts network."
	# @echo "	  - p1.3.1-perceptron"
	# @echo "			Executes the perceptron classifier for dataset problema_real1.txt."
	# @echo "	  - p1.3.1-adaline"
	# @echo "			Executes the adaline classifier for dataset problema_real1.txt."
	# @echo "	  - p1.3.2-nand"
	# @echo "			Executes the simulation for nand.txt."
	# @echo "	  - p1.3.2-nor"
	# @echo "			Executes the simulation for nor.txt."
	# @echo "	  - p1.3.2-xor"
	# @echo "			Executes the simulation for xor.txt."
	# @echo "	  - p1.3.3-perceptron"
	# @echo "			Executes the perceptron classifier for dataset problema_real2.txt."
	# @echo "	  - p1.3.3-adaline"
	# @echo "			Executes the adaline classifier for dataset problema_real2.txt."
	# @echo "	  - p1.3.3.1-perceptron-predict"
	# @echo "			Executes the perceptron classifier for dataset problema_real2.txt and predicts problema_real2_no_etiquetados.txt."
	# @echo "	  - p1.3.3.1-adaline-predict"
	# @echo "			Executes the adaline classifier for dataset problema_real2.txt and predicts problema_real2_no_etiquetados.txt."
	@echo "   -p2"
	@echo "         Executes all simulations for P2."
	@echo "	  - clean"
	@echo "			Removes all objects."
	@echo "	  - help"
	@echo "			Shows this help."
	@echo "Arguments:"
	@echo "	  - INPUT_FILE"
	@echo "			File to read from"
	@echo "	  - OUTPUT_FILE"
	@echo "			File to write to. out/out.txt by default"

.PHONEY: clean
clean:
	@$(rm) $(OBJECTS)
	@echo "INFO: Cleaning completed"
