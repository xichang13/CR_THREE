- [����](#����)
  - [1. �ҵ�������̺���](#1-�ҵ�������̺���)
  - [2. �ҵ�`WM_CLOSE`��Ϣ������](#2-�ҵ�wm_close��Ϣ������)
  - [3. �򲹶�](#3-�򲹶�)
- [crack](#crack)

# ����

ɨ��Ӧ�ó����У��򲹶����ó����ڹر�ʱ�������Ի�����ʾ�Ƿ�رա�

## 1. �ҵ�������̺���

�ڿ����У�����`registerclass`����ע�ᴰ����ʱ��Ҫ�õ����̺���������������ҵ����̺�����ַ��

* ��ʽһ
  * ͨ��`����`����`registerclassa`���ҵ�`kernel32.dll`�е�`registerclassa
  ����
  * `�Ҽ�`->`����`->`��ǰģ��`->`��ģ�����`������`registerclassa`��
  `winmine.exe`�е�`registerclassa`������
  * ͨ���鿴`registerclassa`�����Ĳ����ҵ����̺�����ַ��`01001bc9`
* ��ʽ��
  * ͨ��`���`���ҵ����ڹ��̺���
* ��ʽ��
  * ͨ����Ϣ��������˵�˵���Ϣ�ĵ����Ի���ͨ��`dialogbox`�ҵ����̺���

## 2. �ҵ�`WM_CLOSE`��Ϣ������

�ҵ����̺���֮���ҵ�`WM_CLOSE`��Ϣ�����ﴦ��`WM_CLOSE == 0x0010`���ҵ�`cmp 10`�ĵط�������ɨ��û�иñȽϣ����Կ���֪��`WM_CLOSE`��Ϣ����`defwindowprocw`�����д���

## 3. �򲹶�

�ڲ��ı����ԭ�й��ܵ�ǰ���£����ǽ�`call defwindowprocw`�޸�Ϊ���������Լ��ĺ���`myproc`����`myproc`�д���`defwindowprocw`��`WM_CLOSE`��Ϣ��

1. �ҵ�һ������ڴ�

``` asm
01004A62                         | push ebp                                  |
01004A63                         | mov ebp,esp                               |
01004A65                         | push ebx                                  |
01004A66                         | push ecx                                  |
01004A67                         | push edx                                  |
01004A68                         | push edx                                  |
01004A69                         | push esi                                  | esi:"p{Z"
01004A6A                         | push edi                                  |
01004A6B                         | mov ebx,dword ptr ss:[ebp+C]              | [ebp+0C]:BaseThreadInitThunk
01004A6E                         | cmp ebx,10                                |
01004A71                         | jne winmine.1004A8C                       |
01004A73                         | push 1                                    |
01004A75                         | push winmine.1004AD7                      | 1004AD7:"are you ok?"
01004A7A                         | push 0                                    |
01004A7C                         | push 0                                    |
01004A7E                         | nop                                       |
01004A7F                         | nop                                       |
01004A80                         | nop                                       |
01004A81                         | nop                                       |
01004A82                         | call <user32.MessageBoxA>                 |
01004A87                         | cmp eax,2                                 |
01004A8A                         | je winmine.1004AA9                        |
01004A8C                         | push dword ptr ss:[ebp+14]                | [ebp+14]:RtlInitializeExceptionChain+6B
01004A8F                         | push dword ptr ss:[ebp+10]                |
01004A92                         | push dword ptr ss:[ebp+C]                 | [ebp+0C]:BaseThreadInitThunk
01004A95                         | push dword ptr ss:[ebp+8]                 |
01004A98                         | call dword ptr ds:[<DefWindowProcW>]      |
01004A9E                         | pop edi                                   |
01004A9F                         | pop esi                                   | esi:"p{Z"
01004AA0                         | pop edx                                   |
01004AA1                         | pop ecx                                   |
01004AA2                         | pop ebx                                   |
01004AA3                         | mov esp,ebp                               |
01004AA5                         | pop ebp                                   |
01004AA6                         | ret 10                                    |
01004AA9                         | jmp winmine.1004A9E                       |
```

2. ��ԭ�е�`call defwindowprocw`�޸ĵ��������Լ��ĺ���

``` asm
010021B5                         | call winmine.1004A62                      |
```

# crack

�ҳ� `01.exe` ��ȷ�����кš�

1. ͨ���ַ����������ҵ����ø��ַ����ĺ�����ַ
 
�� `OllyICE` ���Ҽ�ѡ�� `Ultra String Reference` -> `Find ASCII` ���ҵ��ַ��� `incorrect try again!!` ��˫��ת�����ַ����ĵ�ַ��

2. �ڸõ�ַ�������ҵ������Լ�������ַ��������߱Ƚ��ַ����ĺ�����

![�ַ����Ƚ�](pic/������crack/image.png)

���������������ַ����Ƚϵ��ַ������ҳ����кš� `<BrD-SoB>`