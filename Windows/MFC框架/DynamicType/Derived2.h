// Derived2.h
#pragma once
#include "Base.h"
class Derived2 :
    public Base
{
public:
	Derived2();
	~Derived2();

public:
	static const CRuntimeClass classDerived2;
	virtual CRuntimeClass* GetRunTimeClass() const {
		return RUNTIME_CLASS(Derived2);
	}
	static Base* CreateObject();
};

