// Derived1.h
#pragma once
#include "Base.h"
class Derived1 :
    public Base
{
public:
    Derived1();
    ~Derived1();

public:
	static const CRuntimeClass classDerived1;
	virtual CRuntimeClass* GetRunTimeClass() const {
		return RUNTIME_CLASS(Derived1);
	}
	static Base* CreateObject();
};

