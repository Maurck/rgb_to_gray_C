#include <stdio.h>
#include <stdlib.h>
#include <cstdint>
#include <time.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image/stb_image_write.h"

int main(int argc,char* argv[]) {

    int msec = 0;
    clock_t before = clock();

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
        printf("Ingrese 2 parametros (Nombre de la imagen, Extensión de la imagen)\n");
        exit(1);
    }

    // Carga de imagen

    int width, height, channels;

    unsigned char* img = stbi_load(fullName, &width, &height, &channels, 0);

    if (img == NULL) {
        printf("Error al cargar la imagen\n");
        exit(1);
    }



    // Reserva de memoria para transformación a gris

    size_t img_size = width * height * channels;
    int gray_channels = channels == 4 ? 2 : 1;
    size_t gray_img_size = width * height * gray_channels;

    void* gray_img = malloc(gray_img_size);
    if (gray_img == NULL) {
        printf("No se pudo reservar memoria para la imagen gris.\n");
        exit(1);
    }
    // Transformación a gris

    unsigned char* pg = (unsigned char*)gray_img;
    unsigned char* p = img;

    int limit = width * height;

    for (int counter = 0; counter < limit; counter++) {
        *pg = (uint8_t)((*p + *(p + 1) + *(p + 2)) / 3.0);
        if (channels == 4) {
            *(pg + 1) = *(p + 3);

        }

        p += channels;
        pg += gray_channels;
    }

    // Guardado de nueva imagen

    stbi_write_jpg(strcat(argv[1], ".bpm"), width, height, gray_channels, gray_img, 100);

    clock_t difference = clock() - before;
    msec = difference * 1000 / CLOCKS_PER_SEC;
    printf("Tiempo total del programa: %d segundos y %d milisegundos\n", msec / 1000, msec % 1000);

    system("pause");
}

