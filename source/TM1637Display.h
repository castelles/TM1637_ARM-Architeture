/*!
 * @copyright   � 2020 Caio Arthur Sales Telles <csalestelles@gmail.com>
 *
 * @brief       Exemplo de uso do periférico TM1637 na MKL25Z.
 *
 * @file        mkl_TM1637.h
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

#ifndef __TM1637DISPLAY__
#define __TM1637DISPLAY__

#include <inttypes.h>
#include "mkl_DevGPIO.h"

#define SEG_A   0b00000001
#define SEG_B   0b00000010
#define SEG_C   0b00000100
#define SEG_D   0b00001000
#define SEG_E   0b00010000
#define SEG_F   0b00100000
#define SEG_G   0b01000000
#define SEG_DP  0b10000000

#define DEFAULT_BIT_DELAY  2

/*!
 *  @class    mkl_TM1637.
 *
 *  @brief    A classe implementa o serviço de exibição
 *  	 	  do display TM1637.
 *
 *  @details  Esta classe implementa o serviço de exibiçãodo display utilizando o
 *            periférico correspondente.
 *
 *  @section  EXAMPLES USAGE
 *
 *            Uso dos métodos para exibição de dados no display.
 *
 *              display.showNumberDec(24, false, 1, 0);
 *              display.setSegments(segments[], 2, 2);
 */

class TM1637Display {

public:
/*!
 * 	Inicializa um objeto TM1637Display, defininfo os pinos
 * 	de clock e dados
 *
 *  @param pinClk - O valor do pino conectado ao pino de clock do periférico
 *  @param pinDIO - O valor do pino conectado ao pino DIO do módulo
 *  @param bitDelay - O delay em milissegundos entre a transição de bit no buffer conectado ao display
 *
 */
  TM1637Display(mkl_DevGPIO pinClk, mkl_DevGPIO pinDIO, unsigned int bitDelay = DEFAULT_BIT_DELAY);
/*!
 * 	Define o nível do brilho do display
 *
 * 	A nova definição de brilho surte efeito quando um comando para a mudança dos dados exibidos
 *
 * 	@param brightness Um valor de 0 (menos brilho) a 7 (mais brilho)
 * 	@param on Liga ou desliga o display
 */
  void setBrightness(uint8_t brightness, bool on = true);

/*!
 * 	Exibe dados selecionados por um array de segmentos no periférico
 *
 * 	Essa função recebe valores segmentados ´crus´ como entrada e os exibe. O dado segmentado
 * 	é dado por um array de bytes, cada byte correspondendo a um único dígito. Portanto, para
 * 	cada byte, bit 0 é o segmento A, bit 1 é o segmento B, etc.
 * 	O método pode definir todo o display bem como qualquer parte desejada do mesmo. O primeiro
 * 	dígito é dado pelo argumento @ref pos, sendo 0 a posição do dígito definida mais a esquerda.
 * 	O argumento @ref length é o número de dígitos a serem modificados. Outros dígitos não são
 * 	afetados
 *
 * 	@param segments Um array de tamanho @ref length contendo os valores dos segmentos
 * 	@param length O número de dígitos a serem modificados
 * 	@param pos A posição de onde se inicia a modificação (0 - mais a esquerda,
 * 		   3 - mais a direita)
 *
 */
  void setSegments(const uint8_t segments[], uint8_t length = 4, uint8_t pos = 0);

 /*!
  * Limpa/esvazia o display
  */
  void clear();

/*!
 *	Exibe um valor decimal
 *
 *	Exibe o argumento dado no display
 *
 *	@param num O número a sem mostrado
 *	@param leading_zero Quando TRUE, os zeros à esquerda são exibidos. Caso contrário, os dígitos
 *	   	   não utilizados ficam apagados. NOTA: a exibição dos zeros a esquerda não é suportada com números
 *		   negativos.
 *	@param length O número de dígitos a serem exibidos. O usuário deve garantir que o número a ser exibido
 *		   cabe no número de dígitos definidos nesse argumento (por exemplo, se dois dígitos devem ser exibidos,
 *		   o número passado no argumento em @ref num deve ser entre 0 e 99)
 *	@param pos A posição do dígito mais significativo (0- a esquerda, 3- a direita)
 */
  void showNumberDec(int num, bool leading_zero = false, uint8_t length = 4, uint8_t pos = 0);

/*!
 * Exibe um valor decimal com ponto
 *
 * Exibe o argumento dado. Os pontos entre os dígitos podem ser individualmente controlados.
 *
 * @param num O número a ser exibido
 * @param dots O ponto ativado. O argumento é uma máscara de bit, em que cada bit corresponde
 * 		a um ponto entre os dígitos.
 * 		Para exibit os pontos entre cada dígito:
 * 		* 0.000 (0b10000000)
 * 		* 00.00 (0b01000000)
 * 		* 000.0 (0b00100000)
 * 		* 0.0.0.0 (0b11100000)
 * 		Para exibir os dois pontos:
 * 		* 00:00 (0b01000000)
 * 	@param leading_zero Quando TRUE, os zeros à esquerda são exibidos. Caso contrário, os dígitos
 *	   	   não utilizados ficam apagados. NOTA: a exibição dos zeros a esquerda não é suportada com números
 *		   negativos.
 *  @param length O número de dígitos a serem exibidos. O usuário deve garantir que o número a ser exibido
 *		   cabe no número de dígitos definidos nesse argumento (por exemplo, se dois dígitos devem ser exibidos,
 *		   o número passado no argumento em @ref num deve ser entre 0 e 99)
 *	@param pos A posição do dígito mais significativo (0- a esquerda, 3- a direita)
 */

  void showNumberDecEx(int num, uint8_t dots = 0, bool leading_zero = false, uint8_t length = 4, uint8_t pos = 0);

 /*!
  * Exibe um valor hexadecimal com ponto
  *
  * Exibe o argumento dado. Os pontos entre os dígitos podem ser individualmente controlados.
  *
  * @param num O número a ser exibido
  * @param dots O ponto ativado. O argumento é uma máscara de bit, em que cada bit corresponde
  * 		a um ponto entre os dígitos.
  * 		Para exibit os pontos entre cada dígito:
  * 		* 0.000 (0b10000000)
  * 		* 00.00 (0b01000000)
  * 		* 000.0 (0b00100000)
  * 		* 0.0.0.0 (0b11100000)
  * 		Para exibir os dois pontos:
  * 		* 00:00 (0b01000000)
  * @param leading_zero Quando TRUE, os zeros à esquerda são exibidos. Caso contrário, os dígitos
  *	   	   não utilizados ficam apagados. NOTA: a exibição dos zeros a esquerda não é suportada com números
  *		   negativos.
  *  @param length O número de dígitos a serem exibidos. O usuário deve garantir que o número a ser exibido
  *		   cabe no número de dígitos definidos nesse argumento (por exemplo, se dois dígitos devem ser exibidos,
  *		   o número passado no argumento em @ref num deve ser entre 0 e 99)
  *	@param pos A posição do dígito mais significativo (0- a esquerda, 3- a direita)
  */
  void showNumberHexEx(uint16_t num, uint8_t dots = 0, bool leading_zero = false, uint8_t length = 4, uint8_t pos = 0);

 /*!
  * Traduz um único dígito no seu respectivo código de 7 segmentos
  *
  * O método aceita um valor entre 0 e 15 e converte no código necessário para exibir
  * o valor no display de 7 segmentos.
  * Números entre 10 e 15 são convertidos para dígitos hexadecimais (A-F)
  *
  * @param digit Um valor entre 0 e 15
  * @return Um código representando a imagem do dígito no display de 7 segmentos
  */
  uint8_t encodeDigit(uint8_t digit);

 /*!
  * Delay
  */
  void delayMs(unsigned int time);

protected:
   void bitDelay();

   void start();

   void stop();

   bool writeByte(uint8_t b);

   void showDots(uint8_t dots, uint8_t* digits);
   
   void showNumberBaseEx(int8_t base, uint16_t num, uint8_t dots = 0, bool leading_zero = false, uint8_t length = 4, uint8_t pos = 0);


private:
	mkl_DevGPIO m_pinClk;
	mkl_DevGPIO m_pinDIO;
	uint8_t m_brightness;
	unsigned int m_bitDelay;
};

#endif // __TM1637DISPLAY__
