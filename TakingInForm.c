//TakingInForm.c
/************************************************************************************************
파일명칭: TakingInForm.c
기    능: 명함철 끼우기 윈도우 관리
작 성 자: 임다니엘
작성일자: 2022/05/30
*************************************************************************************************/
#include "TakingInForm.h"
#include "BusinessCardBinder.h"
#include "IndexCardBinder.h"
#include "IndexCard.h"
#include "resource.h"
#include <stdio.h>
#include <stdlib.h>
#include <WinUser.h>
#include <CommCtrl.h>
#pragma warning (disable:4996)

/************************************************************************************************
명    칭: TakingInFormProc
기    능: 끼우기윈도우 프로시져함수
*************************************************************************************************/
BOOL CALLBACK TakingInFormProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	BOOL ret;

	switch (message) {
	case WM_INITDIALOG:
		ret = TakingInForm_OnInitDialog(hWnd, wParam, lParam);
		break;
	case WM_COMMAND:
		ret = TakingInForm_OnCommand(hWnd, wParam, lParam);
		break;
	case WM_CLOSE:
		ret = TakingInForm_OnClose(hWnd, wParam, lParam);
		break;
	default:
		ret = FALSE;
		break;
	}
	return ret;
}
/************************************************************************************************
명    칭: TakingInForm_OnInitDialog
기    능: WM_INITDIALOG 메세지 발생 시 시나리오
*************************************************************************************************/
BOOL TakingInForm_OnInitDialog(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	Long i;
	TCHAR ranks[][20] = { "사원", "주임", "대리", "과장", "차장", "부장", "이사", "상무", "전무", "사장" };
	TCHAR domains[][16] = { "naver.com", "gmail.com", "hanmail.com" };

	//1.1. 직급 리스트박스에 항목들을 추가한다.

	for (i = 0; i < (sizeof(ranks) / sizeof(ranks[0])); i++) {
		SendMessage(GetDlgItem(hWnd, IDC_COMBO_RANK), CB_ADDSTRING, (WPARAM)0, (LPARAM)ranks[i]);
	}
	//1.2. 이메일주소 리스트박스에 항목들을 추가한다.
	for (i = 0; i < (sizeof(domains) / sizeof(domains[0])); i++) {
		SendMessage(GetDlgItem(hWnd, IDC_COMBO_EMAILADDRESS), CB_ADDSTRING, (WPARAM)0, (LPARAM)domains[i]);
	}
}
/************************************************************************************************
명    칭: TakingInForm_OnCommand
기    능: WM_COMMAND 메세지 발생 시 시나리오
*************************************************************************************************/
BOOL TakingInForm_OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	BOOL ret;

	switch (LOWORD(wParam)) {
	case IDC_BUTTON_TAKEIN:
		ret = TakingInForm_OnTakeInButtonClicked(hWnd, wParam, lParam);
		break;
	case IDC_EDIT_COMPANYNAME:
		ret = TakingInForm_OnCompanyNameKillFocus(hWnd, wParam, lParam);
		break;
	default:
		ret = FALSE;
		break;
	}
}
/************************************************************************************************
명    칭: TakingInForm_OnTakeInButtonClicked
기    능: 끼우기 버튼을 클릭했을 때 시나리오
*************************************************************************************************/
BOOL TakingInForm_OnTakeInButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	HWND hBusinessCardBinderWnd;
	BusinessCardBinder* businessCardBinder;
	BusinessCard* index;
	BusinessCard businessCard;
	IndexCardBinder* indexCardBinder;
	IndexCard* link;
	IndexCard(*indexCards);
	Long count;
	Long location;
	Long i;
	Long j;
	TVINSERTSTRUCT insertItem;
	TVITEM item;
	HTREEITEM hRootItem;
	HTREEITEM hIndexCardItem = (HTREEITEM)TVI_LAST;
	HTREEITEM hNameItem = (HTREEITEM)TVI_LAST;
	TCHAR companyName[16] = "";
	TCHAR email[16];
	TCHAR domain[16];

	if (HIWORD(wParam) == BN_CLICKED) {
		//2.1. 명함을 읽는다.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_NAME), WM_GETTEXT, (WPARAM)11, (LPARAM)businessCard.personal.name);
		SendMessage(GetDlgItem(hWnd, IDC_COMBO_RANK), WM_GETTEXT, (WPARAM)20, (LPARAM)businessCard.personal.rank);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_CELLPHONENUMBER), WM_GETTEXT, (WPARAM)12, (LPARAM)businessCard.personal.cellphoneNumber);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_EMAILADDRESS), WM_GETTEXT, (WPARAM)16, (LPARAM)email);
		SendMessage(GetDlgItem(hWnd, IDC_COMBO_EMAILADDRESS), WM_GETTEXT, (WPARAM)16, (LPARAM)domain);
		sprintf(businessCard.personal.emailAddress, "%s@%s", email, domain);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANYNAME), WM_GETTEXT, (WPARAM)16, (LPARAM)businessCard.company.name);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_ADDRESS), WM_GETTEXT, (WPARAM)32, (LPARAM)businessCard.company.address);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_TELEPHONENUMBER), WM_GETTEXT, (WPARAM)12, (LPARAM)businessCard.company.telephoneNumber);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_FAXNUMBER), WM_GETTEXT, (WPARAM)12, (LPARAM)businessCard.company.faxNumber);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_URL), WM_GETTEXT, (WPARAM)32, (LPARAM)businessCard.company.url);
		//2.2. 명함철 윈도우를 찾는다.
		hBusinessCardBinderWnd = FindWindow("#32770", "명함철");
		//2.3. 명함철 윈도우의 명함철에서 끼운다.
		businessCardBinder = (BusinessCardBinder*)GetWindowLong(hBusinessCardBinderWnd, GWL_USERDATA);
		index = TakeIn(businessCardBinder, businessCard);
		//2.4. 명함철 위도우에 현재 명함을 쓴다.
		SendMessage(GetDlgItem(hBusinessCardBinderWnd, IDC_EDIT_NAME), WM_SETTEXT,
			(WPARAM)0, (LPARAM)index->personal.name);
		SendMessage(GetDlgItem(hBusinessCardBinderWnd, IDC_EDIT_RANK), WM_SETTEXT,
			(WPARAM)0, (LPARAM)index->personal.rank);
		SendMessage(GetDlgItem(hBusinessCardBinderWnd, IDC_EDIT_CELLPHONENUMBER), WM_SETTEXT,
			(WPARAM)0, (LPARAM)index->personal.cellphoneNumber);
		SendMessage(GetDlgItem(hBusinessCardBinderWnd, IDC_EDIT_EMAILADDRESS), WM_SETTEXT,
			(WPARAM)0, (LPARAM)index->personal.emailAddress);
		SendMessage(GetDlgItem(hBusinessCardBinderWnd, IDC_EDIT_COMPANYNAME), WM_SETTEXT,
			(WPARAM)0, (LPARAM)index->company.name);
		SendMessage(GetDlgItem(hBusinessCardBinderWnd, IDC_EDIT_ADDRESS), WM_SETTEXT,
			(WPARAM)0, (LPARAM)index->company.address);
		SendMessage(GetDlgItem(hBusinessCardBinderWnd, IDC_EDIT_TELEPHONENUMBER), WM_SETTEXT,
			(WPARAM)0, (LPARAM)index->company.telephoneNumber);
		SendMessage(GetDlgItem(hBusinessCardBinderWnd, IDC_EDIT_FAXNUMBER), WM_SETTEXT,
			(WPARAM)0, (LPARAM)index->company.faxNumber);
		SendMessage(GetDlgItem(hBusinessCardBinderWnd, IDC_EDIT_URL), WM_SETTEXT,
			(WPARAM)0, (LPARAM)index->company.url);
		//2.5. 명함철 윈도우의 색인카드철에서 끼운다.
		indexCardBinder = (IndexCardBinder*)GetProp(hBusinessCardBinderWnd, "PROP_INDEXCARDBINDER");
		link = IndexCardBinder_TakeIn(indexCardBinder, index);
		//2.6. 명함철 윈도우의 색인카드철에서 정리한다.
		IndexCardBinder_Arrange(indexCardBinder);
		//2.7. 명함철 윈도우의  색인 카드철에서 목록을 만든다.
		MakeList(indexCardBinder, &indexCards, &count);
		//2.8. 명함철 윈도우 트리 뷰 컨트롤의 모든 항목들을 지운다.
		SendMessage(GetDlgItem(hBusinessCardBinderWnd, IDC_TREE_INDEXCARDBINDER), TVM_DELETEITEM, (WPARAM)0, (LPARAM)TVI_ROOT);
		//2.9. 명함철 윈도우 트리 뷰 컨트롤에 루트 항목을 추가한다.
		item.mask = TVIF_TEXT | TVIF_CHILDREN;
		item.pszText = "회사들";
		item.cChildren = 1;
		insertItem.hParent = NULL;
		insertItem.hInsertAfter = (HTREEITEM)TVI_FIRST;
		insertItem.item = item;
		hRootItem = (HTREEITEM)SendMessage(GetDlgItem(hBusinessCardBinderWnd, IDC_TREE_INDEXCARDBINDER),
			TVM_INSERTITEM, (WPARAM)0, (LPARAM)&insertItem);
		//2.10. 개수만큼 반복한다.
		i = 0;
		while (i < count) {
			//2.10.1. 명함철 윈도우 트리 뷰 컨트롤에 상호 항목을 추가한다.
			link = &(indexCards[i]);
			item.mask = TVIF_TEXT | TVIF_CHILDREN;
			item.pszText = link->companyName;
			item.cChildren = 1;
			insertItem.hParent = hRootItem;
			insertItem.hInsertAfter = hIndexCardItem;
			insertItem.item = item;
			hIndexCardItem = (HTREEITEM)SendMessage(GetDlgItem(hBusinessCardBinderWnd, IDC_TREE_INDEXCARDBINDER),
				TVM_INSERTITEM, (WPARAM)0, (LPARAM)&insertItem);
			//2.10.2. 명함의 개수만큼 반복한다.
			j = 0;
			while (j < link->length) {
				//2.10.2.1. 명함철 윈도우 트리 뷰 컨트롤에 성명 항목을 추가한다.
				index = IndexCard_GetAt(link, j);
				item.mask = TVIF_TEXT | TVIF_CHILDREN;
				item.pszText = index->personal.name;
				item.cChildren = 0;
				insertItem.hParent = hIndexCardItem;
				insertItem.hInsertAfter = hNameItem;
				insertItem.item = item;
				hNameItem = (HTREEITEM)SendMessage(GetDlgItem(hBusinessCardBinderWnd, IDC_TREE_INDEXCARDBINDER),
					TVM_INSERTITEM, (WPARAM)0, (LPARAM)&insertItem);
				j++;
			}
			i++;
		}
		if (indexCards != NULL) {
			free(indexCards);
		}
		//2.11. 명함철 윈도우의 명함철에서 현재위치를 읽는다.
		index = businessCardBinder->current;
		//2.12. 명함철 윈도우의 색인 카드철에서 찾는다.
		link = IndexCardBinder_Find(indexCardBinder, index->company.name);
		//2.13. 명함철 윈도우의 색인 카드에서 찾는다.
		location = IndexCard_Find(link, index);
		//2.14. 명함철 윈도우 트리 뷰 컨트롤의 루트 항목을 펼친다.
		SendMessage(GetDlgItem(hBusinessCardBinderWnd, IDC_TREE_INDEXCARDBINDER), TVM_EXPAND, (WPARAM)0, (LPARAM)hRootItem);
		//2.15. 명함철 윈도우 트리 뷰 컨트롤에서 상호 항목을 찾는다.
		hIndexCardItem = (HTREEITEM)SendMessage(GetDlgItem(hBusinessCardBinderWnd, IDC_TREE_INDEXCARDBINDER),
			TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hRootItem);
		item.mask = TVIF_TEXT | TVIF_HANDLE;
		item.hItem = hIndexCardItem;
		item.pszText = companyName;
		item.cchTextMax = 16;
		SendMessage(GetDlgItem(hBusinessCardBinderWnd, IDC_TREE_INDEXCARDBINDER), TVM_GETITEM, (WPARAM)0, (LPARAM)&item);
		while (hIndexCardItem != NULL && strcmp(index->company.name, companyName) != 0) {
			hIndexCardItem = (HTREEITEM)SendMessage(GetDlgItem(hBusinessCardBinderWnd, IDC_TREE_INDEXCARDBINDER),
				TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)hIndexCardItem);
			item.hItem = hIndexCardItem;
			SendMessage(GetDlgItem(hBusinessCardBinderWnd, IDC_TREE_INDEXCARDBINDER), TVM_GETITEM, (WPARAM)0, (LPARAM)&item);
		}
		//2.16. 명함철 윈도우 트리 뷰 컨트롤의 상호 항목을 펼친다.
		SendMessage(GetDlgItem(hBusinessCardBinderWnd, IDC_TREE_INDEXCARDBINDER), TVM_EXPAND, (WPARAM)0, (LPARAM)hIndexCardItem);
		//2.17. 명함철 윈도우 트리 뷰 컨트롤의 상호 항목에서 이동한다.
		hNameItem = (HTREEITEM)SendMessage(GetDlgItem(hBusinessCardBinderWnd, IDC_TREE_INDEXCARDBINDER),
			TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hIndexCardItem);
		i = 0;
		while (hNameItem != NULL && i < location) {
			hNameItem = (HTREEITEM)SendMessage(GetDlgItem(hBusinessCardBinderWnd, IDC_TREE_INDEXCARDBINDER),
				TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)hNameItem);
			i++;
		}
		//2.18. 명함철 윈도우 트리뷰 컨트롤의 성명 항목을 선택한다.
		item.mask = TVIF_STATE | TVIF_HANDLE;
		item.stateMask = TVIS_SELECTED;
		item.state = TVIS_SELECTED;
		item.hItem = hNameItem;
		SendMessage(GetDlgItem(hBusinessCardBinderWnd, IDC_TREE_INDEXCARDBINDER), TVM_SETITEM, (WPARAM)0, (LPARAM)&item);
		SendMessage(GetDlgItem(hBusinessCardBinderWnd, IDC_TREE_INDEXCARDBINDER), TVM_SELECTITEM, (WPARAM)TVGN_CARET, (LPARAM)hNameItem);
		
		//2.19. 끼우기 윈도우를 닫는다.
		EndDialog(hWnd, 0);
	}
	return TRUE;
}
/************************************************************************************************
명    칭: TakingInForm_OnCompanyNameKillFocus
기    능: 상호의 포커스를 잃었을 때 시나리오
*************************************************************************************************/
BOOL TakingInForm_OnCompanyNameKillFocus(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	HWND hBusinessCardBinderWnd;
	BusinessCardBinder* businessCardBinder;
	BusinessCard* index;
	TCHAR companyName[16];

	if (HIWORD(wParam) == EN_KILLFOCUS) {
		//3.1. 상호를 읽는다.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANYNAME), WM_GETTEXT, (WPARAM)16, (LPARAM)companyName);
		//3.2. 명함철 윈도우를 찾는다.
		hBusinessCardBinderWnd = FindWindow("#32770", "명함철");
		//3.3. 명함철 윈도우의 명함철에서 상호로 찾는다.
		businessCardBinder = (BusinessCardBinder*)GetWindowLong(hBusinessCardBinderWnd, GWL_USERDATA);
		index = FindByCompanyName(businessCardBinder, companyName);
		//3.4. 상호가 있으면
		if (index != NULL) {
			//3.4.1. 회사에 쓴다.
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_ADDRESS), WM_SETTEXT,
				(WPARAM)0, (LPARAM)index->company.address);
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_TELEPHONENUMBER), WM_SETTEXT,
				(WPARAM)0, (LPARAM)index->company.telephoneNumber);
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_FAXNUMBER), WM_SETTEXT,
				(WPARAM)0, (LPARAM)index->company.faxNumber);
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_URL), WM_SETTEXT,
				(WPARAM)0, (LPARAM)index->company.url);
		}
	}
	return TRUE;
}
/************************************************************************************************
명    칭: TakingInForm_OnClose
기    능: WM_CLOSE 메세지 발생 시 시나리오
*************************************************************************************************/
BOOL TakingInForm_OnClose(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	//4.1. 끼우기 윈도우를 닫는다.
	EndDialog(hWnd, 0);

	return TRUE;
}