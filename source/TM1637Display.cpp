/*!
 * @copyright   � 2020 Caio Arthur Sales Telles <csalestelles@gmail.com>
 *
 * @brief       Exemplo de uso do periférico TM1637 na MKL25Z.
 *
 * @file        mkl_TM1637.cpp
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


extern "C" {
  #include <stdlib.h>
  #include <string.h>
  #include <inttypes.h>
}

#include <unistd.h>
#include <TM1637Display.h>

#define TM1637_I2C_COMM1    0x40
#define TM1637_I2C_COMM2    0xC0
#define TM1637_I2C_COMM3    0x80

//
//      A
//     ---
//  F |   | B
//     -G-
//  E |   | C
//     ---
//      D
const uint8_t digitToSegment[] = {
 // XGFEDCBA
  0b00111111,    // 0
  0b00000110,    // 1
  0b01011011,    // 2
  0b01001111,    // 3
  0b01100110,    // 4
  0b01101101,    // 5
  0b01111101,    // 6
  0b00000111,    // 7
  0b01111111,    // 8
  0b01101111,    // 9
  0b01110111,    // A
  0b01111100,    // b
  0b00111001,    // C
  0b01011110,    // d
  0b01111001,    // E
  0b01110001     // F
  };

static const uint8_t minusSegments = 0b01000000;

TM1637Display::TM1637Display(mkl_DevGPIO pinClk, mkl_DevGPIO pinDIO)
{
	// Seta os pins a serem utilizados pelo periférico
	m_pinClk = pinClk;
	m_pinDIO = pinDIO;

    m_pinClk.setPortMode(gpio_input);
    m_pinDIO.setPortMode(gpio_input);
	m_pinClk.writeBit(0);
	m_pinDIO.writeBit(0);

	dotsMask = hideDots;
	digitLength = one;
	digitMode = hide;

}

void TM1637Display::setBrightness(uint8_t _brightness, bool on)
{
	brightness = (brightness & 0x7) | (on? 0x08 : 0x00);
}

void TM1637Display::setSegments(const uint8_t segments[], digitPosition pos)
{
	start();
	writeByte(TM1637_I2C_COMM1);
	stop();

	start();
	writeByte(TM1637_I2C_COMM2 + (pos & 0x03));

	for (uint8_t k=0; k < digitLength; k++)
		writeByte(segments[k]);

	stop();

	start();
	writeByte(TM1637_I2C_COMM3 + (brightness & 0x0f));
	stop();
}

void TM1637Display::setSegments(const uint8_t segments[], digitPosition pos, numLength length)
{

	start();
	writeByte(TM1637_I2C_COMM1);
	stop();

	start();
	writeByte(TM1637_I2C_COMM2 + (pos & 0x03));

	for (uint8_t k=0; k < length; k++)
	  writeByte(segments[k]);

	stop();

	start();
	writeByte(TM1637_I2C_COMM3 + (brightness & 0x0f));
	stop();
}

void TM1637Display::clear()
{
    uint8_t data[] = { 0,0};
	setSegments(data,first,two);
	setSegments(data,third,two);
}

void TM1637Display::ligthSegments()
{
	showNumberDecEx(8,fourth,showDots,hide,one);
	showNumberDecEx(8,third,showDots,hide,one);
	showNumberDecEx(8,second,showDots,hide,one);
	showNumberDecEx(8,first,showDots,hide,one);
}

void TM1637Display::setDigitMode(leadingZero _digitMode) {
	digitMode = _digitMode;
}

void TM1637Display::setLength(numLength _length) {
	digitLength = _length;
}

void TM1637Display::setDoubleDots(bool on)
{
	dotsMask = on ? showDots : hideDots;
}

void TM1637Display::showNumberDec(int num, digitPosition pos)
{
	showNumberDecEx(num, pos, hideDots, digitMode, digitLength);
}

void TM1637Display::showNumberDec(int num, digitPosition pos,
		leadingZero leading_zero, numLength length)
{
	showNumberDecEx(num, pos, dotsMask, leading_zero, length);
}

void TM1637Display::showNumberDecEx(int num, digitPosition pos)
{
	showNumberBaseEx(num < 0? -10 : 10, num < 0? -num : num, dotsMask, digitMode, digitLength, pos);
}

void TM1637Display::showNumberDecEx(int num, digitPosition pos, twoDots dots, leadingZero leading_zero,
                                    numLength length)
{
	showNumberBaseEx(num < 0? -10 : 10, num < 0? -num : num, dots, leading_zero, length, pos);
}

void TM1637Display::showNumberHexEx(uint16_t num, digitPosition pos)
{
	showNumberBaseEx(16, num, dotsMask, digitMode, digitLength, pos);
}

void TM1637Display::showNumberHexEx(uint16_t num, digitPosition pos, twoDots dots, leadingZero leading_zero,
                                    numLength length)
{
	showNumberBaseEx(16, num, dots, leading_zero, length, pos);
}

void TM1637Display::showNumberBaseEx(int8_t base, uint16_t num, twoDots dots, leadingZero leading_zero,
                                    numLength length, digitPosition pos)
{
    bool negative = false;
	if (base < 0) {
	    base = -base;
		negative = true;
	}


    uint8_t digits[4];

	if (num == 0 && !leading_zero) {
		for(uint8_t i = 0; i < (length-1); i++)
			digits[i] = 0;
		digits[length-1] = encodeDigit(0);
	}
	else {
		
		for(int i = length-1; i >= 0; --i)
		{
		    uint8_t digit = num % base;
			
			if (digit == 0 && num == 0 && leading_zero == false)

				digits[i] = 0;
			else
			    digits[i] = encodeDigit(digit);
				
			if (digit == 0 && num == 0 && negative) {
			    digits[i] = minusSegments;
				negative = false;
			}

			num /= base;
		}

		if(dots != 0)
		{
			writeDots(dots, digits);
		}
    }
    setSegments(digits, pos, length);
}

void TM1637Display::fastDelay() {
	unsigned int i;
	int j;

	for (i = 0 ; i < 2; i++) {
		for (j = 0; j < 1000; j++) {}
	}
}

void TM1637Display::start()
{
	m_pinDIO.setPortMode(gpio_output);
	fastDelay();
}

void TM1637Display::stop()
{
	m_pinDIO.setPortMode(gpio_output);
	fastDelay();
	m_pinClk.setPortMode(gpio_input);
	fastDelay();
	m_pinDIO.setPortMode(gpio_input);
	fastDelay();
}

bool TM1637Display::writeByte(uint8_t b)
{
	uint8_t data = b;


	for(uint8_t i = 0; i < 8; i++) {

		m_pinClk.setPortMode(gpio_output);
		fastDelay();

		if (data & 0x01)
			m_pinDIO.setPortMode(gpio_input);
		else
			m_pinDIO.setPortMode(gpio_output);

		fastDelay();


		m_pinClk.setPortMode(gpio_input);
		fastDelay();
		data = data >> 1;
	}

	m_pinClk.setPortMode(gpio_output);
	m_pinDIO.setPortMode(gpio_input);
	fastDelay();


	m_pinClk.setPortMode(gpio_input);
	fastDelay();
	uint8_t ack = m_pinDIO.readBit();
	if (ack == 0) {
		m_pinDIO.setPortMode(gpio_output);
	}

	fastDelay();
	m_pinClk.setPortMode(gpio_output);
	fastDelay();

	return ack;
}

void TM1637Display::writeDots(uint8_t dots, uint8_t* digits)
{
    for(int i = 0; i < 4; ++i)
    {
        digits[i] |= (dots & 0x80);
        dots <<= 1;
    }
}

uint8_t TM1637Display::encodeDigit(uint8_t digit)
{
	return digitToSegment[digit & 0x0f];
}
