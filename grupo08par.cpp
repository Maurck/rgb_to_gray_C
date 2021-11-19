#include <stdio.h>
#include <stdlib.h>
#include <cstdint>
#include <omp.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image/stb_image_write.h"

int main(int argc, char* argv[]) {

    double comienzoDeEjecucion, finalDeEjecucion, duracionTotal = 0;

    comienzoDeEjecucion = omp_get_wtime();

    //Manejo de argumentos 

    char* fullName = NULL;
    char* extension = NULL;

    if (argv[1] != NULL && argv[2] != NULL)
    {
        fullName = (char*)malloc(strlen(argv[1]) + 1);
        extension = (char*)malloc(strlen(argv[2]) + 1);

        if (fullName != NULL) strcpy(fullName, argv[1]);
        if (extension != NULL) strcpy(extension, argv[2]);

        if (fullName != NULL && extension != NULL) strcat(fullName, extension);

    }
    else
    {
        printf("Ingrese 2 parametros (Nombre de la imagen, Extensi�n de la imagen)\n");
        exit(1);
    }

    // Carga de imagen

    int width, height, channels;

    unsigned char* img = stbi_load(fullName, &width, &height, &channels, 0);

    if (img == NULL) {
        printf("Error al cargar la imagen\n");
        exit(1);
    }

    printf("Imagen cargada con un ancho de %dpx, un alto de %dpx y %d canales\n", width, height, channels);

    // Reserva de memoria para transformaci�n a gris

    size_t img_size = width * height * channels;
    int gray_channels = channels == 4 ? 2 : 1;
    size_t gray_img_size = width * height * gray_channels;

    void* gray_img = malloc(gray_img_size);
    if (gray_img == NULL) {
        printf("No se pudo reservar memoria para la imagen gris.\n");
        exit(1);
    }

    // Transformaci�n a gris

    omp_set_num_threads(4);
    int r, k, t;
    double x = channels;
    unsigned char* pg = (unsigned char*)gray_img;
    unsigned char* p = img;
    int counter = 0;
    int limit = width * height;


#pragma omp for
    for (counter = 0; counter < limit; counter++) {
        #pragma omp critical
        *pg = (uint8_t)((*p + *(p + 1) + *(p + 2)) / 3.0);
        if (channels == 4) {
            *(pg + 1) = *(p + 3);

        }

        p += channels;
        pg += gray_channels;
    }


    // Guardado de nueva imagen

    stbi_write_jpg(strcat(argv[1], ".bpm"), width, height, gray_channels, gray_img, 100);

    finalDeEjecucion = omp_get_wtime();
    duracionTotal = finalDeEjecucion - comienzoDeEjecucion;
    printf("\nEl programa se ejecuta en paralelo %d\n", duracionTotal);

    system("pause");
}