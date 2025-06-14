#ifndef PLAYER_H
#define PLAYER_H

#include "Object.h"

class Player : public Object
{
public:
	Player(string _name, shared_ptr<VAO> modelVAO) : Object(_name, modelVAO) {};
	void update(float dTime) override;
};

#endif