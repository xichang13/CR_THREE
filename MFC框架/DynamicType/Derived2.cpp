// Derived2.cpp
#include "Derived2.h"

const CRuntimeClass Derived2::classDerived2 =
{ "Derived2", RUNTIME_CLASS(Base), Derived2::CreateObject };


Derived2::Derived2()
{
}

Derived2::~Derived2()
{
}

Base* Derived2::CreateObject()
{
	return new Derived2;
}
