#include <stdio.h>
#include <stdlib.h>
#include "tdas/list.h"
#include "tdas/heap.h"
#include "tdas/extra.h"
#include <string.h>

//---estructuras---
//-----------------
typedef struct{
char ID [100];
char Nombre [100];
List *listaArtistas;
char album [100];
int entonacion;
int meGusta;
} cancion;
//-----------------
typedef struct{
char NombreArtista [100];
List *listaCanciones;
List *listaAlbum;
int entonacion;
int meGusta;
} artista;
//-----------------
typedef struct{
char NombreAlbum [100];
char artistaPrincipal [100];
List *ListaCanciones;
int entonacion;
int meGusta;
} album;
//-----------------
  
void mostrarMenuPrincipal() {
  limpiarPantalla();
  puts("==========================================");
  puts(" MoodTico Gestión y Recomendación Musical ");
  puts("==========================================");

  puts("1) Cargar CSV");
  puts("2) Mostrar listas");
  puts("3) Buscar");
  puts("4) Reproducir");
  puts("5) Crear playlists");
  puts("6) DJ Matico");
  puts("7) Escribir estado de ánimo y recibir recomendaciones");
  puts("8) Agregar ´No me gusta´ o ´Me gusta´");
  puts("9) Salir");
}

void menuMeGusta(){
  limpiarPantalla();
  char opcion;
  do{
    puts("=====================");
    puts("Seleccione categoria ");
    puts("=====================");
  
    puts("1) Cancion");
    puts("2) Artista");
    puts("3) Album");
    puts("4) Volver al menu principal");

    printf("Ingrese su opción: ");
    scanf(" %c", &opcion);
    
    switch (opcion) {
    case '1':
      break;
    case '2':
      break;
    case '3':  
      break;
    case '4':  
      break;
    }    
    if (opcion != '4') {
        presioneTeclaParaContinuar();
    }
    }while (opcion != '4');
}

int main(){
  char opcion;
  do {
    mostrarMenuPrincipal();
    printf("Ingrese su opción: ");
    scanf(" %c", &opcion);

    switch (opcion) {
    case '1':
      //cargarCSV();
      break;
    case '2':
      break;
    case '3':  
      break;
    case '4':  
      break;
    case '5':  
      break;
    case '6':  
      break;
    case '7':  
      break;
    case '8':
      menuMeGusta();
      break;
    }    
    if (opcion != '9') {
        presioneTeclaParaContinuar();
    }

  } while (opcion != '9');

  return 0;
}
  