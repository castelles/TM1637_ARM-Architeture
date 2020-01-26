/*!
 * @copyright   � 2020 Caio Arthur Sales Telles <csalestelles@gmail.com>
 *
 * @brief       Exemplo de uso do periférico TM1637 na MKL25Z.
 *
 * @file        main.cpp
 * @version     1.0
 * @date        21 de janeiro de 2020
 *
 * @section     HARDWARES & SOFTWARES
 *              +board        FRDM-KL25Z da NXP.
 *              +processor    MKL25Z128VLK4 - ARM Cortex-M0+.
 *              +peripheral   Display TM1637.
 *              +compiler     MCUXpresso IDE
 *              +manual       L25P80M48SF0RM, Rev.3, September 2012
 *              +revisions    Versão(data): Descrição breve.
 *                             ++ 1.0 (21 de janeiro de 2020): Versão inicial.
 *
 * @section     AUTHORS & DEVELOPERS
 *              +institution  Universidade Federal do Amazonas
 *              +courses      Engenharia da Computação/ Engenharia Elétrica
 *              +teacher      Miguel Grimm <miguelgrimm@gmail.com>
 *              +student      Versão inicial:
 *                             ++ Caio Arthur Sales Telles <csalestelles@gmail.com>
 *
 * @section     LICENSE
 *
 *              GNU General Public License (GNU GPL).
 *
 *              Este programa � um software livre; Voc� pode redistribu�-lo
 *              e/ou modific�-lo de acordo com os termos do "GNU General Public
 *              License" como publicado pela Free Software Foundation; Seja a
 *              vers�o 3 da licen�a, ou qualquer vers�o posterior.
 *
 *              Este programa � distribu�do na esperan�a de que seja �til,
 *              mas SEM QUALQUER GARANTIA; Sem a garantia impl�cita de
 *              COMERCIALIZA��O OU USO PARA UM DETERMINADO PROP�SITO.
 *              Veja o site da "GNU General Public License" para mais detalhes.
 *
 * @htmlonly    http://www.gnu.org/copyleft/gpl.html
 */

#include "MKL25Z.H"
#include <stdint.h>
#include "mkl_DevGPIO.h"
#include "TM1637Display.h"

/*!
 * 	Declara��o do led RGB interno da placa
 */
mkl_DevGPIO dio(gpio_PTA2);
mkl_DevGPIO clk(gpio_PTA1);

TM1637Display display(clk,dio);

const uint8_t up[] = {
	SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,			// U
	SEG_A | SEG_B | SEG_E | SEG_F | SEG_G			// P
};

const uint8_t down[] = {
	SEG_B | SEG_C | SEG_D | SEG_E | SEG_G,			// d
	SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F	// O
};

void delayms(unsigned int time) {
	unsigned int i;
	int j;

	for (i = 0 ; i < time; i++) {
		for (j = 0; j < 7000; j++) {}
	}
}
/*!
 *   @brief    Realiza a configura��es das io's e espera pelas interrup��es.
 *
 *   Este programa realiza o teste da classe do perif�rico systick.
 *
 *   @return  sempre retorna o valor 0.
 */
int main(void) {
	display.setBrightness(7);
    while (1) {
    	int floor = 0;
    	while(floor < 6) {
    		display.showNumberDec(floor, false, 1, 0);
    		display.setSegments(up, 2, 2);
    		floor++;
    		delayms(4);
    	}
    	floor--;
    	while(floor >= 0) {
    		display.showNumberDec(floor, false, 1, 0);
    		display.setSegments(down, 2, 2);
    		floor--;
    		delayms(4);
    	}
        /* Espera aqui por uma interrupcao */

    }
}
