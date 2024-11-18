import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

# Load the CSV file to examine its structure and contents
file_path = 'output_table.csv'
tsp_data = pd.read_csv(file_path)

# Calculate distance improvement percentage and time efficiency metrics
tsp_data['Distance Improvement (%)'] = ((tsp_data['Initial Distance'] - tsp_data['Optimized Distance']) / tsp_data['Initial Distance']) * 100
tsp_data['Time per Unit Improvement'] = tsp_data['Final Time (ms)'] / tsp_data['Distance Improvement (%)']

# Set up seaborn style for better visuals
sns.set_theme(style="whitegrid")

# Plot 1: Average Optimized Distance by Heuristic
avg_optimized_distance = tsp_data.groupby(['Constructive Heuristic', 'Perturbative Heuristic'])['Optimized Distance'].mean().reset_index()
plt.figure(figsize=(12, 6))
sns.barplot(data=avg_optimized_distance, x="Constructive Heuristic", y="Optimized Distance", hue="Perturbative Heuristic")
plt.title("Average Optimized Distance by Heuristic Combination")
plt.xticks(rotation=45)
plt.legend(title="Perturbative Heuristic", bbox_to_anchor=(1.05, 1), loc='upper left')
plt.tight_layout()
plt.show()

# Plot 2: Distance Improvement (%) by Constructive and Perturbative Heuristics
plt.figure(figsize=(12, 6))
sns.barplot(data=tsp_data, x="Constructive Heuristic", y="Distance Improvement (%)", hue="Perturbative Heuristic")
plt.title("Distance Improvement (%) by Heuristic Combination")
plt.xticks(rotation=45)
plt.legend(title="Perturbative Heuristic", bbox_to_anchor=(1.05, 1), loc='upper left')
plt.tight_layout()
plt.show()

# Plot 3: Time Efficiency (Time per Unit Improvement) by Heuristic
plt.figure(figsize=(12, 6))
sns.scatterplot(data=tsp_data, x="Distance Improvement (%)", y="Time per Unit Improvement", hue="Constructive Heuristic", style="Perturbative Heuristic", s=100)
plt.title("Time per Unit Improvement by Heuristic Combination")
plt.legend(title="Heuristics", bbox_to_anchor=(1.05, 1), loc='upper left')
plt.tight_layout()
plt.show()

# Box and Whisker Plot: Distribution of Distance Improvements by Heuristic
plt.figure(figsize=(12, 6))
sns.boxplot(data=tsp_data, x="Constructive Heuristic", y="Distance Improvement (%)", hue="Perturbative Heuristic")
plt.title("Box and Whisker Plot")
plt.xticks(rotation=45)
plt.ylabel("Distance Improvement (%)")
plt.tight_layout()
plt.show()

# Heatmap: Average Optimized Distance and Time per Unit Improvement by Heuristic
heatmap_data = tsp_data.groupby(['Constructive Heuristic', 'Perturbative Heuristic'])[['Optimized Distance', 'Time per Unit Improvement']].mean().unstack()
plt.figure(figsize=(12, 6))
sns.heatmap(heatmap_data['Optimized Distance'], annot=True, fmt=".2f", cmap="coolwarm")
plt.title("Heatmap of Average Optimized Distance by Heuristic Combination")
plt.ylabel("Constructive Heuristic")
plt.xlabel("Perturbative Heuristic")
plt.tight_layout()
plt.show()

# 2. Pareto Analysis: Distance vs. Time Trade-off
# plt.figure(figsize=(12, 6))
# sns.scatterplot(data=tsp_data, x="Final Time (ms)", y="Optimized Distance", hue="Constructive Heuristic", style="Perturbative Heuristic", s=100)
# plt.title("Pareto Analysis: Optimized Distance vs. Final Time")
# plt.xlabel("Final Time (ms)")
# plt.ylabel("Optimized Distance")
# plt.legend(title="Heuristics", bbox_to_anchor=(1.05, 1), loc='upper left')
# plt.tight_layout()
# plt.show()

# 3. Impact of Number of Cities on Performance
# plt.figure(figsize=(12, 6))
# sns.lineplot(data=tsp_data, x="Number of Cities", y="Optimized Distance", hue="Constructive Heuristic", style="Perturbative Heuristic", markers=True)
# plt.title("Optimized Distance vs. Number of Cities")
# plt.xlabel("Number of Cities")
# plt.ylabel("Optimized Distance")
# plt.legend(title="Heuristics", bbox_to_anchor=(1.05, 1), loc='upper left')
# plt.tight_layout()
# plt.show()

# plt.figure(figsize=(12, 6))
# sns.lineplot(data=tsp_data, x="Number of Cities", y="Final Time (ms)", hue="Constructive Heuristic", style="Perturbative Heuristic", markers=True)
# plt.title("Final Time (ms) vs. Number of Cities")
# plt.xlabel("Number of Cities")
# plt.ylabel("Final Time (ms)")
# plt.legend(title="Heuristics", bbox_to_anchor=(1.05, 1), loc='upper left')
# plt.tight_layout()
# plt.show()

# 5. Performance Across Different TSP Files
# file_analysis = tsp_data.groupby(['File Name', 'Constructive Heuristic', 'Perturbative Heuristic'])[['Optimized Distance', 'Final Time (ms)']].mean().reset_index()
# plt.figure(figsize=(12, 6))
# sns.barplot(data=file_analysis, x="File Name", y="Optimized Distance", hue="Constructive Heuristic")
# plt.title("Performance Across Different TSP Files: Optimized Distance")
# plt.xticks(rotation=45)
# plt.tight_layout()
# plt.show()

# plt.figure(figsize=(12, 6))
# sns.barplot(data=file_analysis, x="File Name", y="Final Time (ms)", hue="Constructive Heuristic")
# plt.title("Performance Across Different TSP Files: Final Time")
# plt.xticks(rotation=45)
# plt.tight_layout()
# plt.show()
