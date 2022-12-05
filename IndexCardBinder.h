//IndexCardBinder.h
/************************************************************************************************
���ϸ�Ī: IndexCardBinder.h
��    ��: ����ī��ö ���� ���α׷� �ڷ��� �� �Լ� ����
�� �� ��: �ӴٴϿ�
�ۼ�����: 2022/07/05
*************************************************************************************************/
#ifndef _INDEXCARDBINDER_H
#define _INDEXCARDBINDER_H
#include "BinarySearchTree.h"

typedef signed long int Long;
typedef struct _businessCard BusinessCard;
typedef struct _indexCard IndexCard;
typedef struct _indexCardBinder {
	BinaryTree indexCards;
	Long length;
}IndexCardBinder;

void IndexCardBinder_Create(IndexCardBinder* indexCardBinder);
IndexCard* IndexCardBinder_TakeIn(IndexCardBinder* indexCardBinder, BusinessCard* index);
IndexCard* IndexCardBinder_Find(IndexCardBinder* indexCardBinder, char(*companyName));
void MakeList(IndexCardBinder* indexCardBinder, IndexCard* (*indexCards), Long (*count));
void IndexCardBinder_Arrange(IndexCardBinder* indexCardBinder);
	void ArrangeIndexCards(BinaryNode* node);
IndexCard* IndexCardBinder_TakeOut(IndexCardBinder* indexCardBinder, char (*companyName), BusinessCard *index);
void IndexCardBinder_Destroy(IndexCardBinder* indexCardBinder);
	void DestroyIndexCards(BinaryNode* node);

int CompareCompanyNames_Link(void* one, void* other);

#endif //_INDEXCARDBINDER_H