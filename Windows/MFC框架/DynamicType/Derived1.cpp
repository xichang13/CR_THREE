// Derived1.cpp
#include "Derived1.h"

const CRuntimeClass Derived1::classDerived1 = 
{ "Derived1", RUNTIME_CLASS(Base), Derived1::CreateObject};


Derived1::Derived1()
{
}

Derived1::~Derived1()
{
}

Base* Derived1::CreateObject()
{
	return new Derived1;
}
