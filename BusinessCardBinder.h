//BusinessCardBinder.h
/************************************************************************************************
파일명칭: BusinessCardBinder.h
기    능: 명함철 자료형 및 함수 선언
작 성 자: 임다니엘
작성일자: 2022/06/16
*************************************************************************************************/
#ifndef _BUSINESSCARDBINDER_H
#define _BUSINESSCARDBINDER_H
#include "LinkedList.h"

typedef signed long int Long;
typedef struct _personal {
	char name[11];
	char rank[20];
	char cellphoneNumber[12];
	char emailAddress[32];
}Personal;
typedef struct _company {
	char name[16];
	char address[32];
	char telephoneNumber[12];
	char faxNumber[12];
	char url[32];
}Company;
typedef struct _businessCard {
	Personal personal;
	Company company;
}BusinessCard;
typedef struct _businessCardBinder {
	LinkedList businessCards;
	Long length;
	BusinessCard* current;
}BusinessCardBinder;

void BusinessCardBinder_Create(BusinessCardBinder* businessCardBinder);
Long Load(BusinessCardBinder* businessCardBinder);
BusinessCard* TakeIn(BusinessCardBinder* businessCardBinder, BusinessCard businessCard);
void Find(BusinessCardBinder* businessCardBinder, char(*name), BusinessCard** (*indexes), Long* count);
BusinessCard* FindByCompanyName(BusinessCardBinder* businessCardBinder, char(*companyName));
BusinessCard TakeOut(BusinessCardBinder* businessCardBinder, BusinessCard* index);
BusinessCard* BusinessCardBinder_Move(BusinessCardBinder* businessCardBinder, BusinessCard* index);
BusinessCard* BusinessCardBinder_First(BusinessCardBinder* businessCardBinder);
BusinessCard* BusinessCardBinder_Previous(BusinessCardBinder* businessCardBinder);
BusinessCard* BusinessCardBinder_Next(BusinessCardBinder* businessCardBinder);
BusinessCard* BusinessCardBinder_Last(BusinessCardBinder* businessCardBinder);
Long Save(BusinessCardBinder* businessCardBinder);
void BusinessCardBinder_Destroy(BusinessCardBinder* businessCardBinder);

int CompareNames(void* one, void* other);
int CompareCompanyNames(void* one, void* other);
int CompareBusinessCards(void* one, void* other);

#endif //_BUSINESSCARDBINDER_H