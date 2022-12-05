//IndexCard.h
/************************************************************************************************
파일명칭: IndexCard.h
기    능: 색인카드 관리 프로그램 자료형 및 함수 선언
작 성 자: 임다니엘
작성일자: 2022/07/05
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