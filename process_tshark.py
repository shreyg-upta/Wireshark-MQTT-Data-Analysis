import os
import subprocess
import sys

WIRESHARK_FOLDER = "WiresharkData"

def list_pcapng_files():
    """List all .pcapng files in the WiresharkData folder."""
    if not os.path.exists(WIRESHARK_FOLDER):
        print(f"Folder '{WIRESHARK_FOLDER}' not found.")
        exit(1)

    pcapng_files = [f for f in os.listdir(WIRESHARK_FOLDER) if f.endswith('.pcapng')]
    
    if not pcapng_files:
        print(f"No .pcapng files found in '{WIRESHARK_FOLDER}'.")
        exit(1)
    
    return pcapng_files

def select_pcapng_file(pcapng_files):
    """Ask user to select a .pcapng file by index."""
    print(f"Available .pcapng files in '{WIRESHARK_FOLDER}':")
    for i, file in enumerate(pcapng_files):
        print(f"{i}: {file}")
    
    while True:
        try:
            index = int(input("Enter the index of the file to process: "))
            if 0 <= index < len(pcapng_files):
                return pcapng_files[index]
            else:
                print("Invalid index. Try again.")
        except ValueError:
            print("Enter a valid number.")

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("Usage: python3 process_tshark.py <output_csv> <selected_pcapng_file>")
        exit(1)

    output_csv = sys.argv[1]
    selected_pcapng_file = sys.argv[2]

    pcapng_files = list_pcapng_files()
    selected_pcapng = select_pcapng_file(pcapng_files)
    
    selected_pcapng_path = os.path.join(WIRESHARK_FOLDER, selected_pcapng)

    # Save selected pcapng file name
    with open(selected_pcapng_file, "w") as f:
        f.write(selected_pcapng_path)

    print(f"Processing {selected_pcapng_path}...")

    tshark_cmd = [
        "tshark", "-r", selected_pcapng_path, "-T", "fields",
        "-e", "frame.time_relative", "-e", "ip.src", "-e", "ip.dst",
        "-e", "_ws.col.protocol", "-e", "tcp.seq", "-e", "tcp.ack", "-e", "tcp.nxtseq",
        "-e", "_ws.col.info", "-e", "mqtt.msg_text", "-e", "tcp.payload",
        "-E", "header=y", "-E", "separator=,", "-E", "quote=d"
    ]
    
    with open(output_csv, "w") as f:
        subprocess.run(tshark_cmd, stdout=f)
    
    print(f"CSV saved as {output_csv}")
