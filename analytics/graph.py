import pandas as pd
import matplotlib.pyplot as plt

# List of CSV file paths
csv_file_paths = ['data1.csv', 'data2.csv', 'data3.csv', 'data4.csv', 'data5.csv']

# Initialize lists to store averages for each file
avg_objects_list = []
avg_fps_list = []

# Loop through each CSV file and calculate averages for every 50 objects
for csv_file_path in csv_file_paths:
    data = pd.read_csv(csv_file_path, names=['objects', 'fps'])
    avg_objects = data['objects'].rolling(window=50).mean()[49::50]  # Calculate average every 50 objects
    avg_fps = data['fps'].rolling(window=50).mean()[49::50]  # Calculate average every 50 objects
    avg_objects_list.append(avg_objects)
    avg_fps_list.append(avg_fps)

# Create a single graph with averages from all files
plt.figure(figsize=(10, 6))

colors = ['b', 'g', 'r', 'c', 'm']  # Adding 'm' (magenta) for the fifth file
labels = ['File 1', 'File 2', 'File 3', 'File 4', 'File 5']  # Adding 'File 5' label

for i in range(len(csv_file_paths)):
    plt.plot(avg_objects_list[i], avg_fps_list[i], linestyle='-', color=colors[i], label=labels[i])

plt.title('Average FPS vs Amount of Objects (Averaged Every 50 Objects)')
plt.xlabel('Amount of Objects')
plt.ylabel('Average FPS')
plt.legend()
plt.grid(True)
plt.show()
