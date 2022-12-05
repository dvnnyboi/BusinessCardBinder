//IndexCard.h
/************************************************************************************************
���ϸ�Ī: IndexCard.h
��    ��: ����ī�� ���� ���α׷� �ڷ��� �� �Լ� ����
�� �� ��: �ӴٴϿ�
�ۼ�����: 2022/07/05
*************************************************************************************************/
#ifndef _INDEXCARD_H
#define _INDEXCARD_H
#include "Array.h"

typedef signed long int Long;
typedef struct _businessCard BusinessCard;
typedef struct _indexCard {
	char companyName[16];
	Array businessCards;
	Long capacity;
	Long length;
}IndexCard;

void IndexCard_Create(IndexCard* indexCard, Long capacity);
Long IndexCard_TakeIn(IndexCard* indexCard, BusinessCard* index);
Long IndexCard_Find(IndexCard* indexCard, BusinessCard* index);
void IndexCard_Arrange(IndexCard* indexCard);
Long IndexCard_TakeOut(IndexCard* indexCard, Long location);
BusinessCard* IndexCard_GetAt(IndexCard* indexCard, Long location);
void IndexCard_Destroy(IndexCard* indexCard);

int CompareBusinessCards_Location(void* one, void* other);
int CompareNames_Location(void* one, void* other);

#endif //_INDEXCARD_H