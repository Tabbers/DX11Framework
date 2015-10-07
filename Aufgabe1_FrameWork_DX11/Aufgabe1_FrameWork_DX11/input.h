#ifndef _INPUT_H_
#define _INPUT_H_

#include <string.h>

class Input
{
public:
	Input();
	Input(const Input&);
	~Input();
	
	void Init();

	void KeyDown(unsigned int);
	void KeyUp(unsigned int);

	bool Keystate(unsigned int);
	bool KeystateOld(unsigned int i);
	void Swap();

private: 
	// enable keychek for every frame through saving key state values
	bool keystate[256];
	bool keystateOld[256];
};
#endif

