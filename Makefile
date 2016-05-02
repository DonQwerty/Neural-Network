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
# p2: p2.2.1-multilayer4 p2.2.1-multilayer9 p2.2.2-xor p2.2.3-problema2 p2.3-problema3 p2.4-problema4 p2.5-normalizado p2.6-predicciones

# p2.2.1-multilayer4: compile
# ifeq ($(INPUT_FILE),no_input)
# 	./$(TARGET) --mode multilayer --input-file $(P2_FILE) -l 0.1 -h 4
# else
# 	./$(TARGET) --mode multilayer --input-file $(INPUT_FILE) -l 0.1 -h 4
# endif

# p2.2.1-multilayer9: compile
# ifeq ($(INPUT_FILE),no_input)
# 	./$(TARGET) --mode multilayer --input-file $(P2_FILE) -l 0.1 -h 9
# else
# 	./$(TARGET) --mode multilayer --input-file $(INPUT_FILE) -l 0.1 -h 9
# endif

# p2.2.2-xor: compile
# ifeq ($(INPUT_FILE),no_input)
# 	./$(TARGET) --mode multilayer --input-file $(XOR_FILE) -l 0.02 -h 9 -t 100 -e 6000
# else
# 	./$(TARGET) --mode multilayer --input-file $(INPUT_FILE)  -l 0.02 -h 9 -t 100 -e 6000
# endif

# p2.2.3-problema2: compile
# ifeq ($(INPUT_FILE),no_input)
# 	./$(TARGET) --mode multilayer --input-file $(P2_FILE) -l 0.02 -h 4
# else
# 	./$(TARGET) --mode multilayer --input-file $(INPUT_FILE)  -l 0.02 -h 4
# endif

# p2.3-problema3: compile
# ifeq ($(INPUT_FILE),no_input)
# 	./$(TARGET) --mode multilayer --input-file $(P3_FILE) -l 0.02 -h 9
# else
# 	./$(TARGET) --mode multilayer --input-file $(INPUT_FILE)  -l 0.02 -h 9
# endif

# p2.4-problema4: compile
# ifeq ($(INPUT_FILE),no_input)
# 	./$(TARGET) --mode multilayer --input-file $(P4_FILE) -l 0.02 -h 4
# else
# 	./$(TARGET) --mode multilayer --input-file $(INPUT_FILE)  -l 0.02 -h 4
# endif

# p2.5-normalizado: compile
# ifeq ($(INPUT_FILE),no_input)
# 	./$(TARGET) --mode multilayer --input-file $(P4_FILE) -l 0.02 -h 9 -z
# else
# 	./$(TARGET) --mode multilayer --input-file $(INPUT_FILE)  -l 0.02 -h 9 -z
# endif

# p2.6-predicciones: compile
# ifeq ($(INPUT_FILE),no_input)
# 	./$(TARGET) --mode multilayer --input-file $(P2_FILE) -h 8 -l 0.1 -t 70 -s -n $(MULTILAYER_FILE)
# 	./$(TARGET) --mode multilayer --input-file $(P2_NE_FILE) -n $(MULTILAYER_FILE) -o $(OUTPUT_FILE) -f
# else
# 	./$(TARGET) --mode multilayer --input-file $(P2_FILE) -h 8 -l 0.1 -t 70 -s -n $(MULTILAYER_FILE)
# 	./$(TARGET) --mode multilayer --input-file $(INPUT_FILE) -n $(MULTILAYER_FILE) -o $(OUTPUT_FILE) -f
# endif

p3: p3-databases p3.2.2-alfabeto p3.2.3-alfabeto-ruido1 p3.2.3-alfabeto-ruido3 p3.2.3-alfabeto-ruido5 p3.3.1.1-timeserie1 p3.3.1.1-timeserie2 p3.3.1.1-timeserie5 p3.3.1.2-timeserie1 p3.3.1.2-timeserie2 p3.3.1.2-timeserie5 p3.3.1.3-recursiva p3.3.2.2-timeserie1 p3.3.2.2-timeserie5 p3.3.2.3-timeserie5 p3.3.2.3-timeserie15 p3.3.2.4-recursiva

p3-databases:
	gawk -f script/genera-ruido.awk --n_pixel 1 --n_reps 10 data/databases/alfabeto.txt > data/databases/alfabeto1.txt
	gawk -f script/genera-ruido.awk --n_pixel 3 --n_reps 10 data/databases/alfabeto.txt > data/databases/alfabeto3.txt
	gawk -f script/genera-ruido.awk --n_pixel 5 --n_reps 10 data/databases/alfabeto.txt > data/databases/alfabeto5.txt

p3.2.2-alfabeto: compile
	./$(TARGET) -i data/databases/alfabeto.txt -m autoencoder -h 12 -t 100 -s -n data/models/autoencoder.txt

p3.2.3-alfabeto-ruido1: compile p3.2.2-alfabeto p3-databases
	./$(TARGET) -n data/models/autoencoder.txt -m autoencoder -f -i data/databases/alfabeto1.txt

p3.2.3-alfabeto-ruido3: compile p3.2.2-alfabeto p3-databases
	./$(TARGET) -n data/models/autoencoder.txt -m autoencoder -f -i data/databases/alfabeto3.txt

p3.2.3-alfabeto-ruido5: p3.2.2-alfabeto p3-databases compile
	./$(TARGET) -n data/models/autoencoder.txt -m autoencoder -f -i data/databases/alfabeto5.txt

p3.2.4-alfabeto-ruido1: p3-databases compile
	./$(TARGET) -i data/databases/alfabeto1.txt -m autoencoder -h 12 -t 100

p3.2.4-alfabeto-ruido3: p3-databases compile
	./$(TARGET) -i data/databases/alfabeto3.txt -m autoencoder -h 12 -t 100

p3.2.4-alfabeto-ruido5: p3-databases compile
	./$(TARGET) -i data/databases/alfabeto5.txt -m autoencoder -h 12 -t 100

p3.3.1.1-timeserie1: compile
	./$(TARGET) -i data/databases/p3_serie1.txt -m timeserie -h 12 -t 50 -a 1 -d 1

p3.3.1.1-timeserie2: compile
	./$(TARGET) -i data/databases/p3_serie1.txt -m timeserie -h 12 -t 50 -a 2 -d 1

p3.3.1.1-timeserie5: compile
	./$(TARGET) -i data/databases/p3_serie1.txt -m timeserie -h 12 -t 25 -a 1 -d 1 -o salida.txt

p3.3.1.2-timeserie1: compile
	./$(TARGET) -i data/databases/p3_serie1.txt -m timeserie -h 12 -t 25 -a 1 -d 1 -o salida.txt

p3.3.1.2-timeserie2: compile
	./$(TARGET) -i data/databases/p3_serie1.txt -m timeserie -h 12 -t 25 -a 2 -d 1 -o salida.txt

p3.3.1.2-timeserie5: compile
	./$(TARGET) -i data/databases/p3_serie1.txt -m timeserie -h 12 -t 25 -a 5 -d 1 -o salida.txt

p3.3.1.3-recursiva: compile
	./$(TARGET) -i data/databases/p3_serie1.txt -m timeserie -h 20 -t 50 -a 5 -d 1 -b 0 -j 200   -o valores.txt

p3.3.2.2-timeserie1: compile
	./$(TARGET) -i data/databases/p3_serie2N.txt -m timeserie -h 12 -t 50 -a 1 -d 1

p3.3.2.2-timeserie5: compile
	./$(TARGET) -i data/databases/p3_serie2N.txt -m timeserie -h 12 -t 50 -a 5 -d 1

p3.3.2.3-timeserie5: compile
	./$(TARGET) -i data/databases/p3_serie2N.txt -m timeserie -h 12 -t 25 -a 5 -d 1

p3.3.2.3-timeserie15: compile
	./$(TARGET) -i data/databases/p3_serie2N.txt -m timeserie -h 12 -t 25 -a 15 -d 1

p3.3.2.4-recursiva: compile
	./$(TARGET) -i data/databases/p3_serie2N.txt -m timeserie -h 12 -t 50 -a 5 -d 1 -b 0 -j 200 -o valores.txt

# Flycheck (emacs) requirement
.PHONEY: check-syntax
check-syntax:
	@$(LINKER) nul $(LFLAGS) -S ${CHK_SOURCES}

etags:
	find . -type f -iname "*.[ch]" | etags -

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
	# @echo "   -p2"
	# @echo "         Executes all simulations for P2."
	@echo "   - p3"
	@echo "         Executes all simulations for P3."
	@echo "	  - p3-databases"
	@echo "			Generates files with noise for alfabeto.txt."
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
