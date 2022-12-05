//BusinessCardBinder.c
/************************************************************************************************
파일명칭: BusinessCardBinder.c
기    능: 명함철 관리
작 성 자: 임다니엘
작성일자: 2022/06/16
*************************************************************************************************/
#include "BusinessCardBinder.h"
#include "IndexCardBinder.h"
#include "IndexCard.h"
#include <stdio.h>
#include <string.h>
#pragma warning (disable:4996)

#if 0
//Test Scenario
int main(int argc, char* argv[]) {
	BusinessCardBinder businessCardBinder;
	IndexCardBinder indexCardBinder;
	BusinessCard businessCardOne = { {"Hwang", "Department Cheif", "01026178592", "hwang@naver.com"},
					{"SAMSUNG", "YeongTong-Gu, SuWon", "021234567", "027654321", "www.samsung.co.kr"} };
	BusinessCard businessCardTwo = { {"Park", "Section chief", "01083665211", "park@gmail.com"},
					{"HyunDai", "YangJae-Dong, Seoul", "022345678", "028765432", "www.hyundai.co.kr"} };
	BusinessCard businessCardThree = { {"Kim", "Director", "01087326141", "kim@gmail.com"},
					{"LG", "YeoEuiDo-Dong, Seoul", "023456789", "029876543", "www.lg.com"} };
	BusinessCard businessCardFour = { {"Choi", "Section chief", "01041058605", "choi@gmail.com"},
					{"SAMSUNG", "YeongTong-Gu, SuWon", "021234567", "027654321", "www.samsung.co.kr"} };
	BusinessCard businessCardFive = { {"Park", "Deputy chief", "01012345678", "pgd@naver.com"},
					{"SAMSUNG", "YeongTong-Gu, SuWon", "021234567", "027654321", "www.samsung.co.kr"} };
	BusinessCard businessCardSix = { {"Hong", "Employee", "01087654321", "pgd@naver.com"},
					{"HyunDai", "YangJae-Dong, Seoul", "022345678", "028765432", "www.hyundai.co.kr"} };
	BusinessCard businessCard;
	BusinessCard* index;
	BusinessCard* (*indexes);
	IndexCard* link;
	IndexCard(*indexCards);
	Long count;
	Long i;
	Long j;

	//만든다.
	BusinessCardBinder_Create(&businessCardBinder);
	IndexCardBinder_Create(&indexCardBinder);

	//끼운다.
	printf("\n==================================================TakeIn===================================================\n");
	index = TakeIn(&businessCardBinder, businessCardOne);
	printf("Personal: %s\t  %s\t\t%s\t    %s\nCompany: %s\t  %s\t\t%s\t    %s\t    %s\n\n",
		index->personal.name, index->personal.rank, index->personal.cellphoneNumber, index->personal.emailAddress,
		index->company.name, index->company.address, index->company.telephoneNumber, index->company.faxNumber, index->company.url);
	link = IndexCardBinder_TakeIn(&indexCardBinder, index);
	printf("INDEXCARD: %s , capacity %d  , length %d \n", link->companyName, link->capacity, link->length);
	j = 0;
	while (j < link->length) {
		index = IndexCard_GetAt(link, j);
		printf(" %s \n", index->personal.name);
		j++;
	}
	printf("-----------------------------------------------------------------------------------------------------------\n");
	index = TakeIn(&businessCardBinder, businessCardTwo);
	printf("Personal: %s\t  %s\t\t%s\t    %s\nCompany: %s\t  %s\t\t%s\t    %s\t    %s\n\n",
		index->personal.name, index->personal.rank, index->personal.cellphoneNumber, index->personal.emailAddress,
		index->company.name, index->company.address, index->company.telephoneNumber, index->company.faxNumber, index->company.url);
	link = IndexCardBinder_TakeIn(&indexCardBinder, index);
	printf("INDEXCARD: %s , capacity %d  , length %d \n", link->companyName, link->capacity, link->length);
	j = 0;
	while (j < link->length) {
		index = IndexCard_GetAt(link, j);
		printf(" %s \n", index->personal.name);
		j++;
	}
	printf("-----------------------------------------------------------------------------------------------------------\n");
	index = TakeIn(&businessCardBinder, businessCardThree);
	printf("Personal: %s\t  %s\t\t%s\t    %s\nCompany: %s\t  %s\t\t%s\t    %s\t    %s\n\n",
		index->personal.name, index->personal.rank, index->personal.cellphoneNumber, index->personal.emailAddress,
		index->company.name, index->company.address, index->company.telephoneNumber, index->company.faxNumber, index->company.url);
	link = IndexCardBinder_TakeIn(&indexCardBinder, index);
	printf("INDEXCARD: %s , capacity %d  , length %d \n", link->companyName, link->capacity, link->length);
	j = 0;
	while (j < link->length) {
		index = IndexCard_GetAt(link, j);
		printf(" %s \n", index->personal.name);
		j++;
	}
	printf("-----------------------------------------------------------------------------------------------------------\n");
	index = TakeIn(&businessCardBinder, businessCardFour);
	printf("Personal: %s\t  %s\t\t%s\t    %s\nCompany: %s\t  %s\t\t%s\t    %s\t    %s\n\n",
		index->personal.name, index->personal.rank, index->personal.cellphoneNumber, index->personal.emailAddress,
		index->company.name, index->company.address, index->company.telephoneNumber, index->company.faxNumber, index->company.url);
	link = IndexCardBinder_TakeIn(&indexCardBinder, index);
	printf("INDEXCARD: %s , capacity %d  , length %d \n", link->companyName, link->capacity, link->length);
	j = 0;
	while (j < link->length) {
		index = IndexCard_GetAt(link, j);
		printf(" %s \n", index->personal.name);
		j++;
	}
	printf("-----------------------------------------------------------------------------------------------------------\n");
	index = TakeIn(&businessCardBinder, businessCardFive);
	printf("Personal: %s\t  %s\t\t%s\t    %s\nCompany: %s\t  %s\t\t%s\t    %s\t    %s\n\n",
		index->personal.name, index->personal.rank, index->personal.cellphoneNumber, index->personal.emailAddress,
		index->company.name, index->company.address, index->company.telephoneNumber, index->company.faxNumber, index->company.url);
	link = IndexCardBinder_TakeIn(&indexCardBinder, index);
	printf("INDEXCARD: %s , capacity %d  , length %d \n", link->companyName, link->capacity, link->length);
	j = 0;
	while (j < link->length) {
		index = IndexCard_GetAt(link, j);
		printf(" %s \n", index->personal.name);
		j++;
	}
	printf("-----------------------------------------------------------------------------------------------------------\n");
	index = TakeIn(&businessCardBinder, businessCardSix);
	printf("Personal: %s\t  %s\t\t%s\t    %s\nCompany: %s\t  %s\t\t%s\t    %s\t    %s\n\n",
		index->personal.name, index->personal.rank, index->personal.cellphoneNumber, index->personal.emailAddress,
		index->company.name, index->company.address, index->company.telephoneNumber, index->company.faxNumber, index->company.url);
	link = IndexCardBinder_TakeIn(&indexCardBinder, index);
	printf("INDEXCARD: %s , capacity %d  , length %d \n", link->companyName, link->capacity, link->length);
	j = 0;
	while (j < link->length) {
		index = IndexCard_GetAt(link, j);
		printf(" %s \n", index->personal.name);
		j++;
	}
	printf("-----------------------------------------------------------------------------------------------------------\n\n");


	//색인카드철을 정리한다.
	printf("\n==================================================Arrange==================================================\n");
	IndexCardBinder_Arrange(&indexCardBinder);
	MakeList(&indexCardBinder, &indexCards, &count);
	i = 0;
	while (i < count) {
		link = &(indexCards[i]);
		printf("INDEXCARD: %s , capacity %d  , length %d \n", link->companyName, link->capacity, link->length);
		j = 0;
		while (j < link->length) {
			index = IndexCard_GetAt(link, j);
			printf(" %s\n", index->personal.name);
			j++;
		}
		printf("-----------------------------------------------------\n");
		i++;
	}
	if (indexCards != NULL) {
		free(indexCards);
	}

	//찾는다.
	Find(&businessCardBinder, "Park", &indexes, &count);
	printf("\n===================================================Find====================================================\n");
	i = 0;
	while (i < count) {
		printf("Personal: %s\t  %s\t\t%s\t    %s\nCompany: %s\t  %s\t\t%s\t    %s\t    %s\n",
			indexes[i]->personal.name, indexes[i]->personal.rank, indexes[i]->personal.cellphoneNumber, indexes[i]->personal.emailAddress,
			indexes[i]->company.name, indexes[i]->company.address, indexes[i]->company.telephoneNumber, indexes[i]->company.faxNumber, indexes[i]->company.url);
		printf("-----------------------------------------------------------------------------------------------------------\n");
		i++;
	}

	//뺀다.
	businessCard = TakeOut(&businessCardBinder, indexes[0]);
	printf("\n=================================================TakeOut===================================================\n");
	printf("Personal: %s\t  %s\t\t%s\t    %s\nCompany: %s\t  %s\t\t%s\t    %s\t    %s\n\n",
		businessCard.personal.name, businessCard.personal.rank, businessCard.personal.cellphoneNumber, businessCard.personal.emailAddress,
		businessCard.company.name, businessCard.company.address, businessCard.company.telephoneNumber, businessCard.company.faxNumber, businessCard.company.url);
	link = IndexCardBinder_TakeOut(&indexCardBinder, businessCard.company.name, indexes[0]);

	//색인카드목록을 만든다.
	printf("\n================================================MakeList===================================================\n");
	MakeList(&indexCardBinder, &indexCards, &count);
	i = 0;
	while (i < count) {
		link = &(indexCards[i]);
		printf("INDEXCARD: %s , capacity %d  , length %d \n", link->companyName, link->capacity, link->length);
		j = 0;
		while (j < link->length) {
			index = IndexCard_GetAt(link, j);
			printf(" %s\n", index->personal.name);
			j++;
		}
		printf("-----------------------------------------------------\n");
		i++;
	}
	if (indexCards != NULL) {
		free(indexCards);
	}

	//상호로 찾는다.
	printf("\n=============================================FindByCompanyName==============================================\n");
	index = FindByCompanyName(&businessCardBinder, "HyunDai");
	printf("Personal: %s\t  %s\t\t%s\t    %s\nCompany: %s\t  %s\t\t%s\t    %s\t    %s\n",
		index->personal.name, index->personal.rank, index->personal.cellphoneNumber, index->personal.emailAddress,
		index->company.name, index->company.address, index->company.telephoneNumber, index->company.faxNumber, index->company.url);
	printf("-----------------------------------------------------------------------------------------------------------\n");

	//색인카드철에서 찾는다.
	link = IndexCardBinder_Find(&indexCardBinder, index->company.name);

	//뺀다.
	printf("\n=================================================TakeOut===================================================\n");
	businessCard = TakeOut(&businessCardBinder, index);
	printf("Personal: %s\t  %s\t\t%s\t    %s\nCompany: %s\t  %s\t\t%s\t    %s\t    %s\n\n",
		businessCard.personal.name, businessCard.personal.rank, businessCard.personal.cellphoneNumber, businessCard.personal.emailAddress,
		businessCard.company.name, businessCard.company.address, businessCard.company.telephoneNumber, businessCard.company.faxNumber, businessCard.company.url);
	link = IndexCardBinder_TakeOut(&indexCardBinder, businessCard.company.name, index);

	//색인카드목록을 만든다.
	printf("\n================================================MakeList===================================================\n");
	MakeList(&indexCardBinder, &indexCards, &count);
	i = 0;
	while (i < count) {
		link = &(indexCards[i]);
		printf("INDEXCARD: %s , capacity %d  , length %d \n", link->companyName, link->capacity, link->length);
		j = 0;
		while (j < link->length) {
			index = IndexCard_GetAt(link, j);
			printf(" %s \n", index->personal.name);
			j++;
		}
		printf("-----------------------------------------------------\n");
		i++;
	}
	if (indexCards != NULL) {
		free(indexCards);
	}

	//없앤다.
	BusinessCardBinder_Destroy(&businessCardBinder);
	IndexCardBinder_Destroy(&indexCardBinder);

	return 0;
}
#endif
/************************************************************************************************
명    칭: BusinessCardBinder_Create
기    능: 명함철을 만든다.
입    력: 없음
출    력: 없음
*************************************************************************************************/
void BusinessCardBinder_Create(BusinessCardBinder* businessCardBinder) {
	Create(&(businessCardBinder->businessCards));
	businessCardBinder->length = 0;
	businessCardBinder->current = NULL;
}
/************************************************************************************************
명    칭: Load
기    능: 디스크파일로부터 명함철을 입력받는다.
입    력: 없음
출    력: 개수
*************************************************************************************************/
Long Load(BusinessCardBinder* businessCardBinder) {
	BusinessCard businessCard;
	Node* node;
	Long i;
	Long j;
	size_t flag;
	FILE* personals;
	FILE* companies;

	personals = fopen("Personal.dat", "rb");
	companies = fopen("Company.dat", "rb");
	if (personals != NULL && companies != NULL) {
		flag = fread(&i, sizeof(Long), 1, personals);
		flag = fread(&(businessCard.personal), sizeof(Personal), 1, personals);
		while (!feof(personals) && flag != 0) {
			j = 1;
			fseek(companies, 0L, SEEK_SET);
			flag = fread(&(businessCard.company), sizeof(Company), 1, companies);
			while (!feof(companies) && flag != 0 && j < i) {
				j++;
				flag = fread(&(businessCard.company), sizeof(Company), 1, companies);
			}
			node = AppendFromTail(&(businessCardBinder->businessCards), &businessCard, sizeof(BusinessCard));
			businessCardBinder->current = (BusinessCard*)(node + 1);
			businessCardBinder->length++;
			flag = fread(&i, sizeof(Long), 1, personals);
			flag = fread(&(businessCard.personal), sizeof(Personal), 1, personals);
		}
		fclose(personals);
		fclose(companies);
	}
	return businessCardBinder->length;
}
/************************************************************************************************
명    칭: TakeIn
기    능: 명함철의 맨 뒤에 명함을 끼운다.
입    력: 명함
출    력: 위치
*************************************************************************************************/
BusinessCard* TakeIn(BusinessCardBinder* businessCardBinder, BusinessCard businessCard) {
	Node* node;
	
	node = AppendFromTail(&(businessCardBinder->businessCards), &businessCard, sizeof(BusinessCard));
	businessCardBinder->length++;
	businessCardBinder->current = (BusinessCard*)(node + 1);

	return businessCardBinder->current;
}
/************************************************************************************************
명    칭: Find
기    능: 명함철에서 찾는다.
입    력: 성명
출    력: 위치들, 개수
*************************************************************************************************/
void Find(BusinessCardBinder* businessCardBinder, char(*name), BusinessCard** (*indexes), Long* count) {
	Node* (*nodes);
	Long i;

	LinearSearchDuplicate(&(businessCardBinder->businessCards), name, &nodes, count, CompareNames);
	*indexes = (BusinessCard * (*))calloc(businessCardBinder->length, sizeof(BusinessCard*));
	i = 0;
	while (i < *count) {
		(*indexes)[i] = (BusinessCard*)(nodes[i] + 1);
		i++;
	}
	if (nodes != NULL) {
		free(nodes);
	}
}
/************************************************************************************************
명    칭: FindByCompanyName
기    능: 명함철에서 상호로 찾는다.
입    력: 상호
출    력: 위치
*************************************************************************************************/
BusinessCard* FindByCompanyName(BusinessCardBinder* businessCardBinder, char(*companyName)) {
	Node* node;
	BusinessCard* index = NULL;

	node = LinearSearchUnique(&(businessCardBinder->businessCards), companyName, CompareCompanyNames);
	if (node != NULL) {
		index = (BusinessCard*)(node + 1);
	}
	return index;
}
/************************************************************************************************
명    칭: TakeOut
기    능: 명함철 명함을 뺀다.
입    력: 위치
출    력: 명함
*************************************************************************************************/
BusinessCard TakeOut(BusinessCardBinder* businessCardBinder, BusinessCard* index) {
	Node* node;
	BusinessCard businessCard;
	
	node = LinearSearchUnique(&(businessCardBinder->businessCards), index, CompareBusinessCards);
	GetAt(&(businessCardBinder->businessCards), node, &businessCard, sizeof(BusinessCard));
	node = Delete(&(businessCardBinder->businessCards), node);
	businessCardBinder->length--;
	if (businessCardBinder->length >= 1) {
		businessCardBinder->current = (BusinessCard*)(businessCardBinder->businessCards.current + 1);
	}
	else {
		businessCardBinder->current = NULL;
	}
	
	return businessCard;
}
/************************************************************************************************
명    칭: BusinessCardBinder_First
기    능: 이동한다
입    력: 없음
출    력: 위치
*************************************************************************************************/
BusinessCard* BusinessCardBinder_First(BusinessCardBinder* businessCardBinder) {
	Node* node;

	node = First(&(businessCardBinder->businessCards));
	businessCardBinder->current = (BusinessCard*)(node + 1);

	return businessCardBinder->current;
}
/************************************************************************************************
명    칭: BusinessCardBinder_Previous
기    능: 이동한다
입    력: 없음
출    력: 위치
*************************************************************************************************/
BusinessCard* BusinessCardBinder_Previous(BusinessCardBinder* businessCardBinder) {
	Node* node;

	node = Previous(&(businessCardBinder->businessCards));
	businessCardBinder->current = (BusinessCard*)(node + 1);

	return businessCardBinder->current;
}
/************************************************************************************************
명    칭: BusinessCardBinder_Next
기    능: 이동한다
입    력: 없음
출    력: 위치
*************************************************************************************************/
BusinessCard* BusinessCardBinder_Next(BusinessCardBinder* businessCardBinder) {
	Node* node;

	node = Next(&(businessCardBinder->businessCards));
	businessCardBinder->current = (BusinessCard*)(node + 1);

	return businessCardBinder->current;
}
/************************************************************************************************
명    칭: BusinessCardBinder_Last
기    능: 이동한다
입    력: 없음
출    력: 위치
*************************************************************************************************/
BusinessCard* BusinessCardBinder_Last(BusinessCardBinder* businessCardBinder) {
	Node* node;

	node = Last(&(businessCardBinder->businessCards));
	businessCardBinder->current = (BusinessCard*)(node + 1);

	return businessCardBinder->current;
}
/************************************************************************************************
명    칭: BusinessCardBinder_Move
기    능: 이동한다
입    력: 없음
출    력: 위치
*************************************************************************************************/
BusinessCard* BusinessCardBinder_Move(BusinessCardBinder* businessCardBinder, BusinessCard *index) {
	Node* node;

	node = LinearSearchUnique(&(businessCardBinder->businessCards), index, CompareBusinessCards);
	Move(&(businessCardBinder->businessCards), node);
	businessCardBinder->current = (BusinessCard*)(node + 1);

	return businessCardBinder->current;
}
/************************************************************************************************
명    칭: Save
기    능: 디스크파일로 명함철을 출력한다.
입    력: 없음
출    력: 개수
*************************************************************************************************/
Long Save(BusinessCardBinder* businessCardBinder) {
	BusinessCard businessCard;
	Company company;
	Node* node;
	Node* previous = NULL;
	Long i;
	size_t flag;
	FILE* personals;
	FILE* companies;

	personals = fopen("Personal.dat", "wb");
	companies = fopen("Company.dat", "wb+");
	if (personals != NULL && companies != NULL) {
		node = First(&(businessCardBinder->businessCards));
		while (node != previous) {
			GetAt(&(businessCardBinder->businessCards), node, &businessCard, sizeof(BusinessCard));
			i = 1;
			fseek(companies, 0L, SEEK_SET);
			flag = fread(&company, sizeof(Company), 1, companies);
			while (!feof(companies) && flag != 0 && strcmp(businessCard.company.name, company.name) != 0) {
				i++;
				flag = fread(&company, sizeof(Company), 1, companies);
			}
			if (flag == 0) {
				fwrite(&(businessCard.company), sizeof(Company), 1, companies);
			}
			fwrite(&i, sizeof(Long), 1, personals);
			fwrite(&(businessCard.personal), sizeof(Personal), 1, personals);
			previous = node;
			node = Next(&(businessCardBinder->businessCards));
		}
		fclose(personals);
		fclose(companies);
	}
	return businessCardBinder->length;
}
/************************************************************************************************
명    칭: BusinessCardBinder_Destroy
기    능: 명함철을 없앤다.
입    력: 없음
출    력: 없음
*************************************************************************************************/
void BusinessCardBinder_Destroy(BusinessCardBinder* businessCardBinder) {
	Destroy(&(businessCardBinder->businessCards));
}

/************************************************************************************************
함수포인터형 변수로 사용될 함수들
************************************************************************************************/
int CompareNames(void* one, void* other) {
	BusinessCard* one_ = (BusinessCard*)one;
	char(*other_) = (char(*))other;

	return strcmp(one_->personal.name, other_);
}
int CompareCompanyNames(void* one, void* other) {
	BusinessCard* one_ = (BusinessCard*)one;
	char(*other_) = (char(*))other;

	return strcmp(one_->company.name, other_);
}
int CompareBusinessCards(void* one, void* other) {
	Long ret;

	if (one == other) {
		ret = 0;
	}
	else {
		ret = -1;
	}
	return ret;
}