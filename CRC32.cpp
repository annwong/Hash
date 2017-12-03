#include <stdio.h>

unsigned long int crc32_table[256];
unsigned long rCRC32 = 0xFFFFFFFF; //�Ĵ�����Ԥ�ó�ʼֵ
const unsigned long int ulPolynomial = 0x04C11DB7;

unsigned long int Reflect(unsigned long int ref, char ch) 
{
	unsigned long int value(0);

	// ���� bit0 �� bit7 , bit1 �� bit6 ����, ����
	for (int i = 1; i < (ch + 1); i++)
	{
		if (ref & 1)
			value |= 1 << (ch - i);

		ref >>= 1;
	}

	return value;
}

// Init()
void init_crc32_table() 
{
	unsigned long int crc,temp;

	// 256 ��ֵ
	for (int i = 0; i <= 0xFF; i++) // ����CRC32�������ѯ��
	{
		temp = Reflect(i, 8);
		crc32_table[i] = temp << 24;

		for (int j = 0; j < 8; j++)
		{
			unsigned long int t1,t2;
			unsigned long int flag = crc32_table[i] & 0x80000000;

			t1 = (crc32_table[i] << 1);

			if (flag == 0)
				t2 = 0;
			else
				t2 = ulPolynomial;

			crc32_table[i] = t1^t2;
		}

		crc = crc32_table[i];
		crc32_table[i] = Reflect(crc32_table[i], 32);
	}

}

// Update()
void GenerateCRC32(char* DataBuf, unsigned long len) 
{
	//����CRC32ֵ

	char* p = DataBuf;

	for (unsigned long i = 0;i < len;i++)
	{
		rCRC32 = crc32_table[( rCRC32^(*(p+i)) ) & 0xff] ^ (rCRC32 >> 8); //����
	}
}

// Final()
unsigned long GetCRC32()
{
	unsigned long CRC32 = ~rCRC32; //ȡ������WINRAR�Աȣ�CRC32ֵ��ȷ����
	//��ʱ�� CRC32�е�ֵ����CRC
	return CRC32;
}

void main()
{
	unsigned int len = 0;
	char buffer[1024];

	FILE* pFile = NULL;

	pFile = fopen("E:/web.rar", "rb");

	init_crc32_table();

	while ( len = fread(buffer, 1, 1024, pFile) )
	{
		GenerateCRC32(buffer, len);
	}

	unsigned long crc32 = GetCRC32();

	fclose(pFile);

	for (int i = 0; i <= 0xFF; i++)
	{
		printf("0x%x ", crc32_table[i]);
	}

	printf("\n\n0x%x\n\n", crc32);
}