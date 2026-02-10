import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from scipy.ndimage import gaussian_filter

# -------------------------
# User Settings
# -------------------------
csv_file = "output14mm_nt_Hits.csv"
grid_size = 4
full_size = 63.75  # mm
total_photons_per_event = 1000
overlay_sipms = True

# Smoothing and resolution
sigma = 1.0
hist_resolution = 300

df = pd.read_csv(csv_file, comment='#', header=None)
num_events = len(df)

# -------------------------
# Define SiPM centers
# -------------------------
x_centers = np.linspace(-full_size/2, full_size/2, grid_size)
y_centers = np.linspace(-full_size/2, full_size/2, grid_size)

# -------------------------
# Convert hits to positions
# -------------------------
x_hits = []
y_hits = []

for idx, row in df.iterrows():
    for col_idx, count in enumerate(row):
        i = col_idx // grid_size
        j = col_idx % grid_size
        x_center = x_centers[i]
        y_center = y_centers[j]
        x_hits.extend(x_center + np.random.normal(0, sigma, int(count)))
        y_hits.extend(y_center + np.random.normal(0, sigma, int(count)))

# -------------------------
# Histogram bins aligned to full detector
# -------------------------
x_edges = np.linspace(-full_size/2 - full_size/(2*hist_resolution),
                      full_size/2 + full_size/(2*hist_resolution),
                      hist_resolution+1)
y_edges = x_edges.copy()  # square detector

heatmap_counts, _, _ = np.histogram2d(x_hits, y_hits, bins=[x_edges, y_edges])

# -------------------------
# Convert to light efficiency
# -------------------------
heatmap_efficiency = heatmap_counts / (total_photons_per_event * num_events)
heatmap_smooth = gaussian_filter(heatmap_efficiency, sigma=3)

# -------------------------
# Plot
# -------------------------
plt.figure(figsize=(6,6))
im = plt.imshow(
    heatmap_smooth.T,
    origin='lower',
    cmap='viridis',
    extent=[x_edges[0], x_edges[-1], y_edges[0], y_edges[-1]],
    vmin=np.min(heatmap_smooth),
    vmax=np.max(heatmap_smooth)
)

cbar = plt.colorbar(im)
cbar.set_label("Light Efficiency", rotation=270, labelpad=15)

plt.xlabel("X (mm)")
plt.ylabel("Y (mm)")
plt.title("Centered Light Efficiency Map")
plt.tight_layout()
plt.show()
