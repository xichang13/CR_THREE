#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#define _CRT_SECURE_NO_WARNINGS

// ģ��������
int divide(int a, int b) {
    if (b == 0) {
        throw "Divide by zero error";  // �׳��ַ����쳣
    }
    return a / b;
}

// ģ������Խ��
int accessArray(int* array, int index, int size) {
    if (index < 0 || index >= size) {
        throw - 1;  // �׳������쳣
    }
    return array[index];
}

// ���� SEH �쳣�ĺ���
void triggerSEH() {
    int* ptr = nullptr;
    *ptr = 42;  // ��������Υ���쳣
}

int main() {
    srand(time(0));
    int array[5] = { 1, 2, 3, 4, 5 };
    array[1] = rand() % 2;

    try {
        printf("Inside outer try block\n");

        try {
            printf("Inside first nested try block\n");
            int result = divide(10, 0);  // �������������
            printf("Result of division: %d\n", result);
        }
        catch (const char* error) {
            printf("Caught exception in first nested catch: %s\n", error);
            try {
                printf("Inside second nested try block\n");
                int value = accessArray(array, 10, 5);  // ��������Խ�����
                printf("Value at index 10: %d\n", value);
            }
            catch (int errorCode) {
                printf("Caught exception in second nested catch: Error code %d\n", errorCode);

                try {
                    printf("Inside third nested try block\n");
                    throw 42;  // �׳������쳣
                }
                catch (int value) {
                    printf("Caught exception in third nested catch: Value %d\n", value);
                }
            }
        }
    }
    catch (const char* error) {
        printf("Caught exception in outer catch: %s\n", error);
    }
    catch (int errorCode) {
        printf("Caught exception in outer catch: Error code %d\n", errorCode);
    }

    try {
        printf("Inside second outer try block\n");

        try {
            printf("Inside fourth nested try block\n");
            throw "Another error";  // �׳��ַ����쳣
        }
        catch (const char* error) {
            printf("Caught exception in fourth nested catch: %s\n", error);
        }

        try {
            printf("Inside fifth nested try block\n");
            throw 100;  // �׳������쳣
        }
        catch (int value) {
            printf("Caught exception in fifth nested catch: Value %d\n", value);
        }

        try {
            printf("Inside sixth nested try block\n");
            throw "Yet another error";  // �׳��ַ����쳣
        }
        catch (const char* error) {
            printf("Caught exception in sixth nested catch: %s\n", error);
        }
    }
    catch (...) {
        printf("Caught unexpected exception in second outer catch\n");
    }

    printf("Program continues after exception handling\n");
    return 0;
}