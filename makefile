# Detect OS
OS := $(shell uname -s)

# Define variables
TSHARK_CSV = tshark_output.csv
PYTHON_CSV = Data_after_conversion.csv
CPP_CSV = output.csv
CPP_CSV2 = output2.csv
PCAPNG_FILE = selected_pcapng.txt  
PYTHON_SCRIPT1 = HexToASCII.py
PYTHON_SCRIPT2 = plot.py
CPP_EXEC = a.out
CPP_EXEC2 = test
CPP_SOURCE2 = calc_bufferlength_apmd.cpp
CPP_SOURCE = main.cpp

# Check OS and set executable names
ifeq ($(OS),Windows_NT)
    RM = del /F
    PYTHON = python
    EXE_EXT = .exe
else
    RM = rm -f
    PYTHON = python3
    EXE_EXT =
endif

# Default target (run all steps in sequence)
all: select_pcap run_python1 compile_cpp compile_cpp2 run_cpp run_cpp2 run_python2 clean

# Step 1: Select a PCAP file and run TShark
select_pcap: 
	$(PYTHON) process_tshark.py $(TSHARK_CSV) $(PCAPNG_FILE)

# Step 2: Run Python to process TShark CSV
run_python1: select_pcap
	$(PYTHON) $(PYTHON_SCRIPT1) $(TSHARK_CSV) $(PYTHON_CSV)

# Step 3: Compile C++ code
compile_cpp: run_python1
	g++ -o $(CPP_EXEC)$(EXE_EXT) $(CPP_SOURCE) -std=c++11

compile_cpp2: run_python1
	g++ -o $(CPP_EXEC2)$(EXE_EXT) $(CPP_SOURCE2) -std=c++11

# Step 4: Run C++ to process the second CSV
run_cpp: compile_cpp
	./$(CPP_EXEC)$(EXE_EXT) $(PYTHON_CSV) $(CPP_CSV)

run_cpp2: compile_cpp2
	./$(CPP_EXEC2)$(EXE_EXT) $(PYTHON_CSV) $(CPP_CSV2)

# Step 5: Run Python to generate graphs
run_python2: run_cpp run_cpp2 
	$(PYTHON) $(PYTHON_SCRIPT2) $(CPP_CSV)

# Clean up compiled files
clean:
	$(RM) $(CPP_EXEC)$(EXE_EXT) $(CPP_EXEC2)$(EXE_EXT) $(CPP_CSV2) $(TSHARK_CSV) $(PYTHON_CSV) $(CPP_CSV) $(PCAPNG_FILE)
