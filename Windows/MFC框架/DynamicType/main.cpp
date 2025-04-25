// main.cpp
#include "Base.h"
#include "Derived1.h"
#include "Derived2.h"

void ShowClass(Base* obj) {
	printf("%s", obj->GetRunTimeClass()->m_szClassName);
}

int main() {
	Base* base = RUNTIME_CLASS(Derived2)->CreateObject();
	ShowClass(base);

	return 0;
}