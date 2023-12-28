import pandas as pd
import math

# Carga tu conjunto de datos
archivo_csv = 'covid_DB-csv.csv'
df = pd.read_csv(archivo_csv)

# Muestra el conjunto de datos antes de la imputación
print("Conjunto de datos original:")
print(df)

# Imputación de valores numéricos con la media de cada columna
df_numeric = df.select_dtypes(include=['float64', 'int64'])
df_numeric_imputado = df_numeric.apply(lambda col: col.fillna(col.mean()), axis=0)

# Imputación de valores no numéricos con la moda de cada columna
df_non_numeric = df.select_dtypes(exclude=['float64', 'int64'])
df_non_numeric_imputado = df_non_numeric.apply(lambda col: col.fillna(col.mode()[0] if not col.mode().empty else None), axis=0)

# Combina los resultados de la imputación
df_imputado = pd.concat([df_numeric_imputado, df_non_numeric_imputado], axis=1)

columnas_a_obtener = [
    'SARS-Cov-2 exam result',                 
    'Patient age quantile',                  
    'Hematocrit',                    
    'Platelets',                              
    'Mean platelet volume',                   
    'Mean corpuscular hemoglobin concentration (MCHC)',
    'Leukocytes',
    'Basophils',                              
    'Eosinophils',                          
    'Monocytes',                             
    'Proteina C reativa mg/dL'               
]

for columna in df_imputado.columns:
    if columna.strip() not in columnas_a_obtener:
        df_imputado.drop(columna, axis=1, inplace=True)

df_imputado['SARS-Cov-2 exam result'] = df_imputado['SARS-Cov-2 exam result'].replace({'negative': 0, 'positive': 1})

# eliminar todos los que tengan una fila de ceros, pero no contamos la columna edad
df_imputado = df_imputado[( round(df_imputado.iloc[:, 1:-1], 1) != 0).any(axis=1)]

# redondea todo a 6 decimanles
df_imputado = df_imputado.round(6)



# Muestra el conjunto de datos después de la imputación
print("\nConjunto de datos después de la imputación:")
print(df_imputado)

# Guarda el conjunto de datos imputado en un nuevo archivo CSV
archivo_imputado = 'covid_DB-csv-process.csv'
df_imputado.to_csv(archivo_imputado, index=False)

print(f"\nConjunto de datos imputado guardado en '{archivo_imputado}'")

# obteniendo columnas

