#include "ScsiClassHeader.h"

ScsiClass::ScsiClass()
{
	InConstructor = true;
}
//********************
ScsiClass::~ScsiClass()
{
	inDe_Constructor = true;
}
//*******************