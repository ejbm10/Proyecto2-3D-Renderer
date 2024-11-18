#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>
#include <stdlib.h>

void send_serial() {
    // Comando de upload de Arduino, reemplaza las rutas y puertos según sea necesario
    const char *command = "arduino --upload ../pruebaconect/pruebaconect.ino --port /dev/ttyUSB0 --board esp32:esp32:esp32";

    // Ejecutar el comando
    int result = system(command);

    // Verificar el resultado
    if (result == 0) {
        printf("Carga completada con éxito.\n");
    } else {
        printf("Error en la carga.\n");
    }
}

