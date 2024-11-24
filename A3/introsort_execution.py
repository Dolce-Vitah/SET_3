import matplotlib.pyplot as plt

x_file_path = 'X_sort.txt'
y_file_path_first = 'Y_introsort_random.txt'
y_file_path_second = 'Y_introsort_reversed.txt'
y_file_path_third = 'Y_introsort_nearly_sorted.txt'

with open(x_file_path, 'r') as file:
    x_values = [int(line.strip()) for line in file]

with open(y_file_path_first, 'r') as file:
    y_values_random = [float(line.strip()) for line in file]

with open(y_file_path_second, 'r') as file:
    y_values_reversed = [float(line.strip()) for line in file]

with open(y_file_path_third, 'r') as file:
    y_values_nearly_sorted = [float(line.strip()) for line in file]

plt.figure(figsize=(10, 6))
plt.plot(x_values, y_values_random, label="Массивы из случайных значений", color='b')
plt.plot(x_values, y_values_reversed, label='Массивы, отсортированные в обратном порядке', color='g')
plt.plot(x_values, y_values_nearly_sorted, label='Почти отсортированные массивы', color='r')
plt.title('Время выполнения алгоритма Introsort Sort', fontsize=14)
plt.xlabel('Размер массива', fontsize=12)
plt.ylabel('Время выполнения (микросекунды)', fontsize=12)
plt.ylim(0, 6000)
plt.grid(alpha=0.3)
plt.legend()
plt.tight_layout()

plt.show()
