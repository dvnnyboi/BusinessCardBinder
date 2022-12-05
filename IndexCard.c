//IndexCard.c
/************************************************************************************************
파일명칭: IndexCard.c
기    능: 색인카드 관리 프로그램
작 성 자: 임다니엘
작성일자: 2022/07/05
*************************************************************************************************/
#include "BusinessCardBinder.h"
#include "IndexCard.h"
#include <string.h>

/************************************************************************************************
명    칭: IndexCard_Create
기    능: 명함들 배열을 만든다.
입    력: 할당량
출    력: 없음
*************************************************************************************************/
void IndexCard_Create(IndexCard* indexCard, Long capacity) {
	Array_Create(&(indexCard->businessCards), capacity, sizeof(BusinessCard*));
	indexCard->capacity = capacity;
	indexCard->length = 0;
}
/************************************************************************************************
명    칭: IndexCard_TakeIn
기    능: 명함들 배열에 끼운다.
입    력: 명함 링크
출    력: 위치
*************************************************************************************************/
Long IndexCard_TakeIn(IndexCard* indexCard, BusinessCard* index) {
	Long location;

	if (indexCard->capacity > indexCard->length) {
		location = Store(&(indexCard->businessCards), indexCard->length, &index, sizeof(BusinessCard*));
	}
	else {
		location = AppendFromRear(&(indexCard->businessCards), &index, sizeof(BusinessCard*));
		indexCard->capacity++;
	}
	indexCard->length++;

	return location;
}
/************************************************************************************************
명    칭: IndexCard_Find
기    능: 명함들 배열에서 찾는다.
입    력: 명함 링크
출    력: 위치
*************************************************************************************************/
Long IndexCard_Find(IndexCard* indexCard, BusinessCard* index) {
	Long location;

	location = Array_LinearSearchUnique(&(indexCard->businessCards), index, sizeof(BusinessCard*), CompareBusinessCards_Location);

	return location;
}
/************************************************************************************************
명    칭: IndexCard_Arrange
기    능: 명함들 배열을 정리한다.
입    력: 없음
출    력: 없음
*************************************************************************************************/
void IndexCard_Arrange(IndexCard* indexCard) {
	SelectionSort(&(indexCard->businessCards), sizeof(BusinessCard*), CompareNames_Location);
}
/************************************************************************************************
명    칭: IndexCard_TakeOut
기    능: 명함들 배열에서 뺀다.
입    력: 위치
출    력: 위치
*************************************************************************************************/
Long IndexCard_TakeOut(IndexCard* indexCard, Long location) {
	location = Array_Delete(&(indexCard->businessCards), location, sizeof(BusinessCard*));
	indexCard->capacity--;
	indexCard->length--;

	return location;
}
/************************************************************************************************
명    칭: IndexCard_GetAt
기    능: 명함들 배열에서 읽어온다.
입    력: 위치
출    력: 명함 링크
*************************************************************************************************/
BusinessCard* IndexCard_GetAt(IndexCard* indexCard, Long location){
	BusinessCard* index;

	Array_GetAt(&(indexCard->businessCards), location, &index, sizeof(BusinessCard*));

	return index;
}
/************************************************************************************************
명    칭: IndexCard_Destroy
기    능: 명함들 배열을 없앤다.
입    력: 없음
출    력: 없음
*************************************************************************************************/
void IndexCard_Destroy(IndexCard* indexCard) {
	Array_Destroy(&(indexCard->businessCards));
}

/************************************************************************************************/
int CompareBusinessCards_Location(void* one, void* other) {
	BusinessCard** one_ = (BusinessCard**)one;
	int ret = -1;

	if (*one_ == other) {
		ret = 0;
	}

	return ret;
}
int CompareNames_Location(void* one, void* other) {
	BusinessCard** one_ = (BusinessCard**)one;
	BusinessCard** other_ = (BusinessCard**)other;

	return strcmp((*one_)->personal.name, (*other_)->personal.name);
}