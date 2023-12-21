#include <stdio.h>
#include <string.h>
#define PAGE_SIZE 8 

struct Page {
  int pageNumber;
  char data[PAGE_SIZE];
};

#define NUM_PAGES 5

struct VirtualMemory {
  struct Page pages[NUM_PAGES];  
};

int last_page = 0;
// Función para escribir frases en las páginas
void clearInputBuffer() {
  int c;
  while ((c = getchar()) != '\n' && c != EOF);
}
void escribir_paginas(struct VirtualMemory *mv) {

  char frase[100];

  for(int i = 0; i < NUM_PAGES; i++) {
    printf("Ingrese frase para página %d: ", i);
    fgets(frase, 100, stdin);
    strcpy(mv->pages[i].data, frase);  
  }

}
void insertar_recurso(struct VirtualMemory *mv) {
   clearInputBuffer();
  char recurso[100];
  printf("Ingrese el recurso: ");
  fgets(recurso, 100, stdin);
  int recurso_len = strlen(recurso);
  int num_paginas = (recurso_len + PAGE_SIZE - 1) / PAGE_SIZE;  // Calcular número de páginas necesarias
  int char_index = 0;
  if(num_paginas>NUM_PAGES){
    printf("Error, memoria insuficiente\n");
    return;
  }
  if(num_paginas+last_page> NUM_PAGES){
    printf("Error, memoria insuficiente\n");
    return;
  }
  for (int i = last_page; i < last_page+num_paginas; i++) {
    int caracteres_a_copiar = (recurso_len - char_index < PAGE_SIZE) ? (recurso_len - char_index) : PAGE_SIZE;
    strncpy(mv->pages[i].data, recurso + char_index, caracteres_a_copiar);
    char_index += caracteres_a_copiar;
  }
  last_page = last_page + num_paginas;
}
// Función para leer una pág3ina
void leer_pagina(struct VirtualMemory *mv, int num_pagina) {
  printf("Contenido de la página %d:\n", num_pagina);
  printf("%s\n", mv->pages[num_pagina].data);
}
void ver_paginas(struct VirtualMemory *mv) {
  printf("Páginas de la memoria virtual:\n");
  for(int i = 0; i < NUM_PAGES; i++) {
    printf("Página %d: %s\n", i, mv->pages[i].data);
  }
}

void escribir_archivo(struct VirtualMemory *mv, char *nombre_archivo) {

  FILE *f = fopen(nombre_archivo, "w");

  if(f == NULL) {
    printf("Error abriendo el archivo");
    return;
  }

  for(int i = 0; i < NUM_PAGES; i++) {
    fputs(mv->pages[i].data, f);
  }

  fclose(f);

}

// Carga contenido de archivo en páginas
void cargar_archivo(struct VirtualMemory *mv, char *nombre_archivo) {

  FILE *f = fopen(nombre_archivo, "r");

  if(f == NULL) {
    printf("Error abriendo el archivo");
    return;
  }
  char *datos = mv->pages[0].data;

  for(int i = 0; i < NUM_PAGES; i++) {
    fgets(datos, 100, f); 
    datos = mv->pages[i+1].data; 
  }

  fclose(f);  

}

int main(void) {

  struct VirtualMemory vm;
  struct VirtualMemory from_file;
  char filename[25] = ".//lab.txt";
  char nombre_archivo[50];

  int opcion;
  do {
    printf("\nMenú: \n");
    printf("1. Ingresar recurso\n");
    printf("2. Imprimir contenido de una página\n");
    printf("3. Imprimir todas las paginas\n");
    printf("4. Escribir archivo\n");
    printf("5. Cargar archivo\n");
    printf("6. Salir\n");
    printf("Seleccione una opción: ");
   
    scanf("%d", &opcion);
    switch (opcion) {
      case 1:
        
        insertar_recurso(&vm);
        break;
      case 2:
        printf("¿Qué página desea imprimir? ");
        int pagina;
        scanf("%d", &pagina);
        leer_pagina(&vm, pagina);
        break;
      case 3:
        ver_paginas(&vm);
        break;

      case 4:
        escribir_archivo(&vm, filename);
        break;
      case 5:
        cargar_archivo(&from_file, filename);
        printf("\nCargando del archivo\n");
        for(int i = 0; i<NUM_PAGES; i++){
          leer_pagina(&from_file, i);
        }
        break;
      case 6:
        return 0;
    }
  } while (opcion !=6);
  return 0;
}