# compiler
CC=g++

# Optimization options
OPTIM_FLAG = -O3 -DNDEBUG -I ~/TPC++/EigenLibrary/Eigen -std=c++11 -Wall
# Debug options
DEBUG_FLAG = -g -I ~/TPC++/EigenLibrary/Eigen -std=c++11 -Wall
# Choose how to compile here
CXX_FLAGS = $(OPTIM_FLAG)

# The name of the executable file
PROG = ttucm # Tensile test of a unidirectional composite material

# Sources to compile
SRC = main.cc Cell.cpp FileTools.cpp

# The compilation line
$(PROG) : $(SRC)
	$(CC) $(SRC) $(CXX_FLAGS) -o $(PROG)

# Compile and run
run : $(PROG)
	./$(PROG)

# Delete compilation files
clean :
	rm *.o *∼ $(PROG) ./results/*

# Delete results files
clean_results :
	rm ./results/*

# Clean both compilation and results files
clean_all :
	clean
	clean_results
