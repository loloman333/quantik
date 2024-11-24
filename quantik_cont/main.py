import numpy as np
import torch
from torch.utils.data import Dataset, DataLoader
import numpy as np
import torch
from torch.utils.data import Dataset
import matplotlib.pyplot as plt

device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
print(device)

class QuantikDataset(Dataset):
    def __init__(self, file_path):
        self.file_path = file_path
        self.state_dtype = np.uint64  # Matches `uint64_t` in C++
        self.number_dtype = np.uint8    # Matches `unsigned char` in C++
        self.state_size = np.dtype(self.state_dtype).itemsize  # 8 bytes
        self.code_size = np.dtype(self.number_dtype).itemsize    # 1 byte
        self.record_size = self.state_size + self.code_size    # 9 bytes

        # Load the binary data
        self._load_data()

    def _load_data(self):
        # Load the entire file as a flat uint8 array
        raw_data = np.fromfile(self.file_path, dtype=np.uint8)

        # Ensure the total size is a multiple of the record size
        assert len(raw_data) % self.record_size == 0, "Corrupted or incomplete file."

        # Reshape into rows of 9 bytes (8 for state, 1 for code)
        raw_data = raw_data.reshape(-1, self.record_size)

        # Split into states (first 8 bytes) and codes (last byte)
        self.states = raw_data[:, :self.state_size].view(self.number_dtype).reshape(-1, 8)
        self.codes = raw_data[:, self.state_size].view(self.number_dtype).reshape(-1, 1)

    def __len__(self):
        return len(self.states)

    def __getitem__(self, idx):
        # Retrieve the state and code at the given index
        state = self.states[idx]
        code = self.codes[idx]

        # Convert to PyTorch tensors
        state_tensor = torch.tensor(state, dtype=torch.float32)
        code_tensor = torch.tensor(code, dtype=torch.float32)

        # Normalize the state_tensor to have values between 0 and 8
        state_tensor = state_tensor / 8
        
        return state_tensor, code_tensor

# File path to the binary data
file_path = "./data/data.qtk"

# Create the dataset and data loader
dataset = QuantikDataset(file_path)
data_loader = DataLoader(dataset, batch_size=1024, shuffle=False)

# Example usage
for batch_idx, (states, targets) in enumerate(data_loader):
    print(f"Batch {batch_idx}:")
    print("States:", states.shape)
    print("Targets:", targets.shape)
    print("First state:", states[0])
    print("First target:", targets[0])
    print("Output types:")
    print("States:", states.dtype)
    print("Targets:", targets.dtype)
    break

# train a simple fully connected network

import torch.nn as nn
import torch.optim as optim

model = nn.Sequential(
    nn.Linear(8, 128),
    nn.BatchNorm1d(128),
    nn.ReLU(),
    nn.Linear(128, 64),
    nn.BatchNorm1d(64),
    nn.ReLU(),
    nn.Linear(64, 32),
    nn.BatchNorm1d(32),
    nn.ReLU(),
    nn.Linear(32, 1)
)
model = model.to(device)

criterion = nn.MSELoss()
optimizer = optim.Adam(model.parameters(), lr=0.001)


losses = []
avg_losses = []
for epoch in range(10):
    for i, (states, targets) in enumerate(data_loader):
        optimizer.zero_grad()
        outputs = model(states)
        loss = criterion(outputs, targets)
        loss.backward()
        optimizer.step()
        if i % 100 == 0:
            print(f"Epoch {epoch}, Batch {i}, Loss: {loss.item()}")
            losses.append(loss.item())
            avg_loss = np.mean(losses[-5:])
            avg_losses.append(avg_loss)
            plt.cla()
            plt.plot(losses, label='loss', color='blue')
            plt.plot(avg_losses, label='moving avg of last 100 losses', color='orange')
            plt.legend()
            plt.draw()
            plt.pause(0.01)
        
    torch.save(model.state_dict(), f"model_epoch_{epoch}.pth")
    
plt.show()

