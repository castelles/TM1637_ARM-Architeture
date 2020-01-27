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

TM1637Display::TM1637Display(mkl_DevGPIO pinClk, mkl_DevGPIO pinDIO, unsigned int bitDelay)
{
	// Seta os pins a serem utilizados pelo periférico
	m_pinClk = pinClk;
	m_pinDIO = pinDIO;
	m_bitDelay = bitDelay;

    m_pinClk.setPortMode(gpio_input);
    m_pinDIO.setPortMode(gpio_input);
	m_pinClk.writeBit(0);
	m_pinDIO.writeBit(0);
}

void TM1637Display::setBrightness(uint8_t brightness, bool on)
{
	m_brightness = (brightness & 0x7) | (on? 0x08 : 0x00);
}

void TM1637Display::setSegments(const uint8_t segments[], uint8_t length, uint8_t pos)
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
	writeByte(TM1637_I2C_COMM3 + (m_brightness & 0x0f));
	stop();
}

void TM1637Display::clear()
{
    uint8_t data[] = { 0, 0, 0, 0 };
	setSegments(data);
}

void TM1637Display::showNumberDec(int num, bool leading_zero, uint8_t length, uint8_t pos)
{
  showNumberDecEx(num, 0, leading_zero, length, pos);
}

void TM1637Display::showNumberDecEx(int num, uint8_t dots, bool leading_zero,
                                    uint8_t length, uint8_t pos)
{
  showNumberBaseEx(num < 0? -10 : 10, num < 0? -num : num, dots, leading_zero, length, pos);
}

void TM1637Display::showNumberHexEx(uint16_t num, uint8_t dots, bool leading_zero,
                                    uint8_t length, uint8_t pos)
{
  showNumberBaseEx(16, num, dots, leading_zero, length, pos);
}

void TM1637Display::showNumberBaseEx(int8_t base, uint16_t num, uint8_t dots, bool leading_zero,
                                    uint8_t length, uint8_t pos)
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
			showDots(dots, digits);
		}
    }
    setSegments(digits, length, pos);
}

void TM1637Display::delayMs(unsigned int time) {
		unsigned int i;
	int j;

	for (i = 0 ; i < time; i++) {
		for (j = 0; j < 5000; j++) {}
	}
}

void TM1637Display::bitDelay()
{
	delayMs(m_bitDelay);
}

void TM1637Display::start()
{
	m_pinDIO.setPortMode(gpio_output);
	bitDelay();
}

void TM1637Display::stop()
{
	m_pinDIO.setPortMode(gpio_output);
	bitDelay();
	m_pinClk.setPortMode(gpio_input);
	bitDelay();
	m_pinDIO.setPortMode(gpio_input);
	bitDelay();
}

bool TM1637Display::writeByte(uint8_t b)
{
	uint8_t data = b;


	for(uint8_t i = 0; i < 8; i++) {

		m_pinClk.setPortMode(gpio_output);
		bitDelay();

		if (data & 0x01)
			m_pinDIO.setPortMode(gpio_input);
		else
			m_pinDIO.setPortMode(gpio_output);

		bitDelay();


		m_pinClk.setPortMode(gpio_input);
		bitDelay();
		data = data >> 1;
	}

	m_pinClk.setPortMode(gpio_output);
	m_pinDIO.setPortMode(gpio_input);
	bitDelay();


	m_pinClk.setPortMode(gpio_input);
	bitDelay();
	uint8_t ack = m_pinDIO.readBit();
	if (ack == 0) {
		m_pinDIO.setPortMode(gpio_output);
	}

	bitDelay();
	m_pinClk.setPortMode(gpio_output);
	bitDelay();

	return ack;
}

void TM1637Display::showDots(uint8_t dots, uint8_t* digits)
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
