#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>

//#define DEBUG
#define MAX_FILE_NAME_LENGTH 127	//�ļ�������󳤶�
#define HEAD_LENGTH			 8		//�ļ�ͷ���Ƶĳ���
#define PAK_COUNT			 31		//�������ļ��ĸ���

#define PAK_START_OFFSET	(0x90)	//��һ������ʼ��ƫ��λ��

typedef unsigned long ULONG;	//���ڴ���4���ֽڵ�����

unsigned char headData[] = 
	{ 0x46, 0x42, 0x49, 0x4E,	//�ļ���ʽ��NIBF
	  0x1E, 0x00, 0x00, 0x00	//�ļ�����30����Ч�ļ��������ļ�ͷʵ����31����
	};

void fileHeadHandle (FILE *outFile);
long getFileLength (FILE *fp);
void writeFileLength (FILE *fp, ULONG *lengthTab, int count);

int main(int argc, char * argv[])
{
	FILE *pak,
		 *src;
	ULONG		  num,
				  fileLength[PAK_COUNT],	//����PAK_COUNT���ļ����ĳ���
				  len,
				  i;
	char srcName[MAX_FILE_NAME_LENGTH];
	char outName[MAX_FILE_NAME_LENGTH] = "EventMessageDungeonGimmic.bin";
	unsigned char *dat;
	unsigned char c;

	#ifdef DEBUG
		printf ("sizeof(unsigned long) = %d\n", sizeof (unsigned long));
	#endif

	pak = fopen (outName, "wb");
	if (pak == NULL)
	{
		printf ("Can't create \"%s\" file\n", outName);
		return 1;
	}

	#ifdef DEBUG
		printf ("File \"%s\" created.\n", outName);
	#endif

	fileHeadHandle (pak);	


	for(i = 1; i < PAK_COUNT; i++)//�������ļ�
	{
		sprintf (srcName, "%s%d", outName, i + 1);


		src = fopen (srcName, "rb");//��Ҫ�����Դ�ļ�
		if (src == NULL)
		{
			printf ("No \"%s\" file.\n", srcName);
			return 2;
		}

		#ifdef DEBUG
			printf ("file \"%s\" is being packing\n", srcName);
		#endif

		//��ȡ�ð��ĳ���
		if ((fileLength [i] = getFileLength (src)) < 0)
		{
			printf ("Wrong file length\n");
			return 4;
		}
		

		dat = (unsigned char *) malloc (sizeof (unsigned char) * fileLength [i]);

		fseek (src, 0, SEEK_SET);
		fread (dat, sizeof(unsigned char), fileLength[i], src);
		fwrite (dat, sizeof(unsigned char), fileLength[i], pak);
		free (dat);
		fclose (src);
	}

	writeFileLength (pak, fileLength, PAK_COUNT);
	fclose(pak);

	return 0;
}

//�ļ�ͷ��д�룬�������ĳ���Ԥ��Ϊ0
//��������ʱFILEָ��ָ���ļ���ĩβ
void fileHeadHandle (FILE *outFile)
{
	int i;
	ULONG zeroField[PAK_COUNT + 3] = {0};


	#ifdef DEBUG
		printf ("Start insert head data\n");
	#endif

	fwrite ( headData, sizeof (unsigned char), HEAD_LENGTH, outFile);

	#ifdef DEBUG
		printf ("Head data inserted\n");
	#endif

	
	fwrite ( zeroField, sizeof (ULONG), PAK_COUNT + 3, outFile);
	
	#ifdef DEBUG
		printf ("Now we're in the offset of %lx\n", ftell (outFile));
	#endif
		
}

//��ȡ���ĳ���
//���ı�������ļ�ָ��ĵ�ַ
long getFileLength (FILE *fp)
{
	long orgAddr,
		 fileLength;
	orgAddr = ftell (fp);
	if ( (fseek (fp, 0, SEEK_END)) )
	{
		printf ("Error when caculating file length.\n");
		return -1;
	}
	fileLength = ftell(fp);

	fseek (fp, orgAddr, SEEK_CUR);

	#ifdef DEBUG
		printf ("the file lentgh is %ld\n", fileLength);
	#endif

	return fileLength;
}

void writeFileLength (FILE *fp, ULONG *lengthTable, int count)
{
	int i;
	fseek (fp, HEAD_LENGTH, SEEK_SET);

	fwrite (lengthTable, sizeof (ULONG), count, fp);
}

