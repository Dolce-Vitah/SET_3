import matplotlib.pyplot as plt

x_file_path = 'X_dots.txt'
y_file_path = 'Y_rel_deviation.txt'

with open(x_file_path, 'r') as file:
    x_values = [int(line.strip()) for line in file]

with open(y_file_path, 'r') as file:
    y_values = [float(line.strip()) for line in file]

plt.figure(figsize=(10, 6))
plt.plot(x_values, y_values, label="Относительное отклонение", color='g', marker='o', markersize=3, linewidth=1.5)
plt.title('Зависимость относительного отклонения от N', fontsize=14)
plt.xlabel('Кол-во случайно сгенерированных точек (N)', fontsize=12)
plt.ylabel('Относительное отклонение (%)', fontsize=12)
plt.ylim(0, 15)
plt.grid(alpha=0.3)
plt.legend()
plt.tight_layout()

plt.show()
