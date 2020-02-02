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

// Mostra os zeros á esquerda caso haja
enum leadingZero : bool {
	show = true,
	hide = false
};

enum numLength : uint8_t {
	one = 1,
	two = 2,
	three = 3,
	four = 0
};

enum digitPosition : uint8_t {
	first = 0,
	second = 1,
	third = 2,
	fourth = 3
};

enum twoDots : uint8_t {
	showDots = 0b11100000,
	hideDots = 0
};

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
	TM1637Display(mkl_DevGPIO pinClk, mkl_DevGPIO pinDIO);
/*!
 * 	Define o nível do brilho do display
 *
 * 	A nova definição de brilho surte efeito quando um comando para a mudança dos dados exibidos
 *
 * 	@param brightness Um valor de 0 (menos brilho) a 7 (mais brilho)
 * 	@param on Liga ou desliga o display
 */
	void setBrightness(uint8_t _brightness, bool on = true);

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
 * 	@param pos A posição de onde se inicia a modificação (0 [first] - mais a esquerda,
 * 		   3 [fourth] - mais a direita)
 *
 */
	void setSegments(const uint8_t segments[], digitPosition);

  //! @overload
	void setSegments(const uint8_t segments[], digitPosition pos, numLength length);

/*!
 * Limpa/esvazia o display
 */
	void clear();

/*!
 * Testa todos os segmentos do displau
 *
 */
	void ligthSegments();

/*!
 * Define o modo dígito que será exibido
 *
 * @param _digitMode é o modo do dígito, caso seu valor seja hide, ele não exibe os zeros a esquerda
 * 		do número, caso seja show, ele exibe esses zeros
 */
	void setDigitMode(leadingZero _digitMode);
/*!
 * Define o número de algarismo acesos para exibir o número
 *
 * @param _length é esse número, indo do valor one (um dígito aceso para exxibir o algarismo) até four (quatro dígitos acesos)
 */
	void setLength(numLength _length);

/*!
 * Define se irá exibir os dois pontos do display
 *
 * @param on se for true exibe os dois pontos, se não apaga os dois pontos
 */
	void setDoubleDots(bool on);

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
	void showNumberDec(int num, digitPosition pos);

//! @overload
	void showNumberDec(int num, digitPosition pos, leadingZero leading_zero, numLength length);

/*!
 * Exibe um valor decimal com ponto
 *
 * Exibe o argumento dado. Os pontos entre os dígitos podem ser individualmente controlados.
 *
 * @param num O número a ser exibido
 * @param dots O ponto ativado. O argumento é uma máscara de bit, em que cada bit corresponde
 * 		a um ponto entre os dígitos.
 * 		Para exibir os dois pontos:
 * 		* 00:00 (0b11100000)
 * 	@param leading_zero Quando TRUE, os zeros à esquerda são exibidos. Caso contrário, os dígitos
 *	   	   não utilizados ficam apagados. NOTA: a exibição dos zeros a esquerda não é suportada com números
 *		   negativos.
 *  @param length O número de dígitos a serem exibidos. O usuário deve garantir que o número a ser exibido
 *		   cabe no número de dígitos definidos nesse argumento (por exemplo, se dois dígitos devem ser exibidos,
 *		   o número passado no argumento em @ref num deve ser entre 0 e 99)
 *	@param pos A posição do dígito mais significativo (0- a esquerda, 3- a direita)
 */
	void showNumberDecEx(int num, digitPosition pos);

//! @overload
	void showNumberDecEx(int num, digitPosition pos, twoDots dots,
			leadingZero leading_zero,numLength length);

/*!
 * Exibe um valor hexadecimal com ponto
 *
 * Exibe o argumento dado. Os pontos entre os dígitos podem ser individualmente controlados.
 *
 * @param num O número a ser exibido
 * @param dots O ponto ativado. O argumento é uma máscara de bit, em que cada bit corresponde
 * 		a um ponto entre os dígitos.
 * 		Para exibir os dois pontos:
 * 		* 00:00 (0b11100000)
 * @param leading_zero Quando TRUE, os zeros à esquerda são exibidos. Caso contrário, os dígitos
 *	   	   não utilizados ficam apagados. NOTA: a exibição dos zeros a esquerda não é suportada com números
 *		   negativos.
 *  @param length O número de dígitos a serem exibidos. O usuário deve garantir que o número a ser exibido
 *		   cabe no número de dígitos definidos nesse argumento (por exemplo, se dois dígitos devem ser exibidos,
 *		   o número passado no argumento em @ref num deve ser entre 0 e 99)
 *	@param pos A posição do dígito mais significativo (0- a esquerda, 3- a direita)
 */
	void showNumberHexEx(uint16_t num, digitPosition pos);

//! @overload
	void showNumberHexEx(uint16_t num, digitPosition pos, twoDots dots,
			leadingZero leading_zero, numLength length);

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

protected:

/*!
 * Delay
 */
	void fastDelay();

	void start();

	void stop();

	bool writeByte(uint8_t b);

	void writeDots(uint8_t dots, uint8_t* digits);
   
	void showNumberBaseEx(int8_t base, uint16_t num, twoDots dots, leadingZero leading_zero,
           numLength length, digitPosition pos);

private:
	mkl_DevGPIO m_pinClk;
	mkl_DevGPIO m_pinDIO;
	uint8_t brightness;

/*!
 * Atributos enumerados
 */
	numLength digitLength = one;
	leadingZero digitMode = hide;
	digitPosition position;
	twoDots dotsMask = hideDots;
};

#endif // __TM1637DISPLAY__
