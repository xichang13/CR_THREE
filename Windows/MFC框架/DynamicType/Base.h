// Base.h
#pragma once
#include <Windows.h>
#include <stdio.h>

class Base;

struct CRuntimeClass {
	const char* m_szClassName;
	CRuntimeClass* m_pBaseClass;
	Base* (*m_pfnCreateObject)();
	Base* CreateObject() {
		return m_pfnCreateObject();
	}
};

#define RUNTIME_CLASS(class_name) ((CRuntimeClass*)(&class_name::class##class_name))

class Base
{
public:
	Base();
	~Base();

public:
	static const CRuntimeClass classBase;
	virtual CRuntimeClass* GetRunTimeClass() const {
		return RUNTIME_CLASS(Base);
	}
};
