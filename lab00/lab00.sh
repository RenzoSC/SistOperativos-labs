#### 1)
grep "model name" ./proc/cpuinfo # AMD Ryzen 5 4500 6-Core Processor

#### 2)

grep "model name" ./proc/cpuinfo | wc -l # 12

#### 3)

# Descargamos el archivo
# Hacemos el reemplazo con sed y direccionamos el output al archivo nuevo
# Finalmente eliminamos el archivo original

wget https://www.gutenberg.org/files/11/11-0.txt && sed 's/Alice/Renzo/g' 11-0.txt > Renzo_in_wonderland.txt && rm 11-0.txt

#### 4)

sort -k5n weather_cordoba.in > min_max_cordoba.in && head -n1 min_max_cordoba.in  | awk '{printf "Minima temp maxima en %u/%u/%u\n", $1, $2, $3}' && tail -n1 min_max_cordoba.in  | awk '{printf "Maxima temp maxima en %u/%u/%u\n", $1, $2, $3}'

#### 5)

sort -k3n atpplayers.in 

#### 6)

# Primero imprimimos todas las lineas pero con (goles a favor - goles en contra) como ultima columna
# Despues ordenamos segun la columna de puntajes y en caso de empate segun la nueva columna
# Al resultado ordenado le sacamos la ultima columna y lo imprimimos

awk '{resta = $7 - $8; print $0, resta}' superliga.in | sort -k2nr -k9nr | cut -d' ' --fields=1-8

#### 7)
ip link show | grep -oG 'link/ether [0-9a-fA-F:]\+'
# Imprime la MAC address

#### 8)
# Primero crea la carpeta y se posiciona dentro de la misma, crea 10 archivos con el formato
# fma_S1_XX_es.srt y luego los renombra mediante un ciclo for para sacarle el sufijo _es

mkdir serie && cd serie && touch fma_S1_{01..10}_es.srt && for f in *_es*; do mv $f ${f%_es.srt}.srt; done

