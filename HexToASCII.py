import csv

def hex_to_ascii(hex_string):
    try:
        return bytes.fromhex(hex_string).decode('utf-8')
    except ValueError:
        return 'Invalid Hex'

def convert_csv(input_csv, output_txt):
    with open(input_csv, mode='r', newline='', encoding='ISO-8859-1') as infile:  # Use ISO-8859-1
        reader = csv.reader(infile)
        data = [row for row in reader]
        data[0].append('ASCIIinfo')
        for row in data[1:]:  # Skipping the header
            if len(row) >= 10:
                row.append(hex_to_ascii(row[9]))  # Column 10 is index 9
            else:
                row.append('Missing Data')

    with open(output_txt, mode='w', newline='', encoding='utf-8') as outfile:  # Keep output UTF-8
        writer = csv.writer(outfile, delimiter='|')
        writer.writerows(data)


if __name__ == "__main__":
    input_csv = 'tshark_output.csv'
    output_csv = 'Data_after_conversion.csv'
    convert_csv(input_csv, output_csv)
    print(f"Conversion complete. Output saved to {output_csv}")
