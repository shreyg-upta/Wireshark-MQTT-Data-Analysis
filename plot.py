import pandas as pd
import matplotlib.pyplot as plt
import re

def extract_filename_info():
    try:
        with open("selected_pcapng.txt", "r") as file:
            filename = file.readline().strip()  # Read first line and remove whitespace
    except FileNotFoundError:
        print("Error: Could not open selected_pcapng.txt!")
        return None, None

    # Extract "nXms" (where X is a digit 1-9)
    match = re.search(r"([1-9])0ms", filename)
    ms_value = int(match.group(1)) if match else None

    # Check if filename contains "laptop" (case insensitive)
    is_laptop = "laptop" in filename.lower()

    return ms_value, is_laptop

def plot_subplots(csv_file, csv_file2):
    # Extract filename information for title
    ms_value, is_laptop = extract_filename_info()
    
    # Read the first CSV file
    data = pd.read_csv(csv_file)

    # Ask the user if they follow the original definition of buffer length
    original_definition = input("Do you follow the original definition of buffer length? (1/0): ").strip() == '1'

    # Extract columns from the first CSV
    x = data.iloc[:, 0]  # First column as x-axis
    y1 = data.iloc[:, 1]  # Second column as y-axis for first subplot
    y2 = data.iloc[:, 2]  # Third column as y-axis for second subplot

    y3 = None  # Initialize y3 as None
    if not original_definition:
        # Only extract y3 if the dataset has at least 4 columns
        if data.shape[1] > 3:
            y3 = data.iloc[:, 3]  

    # Read the second CSV only if original_definition is True
    x2, y4 = None, None
    if original_definition:
        data2 = pd.read_csv(csv_file2)
        x2 = data2.iloc[:, 0]  # First column as x-axis
        y4 = data2.iloc[:, 1]  # Second column as y-axis

    # Create subplots with shared x-axis
    fig, axes = plt.subplots(3, 1, figsize=(8, 10), sharex=True)
    
    # Plot data on subplots
    axes[0].plot(x, y1, linestyle='-', color='b')
    axes[0].set_ylabel('Y1')
    axes[0].set_title('Delay')
    
    axes[1].plot(x, y2, linestyle='-', color='g')
    axes[1].set_ylabel('Y2')
    axes[1].set_title('Number of Retransmissions for the Signal')
    
    if original_definition and x2 is not None and y4 is not None:
        axes[2].plot(x2, y4, linestyle='-', color='r')
    elif y3 is not None:
        axes[2].plot(x, y3, linestyle='-', color='r')

    axes[2].set_ylabel('Y3')
    axes[2].set_xlabel('Time Instants')
    axes[2].set_title('Buffer Length for the Signal')

    # Generate title based on extracted filename info
    title_text = f"for the file: selected_pcapng.txt | Milliseconds: {ms_value*10 if ms_value else 'Not Found'} | {'Laptop' if is_laptop else 'RPi'}"
    
    # Add title to the top
    fig.suptitle(title_text, fontsize=12, fontweight='bold')

    # Adjust layout
    plt.tight_layout(rect=[0, 0, 1, 0.97])  # Leave space for suptitle
    plt.show()

# Call the function with the CSV file names
plot_subplots('output.csv', 'output2.csv')
