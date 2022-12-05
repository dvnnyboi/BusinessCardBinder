//IndexCard.c
/************************************************************************************************
���ϸ�Ī: IndexCard.c
��    ��: ����ī�� ���� ���α׷�
�� �� ��: �ӴٴϿ�
�ۼ�����: 2022/07/05
*************************************************************************************************/
#include "BusinessCardBinder.h"
#include "IndexCard.h"
#include <string.h>

/************************************************************************************************
��    Ī: IndexCard_Create
��    ��: ���Ե� �迭�� �����.
��    ��: �Ҵ緮
��    ��: ����
*************************************************************************************************/
void IndexCard_Create(IndexCard* indexCard, Long capacity) {
	Array_Create(&(indexCard->businessCards), capacity, sizeof(BusinessCard*));
	indexCard->capacity = capacity;
	indexCard->length = 0;
}
/************************************************************************************************
��    Ī: IndexCard_TakeIn
��    ��: ���Ե� �迭�� �����.
��    ��: ���� ��ũ
��    ��: ��ġ
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
��    Ī: IndexCard_Find
��    ��: ���Ե� �迭���� ã�´�.
��    ��: ���� ��ũ
��    ��: ��ġ
*************************************************************************************************/
Long IndexCard_Find(IndexCard* indexCard, BusinessCard* index) {
	Long location;

	location = Array_LinearSearchUnique(&(indexCard->businessCards), index, sizeof(BusinessCard*), CompareBusinessCards_Location);

	return location;
}
/************************************************************************************************
��    Ī: IndexCard_Arrange
��    ��: ���Ե� �迭�� �����Ѵ�.
��    ��: ����
��    ��: ����
*************************************************************************************************/
void IndexCard_Arrange(IndexCard* indexCard) {
	SelectionSort(&(indexCard->businessCards), sizeof(BusinessCard*), CompareNames_Location);
}
/************************************************************************************************
��    Ī: IndexCard_TakeOut
��    ��: ���Ե� �迭���� ����.
��    ��: ��ġ
��    ��: ��ġ
*************************************************************************************************/
Long IndexCard_TakeOut(IndexCard* indexCard, Long location) {
	location = Array_Delete(&(indexCard->businessCards), location, sizeof(BusinessCard*));
	indexCard->capacity--;
	indexCard->length--;

	return location;
}
/************************************************************************************************
��    Ī: IndexCard_GetAt
��    ��: ���Ե� �迭���� �о�´�.
��    ��: ��ġ
��    ��: ���� ��ũ
*************************************************************************************************/
BusinessCard* IndexCard_GetAt(IndexCard* indexCard, Long location){
	BusinessCard* index;

	Array_GetAt(&(indexCard->businessCards), location, &index, sizeof(BusinessCard*));

	return index;
}
/************************************************************************************************
��    Ī: IndexCard_Destroy
��    ��: ���Ե� �迭�� ���ش�.
��    ��: ����
��    ��: ����
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