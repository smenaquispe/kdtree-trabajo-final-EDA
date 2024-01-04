import csv
import matplotlib.pyplot as plt

# Leer el primer archivo CSV
archivo_csv1 = 'range.txt'
columna_x_1 = 4  # Columna 3 (la indexación comienza desde 0)
columna_y_1 = 5  # Columna 4 (la indexación comienza desde 0)

x1 = []
y1 = []

with open(archivo_csv1, 'r') as archivo:
    lector_csv = csv.reader(archivo)
    next(lector_csv)  # Salta la primera fila si contiene encabezados
    for fila in lector_csv:
        x1.append(float(fila[columna_x_1]))
        y1.append(float(fila[columna_y_1]))

# Leer el segundo archivo CSV
archivo_csv2 = 'covid_DB-csv-process.csv'
columna_x_2 = 4  # Columna 3 (la indexación comienza desde 0)
columna_y_2 = 5  # Columna 4 (la indexación comienza desde 0)

x2 = []
y2 = []

with open(archivo_csv2, 'r') as archivo:
    lector_csv = csv.reader(archivo)
    next(lector_csv)  # Salta la primera fila si contiene encabezados
    for fila in lector_csv:
        x2.append(float(fila[columna_x_2]))
        y2.append(float(fila[columna_y_2]))

# Crea la gráfica de dispersión con colores diferentes y transparencia
plt.scatter(x2, y2, color='red', label='Total')
plt.scatter(x1, y1, color='blue', label='Range')

# Configuración adicional
plt.title('Gráfica de Dispersión')
plt.xlabel('Mean corpuscular hemoglobin concentration (MCHC)')
plt.ylabel('Leukocytes')
plt.legend()  # Muestra la leyenda
plt.show()
