/*
 *	�������
 *
 *	����Ϊ��Ҫ���ɵİ����ļ�����ʽΪ *.PAK
 *	��Ҫ����İ���Ҫ���ʽΪ	_*.PAK_$$.PAK		($$����С������ţ�����84)
 *						����	_*.PAK_$$.zip.PAK	($$����С������ţ�����84)
 *
*/

#include "../common.h"
#include "handleAllPak.c"

inline void getSrcName_i (char *srcName, char *outName, ULONG i);
inline void getZipSrcName (char *srcName, char *outName, ULONG i);

int main(int argc, char * argv[])
{
	FILE *fpPak,	//�������ɵ����ļ�
		 *fpSrc;	//������
	ULONG		  num,
				  fileLength[PAK_COUNT],	//����PAK_COUNT���ļ����ĳ���
				  len,
				  i;
	char srcName[MAX_FILE_NAME_LENGTH];
	char outName[MAX_FILE_NAME_LENGTH] = "SPR_NCLR.PAK";
	BYTE *dat;
	BYTE c;

	#ifdef DEBUG
		printf ("sizeof(unsigned long) = %d\n", sizeof (unsigned long));
	#endif

	fpPak = fopen (outName, "wb");
	if (fpPak == NULL)
	{
		printf ("Can't create \"%s\" file\n", outName);
		return 1;
	}

	#ifdef DEBUG
		printf ("File \"%s\" created.\n", outName);
	#endif

	fileHeadHandle (fpPak);	


	for(i = 0; i < PAK_COUNT; i++)//�������ļ�
	{

		getSrcName_i (srcName, outName, i);


		fpSrc = fopen (srcName, "rb");//��Ҫ�����Դ�ļ�
		if (fpSrc == NULL)
		{
			getZipSrcName (srcName, outName, i);
			fpSrc = fopen (srcName, "rb");
			if (fpSrc == NULL)
			{
				printf ("No \"%s\" file.\n", srcName);
				return 2;
			}

		}

		#ifdef DEBUG
			printf ("file \"%s\" is being packing\n", srcName);
		#endif

		//��ȡ�ð��ĳ���
		if ((fileLength [i] = getFileLength (fpSrc)) < 0)
		{
			printf ("Wrong file length\n");
			return 4;
		}
		

		dat = (BYTE *) malloc (sizeof (BYTE) * fileLength [i]);

		fseek (fpSrc, 0, SEEK_SET);
		fread (dat, sizeof(BYTE), fileLength[i], fpSrc);
		fwrite (dat, sizeof(BYTE), fileLength[i], fpPak);
		free (dat);
		fclose (fpSrc);
	}

	writeFileLength (fpPak, fileLength, PAK_COUNT);
	fclose(fpPak);

	return 0;
}

inline void getSrcName_i (char *srcName, char *outName, ULONG i)
{
	sprintf (srcName, "_%s_%d.PAK", outName, i);
}

inline void getZipSrcName (char *srcName, char *outName, ULONG i)
{
		sprintf (srcName, "_%s_%d.zip.PAK", outName, i);
}
