/*
 * Callback.h
 *
 *  Created on: Aug 1, 2019
 *      Author: Guilherme Souza da Silva
 *      Email: gsds@icomp.ufam.edu.br
 */

#pragma once

#include <functional>

class Callback{

protected:
	std::function<void(void)> f;
public:
	void attach(std::function<void(void)> f);
	void detach();
	bool exec();
};
