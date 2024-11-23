import os

def merge_files(input_folder, output_file):
    """
    Merge binary files containing Quantik game states and target values into one file.
    
    Args:
        input_folder (str): Folder containing the files named 'level_0.bin', 'level_1.bin', ..., 'level_16.bin'.
        output_file (str): Path to the output merged file.
        state_size (int): Size of each game state in bytes (default: 16).
    """
    with open(output_file, "wb") as outfile:
        for level in range(17):  # Assuming levels range from 0 to 16
            input_path = os.path.join(input_folder, f"level{level}.qtk")
            if os.path.exists(input_path):
                print(f"Merging {input_path}...")
                with open(input_path, "rb") as infile:
                    while chunk := infile.read(1024):  # Read in chunks to avoid memory issues
                        outfile.write(chunk)
            else:
                print(f"Warning: {input_path} not found. Skipping.")
    print(f"All files merged into {output_file}")

# Example usage
input_folder = "../quantik/data"  # Replace with your folder path
output_file = "./data/data.qtk"
merge_files(input_folder, output_file)
