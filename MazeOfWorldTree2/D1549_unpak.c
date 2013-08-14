#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_FILE_COUNT 99
int main(int argc, char *argv[])
{
	FILE *pak,	//Դ�ļ�
		 *out,	//����ĸ������Ķ����ļ�
		 *txt;	//Ŀ���ļ�

	unsigned long num,				//�����ļ����ĸ���
				  base[100] = {0},	//�����ļ���Դ�ļ��еĻ�ַ
				  flag[100] = {0},	//�Ƿ�LZѹ���ı�־λ
				  len,				//�����ļ����ĳ���
				  i,
				  lzCount = 0;		//LZѹ�����ĸ���
	char name[40],		//���ļ����ļ���
		 idx[40],
		 *buffer;		

	unsigned char *dat;
	

	//û������Դ�ļ���
	if (argc < 2)
	{
		printf ("No source file name input\n");
		return 1;
	}

	//��Դ�ļ�

	strcpy (name, argv[1]);
	if (strstr (name, ".bin") == NULL)
	{
		strcat (name, ".bin");
	}	

	pak = fopen (name, "rb");

	if (pak == NULL)
	{
		printf ("file \"%s\" did not exist.\n", name);
		return 2;
	}



	//Դ�ļ��ĵ�һ���ֽ�Ϊ�ڲ��ļ��ĸ���
	fread (&num, 4, 1, pak);//�ļ�����
	printf ("%d documents founded.\n", num);
	
	//�����ȡ�����ļ���Ŀ����MAX_FILE_COUNT�������ʾ�����˳�����
	if(num > MAX_FILE_COUNT)
		{
			printf ("error num!\n");
			return 4;
		}

	//����Ŀ���ļ�
	strcpy (name, argv[1]);
	if ( (buffer = strstr (name, ".bin")) != NULL )
	{
		strcpy (buffer ,"");
	}
	strcat (name, ".idx");
	txt = fopen (name, "wb");
	if ( txt == NULL )
	{
		printf ("Can't create %s file.\n", name);
		return 3;
	}

	printf ("%s flie created.\n", name);

	fprintf (txt, "0x%06X\r\n", num);

	//��num���ļ��Ļ�ַ����base�����ڣ��Ƿ���LZѹ����־����flag��
	for (i = 0; i < num + 1; i++)
	{
		fread (&base[i], 4, 1, pak);
		if(base[i] & 0x80000000)
		{
			lzCount++;
			flag[i] = 1;
			base[i] -= 0x80000000;
		}
	}

	printf ("%d LZ files found\n", lzCount);
	//�ļ��ܳ���==base[num]

	for(i = 0; i < num; i++)//�������ļ�
	{
		fseek (pak, base[i], 0);
		len = base[i+1] - base[i];

		name[0] = '_';
		name[1] = 0;
		
		strcat (name, argv[1]);

		//���flagλ�б�־�����׺ΪLZ��ʾΪLZѹ����
		if(flag[i])
			sprintf (idx, "_%02d.lz", i);
		else 
			sprintf (idx, "_%02d.bin", i);
		strcat (name, idx);

		fprintf (txt, "%s\r\n", name);
		fprintf (txt, "%d\r\n", flag[i]);
		out = fopen (name, "wb");		//����ļ�
		
		dat = (unsigned char *) malloc ( (sizeof (char)) * len);

		fread (dat, 1, len, pak);
		fwrite (dat, 1, len, out);
		free (dat);
		fclose (out);
	}
	fclose (txt);
	fclose (pak);

	return 0;
}
