# Monitoreo usando ThingSpeak, Blynk y deteccion de enfermedades usando Jupyter Notebook.
<hr>

## Redes de comunicaciones 1.  
## Ingenieria de Sistemas 2021-1.
### Proyecto del curso.

* _Julian Andres Sanchez Rivera - 20181020169_  

<hr>
### Descripcion:

El presente repositorio, contiene el documentado de los procesos llevados a cabo para el desarrollo
de un sistema de monitoreo en una huerta, automatizacion de riego, comunicacion serial de entre controladores,
envio de informacion via WiFi, a un servidor de ThingSpeak, graficado e interpretacion de datos obtenidos, 
procesamiento de imagenes usando MachineLearning y el algoritmo de Birch, para la deteccion de enfermedades
usando Jupyter Notebook basado en python, adicionalmete utilizamos Blynk para la notificacion de variables de 
estado.

<hr>
## Librerias para arduino

* <code> SPI.h </code>
* <code> ThingSpeak.h </code>
* <code> WiFi.h </code>
* <code> WiFiCliente.h </code>
* <code> BlynkSimpleEsp32.h </code>
* <code> SimpleDHT.h </code>
* <code> LiquidCrystal.h </code>

__Nota:__
No olvide instalar los complementos para la deteccion del modulo WiFi, y compatibilidad de Blynk.
* _Blynk_: https://github.com/blynkkk/blynk-library/releases
* _ESP32_: https://github.com/espressif/arduino-esp32

<hr>

Para comodidad, haga la instalacion de <code> Anaconda </code>:
* _Anaconda_: https://www.anaconda.com/

## Librerias usadas para JupyterNotebook

* <code> Numpy </code>
* <code> Matplotlib </code>
* <code> Skimage.feature </code>
* <code> PIL </code>
* <code> Sklearn </code>

__Recordatorio__: 

  - Los codigos utilizados para Arduino y la ESP-32 esta por separado.
  - El codigo de JupyterNotebook se ejecuta de forma secuencial
  - Este ultimo fue generado apartir de un tutorial de cuantificacion de plantaciones:
    https://gidahatari.com/ih-es/tutorial-de-cuantificacion-de-plantaciones-con-python-y-scikit-learn
    
_El objetivo de este proyecto es netamente educativo_
