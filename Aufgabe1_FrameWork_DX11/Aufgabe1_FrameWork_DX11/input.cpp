#include "input.h"



Input::Input()
{
}

Input::Input(const Input &other)
{
}


Input::~Input()
{
}

void Input::Init()
{
	for (int i = 0; i < 256; i++)
	{
		keystate[i] = false;
		keystateOld[i] = false;
	}

	return;
}

void Input::KeyDown(unsigned int i)
{
	keystate[i] = true;
	return;
}

void Input::KeyUp(unsigned int i)
{
	keystate[i] = false;
	return;
}

bool Input::Keystate(unsigned int i)
{
	return keystate[i];
}
bool Input::KeystateOld(unsigned int i)
{
	return keystateOld[i];
}
void Input::Swap()
{
	memcpy(keystateOld, keystate, 256 * sizeof(bool));
}
