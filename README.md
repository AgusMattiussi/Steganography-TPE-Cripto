# TPE-Cripto - Secreto Compartido con Estenografía

## Instrucciones de uso

### Compilación

La compilación se realiza con el siguiente comando:

```bash
make all
```

### Ejecución

En el directorio `bin` se encuentra el archivo ejecutable:

* `main`

Para ejecutar el mismo:
```bash
./bin/main [d|r] imageName k directory
```
Significado de cada uno de los parámetros: 
| Parámetro |  Descripción | 
|----       | ------------------ |
| d             | Indica que se va a distribuir una imagen secreta en otras imágenes. |
| r             | Indica que se va a recuperar una imagen secreta a partir de otras imágenes.  |
| imageName     |  Corresponde al nombre de un archivo de extensión .bmp. En el caso de que se haya elegido la opción `d` este archivo debe existir ya que es la imagen a ocultar y debe ser una imagen en blanco y negro (8 bits por pixel) Si se eligió la opción `r` este archivo será el archivo de salida, con la imagen secreta revelada al finalizar el programa.  |
| k             | El número corresponde a la cantidad mínima de sombras necesarias para recuperar el secreto en un esquema (k, n).  |
| directory     | El directorio donde se encuentran las imágenes en las que se distribuirá el secreto (en el caso de que se haya elegido la opción `d`), o donde están las imágenes que contienen oculto el secreto (en el caso de que se haya elegido la opción `r`).  |


### Limpieza

La limpieza de los archivos generados se realiza con el siguiente comando:

```bash
make clean
```

## Colaboradores

- Anselmo, Sol Victoria
- Mattiussi, Agustín Hernán
- Sasso, Julían
- Vazquez Currie, Malena