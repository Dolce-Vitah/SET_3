import matplotlib.pyplot as plt

x_file_path = 'X_dots.txt'
y_file_path = 'Y_area_tight_frame.txt'

with open(x_file_path, 'r') as file:
    x_values = [int(line.strip()) for line in file]

with open(y_file_path, 'r') as file:
    y_values = [float(line.strip()) for line in file]

plt.figure(figsize=(10, 6))
plt.plot(x_values, y_values, label="Значение площади области", color='b', marker='o', markersize=3, linewidth=1.5)
plt.title('Зависимость площади от N', fontsize=14)
plt.xlabel('Кол-во случайно сгенерированных точек (N)', fontsize=12)
plt.ylabel('Приближенное значение площади области', fontsize=12)
plt.ylim(0.85, 1.1)
plt.grid(alpha=0.3)
plt.legend()
plt.tight_layout()

plt.show()
