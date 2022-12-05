//IndexCardBinder.c
/************************************************************************************************
���ϸ�Ī: IndexCardBinder.c
��    ��: ����ī��ö ���� ���α׷�
�� �� ��: �ӴٴϿ�
�ۼ�����: 2022/07/05
*************************************************************************************************/
#include "BusinessCardBinder.h"
#include "IndexCardBinder.h"
#include "IndexCard.h"
#include <string.h>
#pragma warning (disable:4996)

/************************************************************************************************
��    Ī: IndexCardBInder_Create
��    ��: ����ī��ö�� �����.
��    ��: ����
��    ��: ����
*************************************************************************************************/
void IndexCardBinder_Create(IndexCardBinder* indexCardBinder) {
	BinaryTree_Create(&(indexCardBinder->indexCards));
	indexCardBinder->length = 0;
}
/************************************************************************************************
��    Ī: IndexCardBinder_TakeIn
��    ��: ����ī��ö���� �����.
��    ��: ���� ��ũ
��    ��: ����ī�� ��ũ
*************************************************************************************************/
IndexCard* IndexCardBinder_TakeIn(IndexCardBinder* indexCardBinder, BusinessCard* index) {
	IndexCard indexCard;
	BinaryNode* node;

	strcpy(indexCard.companyName, index->company.name);
	node = Search(&(indexCardBinder->indexCards), &indexCard, CompareCompanyNames_Link);
	if (node == NULL) {
		node = Insert(&(indexCardBinder->indexCards), &indexCard, sizeof(IndexCard), CompareCompanyNames_Link);
		indexCardBinder->length++;
		IndexCard_Create((IndexCard*)(node + 1), 3);
	}
	IndexCard_TakeIn((IndexCard*)(node + 1), index);

	return (IndexCard*)(node + 1);
}
/************************************************************************************************
��    Ī: IndexCardBinder_Find
��    ��: ����ī��ö���� ã�´�.
��    ��: ��ȣ
��    ��: ����ī�� ��ũ
*************************************************************************************************/
IndexCard* IndexCardBinder_Find(IndexCardBinder* indexCardBinder, char(*companyName)) {
	IndexCard indexCard;
	BinaryNode* node;
	IndexCard* link = NULL;

	strcpy(indexCard.companyName, companyName);
	node = Search(&(indexCardBinder->indexCards), &indexCard, CompareCompanyNames_Link);

	if (node != NULL) {
		link = (IndexCard*)(node + 1);
	}

	return link;
}
/************************************************************************************************
��    Ī: MakeList
��    ��: ����ī�� ����� �����.
��    ��: ����
��    ��: ����ī���, ����
*************************************************************************************************/
void MakeList(IndexCardBinder* indexCardBinder, IndexCard* (*indexCards), Long(*count)) {
	MakeKeys(&(indexCardBinder->indexCards), indexCards, count, sizeof(IndexCard));
}
/************************************************************************************************
��    Ī: IndexCardBinder_Arrange
��    ��: ����ī��ö�� �����Ѵ�.
��    ��: ����
��    ��: ����
*************************************************************************************************/
void IndexCardBinder_Arrange(IndexCardBinder* indexCardBinder) {
	MakeBalance(&(indexCardBinder->indexCards), sizeof(IndexCard));
	ArrangeIndexCards(indexCardBinder->indexCards.root);
}
/************************************************************************************************
��    Ī: ArrangeIndexCards
��    ��: ����ī��ö�� ���Ե� �迭�� ������ ����Լ�.
��    ��: ����
��    ��: ����
*************************************************************************************************/
void ArrangeIndexCards(BinaryNode* node) {
	if (node != NULL) {
		ArrangeIndexCards(node->left);
		IndexCard_Arrange((IndexCard*)(node + 1));
		ArrangeIndexCards(node->right);
	}
}
/************************************************************************************************
��    Ī: IndexCardBinder_TakeOut
��    ��: ����ī��ö���� ����.
��    ��: ��ȣ, ���� ��ũ
��    ��: ����ī�� ��ũ
*************************************************************************************************/
IndexCard* IndexCardBinder_TakeOut(IndexCardBinder* indexCardBinder, char(*companyName), BusinessCard* index) {
	IndexCard indexCard;
	IndexCard* link;
	BinaryNode* node;
	Long location;

	strcpy(indexCard.companyName, companyName);
	node = Search(&(indexCardBinder->indexCards), &indexCard, CompareCompanyNames_Link);
	link = (IndexCard*)(node + 1);
	location = IndexCard_Find(link, index);
	IndexCard_TakeOut(link, location);

	if (link->length <= 0) {
		BinaryTree_Delete(&(indexCardBinder->indexCards), link, sizeof(IndexCard), CompareCompanyNames_Link);
		indexCardBinder->length--;
		link = NULL;
	}

	return link;
}
/************************************************************************************************
��    Ī: IndexCardBInder_Destroy
��    ��: ����ī��ö�� ���ش�.
��    ��: ����
��    ��: ����
*************************************************************************************************/
void IndexCardBinder_Destroy(IndexCardBinder* indexCardBinder) {
	DestroyIndexCards(indexCardBinder->indexCards.root);
	BinaryTree_Destroy(&(indexCardBinder->indexCards));
}
/************************************************************************************************
��    Ī: DestroyIndexCards
��    ��: ����ī��ö�� ���Ե� �迭�� ���ִ� ����Լ�.
��    ��: ����
��    ��: ����
*************************************************************************************************/
void DestroyIndexCards(BinaryNode* node) {
	if (node != NULL) {
		DestroyIndexCards(node->left);
		DestroyIndexCards(node->right);
		IndexCard_Destroy((IndexCard*)(node + 1));
	}
}

/************************************************************************************************/
int CompareCompanyNames_Link(void* one, void* other) {
	IndexCard* one_ = (IndexCard*)one;
	IndexCard* other_ = (IndexCard*)other;
	
	return strcmp(one_->companyName, other_->companyName);
}