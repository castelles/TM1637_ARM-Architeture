/*
 * Callback.cpp
 *
 *  Created on: Aug 1, 2019
 *      Author: guilherme
 */

#include "Callback.h"

void Callback::attach(std::function<void(void)> f){
	this->f = f;
}

void Callback::detach(){
	this->f = NULL;
}

bool Callback::exec(){
	if ( this-> f){
		this->f();
		return true;
	}
	return false;
}
