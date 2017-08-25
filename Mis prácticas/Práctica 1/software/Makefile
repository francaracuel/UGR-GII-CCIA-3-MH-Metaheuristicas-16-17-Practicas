###############################################################################
##
## Francisco Javier Caracuel Beltrán
##
## Curso 2016/2017 - Metaheurísticas - GII - CCIA
##
## Makefile asociado a la práctica 1. APC.
##
###############################################################################

###############################################################################
##
BIN = ./bin
OBJ = ./obj
LIB = ./lib
INCLUDE = ./include
SRC = ./src
##
###############################################################################

###############################################################################
##
CC = g++
CCO = gcc
DEBUG = -g
OPTIMIZE = -O3
CFLAGS = -c -fopenmp $(OPTIMIZE) -std=c++11
CCOFLAGS = -c -fopenmp $(OPTIMIZE)
LFLAGS = -fopenmp
##
###############################################################################

###############################################################################
##
EXEC_FILE = p1
BINS = $(BIN)/$(EXEC_FILE)
OBJS = $(OBJ)/main.o $(OBJ)/arff.o $(OBJ)/sorter.o $(OBJ)/local_search.o $(OBJ)/genetic_a.o $(OBJ)/arff_utils.o $(OBJ)/arff_scanner.o $(OBJ)/arff_parser.o $(OBJ)/arff_lexer.o $(OBJ)/arff_token.o $(OBJ)/arff_data.o $(OBJ)/arff_instance.o $(OBJ)/arff_value.o $(OBJ)/arff_attr.o
##
###############################################################################

###############################################################################
## Reglas
##

## $@ -> nombre del objetivo
## $< -> todas las dependencias de la regla
## Añadir -I$(INCLUDE) cuando se tengenetic_a una clase con .h y .cpp

all: init \
	$(BINS) \
	end

###################
## Ejecutables
$(BIN)/$(EXEC_FILE): $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o $@ 
##
###################

###################
## Objetos
$(OBJ)/main.o: $(SRC)/main.cpp
	$(CC) $(CFLAGS) $< -o $@ -I$(INCLUDE)
	
$(OBJ)/arff.o: $(SRC)/arff.cpp $(INCLUDE)/arff.h
	$(CC) $(CFLAGS) $< -o $@ -I$(INCLUDE)
	
$(OBJ)/sorter.o: $(SRC)/sorter.cpp $(INCLUDE)/sorter.h
	$(CC) $(CFLAGS) $< -o $@ -I$(INCLUDE)
	
$(OBJ)/local_search.o: $(SRC)/local_search.cpp $(INCLUDE)/local_search.h
	$(CC) $(CFLAGS) $< -o $@ -I$(INCLUDE)
	
$(OBJ)/genetic_a.o: $(SRC)/genetic_a.cpp $(INCLUDE)/genetic_a.h
	$(CC) $(CFLAGS) $< -o $@ -I$(INCLUDE)
	
$(OBJ)/arff_utils.o: $(SRC)/arff_utils.cpp $(INCLUDE)/arff_utils.h
	$(CC) $(CFLAGS) $< -o $@ -I$(INCLUDE)

$(OBJ)/arff_scanner.o: $(SRC)/arff_scanner.cpp $(INCLUDE)/arff_scanner.h
	$(CC) $(CFLAGS) $< -o $@ -I$(INCLUDE)

$(OBJ)/arff_parser.o: $(SRC)/arff_parser.cpp $(INCLUDE)/arff_parser.h
	$(CC) $(CFLAGS) $< -o $@ -I$(INCLUDE)

$(OBJ)/arff_lexer.o: $(SRC)/arff_lexer.cpp $(INCLUDE)/arff_lexer.h
	$(CC) $(CFLAGS) $< -o $@ -I$(INCLUDE)

$(OBJ)/arff_token.o: $(SRC)/arff_token.cpp $(INCLUDE)/arff_token.h
	$(CC) $(CFLAGS) $< -o $@ -I$(INCLUDE)

$(OBJ)/arff_data.o: $(SRC)/arff_data.cpp $(INCLUDE)/arff_data.h
	$(CC) $(CFLAGS) $< -o $@ -I$(INCLUDE)

$(OBJ)/arff_instance.o: $(SRC)/arff_instance.cpp $(INCLUDE)/arff_instance.h
	$(CC) $(CFLAGS) $< -o $@ -I$(INCLUDE)

$(OBJ)/arff_value.o: $(SRC)/arff_value.cpp $(INCLUDE)/arff_value.h
	$(CC) $(CFLAGS) $< -o $@ -I$(INCLUDE)

$(OBJ)/arff_attr.o: $(SRC)/arff_attr.cpp $(INCLUDE)/arff_attr.h
	$(CC) $(CFLAGS) $< -o $@ -I$(INCLUDE)

##
###################

###################
## Limpieza
clean:
	\rm $(OBJS)

mr.proper:
	-rm $(OBJS) $(BINS)

##
###################

###################
## Mensajes

init:
	@echo
	@echo //////////////////////////////////////////////
	@echo //
	@echo // Francisco Javier Caracuel Beltrán
	@echo //
	@echo // Curso 2016/2017 - Metaheurísticas - GII - CCIA
	@echo //
	@echo // Práctica 1. APC.
	@echo //
	@echo //////////////////////////////////////////////
	@echo //
	@echo // Compilando...
	@echo //
	@echo

end:
	@echo
	@echo //
	@echo // Compilación terminada
	@echo //
	@echo //////////////////////////////////////////////
	@echo

init-clean:
	@echo
	@echo //////////////////////////////////////////////
	@echo //
	@echo // Borrando archivos...
	@echo //
	@echo

end-clean:
	@echo
	@echo //
	@echo //////////////////////////////////////////////
	@echo

##
###################

##
###############################################################################
