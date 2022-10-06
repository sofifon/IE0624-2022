# PID,GPIO,ADC, comunicaciones USART y SPI

Se debe desarrollar una incubadora de huevos autom´atica basada en el Arduino UNO. El sistema debe sensar la
temperatura cuyo rango deseado de operaci´on es de [30,42]ºC y visualizar la temperatura y humedad en una pantalla
LCD PCD8544. Tambi´en debe incluir la funci´on de comunicaci´on con una PC para guardar un registro de estos datos en
un archivo de texto plano. Debe cumplir los siguientes requerimientos:
- Debe medir la temperatura de la incubadora utilizando un termistor o un RTD, debe investigar el sensor escogido para que cumpla con los valores de operaci´on (incluir la hoja de datos como anexo).
- Debe medir la humedad de la incubadora, investigue el sensor escogido (incluir la hoja de datos como anexo), para efectos de la simulaci´on puede utilizar una fuente variable de voltaje que represente el rango de operaci´on del mismo.
- Debe utilizar un potenciometro para establecer la temperatura de operaci´on. El sistema debe ser capaz de mantener esta temperatura por lo que debe utilizar un control PID (ver figura 2).
- En la pantalla LCD debe mostrar la temperatura de operaci´on, la se˜nal de control (salida del controlador), la temperatura sensada y humedad de operaci´on.
- Para efectos de la simulaci´on utilice una resistencia encargada de calentar la incubadora que opera en el rango de [0,12]VDC, con una potencia maxima de 5W. Suponga que calienta la incubadora en un rango de temperatura de [0,80]ºC.
- En caso de medir una temperatura menor a 30ºC debe encender un LED de alarma color azul y en caso de medir una temperatura superior a 42ºC debe encender un LED de alarma color rojo, debe asegurarse de no superar esta temperatura.
- Debe tener un circuito capaz de manejar la resistencia de poder que calienta la incubadora.
- Debe incluir un switch para habilitar la comunicaci´on con la PC.
- Los datos obtenidos de temperatura y humedad se enviar´an hacia la computadora a trav´es del puerto serial con el bloque USART.
- Del lado de la computadora se tendr´a un programa en python que leer´a el puerto serial y guardar´a el registro de datos como un archivo CSV
