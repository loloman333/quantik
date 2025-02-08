from torch import nn, torch
from torch.utils.data import DataLoader
import matplotlib.pyplot as plt
import numpy as np

from data import QuantikDataset

def build_model():
    
    return nn.Sequential(
        nn.Linear(16, 128),
        nn.BatchNorm1d(128),
        nn.ReLU(),
        nn.Linear(128, 64),
        nn.BatchNorm1d(64),
        nn.ReLU(),
        nn.Linear(64, 64),
        nn.BatchNorm1d(64),
        nn.ReLU(),
        nn.Linear(64, 32),
        nn.BatchNorm1d(32),
        nn.ReLU(),
        nn.Linear(32, 1)
    )
    
# Hyperparameters
file_path = "./data/data.qtk"
file_path = "../quantik/data/level4.qtk"
batchsize = 1024
shuffle_data = True
num_epochs = 100
seed = 12345678
use_extensions = {}

plot_progress = True
plot_every_x_batches = 100
plot_every_x_epoch_lines = 0
save_every_x_epochs = 0


# Main
if __name__ == "__main__":
    
    if seed is not None:
        torch.manual_seed(seed)
        np.random.seed(seed)

    print("Loading dataset...")    
    dataset = QuantikDataset(file_path, use_extensions)
    data_loader = DataLoader(dataset, batch_size=batchsize, shuffle=shuffle_data)
    print("done!")    
    
    device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
    print("Using the following device: ", device)

    model = build_model().to(device)
    model = model.to(device)
    print("Model: \n", model)

    criterion = nn.MSELoss()
    optimizer = torch.optim.Adam(model.parameters(), lr=0.001)
    print("Optimizer: ", optimizer)
    print("Criterion: ", criterion)

    
    # plot setup
    plot_step = 0
    avg_losses = []
    avg_losses_10 = []
    avg_losses_100 = []
    epochs = []
    
    for epoch in range(num_epochs):
        
        batch_losses = []
        for i, (states, targets) in enumerate(data_loader):
            
            # cuda
            states = states.to(device)
            targets = targets.to(device)
            
            # backprop
            optimizer.zero_grad()
            outputs = model(states)
            loss = criterion(outputs, targets)
            loss.backward()
            optimizer.step()
            
            # plot
            batch_losses.append(loss.item())
            if plot_progress and i % plot_every_x_batches == 0:
                
                # Prepare loss lists
                avg_loss = np.mean(batch_losses)
                batch_losses = []
                avg_losses.append(avg_loss)
                avg_loss_10 = np.mean(avg_losses[-10:])
                avg_losses_10.append(avg_loss_10)
                avg_loss_100 = np.mean(avg_losses_10[-10:])
                avg_losses_100.append(avg_loss_100)
                
                # Plot
                print(f"Epoch {epoch + 1}, Batch {i * plot_every_x_batches}, Loss: {avg_loss}")
                plt.cla()
                plt.plot(avg_losses, label='loss', color='blue')
                plt.plot(avg_losses_10, label='moving avg of last 10 losses', color='orange')
                plt.plot(avg_losses_100, label='moving avg of last 100 losses', color='red')
                for e in epochs:
                    plt.axvline(e, color='green', linestyle='--')            
                plt.legend()
                plt.draw()
                plt.pause(0.00000001)
                plot_step += 1
        
        if plot_every_x_epoch_lines != 0 and epoch % plot_every_x_epoch_lines == 0: epochs.append(plot_step)
        
        if save_every_x_epochs != 0 and epoch % save_every_x_epochs == 0:
            torch.save(model.state_dict(), f"./model_checkpoints/model_epoch_{epoch}.pth")
        
    plt.show()