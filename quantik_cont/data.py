import os
from torch.utils.data import Dataset
import numpy as np
import multiprocessing
import torch

def merge_files(input_folder, output_file):
    """
    Merge binary files containing Quantik game states and target values into one file.
    
    Args:
        input_folder (str): Folder containing the files named 'level_0.bin', 'level_1.bin', ..., 'level_16.bin'.
        output_file (str): Path to the output merged file.
        state_size (int): Size of each game state in bytes (default: 16).
    """
    with open(output_file, "wb") as outfile:
        for level in range(17):
            input_path = os.path.join(input_folder, f"level{level}.qtk")
            if os.path.exists(input_path):
                print(f"Merging {input_path}...")
                with open(input_path, "rb") as infile:
                    while chunk := infile.read(1024):
                        outfile.write(chunk)
            else:
                print(f"Warning: {input_path} not found. Skipping.")
    print(f"All files merged into {output_file}")


class QuantikDataset(Dataset):
    def __init__(self, file_path, use_extensions):
        self.file_path = file_path
        self.state_dtype = np.uint64  # Matches `uint64_t` in C++
        self.number_dtype = np.uint8    # Matches `unsigned char` in C++
        self.state_size = np.dtype(self.state_dtype).itemsize  # 8 bytes
        self.code_size = np.dtype(self.number_dtype).itemsize    # 1 byte
        self.record_size = self.state_size + self.code_size    # 9 bytes
        self.use_extensions = use_extensions

        if "I" in use_extensions:            
            self.ext_I_piece_mapping = {0: 0, 1: 1, 2: 5, 3: 2, 4: 6, 5: 3, 6: 7, 7: 4, 8: 8}
            
        # Load the binary data
        self._load_data()
        
    def decode(self, encodings):
        states = np.zeros((encodings.shape[0], 16), dtype=self.number_dtype)
        with multiprocessing.Pool() as pool:
            states = pool.map(self._decode_row, encodings)
        return states

    def _decode_row(self, encoding):
        row = np.zeros((16,), dtype=self.number_dtype)
        for i in range(15, -1, -1):
            remainder = encoding % 9
            encoding //= 9
            row[i] = remainder[0]
        return row

    def _load_data(self):
        # Load the entire file as a flat uint8 array
        raw_data = np.fromfile(self.file_path, dtype=np.uint8)
        assert len(raw_data) % self.record_size == 0, "Corrupted or incomplete file."
        # Reshape into rows of 9 bytes (8 for state, 1 for code)
        raw_data = raw_data.reshape(-1, self.record_size)        

        # Split into states (first 8 bytes) and codes (last byte)
        self.codes = raw_data[:, self.state_size].reshape(-1, 1)        
        self.states = self.decode(raw_data[:, :self.state_size].view(self.state_dtype))
        
        # Extension I: Remap Piece Representations
        if "I" in self.use_extensions:
            self.states = np.vectorize(self.ext_I_piece_mapping.get)(self.states)


    def __len__(self):
        return len(self.states)

    def __getitem__(self, idx):
        # Retrieve the state and code at the given index
        state = self.states[idx]
        code = self.codes[idx]

        # Convert to PyTorch tensors
        state_tensor = torch.tensor(state, dtype=torch.float32)
        code_tensor = torch.tensor(code, dtype=torch.float32)

        # Normalize the state_tensor to have values between 0 and 1
        state_tensor = state_tensor / 8
        
        # Normalize the code_tensor to have values between 0 and 1
        code_tensor = code_tensor / 21
                
        return state_tensor, code_tensor


if __name__ == "__main__":
    
    input_folder = "../quantik/data"
    output_file = "./data/data.qtk"
    merge_files(input_folder, output_file)
