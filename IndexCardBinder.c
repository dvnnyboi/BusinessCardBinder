//IndexCardBinder.c
/************************************************************************************************
파일명칭: IndexCardBinder.c
기    능: 색인카드철 관리 프로그램
작 성 자: 임다니엘
작성일자: 2022/07/05
*************************************************************************************************/
#include "BusinessCardBinder.h"
#include "IndexCardBinder.h"
#include "IndexCard.h"
#include <string.h>
#pragma warning (disable:4996)

/************************************************************************************************
명    칭: IndexCardBInder_Create
기    능: 색인카드철을 만든다.
입    력: 없음
출    력: 없음
*************************************************************************************************/
void IndexCardBinder_Create(IndexCardBinder* indexCardBinder) {
	BinaryTree_Create(&(indexCardBinder->indexCards));
	indexCardBinder->length = 0;
}
/************************************************************************************************
명    칭: IndexCardBinder_TakeIn
기    능: 색인카드철에서 끼운다.
입    력: 명함 링크
출    력: 색인카드 링크
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
명    칭: IndexCardBinder_Find
기    능: 색인카드철에서 찾는다.
입    력: 상호
출    력: 색인카드 링크
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
명    칭: MakeList
기    능: 색인카드 목록을 만든다.
입    력: 없음
출    력: 색인카드들, 개수
*************************************************************************************************/
void MakeList(IndexCardBinder* indexCardBinder, IndexCard* (*indexCards), Long(*count)) {
	MakeKeys(&(indexCardBinder->indexCards), indexCards, count, sizeof(IndexCard));
}
/************************************************************************************************
명    칭: IndexCardBinder_Arrange
기    능: 색인카드철을 정리한다.
입    력: 없음
출    력: 없음
*************************************************************************************************/
void IndexCardBinder_Arrange(IndexCardBinder* indexCardBinder) {
	MakeBalance(&(indexCardBinder->indexCards), sizeof(IndexCard));
	ArrangeIndexCards(indexCardBinder->indexCards.root);
}
/************************************************************************************************
명    칭: ArrangeIndexCards
기    능: 색인카드철의 명함들 배열을 정리할 재귀함수.
입    력: 없음
출    력: 없음
*************************************************************************************************/
void ArrangeIndexCards(BinaryNode* node) {
	if (node != NULL) {
		ArrangeIndexCards(node->left);
		IndexCard_Arrange((IndexCard*)(node + 1));
		ArrangeIndexCards(node->right);
	}
}
/************************************************************************************************
명    칭: IndexCardBinder_TakeOut
기    능: 색인카드철에서 뺀다.
입    력: 상호, 명함 링크
출    력: 색인카드 링크
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
명    칭: IndexCardBInder_Destroy
기    능: 색인카드철을 없앤다.
입    력: 없음
출    력: 없음
*************************************************************************************************/
void IndexCardBinder_Destroy(IndexCardBinder* indexCardBinder) {
	DestroyIndexCards(indexCardBinder->indexCards.root);
	BinaryTree_Destroy(&(indexCardBinder->indexCards));
}
/************************************************************************************************
명    칭: DestroyIndexCards
기    능: 색인카드철의 명함들 배열을 없애는 재귀함수.
입    력: 없음
출    력: 없음
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