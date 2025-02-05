# Define variables
TSHARK_CSV = tshark_output.csv
PYTHON_CSV = Data_after_conversion.csv
CPP_CSV = output.csv
CPP_CSV = output2.csv
PCAPNG_FILE = selected_pcapng.txt  # File to store selected pcapng filename
PYTHON_SCRIPT1 = HexToASCII.py
PYTHON_SCRIPT2 = plot.py
CPP_EXEC = a.out
CPP_EXEC2 = test
CPP_SOURCE2 = calc_bufferlength_apmd.cpp
CPP_SOURCE = main.cpp

# Default target (run all steps in sequence)
all: select_pcap run_python1 compile_cpp compile_cpp2 run_cpp run_cpp2 run_python2 clean

# Step 1: Select a PCAP file and run TShark
select_pcap: 
	python3 process_tshark.py $(TSHARK_CSV) $(PCAPNG_FILE)  # Save selected file

# Step 2: Run Python to process TShark CSV
run_python1: select_pcap
	python3 $(PYTHON_SCRIPT1) $(TSHARK_CSV) $(PYTHON_CSV)  # No arguments needed

# Step 3: Compile C++ code
compile_cpp: run_python1
	g++ -o $(CPP_EXEC) $(CPP_SOURCE) -std=c++11  # Enable C++11 to fix warnings

compile_cpp2: run_python1
	g++ -o $(CPP_EXEC2) $(CPP_SOURCE2) -std=c++11  # Enable C++11 to fix warnings

# Step 4: Run C++ to process the second CSV
run_cpp: compile_cpp
	./$(CPP_EXEC) $(PYTHON_CSV) $(CPP_CSV)  # No arguments needed

run_cpp2: compile_cpp2
	./$(CPP_EXEC2) $(PYTHON_CSV) $(CPP_CSV2)  # No arguments needed

# Step 5: Run Python to generate graphs
run_python2: run_cpp run_cpp2 
	python3 $(PYTHON_SCRIPT2) $(CPP_CSV)  # No arguments needed

# Clean up compiled files
clean:
	rm -f $(CPP_EXEC) $(CPP_EXEC2) $(CPP_CSV2) $(TSHARK_CSV) $(PYTHON_CSV) $(CPP_CSV) $(PCAPNG_FILE)
