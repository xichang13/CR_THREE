- [teb��peb](#teb��peb)
  - [teb��Thread Environment Block�� �̻߳�����](#tebthread-environment-block-�̻߳�����)
  - [peb��Process Environment Block�� ���̻�����](#pebprocess-environment-block-���̻�����)
- [Windbg����](#windbg����)

# teb��peb

## teb��Thread Environment Block�� �̻߳�����

**TEB**�а������̵߳Ķ�ջָ�롢TLS���̱߳��ش洢��ָ�롢�쳣��������ָ�롢�û�ģʽ��ҳ��ָ�����Ϣ��**TEB**�е�`FS`�μĴ���ͨ��������Ϊ`fs:[0]`��ָ��ǰ�̵߳�TEB�ṹ�塣

![_teb](pic/teb��peb/image.png)

ͨ��TEB�ṹ�����ǿ��Կ���`+0x18`�������λ�ַ��Ҳ����`fs:[0]`��ָ��TEB�ṹ�塣`+0x30`��PEB���̻������ַ��ָ��PEB�ṹ�塣

## peb��Process Environment Block�� ���̻�����

**PEB**�а����˽��̵Ĵ��롢���ݶ�ָ�롢���̵Ļ�����������������������Ϣ�Լ����ص�dll��Ϣ�ȡ�**PEB**�ṹ���е�`FS�μĴ���`ͨ��������Ϊ`0x30`��ָ��ǰ���̵�**PEB**�ṹ�塣

![_peb](pic/teb��peb/image-2.png)

��PEB�ṹ���У�`+0x0c`ָ��_PEB_LDR_DATA����̬���ӿ����ݣ��ṹ���_LDR_DATA_TABLE_ENTRY����̬���ӿ����ݱ����`+0x08`ָ����̻�ַ��`+0x10`ָ��_RTL_USER_PROCESS_PARAMETERS�����̲������ṹ�塣

![_LDR_DATA_TABLE_ENTRY](pic/teb��peb/image-3.png)

# Windbg����

* !peb �鿴���̻�����
* !teb �鿴�̻߳�����
* g $exentry ��ת��������ڵ�
* dt -rv _TEB �鿴teb�ṹ�������Լ��ӽṹ������
* dt !_TEB �鿴_teb�ṹ������
* dt !_PEB �鿴_peb�ṹ������
* dt _peb [address] ʹ��peb�ṹ��鿴�ڴ��ַ������
* dd [address] �鿴�ڴ��ַ������
* du [address] �鿴�ڴ��ַ�����ݣ��ַ�����ʽ��
* r $tib �鿴�̻߳������ַ
* r $peb �鿴���̻������ַ
* x [dllname]!functionname ����dll�еĺ�����ַ - dll���ͺ�����������ʹ��ͨ���