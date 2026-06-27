## Instrucciones de Compilación y Ejecución

### Compilación
Usa el siguiente comando en la terminal para compilar el programa junto con las librerías de TDAs:
 ````
 gcc tdas/*.c main.c -Wno-unused-result -o Proyecto-Matico
 ````

 ### Ejecución
Una vez compilado, ejecuta el programa con:
 ````
./Proyecto-Matico
 ````


 ## funciones principales:

### menuListas:
Abre un menu que da 4 opciones, cada una para un tipo de elemento y una para volver al menu principal, dependiendo del tipo de elemento de despliega la totalidad de elementos de ese tipo y se muestran por pantalla al usuario

### menuBuscar:
Abre un menu con 3 opciones, cada una para hacer una busqueda especifica ya sea para una cancion, un artista mas todas las canciones de ese artista o algun album en especifico que me muestra el dueño de el album mas las canciones que esten compuestas dentro.

 ### menuReproducir:

### menuPlaylist:
Abre un menu que da 4 opciones, crear una playlist al gusto propio del usuario, eliminar canciones dentro de la playlist , eliminar toda la playlist, y ver la playlist con las canciones que estan dentro.

### menuDjMatico: 
Abre un menu que da 5 opciones al usuario, como por ejemplo elegir una cancion random, ver el estado actual de la cancion como su nombre de la cancion,nombre del artista y album, crear una secuencia de canciones sugeridas segun el estado de animo de la cancion random y un cambio de ritmo si es que el usuario ya no esta triste y quiere cambiar la vibra.

### recomendarPorEstadoDeAnimo:
se habre un menu que le da 3 opciones al usuario para saber que tipo de emocion le gustaria escuchar en sus canciones, tambien se despliega tras seleccionar la emocion si se quiere de algun artista en especifico u/o album en especifico.

### menuMeGusta:
Abre un menú que le da 4 opciones al usuario para saber que tipo de dato quiere seleccionar y uno para volver al menu principal, despues se le pregunta si quiere marcar con -1 o 1 y se le pide el nombre del elemento a marcar.

### guardarPlaylists:
Lee la lista de playlist creadas y se guarda en una linea de un archivo el nombre seguido de las canciones, todo separado por “ | ”.

## problemas y limitaciones: 
1) Las funciones tanto de búsqueda y las que piden que el usuario ingrese algún nombre de canción, artista y álbum son rígidas y necesitan que sean exactamente iguales.

2) El csv necesita contener de antemano la emoción que más representa a la canción.

3) El reproducir NO hace que realmente se reproduzca la musica, es mas parecido a un seleccionar.

4) El elegir canciones como “no me gusta” no implica que no se recomiende en otras funciones o que se recomiende menos el artista del dicha canción o el album. 

5) El usuario no se usa realmente pero se mantiene el el programa para ser fiel a lo entregado en el informe.

6) Dos o mas canciones no pueden tener nombres exactamente iguales en el csv, ya que en el cargar csv las guardamos en un mapa por sus nombres y no por ID.

7) El flujo de datos es para una memoria no tan extensa, si acaso tuvieramos miles de datos o millones, la funcionalidad de MoodTico seria mas ineficiente ya que el diseño es monolitico y reside en la ram

## contribución:

### Yaril Salinas:
Función mostrarMenuPrincipal, menuMeGusta, menuListas, guardarPlaylists, ademas crear el esqueleto del main y agregar la carpeta tdas, revisar el programa y comentar que se puede agregar y que cosas no funcionan correctamente.

### Sergio Torres:
Función MenuBuscar, MenuReproducir, trabajo constantemente en coordinación del trabajo, además de colaborar activamente en la presentacion y 

### Sebastian Rojas:
nexo con el docente además de las funciones crearPlaylist, CrearUsuario y el Sistema De Recomendación Por Estado De Ánimo.

### Mario Saavedra:
Desarrollo del modulo de djMatico,consistencia de referencia en la memoria ram ,algoritmos lógicos por emociones y cargar el csv

## problemas técnicos durante el desarrollo:
Sebastian Rojas: debido a perdida de avances debido al mal uso propio del git push y git pull, se solicito en 4 ocaciones la recuperacion de funciones al bot de replit para reprogramar funciones que previamente ya habian sido programadas, esto conllevo a un alto volumen de lineas programadas por el bot, en caso de descuento, Sebastian Rojas es responsable y por ende si se realiza algún descuento ojala sea a mi, Sebastian Rojas.

