import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

# --- Read CSV ---
csv_file = "output14mm_nt_Hits.csv"
df = pd.read_csv(csv_file, comment='#', header=None)

grid_size = 4
grid = np.zeros((grid_size, grid_size))

# Fill 4x4 grid from CSV
for i in range(grid_size):
    for j in range(grid_size):
        col_idx = i*grid_size + j
        if col_idx < df.shape[1]:
            grid[i,j] = df[col_idx].sum()

# --- Efficiency ---
total_photons_per_event = 1000
num_events = len(df)
grid_efficiency = grid / (total_photons_per_event * num_events)

# --- Physical coordinates ---
full_size = 63.75  # mm
cell_size = full_size / (grid_size - 1)  # spacing between centers
x_positions = np.linspace(-full_size/2, full_size/2, grid_size)
y_positions = np.linspace(-full_size/2, full_size/2, grid_size)

# Compute cell edges for imshow
x_edges = np.linspace(-full_size/2 - cell_size/2, full_size/2 + cell_size/2, grid_size+1)
y_edges = np.linspace(-full_size/2 - cell_size/2, full_size/2 + cell_size/2, grid_size+1)

# --- Plot ---
plt.figure(figsize=(6,6))
im = plt.imshow(grid_efficiency.T, origin='lower', cmap='viridis',
                extent=[x_edges[0], x_edges[-1], y_edges[0], y_edges[-1]],
                vmin=0, vmax=1)

cbar = plt.colorbar(im)
cbar.set_label('Light Efficiency', rotation=270, labelpad=15)

# Add text at cell centers
for i in range(grid_size):
    for j in range(grid_size):
        plt.text(x_positions[i], y_positions[j], f"{grid_efficiency[i,j]:.2f}",
                 ha='center', va='center', color='white', fontsize=12)

plt.title('4x4 SiPM Light Efficiency')
plt.xlabel('X (mm)')
plt.ylabel('Y (mm)')
plt.tight_layout()
plt.show()
