#include <stdio.h>

#include "../common.h"


//�ı���������
typedef struct txtExtent
{
	struct txtExtent * next;
	ULONG start, end;
	int iWidth;
} txtExt;
