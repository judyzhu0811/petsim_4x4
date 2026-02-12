import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from scipy.ndimage import gaussian_filter

# -------------------------
# User Settings
# -------------------------
csv_file = "build/output14mm_nt_Hits.csv"  # CSV with hits per SiPM
grid_size = 4                               # 4x4 SiPM grid
full_size = 63.75                           # mm (total detector width)
total_photons_per_event = 1000
overlay_sipms = True                        # Show SiPM centers on plot

# Smoothing and histogram resolution
sigma_smooth = 10                          # Gaussian blur sigma (in pixels)
hist_resolution = 100                       

# -------------------------
# Load CSV Data
# -------------------------
df = pd.read_csv(csv_file, comment='#', header=None)
num_events = len(df)

# -------------------------
# Define SiPM centers (evenly spaced)
# -------------------------
x_centers = np.linspace(-full_size/2, full_size/2, grid_size)
y_centers = np.linspace(-full_size/2, full_size/2, grid_size)

# -------------------------
# Convert hits to positions
# -------------------------
x_hits, y_hits = [], []

for event_idx, row in df.iterrows():
    for col_idx, count in enumerate(row):
        if count <= 0:
            continue
        
        # Map flat index to 2D grid (row-major order)
        j = col_idx // grid_size  # Y index
        i = col_idx % grid_size   # X index

        x_center = x_centers[i]
        y_center = y_centers[j]

        # Add jitter around SiPM center for each hit
        x_hits.extend(np.random.normal(x_center, 3.0, int(count)))
        y_hits.extend(np.random.normal(y_center, 3.0, int(count)))

# -------------------------
# Define histogram bins
# -------------------------
x_edges = np.linspace(-full_size/2, full_size/2, hist_resolution + 1)
y_edges = x_edges.copy()

# -------------------------
# Generate 2D histogram
# -------------------------
heatmap_counts, _, _ = np.histogram2d(x_hits, y_hits, bins=[x_edges, y_edges])

# -------------------------
# Smooth heatmap
# -------------------------
heatmap_smooth = gaussian_filter(heatmap_counts, sigma=sigma_smooth)

# -------------------------
# Normalize heatmap for visualization
# -------------------------
# Scale maximum to 0.8 to match your colorbar vmax
if heatmap_smooth.max() > 0:
    heatmap_smooth = heatmap_smooth / heatmap_smooth.max() * 0.8

# -------------------------
# Plot heatmap (no red dots)
# -------------------------
plt.figure(figsize=(6,6))
im = plt.imshow(
    heatmap_smooth.T,
    origin='lower',
    cmap='viridis',
    extent=[x_edges[0], x_edges[-1], y_edges[0], y_edges[-1]],
    vmin=0,
    vmax=0.8
)

cbar = plt.colorbar(im)
cbar.set_label("Light Efficiency", rotation=270, labelpad=15)

plt.xlabel("X (mm)")
plt.ylabel("Y (mm)")
plt.title("Centered Light Efficiency Map")
plt.tight_layout()
plt.show()

