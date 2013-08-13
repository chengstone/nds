#include "../common.h"

#include "getPakInfo.c"

int main(int argc, char *argv[])
{
	FILE *fpPak,	//Դ�ļ�
		 *fpOut,	//����ĸ������Ķ����ļ�
		 *fpTxt;	//������������ļ�

	ULONG	i,
			pakCount,							//�����ļ����ĸ���
			base[MAX_FILE_COUNT] = {0},			//�����ļ���Դ�ļ��еĻ�ַ
			size[MAX_FILE_COUNT] = {0},			//���Ĵ�С
			zipedSize[MAX_FILE_COUNT] = {0},	//ѹ������Ĵ�С
			zipFlag[MAX_FILE_COUNT] = {0},		//�Ƿ�ѹ���ı�־λ
			pakLen,								//�����ļ����ĳ���
			zipCount = 0;						//ѹ�����ĸ���

	char fileName[MAX_FILE_NAME_LENGTH],		//���ļ����ļ���
		 idx[40],
		 *buffer;		

	BYTE *dat;
	

	//û������Դ�ļ���
	if (argc < 2)
	{
		printf ("No source file fileName input\n");
		return 1;
	}

	//��Դ�ļ�

	strcpy (fileName, argv[1]);
	if (strstr (fileName, ".PAK") == NULL)
	{
		strcat (fileName, ".PAK");
	}	


	fpPak = fopen (fileName, "rb");

	if (fpPak == NULL)
	{
		printf ("file \"%s\" did not exist.\n", fileName);
		return 2;
	}
	else
	{
		#ifdef DEBUG
			printf ("file \"%s\" opened.\n", fileName);
		#endif
	}

	//��ð�����Ŀ
	pakCount = getPakCount (fpPak);

	//����Ŀ���ļ�
	strcpy (fileName, argv[1]);
	if ( (buffer = strstr (fileName, ".PAK")) != NULL )
	{
		strcpy (buffer ,"");
	}
	strcat (fileName, ".idx");
	fpTxt = fopen (fileName, "w");
	if ( fpTxt == NULL )
	{
		printf ("Can't create %s file.\n", fileName);
		return 3;
	}
	printf ("%s flie created.\n", fileName);

	//��ȡ�ļ�ƫ�Ƶ�ַ,�������йظ�����������Ϣ���ļ���������Ŀ����С���Ƿ�ѹ����
	handlePakInfo (fpPak, pakCount, base, zipFlag, fpTxt, argv[1]);

	fclose (fpTxt);

	for(i = 0; i < pakCount; i++)//�������ļ�
	{
		fseek (fpPak, base[i], 0);
		pakLen = base[i+1] - base[i];

		fileName[0] = '_';
		fileName[1] = 0;
		
		strcat (fileName, argv[1]);

		//���zipFlagλ�б�־�����׺Ϊzip��ʾΪzipѹ����
		if(zipFlag[i])
			sprintf (idx, "_%02d.zip.PAK", i);
		else 
			sprintf (idx, "_%02d.PAK", i);
		strcat (fileName, idx);
		fpOut = fopen (fileName, "wb");		//����ļ�
		
		dat = (BYTE *) malloc ( (sizeof (BYTE)) * pakLen);

		fread (dat, sizeof (BYTE), pakLen, fpPak);
		fwrite (dat, sizeof (BYTE), pakLen, fpOut);
		free (dat);
		fclose (fpOut);
	}

	fclose (fpPak);

	return 0;
}
