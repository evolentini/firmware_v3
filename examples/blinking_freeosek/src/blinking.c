/* Copyright 2017, Esteban Volentini - Facet UNT, Fi UNER
 * Copyright 2014, Mariano Cerdeiro
 * Copyright 2014, Pablo Ridolfi
 * Copyright 2014, Juan Cecconi
 * Copyright 2014, Gustavo Muro
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/** @file blinking.h
 **
 ** @brief Ejemplo de un led parpadeando con FreeOSEK
 **
 ** Ejemplo de un led parpadeando utilizando la capa de abstraccion de 
 ** hardware y el sistema operativo FreeOSEK.
 ** 
 ** | RV | YYYY.MM.DD | Autor       | Descripción de los cambios              |
 ** |----|------------|-------------|-----------------------------------------|
 ** |  2 | 2017.10.16 | evolentini  | Correción en el formato del archivo     |
 ** |  1 | 2017.09.21 | evolentini  | Version inicial del archivo             |
 ** 
 ** @defgroup ejemplos Proyectos de ejemplo
 ** @brief Proyectos de ejemplo de la Especialización en Sistemas Embebidos
 ** @{ 
 */

/* === Inclusiones de cabeceras ============================================ */
#include <stdint.h>
#include "blinking.h"
#include "led.h"
#include "os.h"

/* === Definicion y Macros ================================================= */

/* === Declaraciones de tipos de datos internos ============================ */

/* === Declaraciones de funciones internas ================================= */

/** @brief Función para generar demoras
 ** Función basica que genera una demora para permitir el parpadeo de los leds
 */
void Delay(void);

/* === Definiciones de variables internas ================================== */

/* === Definiciones de variables externas ================================== */

/* === Definiciones de funciones internas ================================== */

/** @brief Tarea de configuración
 **
 ** Esta tarea arranca automaticamente en el modo de aplicacion Normal.
 */
TASK(Configuracion) {

   /* Inicializaciones y configuraciones de dispositivos */
   Init_Leds();

   /* Arranque de la alarma para la activación periorica de la tarea Baliza */
   SetRelAlarm(ActivarBaliza, 350, 250);

   /* Terminación de la tarea */
   TerminateTask();
}

/** @brief Tarea que destella un led
 **
 ** Esta tarea se activa cada vez que expira la alarma ActivarBaliza
 **
 */
TASK(Baliza) {

   Led_Toggle(RGB_B_LED);

   /* Terminación de la tarea */
   TerminateTask();
}

/* === Definiciones de funciones externas ================================== */

/** @brief Función para interceptar errores
 **
 ** Esta función es llamada desde el sistema operativo si una función de 
 ** interface (API) devuelve un error. Esta definida para facilitar la
 ** depuración y detiene el sistema operativo llamando a la función
 ** ShutdownOs.
 **
 ** Los valores:
 **    OSErrorGetServiceId
 **    OSErrorGetParam1
 **    OSErrorGetParam2
 **    OSErrorGetParam3
 **    OSErrorGetRet
 ** brindan acceso a la interface que produjo el error, los parametros de 
 ** entrada y el valor de retorno de la misma.
 **
 ** Para mas detalles consultar la especificación de OSEK:
 ** http://portal.osek-vdx.org/files/pdf/specs/os223.pdf
 */
void ErrorHook(void) {
   Led_On(RED_LED);
   ShutdownOS(0);
}

/** @brief Función principal del programa
 **
 ** @returns 0 La función nunca debería terminar
 **
 ** \remark En un sistema embebido la función main() nunca debe terminar.
 **         El valor de retorno 0 es para evitar un error en el compilador.
 */
int main(void) {

   /* Inicio del sistema operatvio en el modo de aplicación Normal */
   StartOS(Normal);

   /* StartOs solo retorna si se detiene el sistema operativo */
   while(1);

   /* El valor de retorno es solo para evitar errores en el compilador*/
   return 0;
}

/* === Ciere de documentacion ============================================== */

/** @} Final de la definición del modulo para doxygen */

