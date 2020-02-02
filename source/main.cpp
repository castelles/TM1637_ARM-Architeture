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
 *                             ++ 1.0 (26 de janeiro de 2020): Versão inicial.
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
 *              Este programa é um software livre; Você pode redistribuí-lo
 *              e/ou modificá-lo de acordo com os termos do "GNU General Public
 *              License" como publicado pela Free Software Foundation; Seja a
 *              versão da licença, ou qualquer vers�o posterior.
 *
 *              Este programa é distribuído na esperança de que seja útil,
 *              mas SEM QUALQUER GARANTIA; Sem a garantia implícita de
 *              COMERCIALIZAÇÃO OU USO PARA UM DETERMINADO PROPÓSITO.
 *              Veja o site da "GNU General Public License" para mais detalhes.
 *
 *              Para mais informações ou dúvidas sobre o funcionamento do periférico
 *              TM1637 e/ou o código abaixo, entrar em contato com Caio Arthur.
 *
 * @htmlonly    http://www.gnu.org/copyleft/gpl.html
 */

#include "MKL25Z.H"
#include <stdint.h>
#include "mkl_DevGPIO.h"
#include "TM1637Display.h"

/*!
 * 	Declaração dos pinos de clock e dados do periférico
 */

mkl_DevGPIO dio(gpio_PTA2);
mkl_DevGPIO clk(gpio_PTA1);

/*!
 *	Declaração do display
 */

TM1637Display display(clk,dio);

void delayms(unsigned int time) {
	unsigned int i;
	int j;

	for (i = 0 ; i < time; i++) {
		for (j = 0; j < 7000; j++) {}
	}
}

void setup(){
	display.setBrightness(7);
	display.setDigitMode(hide);
	display.setLength(one);
	display.setDoubleDots(false);
}

uint8_t data[] = { };
/*!
 *   @brief    Realiza a simulação do funcionamento do elevador na subida e na descida
 *
 *   Este programa realiza o teste da classe do periférico TM1637.
 *
 *   @return  sempre retorna o valor 0.
 */
int main(void) {

	setup();

    while (1) {
    	while(floor < 16) {
    		display.showNumberHexEx(floor, first);
    		display.showNumberHexEx(floor, second);
    		display.showNumberHexEx(floor, third);
    		display.showNumberHexEx(floor, fourth);
    		floor++;

    	}
    }
}
