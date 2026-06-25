#include <stdio.h>
#include <stdlib.h>
#include "tdas/list.h"
#include "tdas/heap.h"
#include "tdas/extra.h"
#include <string.h>
#include <time.h> 
#include "tdas/map.h"
// a
//---estructuras---
//-----------------
typedef struct{
char ID [100];
char Nombre [100];
List *listaArtistas;
char album [100];
int emocion;
int meGusta;
List *cancionesCompatibles;
int visitado;
} cancion;
//-----------------
typedef struct{
char NombreArtista [100];
List *listaCanciones;
List *listaAlbum;
int meGusta;
} artista;
//-----------------
typedef struct{
char NombreAlbum [100];
char artistaPrincipal [100];
List *ListaCanciones;
int meGusta;
} album;
//-----------------
typedef struct{
char NombreUsuario[100];
List *ListaAlbumes;
} Usuario;

List *listaUsuarios = NULL;
List *catalogoGlobalCanciones = NULL; //nuevo
cancion *cancionActualDJ = NULL; //nuevo
List *colaReproduccionDJ = NULL; //nuevo
Map *mapaCanciones = NULL; //nuevo Mapa
Map *mapaArtistas = NULL; //nuevo Mapa
Map *mapaAlbumes = NULL; //nuevo Mapa

// ==========================================
// 3. PROTOTIPOS DE FUNCIONES
// ==========================================
void mostrarMenuPrincipal(); 
void menuMeGusta();
void crearUsuario();
void generarConexionesDelGrafo(); //nuevo
void cargarCSV(); //nuevo
void limpiarVisitadosDJ(); //nuevo
void generarSecuenciaBFS(); //nuevo
void menuDjMatico(); //nuevo

int is_equal_string(void * key1, void * key2) {
    if (key1 == NULL || key2 == NULL) return 0; 

    if (strcmp((char*)key1, (char*)key2) == 0) {
        return 1; 
    }
    return 0;
}

void mostrarMenuPrincipal() {
  limpiarPantalla();
  puts("==========================================");
  puts(" MoodTico Gestion y Recomendacion Musical ");
  puts("==========================================");
  puts("1) Mostrar listas");
  puts("2) Buscar");
  puts("3) Reproducir");
  puts("4) Crear playlists");
  puts("5) DJ Matico");
  puts("6) Escribir estado de animo y recibir recommendations");
  puts("7) Agregar No me gusta o Me gusta");
  puts("8) Salir");
}

void menuMeGusta(){
    limpiarPantalla();
    char opcion;
    char busqueda[100];
    int nuevoEstado;
    do{
        puts("=====================");
        puts("Seleccione categoria ");
        puts("=====================");
        puts("1) Cancion");
        puts("2) Artista");
        puts("3) Album");
        puts("4) Volver al menu principal");
        printf("Ingrese su opcion: ");
        scanf(" %c", &opcion);
        if (opcion >= '1' && opcion <= '3') { // es valida opcion
            do {
                printf("Ingrese 1 para 'Me Gusta' o -1 para 'No Me Gusta': ");
                if (scanf("%d", &nuevoEstado) != 1) {
                    while(getchar() != '\n'); 
                    nuevoEstado = 0;
                }
                if (nuevoEstado != 1 && nuevoEstado != -1) {
                    puts("Valor no valido. Por favor intente de nuevo.");
                }
            } while (nuevoEstado != 1 && nuevoEstado != -1);
        }
        switch (opcion) {
        case '1':
            puts("Escriba Nombre de la cancion:  ");
            scanf(" %99[^\n]", busqueda);
            while(getchar() != '\n');
            
            MapPair *par = map_search(mapaCanciones, busqueda);
            if(par != NULL){
                    cancion *c = (cancion *)par -> value;
                    c->meGusta = nuevoEstado;
                    printf("Cancion '%s' actualizada correctamente.\n", c->Nombre);
                    break;
            } else{
                puts("Cancion no encontrada.");
            }
        break;

            case '2':
                puts("Escriba Nombre del artista: ");
                scanf(" %99[^\n]", busqueda);
                while(getchar() != '\n');

                MapPair *parArtista = (MapPair *)map_search(mapaArtistas, busqueda);
                if (parArtista != NULL) {
                    artista *artistaEncontrado = (artista *)parArtista->value;
                    artistaEncontrado->meGusta = nuevoEstado; 
                    printf("Valoracion actualizada para el artista '%s'.\n", busqueda);
                } else {
                    puts("Artista no encontrado.");
                }
            break;

            case '3':  
                puts("Escriba Nombre del album: ");
                scanf(" %99[^\n]", busqueda);
                getchar();
                
                MapPair *parAlbum = (MapPair *)map_search(mapaAlbumes, busqueda);
                if (parAlbum != NULL) {
                    album *albumEncontrado = (album *)parAlbum->value;
                    albumEncontrado->meGusta = nuevoEstado; 
                    printf("Valoracion actualizada para el album '%s'.\n", busqueda);
                } else {
                    puts("Album no encontrado.");
                }
            break;

        case '4': //salir del menu
        break;
        }    
        if (opcion != '4') {
            presioneTeclaParaContinuar();
        }
    }while (opcion != '4');
}


void crearUsuario() {
  limpiarPantalla();
  puts("==========================================");
  puts("            Crear nuevo usuario           ");
  puts("==========================================");

  if (listaUsuarios == NULL)
    listaUsuarios = list_create();

  Usuario *nuevoUsuario = (Usuario *)malloc(sizeof(Usuario));
  if (nuevoUsuario == NULL) {
    puts("Error: no se pudo reservar memoria para el usuario.");
    return;
  }

  printf("Ingrese nombre de usuario: ");
  scanf(" %99[^\n]", nuevoUsuario->NombreUsuario);
  if (strlen(nuevoUsuario->NombreUsuario) == 0) {
    puts("Error: el nombre de usuario no puede estar vacío.");
    free(nuevoUsuario);
    return;
  }

  Usuario *u = (Usuario *)list_first(listaUsuarios);
  while (u != NULL) {
    if (strcmp(u->NombreUsuario, nuevoUsuario->NombreUsuario) == 0) {
      puts("Error: ya existe un usuario con ese nombre.");
      free(nuevoUsuario);
      return;
    }
    u = (Usuario *)list_next(listaUsuarios);
  }

  nuevoUsuario->ListaAlbumes = list_create();
  list_pushBack(listaUsuarios, nuevoUsuario);

  printf("Usuario '%s' creado exitosamente.\n", nuevoUsuario->NombreUsuario);
}

void generarConexionesDelGrafo() {
    if (catalogoGlobalCanciones == NULL) return;
    int total = 0;
    cancion *c = (cancion *)list_first(catalogoGlobalCanciones);
    while (c != NULL) {
        total++;
        c = (cancion *)list_next(catalogoGlobalCanciones);
    }
    cancion **arreglo = (cancion **)malloc(total * sizeof(cancion *));
    int i = 0;
    c = (cancion *)list_first(catalogoGlobalCanciones);
    while (c != NULL) {
        arreglo[i] = c;
        i++;
        c = (cancion *)list_next(catalogoGlobalCanciones);
    }
    for (int j = 0; j < total; j++) {
        for (int k = 0; k < total; k++) {
            if (j != k && arreglo[j]->emocion == arreglo[k]->emocion) {
                list_pushBack(arreglo[j]->cancionesCompatibles, arreglo[k]);
            }
        }
    }

    free(arreglo);
}

void cargarCSV() {
    if (catalogoGlobalCanciones == NULL) {
        catalogoGlobalCanciones = list_create();
    }
    if (mapaCanciones == NULL) mapaCanciones = map_create(is_equal_string); 
    if (mapaArtistas == NULL)  mapaArtistas = map_create(is_equal_string); 
    if (mapaAlbumes == NULL)   mapaAlbumes = map_create(is_equal_string);

    FILE *archivo = fopen("canciones.csv", "r");
    if (archivo == NULL) {
        puts("\n[Error] No se pudo encontrar 'canciones.csv'. Asegurate de que este en la misma carpeta.");
        return;
    }

    char linea[1024];
    int primeraLinea = 1;

    while (fgets(linea, sizeof(linea), archivo) != NULL) {
        if (primeraLinea) {
            primeraLinea = 0;
            continue; 
        }
        linea[strcspn(linea, "\r\n")] = 0;
        char *id = strtok(linea, ",");
        char *nombre = strtok(NULL, ",");
        char *artista_csv = strtok(NULL, ",");
        char *album_csv = strtok(NULL, ",");
        char *emocion_str = strtok(NULL, ",");

        if (id == NULL || nombre == NULL || artista_csv == NULL || album_csv == NULL || emocion_str == NULL) {
            continue; 
        }

        // 1. Reservar e inicializar la estructura de la canción
        cancion *nuevaCancion = (cancion *)malloc(sizeof(cancion));
        nuevaCancion->listaArtistas = list_create();
        nuevaCancion->cancionesCompatibles = list_create(); 
        nuevaCancion->visitado = 0;
        nuevaCancion->meGusta = 1;

        strcpy(nuevaCancion->ID, id);
        strcpy(nuevaCancion->Nombre, nombre);
        strcpy(nuevaCancion->album, album_csv);
        nuevaCancion->emocion = atoi(emocion_str);

        char *nombreArtista = (char *)malloc(strlen(artista_csv) + 1);
        strcpy(nombreArtista, artista_csv);

        list_pushBack(nuevaCancion->listaArtistas, nombreArtista);
        map_insert(mapaCanciones, nuevaCancion->Nombre, nuevaCancion);

        // 2. Procesamiento del Artista utilizando 'Pair'
        MapPair *parArtista = (MapPair *)map_search(mapaArtistas, artista_csv);
        artista *art = NULL;

        if (parArtista == NULL) {
            // El artista no existe, lo creamos desde cero
            art = (artista *)malloc(sizeof(artista));
            strcpy(art->NombreArtista, artista_csv);
            art->listaCanciones = list_create();
            art->listaAlbum = list_create();
            art->meGusta = 1;      
            map_insert(mapaArtistas, art->NombreArtista, art);
        } else {
            // El artista ya existe, extraemos el puntero real desde el valor del Pair
            art = (artista *)parArtista->value;
        }
        list_pushBack(art->listaCanciones, nuevaCancion);

        // 3. Procesamiento del Álbum utilizando 'Pair'
        MapPair *parAlbum = (MapPair *)map_search(mapaAlbumes, album_csv);
        album *alb = NULL;

        if (parAlbum == NULL) {
            // El álbum no existe, lo creamos desde cero
            alb = (album *)malloc(sizeof(album));
            strcpy(alb->NombreAlbum, album_csv);
            strcpy(alb->artistaPrincipal, artista_csv);
            alb->ListaCanciones = list_create();
            alb->meGusta = 1;

            map_insert(mapaAlbumes, alb->NombreAlbum, alb);
            list_pushBack(art->listaAlbum, alb);
        } else {
            // El álbum ya existe, extraemos el puntero real desde el valor del Pair
            alb = (album *)parAlbum->value;
        }
        list_pushBack(alb->ListaCanciones, nuevaCancion);

        // 4. Agregar la canción al catálogo global general
        list_pushBack(catalogoGlobalCanciones, nuevaCancion);
    }

    fclose(archivo);
    generarConexionesDelGrafo();
    puts("\n[Exito] Archivo 'canciones.csv' cargado correctamente.");
}

void limpiarVisitadosDJ() {
    if (catalogoGlobalCanciones == NULL) return;
    
    cancion *actual = (cancion *)list_first(catalogoGlobalCanciones);
    while (actual != NULL) {
        actual->visitado = 0;
        actual = (cancion *)list_next(catalogoGlobalCanciones);
    }
}

void generarSecuenciaBFS() {
    if (cancionActualDJ == NULL) {
        puts("\n[Error] El DJ no sabe por donde empezar. Lanza una cancion al azar primero.");
        return;
    }
    
    limpiarVisitadosDJ();
    if (colaReproduccionDJ == NULL) {
        colaReproduccionDJ = list_create();
    } else {
        while (list_first(colaReproduccionDJ) != NULL) {
            list_popFront(colaReproduccionDJ);
        }
    }

    List *colaBFS = list_create();
    list_pushBack(colaBFS, cancionActualDJ);
    cancionActualDJ->visitado = 1;

    puts("\n==========================================");
    puts("      SETLIST GENERADO POR DJ MATICO     ");
    puts("==========================================");

    while (list_first(colaBFS) != NULL) {
        cancion *sonando = (cancion *)list_popFront(colaBFS); 
        printf("-> %s | Album: %s \n", sonando->Nombre, sonando->album);
        
        list_pushBack(colaReproduccionDJ, sonando); 

        if (sonando->cancionesCompatibles != NULL) {
            cancion *vecino = (cancion *)list_first(sonando->cancionesCompatibles);
            while (vecino != NULL) {
                if (vecino->visitado == 0) {
                    vecino->visitado = 1; 
                    list_pushBack(colaBFS, vecino); 
                }
                vecino = (cancion *)list_next(sonando->cancionesCompatibles);
            }
        }
    }
    puts("==========================================\n");
    free(colaBFS); 
}

void menuDjMatico() {
    limpiarPantalla();
    int opcionDJ; 
    do {
        puts("==========================================");
        puts("                DJ Matico                 ");
        puts("==========================================");
        puts("1) Lanzar cancion al azar");
        puts("2) Ver cancion actual");
        puts("3) Armar secuencia musical automatica");
        puts("4) Siguiente cancion en la cola");
        puts("5) Cambiar de ambiente musical (Saltar emocion)");
        puts("6) Volver al menu principal");
        printf("Ingrese su opcion: ");
        
        scanf("%d", &opcionDJ); 
        while(getchar() != '\n');
        switch(opcionDJ) {
            case 1:
                if (catalogoGlobalCanciones == NULL || list_first(catalogoGlobalCanciones) == NULL) {
                    puts("\n[Error] No hay canciones cargadas en el sistema.");
                } else {
                    int totalCanciones = 150; 
                    int indiceAzar = rand() % totalCanciones;
                    cancionActualDJ = (cancion *)list_first(catalogoGlobalCanciones);
                    for (int i = 0; i < indiceAzar; i++) {
                        cancionActualDJ = (cancion *)list_next(catalogoGlobalCanciones);
                        if (cancionActualDJ == NULL) {
                            cancionActualDJ = (cancion *)list_first(catalogoGlobalCanciones);
                            break;
                        }
                    }
                    puts("\n>> [DJ Matico ha lanzado una nueva pista al azar] <<");
                    printf("Sonando: %s - Artista: %s - Album: %s (Emocion: %d)\n", 
                           cancionActualDJ->Nombre, 
                           (char *)list_first(cancionActualDJ->listaArtistas), 
                           cancionActualDJ->album,
                           cancionActualDJ->emocion);
                }
                break;
                
            case 2: 
                if (cancionActualDJ == NULL) {
                    puts("\n[DJ Matico] No hay ninguna cancion sonando. Lanza una al azar primero.");
                } else {
                    puts("\n--- CANCION ACTUAL ---");
                    printf("ID: %s\n", cancionActualDJ->ID);
                    printf("Nombre: %s\n", cancionActualDJ->Nombre);
                    printf("Album: %s\n", cancionActualDJ->album);
                    printf("Emocion: %d\n", cancionActualDJ->emocion);
                }
                break;
                
            case 3:
                generarSecuenciaBFS();
                break;
                
            case 4:
                if (colaReproduccionDJ == NULL || list_first(colaReproduccionDJ) == NULL) {
                    puts("\n[DJ Matico] No hay mas canciones en la cola. ¡Genera un nuevo setlist!");
                } else {
                    cancionActualDJ = (cancion *)list_popFront(colaReproduccionDJ);
                    puts("\n>> [Cambiando de pista...] <<");
                    printf("Sonando ahora: %s - Album: %s\n", cancionActualDJ->Nombre, cancionActualDJ->album);
                }
                break;

            case 5:
                if (cancionActualDJ == NULL) {
                    puts("\n[Error] No hay ninguna cancion sonando para cambiar de estilo.");
                } else {
                    int emocionRechazada = cancionActualDJ->emocion;

                    if (colaReproduccionDJ != NULL) {
                        while (list_first(colaReproduccionDJ) != NULL) {
                            list_popFront(colaReproduccionDJ);
                        }
                    }
                    int totalCanciones = 150;
                    cancion *nuevaCancion = NULL;
                    do {
                        int indiceAzar = rand() % totalCanciones;
                        nuevaCancion = (cancion *)list_first(catalogoGlobalCanciones);
                        for (int i = 0; i < indiceAzar; i++) {
                            nuevaCancion = (cancion *)list_next(catalogoGlobalCanciones);
                            if (nuevaCancion == NULL) nuevaCancion = (cancion *)list_first(catalogoGlobalCanciones);
                        }
                    } while (nuevaCancion->emocion == emocionRechazada); 
                    
                    cancionActualDJ = nuevaCancion;
                    
                    puts("\n>> [CAMBIO DE VIBRA] <<");
                    printf("Has rechazado el estilo actual (Emocion %d).\n", emocionRechazada);
                    printf("Saltando a un nuevo estilo... Sonando: %s (Emocion %d)\n", cancionActualDJ->Nombre, cancionActualDJ->emocion);
                    puts("\nRecalibrando el algoritmo del DJ...");
                    generarSecuenciaBFS(); 
                }
                break;
        }
        
        if (opcionDJ != 6) {
            presioneTeclaParaContinuar();
            limpiarPantalla();
        }
        
    } while (opcionDJ != 6);
}

void menuListas() {
    char opcion;
    do {
        limpiarPantalla();
        puts("=====================");
        puts("   Mostrar Listas    ");
        puts("=====================");
        puts("1) Canciones");
        puts("2) Artistas");
        puts("3) Albumes");
        puts("4) Volver al menu principal");
        printf("Ingrese su opcion: ");
        scanf(" %c", &opcion);
        while(getchar() != '\n');
        switch (opcion) {
            case '1':
                puts("\n--- Lista de Canciones ---");
                MapPair *parC = map_first(mapaCanciones);
                while (parC != NULL) {
                    cancion *c = (cancion *)parC->value;
                    printf("ID: %s | Nombre: %s | Album: %s\n", c->ID, c->Nombre, c->album);
                    parC = map_next(mapaCanciones);
                }
                break;

            case '2':
                puts("\n--- Lista de Artistas ---");
                MapPair *parA = map_first(mapaArtistas);
                while (parA != NULL) {
                    artista *a = (artista *)parA->value;
                    printf("Artista: %s\n", a->NombreArtista);
                    parA = map_next(mapaArtistas);
                }
                break;

            case '3':
                puts("\n--- Lista de Albumes ---");
                MapPair *parAl = map_first(mapaAlbumes);
                while (parAl != NULL) {
                    album *al = (album *)parAl->value;
                    printf("Album: %s | Artista: %s\n", al->NombreAlbum, al->artistaPrincipal);
                    parAl = map_next(mapaAlbumes);
                }
                break;

            case '4':
                break;
        }

        if (opcion != '4') {
            presioneTeclaParaContinuar();
        }
    } while (opcion != '4');
}

void menuBuscar(){
    char opcion;
    char busqueda[100];
    do{
        limpiarPantalla();
        puts("=====================");
        puts("       Buscar");
        puts("=====================");
        puts("1) Buscar Cancion");
        puts("2) Buscar Artista");
        puts("3) Buscar Album");
        puts("4) Volver");
        printf("Ingrese su opcion: ");
        scanf(" %c", &opcion);
        while(getchar() != '\n');

        switch(opcion) {

            case '1': {
                printf("Ingrese nombre de la cancion: ");
                scanf(" %99[^\n]", busqueda);
                while(getchar() != '\n');

                MapPair *par = map_search(mapaCanciones, busqueda);
                if(par != NULL){
                    cancion *c = (cancion *)par -> value;
                    printf("\n ---------- Cancion Encontrada ---------- \n");
                    printf("ID: %s\n", c -> ID);
                    printf("ID: %s\n", c -> Nombre);
                    printf("ID: %s\n", c -> album);
                    
                }
                else{
                    puts("Cancion no encontrada.");
                }
                break;
            }
            case '2': {
                printf("Ingrese nombre del artista: ");
                scanf(" %99[^\n]", busqueda);
                while(getchar() != '\n');

                MapPair *par = map_search(mapaArtistas, busqueda);
                if(par != NULL){
                    artista *a = (artista *)par -> value;
                    printf("\n ---------- Artista Encontrado ---------- \n");
                    printf("Artista: %s\n", a -> NombreArtista);
                    puts("\n Canciones: \n");
                    cancion *c = (cancion *)list_first(a -> listaCanciones);
                    while(c != NULL){
                        printf(" - %s\n", c -> Nombre);
                        c = (cancion *)list_next(a -> listaCanciones);;
                    }
                }
                else{
                    puts("Artista no encontrado.");
                }
                break;
            }
            case '3': {
                    printf("Ingrese nombre del album: ");
                    scanf(" %99[^\n]", busqueda);
                    while(getchar() != '\n');

                    MapPair *par = map_search(mapaAlbumes, busqueda);

                    if(par != NULL) {
                        album *a = (album *)par->value;

                        printf("\n--- ALBUM ENCONTRADO ---\n");
                        printf("Album: %s\n", a->NombreAlbum);
                        printf("Artista principal: %s\n", a->artistaPrincipal);

                        puts("\nCanciones:");
                        cancion *c = (cancion *)list_first(a->ListaCanciones);

                        while(c != NULL) {
                            printf("- %s\n", c->Nombre);
                            c = (cancion *)list_next(a->ListaCanciones);
                        }
                    }
                    else {
                        puts("Album no encontrado.");
                    }
                    break;
                }
            }
            if(opcion != '4'){
                presioneTeclaParaContinuar();
            }
    }while(opcion != '4');
}

int main(){
  srand(time(NULL)); //nuevo
  int opcion; //nuevo 
  limpiarPantalla();
    puts("Iniciando componentes de MoodTico...\n");
    crearUsuario();
    presioneTeclaParaContinuar();
    limpiarPantalla();
    cargarCSV();
    presioneTeclaParaContinuar();
  do {
    mostrarMenuPrincipal();
    printf("Ingrese su opcion: ");
    scanf(" %d", &opcion);

    switch (opcion) {
        case 1: // Mostrar listocas
            menuListas();
            break;
        case 2: // Buscar
            menuBuscar();
            break;
        case 3: // Reproducir
            break;
        case 4: // Crear playlists
            break;
        case 5:
            menuDjMatico(); 
            break;
        case 6: // Estado de ánimo y recomendaciones
            break;
        case 7:
            menuMeGusta();
            break;
        case 8: // Salir
            puts("\nSaliendo de MoodTico.");
            break;
    }    
    if (opcion != 8) {
            presioneTeclaParaContinuar();
    }

  } while (opcion != 8);

  return 0;
}





