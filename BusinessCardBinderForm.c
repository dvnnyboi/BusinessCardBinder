//BusinessCardBinderForm.c
/************************************************************************************************
파일명칭: BusinessCardBinderForm.c
기    능: 명함철 메인윈도우 관리
작 성 자: 임다니엘
작성일자: 2022/05/30
*************************************************************************************************/
#include "BusinessCardBinderForm.h"
#include "TakingInForm.h"
#include "FindingForm.h"
#include "TakingOutForm.h"
#include "BusinessCardBinder.h"
#include "IndexCardBinder.h"
#include "IndexCard.h"
#include "resource.h"
#include <stdio.h>
#include <stdlib.h>
#include <WinUser.h>
#include <CommCtrl.h>
#pragma warning (disable:4996)

//WinMain 함수 정의
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	int response;
	response = DialogBox(hInstance, MAKEINTRESOURCE(IDD_BUSINESSCARDBINDERFORM), NULL, BusinessCardBinderFormProc);

	return response;
}
/************************************************************************************************
명    칭: BusinessCardBinderFormProc
기    능: 메인윈도우 프로시져함수
*************************************************************************************************/
BOOL CALLBACK BusinessCardBinderFormProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	BOOL ret;

	switch (message) {
	case WM_INITDIALOG:
		ret = BusinessCardBinderForm_OnInitDialog(hWnd, wParam, lParam);
		break;
	case WM_COMMAND:
		ret = BusinessCardBinderForm_OnCommand(hWnd, wParam, lParam);
		break;
	case WM_NOTIFY:
		ret = BusinessCardBinderForm_OnNotify(hWnd, wParam, lParam);
		break;
	case WM_CLOSE:
		ret = BusinessCardBinderForm_OnClose(hWnd, wParam, lParam);
		break;
	default:
		ret = FALSE;
		break;
	}
	return ret;
}
/************************************************************************************************
명    칭: BusinessCardBinderForm_OnInitDialog
기    능: WM_INITDIALOG 메세지 발생 시 시나리오
*************************************************************************************************/
BOOL BusinessCardBinderForm_OnInitDialog(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	BusinessCardBinder* businessCardBinder;
	BusinessCard* index;
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

	//1.1.명함철을 만든다.
	businessCardBinder = (BusinessCardBinder*)malloc(sizeof(BusinessCardBinder));
	BusinessCardBinder_Create(businessCardBinder);
	SetWindowLong(hWnd, GWL_USERDATA, (LONG)businessCardBinder);
	//1.2. 색인카드철을 만든다.
	indexCardBinder = (IndexCardBinder*)malloc(sizeof(IndexCardBinder));
	IndexCardBinder_Create(indexCardBinder);
	SetProp(hWnd, "PROP_INDEXCARDBINDER", (HANDLE)indexCardBinder);
	//1.3. 트리 뷰 컨트롤에 루트 항목을 추가한다.
	item.mask = TVIF_TEXT | TVIF_CHILDREN;
	item.pszText = "회사들";
	item.cChildren = 1;
	insertItem.hParent = NULL;
	insertItem.hInsertAfter = (HTREEITEM)TVI_FIRST;
	insertItem.item = item;
	hRootItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER), TVM_INSERTITEM, (WPARAM)0, (LPARAM)&insertItem);
	//1.4. 명함철에서 적재한다.
	count = Load(businessCardBinder);
	if (count > 0) {
		//1.5. 개수만큼 반복한다.
		index = BusinessCardBinder_First(businessCardBinder);
		i = 0;
		while (i < count) {
			//1.5.1. 색인카드철에서 명함을 끼운다.
			link = IndexCardBinder_TakeIn(indexCardBinder, index);
			index = BusinessCardBinder_Next(businessCardBinder);
			i++;
		}
		//1.6. 색인카드철에서 정리한다.
		IndexCardBinder_Arrange(indexCardBinder);
		//1.7. 색인카드철에서 목록을 만든다.
		MakeList(indexCardBinder, &indexCards, &count);
		//1.8. 개수만큼 반복한다.
		i = 0;
		while (i < count) {
			//1.8.1. 트리 뷰 컨트롤에 상호 항목을 추가한다.
			link = &(indexCards[i]);
			item.mask = TVIF_TEXT | TVIF_CHILDREN;
			item.pszText = link->companyName;
			item.cChildren = 1;
			insertItem.hParent = hRootItem;
			insertItem.hInsertAfter = hIndexCardItem;
			insertItem.item = item;
			hIndexCardItem= (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER), TVM_INSERTITEM, (WPARAM)0, (LPARAM)&insertItem);
			//1.8.2. 명함의 개수만큼 반복한다.
			j = 0;
			while (j < link->length) {
				//1.8.2.1. 트리 뷰 컨트롤에 성명 항목을 추가한다.
				index = IndexCard_GetAt(link, j);
				item.mask = TVIF_TEXT | TVIF_CHILDREN;
				item.pszText = index->personal.name;
				item.cChildren = 0;
				insertItem.hParent = hIndexCardItem;
				insertItem.hInsertAfter = hNameItem;
				insertItem.item = item;
				hNameItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER), TVM_INSERTITEM, (WPARAM)0, (LPARAM)&insertItem);
				j++;
			}
			i++;
		}
		if (indexCards != NULL) {
			free(indexCards);
		}
		//1.9. 명함철에서 첫 명함으로 이동한다.
		index = BusinessCardBinder_First(businessCardBinder);
		//1.10. 현재 명함을 쓴다.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_NAME), WM_SETTEXT, (WPARAM)0, (LPARAM)index->personal.name);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_RANK), WM_SETTEXT, (WPARAM)0, (LPARAM)index->personal.rank);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_CELLPHONENUMBER), WM_SETTEXT, (WPARAM)0, (LPARAM)index->personal.cellphoneNumber);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_EMAILADDRESS), WM_SETTEXT, (WPARAM)0, (LPARAM)index->personal.emailAddress);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANYNAME), WM_SETTEXT, (WPARAM)0, (LPARAM)index->company.name);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_ADDRESS), WM_SETTEXT, (WPARAM)0, (LPARAM)index->company.address);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_TELEPHONENUMBER), WM_SETTEXT, (WPARAM)0, (LPARAM)index->company.telephoneNumber);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_FAXNUMBER), WM_SETTEXT, (WPARAM)0, (LPARAM)index->company.faxNumber);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_URL), WM_SETTEXT, (WPARAM)0, (LPARAM)index->company.url);
		//1.11. 색인 카드철에서 찾는다.
		link = IndexCardBinder_Find(indexCardBinder, index->company.name);
		//1.12. 색인 카드에서 찾는다.
		location = IndexCard_Find(link, index);
		//1.13. 트리 뷰 컨트롤의 루트 항목을 펼친다.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER), TVM_EXPAND, (WPARAM)0, (LPARAM)hRootItem);
		//1.14. 트리 뷰 컨트롤에서 상호 항목을 찾는다.
		hIndexCardItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER),
			TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hRootItem);
		item.mask = TVIF_TEXT | TVIF_HANDLE;
		item.hItem = hIndexCardItem;
		item.pszText = companyName;
		item.cchTextMax = 16;
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER), TVM_GETITEM, (WPARAM)0, (LPARAM)&item);
		while (hIndexCardItem != NULL && strcmp(index->company.name, companyName) != 0) {
			hIndexCardItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER),
				TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)hIndexCardItem);
			item.hItem = hIndexCardItem;
			SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER), TVM_GETITEM, (WPARAM)0, (LPARAM)&item);
		}
		//1.15. 트리 뷰 컨트롤의 상호 항목을 펼친다.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER), TVM_EXPAND, (WPARAM)0, (LPARAM)hIndexCardItem);
		//1.16. 트리 뷰 컨트롤의 상호 항목에서 이동한다.
		hNameItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hIndexCardItem);
		i = 0;
		while (hNameItem != NULL && i < location) {
			hNameItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)hNameItem);
			i++;
		}
		//1.17. 트리뷰 컨트롤의 성명 항목을 선택한다.
		item.mask = TVIF_STATE | TVIF_HANDLE;
		item.stateMask = TVIS_SELECTED;
		item.state = TVIS_SELECTED;
		item.hItem = hNameItem;
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER), TVM_SETITEM, (WPARAM)0, (LPARAM)&item);
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER), TVM_SELECTITEM, (WPARAM)TVGN_CARET, (LPARAM)hNameItem);
	}
	return TRUE;
}
/************************************************************************************************
명    칭: BusinessCardBinderForm_OnCommand
기    능: WM_COMMAND 메세지 발생 시 시나리오
*************************************************************************************************/
BOOL BusinessCardBinderForm_OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	BOOL ret;

	switch (LOWORD(wParam)) {
	case IDC_BUTTON_TAKEIN:
		ret = BusinessCardBinderForm_OnTakeInButtonClicked(hWnd, wParam, lParam);
		break;
	case IDC_BUTTON_FIND:
		ret = BusinessCardBinderForm_OnFindButtonClicked(hWnd, wParam, lParam);
		break;
	case IDC_BUTTON_TAKEOUT:
		ret = BusinessCardBinderForm_OnTakeOutButtonClicked(hWnd, wParam, lParam);
		break;
	case IDC_BUTTON_ARRANGE:
		ret = BusinessCardBinderForm_OnArrangeButtonClicked(hWnd, wParam, lParam);
		break;
	case IDC_BUTTON_FIRST:
		ret = BusinessCardBinderForm_OnFirstButtonClicked(hWnd, wParam, lParam);
		break;
	case IDC_BUTTON_PREVIOUS:
		ret = BusinessCardBinderForm_OnPreviousButtonClicked(hWnd, wParam, lParam);
		break;
	case IDC_BUTTON_NEXT:
		ret = BusinessCardBinderForm_OnNextButtonClicked(hWnd, wParam, lParam);
		break;
	case IDC_BUTTON_LAST:
		ret = BusinessCardBinderForm_OnLastButtonClicked(hWnd, wParam, lParam);
		break;
	default:
		ret = FALSE;
		break;
	}
	return ret;
}
/************************************************************************************************
명    칭: BusinessCardBinderForm_OnTakeInBunttonClicked
기    능: 끼우기 버튼을 클릭했을 때 시나리오
*************************************************************************************************/
BOOL BusinessCardBinderForm_OnTakeInButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	//2.1. 끼우기 윈도우를 출력한다.
	if (HIWORD(wParam) == BN_CLICKED) {
		DialogBox((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDD_TAKINGINFORM), NULL, TakingInFormProc);
	}
	return TRUE;
}
/************************************************************************************************
명    칭: BusinessCardBinderForm_OnFindBunttonClicked
기    능: 찾기 버튼을 클릭했을 때 시나리오
*************************************************************************************************/
BOOL BusinessCardBinderForm_OnFindButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	//3.1. 찾기 윈도우를 출력한다.
	if (HIWORD(wParam) == BN_CLICKED) {
		DialogBox((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDD_FINDINGFORM), NULL, FindingFormProc);
	}
	return TRUE;
}
/************************************************************************************************
명    칭: BusinessCardBinderForm_OnTakeOutBunttonClicked
기    능: 빼기 버튼을 클릭했을 때 시나리오
*************************************************************************************************/
BOOL BusinessCardBinderForm_OnTakeOutButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	BusinessCardBinder* businessCardBinder;

	businessCardBinder = (BusinessCardBinder*)GetWindowLong(hWnd, GWL_USERDATA);
	if (businessCardBinder->length > 0) {
		//4.1. 빼기 윈도우를 출력한다.
		if (HIWORD(wParam) == BN_CLICKED) {
			DialogBox((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDD_TAKINGOUTFORM), NULL, TakingOutFormProc);
		}
	}
	return TRUE;
}
/************************************************************************************************
명    칭: BusinessCardBinderForm_OnArrangeBunttonClicked
기    능: 정리하기 버튼을 클릭했을 때 시나리오
*************************************************************************************************/
BOOL BusinessCardBinderForm_OnArrangeButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	BusinessCardBinder* businessCardBinder;
	BusinessCard* index;
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

	if (HIWORD(wParam) == BN_CLICKED) {
		//5.1. 색인카드철에서 정리한다.
		indexCardBinder = (IndexCardBinder*)GetProp(hWnd, "PROP_INDEXCARDBINDER");
		IndexCardBinder_Arrange(indexCardBinder);
		//5.2. 색인 카드철에서 목록을 만든다.
		MakeList(indexCardBinder, &indexCards, &count);
		//5.3. 트리 뷰 컨트롤의 모든 항목들을 지운다.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER), TVM_DELETEITEM, (WPARAM)0, (LPARAM)TVI_ROOT);
		//5.4. 트리 뷰 컨트롤에 루트 항목을 추가한다.
		item.mask = TVIF_TEXT | TVIF_CHILDREN;
		item.pszText = "회사들";
		item.cChildren = 1;
		insertItem.hParent = NULL;
		insertItem.hInsertAfter = (HTREEITEM)TVI_FIRST;
		insertItem.item = item;
		hRootItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER), TVM_INSERTITEM, (WPARAM)0, (LPARAM)&insertItem);
		//5.5. 개수만큼 반복한다.
		i = 0;
		while (i < count) {
			//5.5.1. 트리 뷰 컨트롤에 상호 항목을 추가한다.
			link = &(indexCards[i]);
			item.mask = TVIF_TEXT | TVIF_CHILDREN;
			item.pszText = link->companyName;
			item.cChildren = 1;
			insertItem.hParent = hRootItem;
			insertItem.hInsertAfter = hIndexCardItem;
			insertItem.item = item;
			hIndexCardItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER), TVM_INSERTITEM, (WPARAM)0, (LPARAM)&insertItem);
			//5.5.2. 명함의 개수만큼 반복한다.
			j = 0;
			while (j < link->length) {
				//5.5.2.1. 트리 뷰 컨트롤에 성명 항목을 추가한다.
				index = IndexCard_GetAt(link, j);
				item.mask = TVIF_TEXT | TVIF_CHILDREN;
				item.pszText = index->personal.name;
				item.cChildren = 0;
				insertItem.hParent = hIndexCardItem;
				insertItem.hInsertAfter = hNameItem;
				insertItem.item = item;
				hNameItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER), TVM_INSERTITEM, (WPARAM)0, (LPARAM)&insertItem);
				j++;
			}
			i++;
		}
		if (indexCards != NULL) {
			free(indexCards);
		}
		//5.6. 명함철에서 처음으로 이동한다.
		businessCardBinder = (BusinessCardBinder*)GetWindowLong(hWnd, GWL_USERDATA);
		index = BusinessCardBinder_First(businessCardBinder);
		
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_NAME), WM_SETTEXT, (WPARAM)0, (LPARAM)index->personal.name);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_RANK), WM_SETTEXT, (WPARAM)0, (LPARAM)index->personal.rank);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_CELLPHONENUMBER), WM_SETTEXT, (WPARAM)0, (LPARAM)index->personal.cellphoneNumber);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_EMAILADDRESS), WM_SETTEXT, (WPARAM)0, (LPARAM)index->personal.emailAddress);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANYNAME), WM_SETTEXT, (WPARAM)0, (LPARAM)index->company.name);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_ADDRESS), WM_SETTEXT, (WPARAM)0, (LPARAM)index->company.address);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_TELEPHONENUMBER), WM_SETTEXT, (WPARAM)0, (LPARAM)index->company.telephoneNumber);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_FAXNUMBER), WM_SETTEXT, (WPARAM)0, (LPARAM)index->company.faxNumber);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_URL), WM_SETTEXT, (WPARAM)0, (LPARAM)index->company.url);
		//5.7. 색인 카드철에서 찾는다.
		link = IndexCardBinder_Find(indexCardBinder, index->company.name);
		//5.8. 색인 카드에서 찾는다.
		location = IndexCard_Find(link, index);
		//5.9. 트리 뷰 컨트롤의 루트 항목을 펼친다.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER), TVM_EXPAND, (WPARAM)0, (LPARAM)hRootItem);
		//5.10. 트리 뷰 컨트롤에서 상호 항목을 찾는다.
		hIndexCardItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER),
			TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hRootItem);
		item.mask = TVIF_TEXT | TVIF_HANDLE;
		item.hItem = hIndexCardItem;
		item.pszText = companyName;
		item.cchTextMax = 16;
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER), TVM_GETITEM, (WPARAM)0, (LPARAM)&item);
		while (hIndexCardItem != NULL && strcmp(index->company.name, companyName) != 0) {
			hIndexCardItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER),
				TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)hIndexCardItem);
			item.hItem = hIndexCardItem;
			SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER), TVM_GETITEM, (WPARAM)0, (LPARAM)&item);
		}
		//5.11. 트리 뷰 컨트롤의 상호 항목을 펼친다.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER), TVM_EXPAND, (WPARAM)0, (LPARAM)hIndexCardItem);
		//5.12. 트리 뷰 컨트롤의 상호 항목에서 이동한다.
		hNameItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hIndexCardItem);
		i = 0;
		while (hNameItem != NULL && i < location) {
			hNameItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)hNameItem);
			i++;
		}
		//5.13. 트리뷰 컨트롤의 성명 항목을 선택한다.
		item.mask = TVIF_STATE | TVIF_HANDLE;
		item.stateMask = TVIS_SELECTED;
		item.state = TVIS_SELECTED;
		item.hItem = hNameItem;
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER), TVM_SETITEM, (WPARAM)0, (LPARAM)&item);
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER), TVM_SELECTITEM, (WPARAM)TVGN_CARET, (LPARAM)hNameItem);
	}

	return TRUE;
}
/************************************************************************************************
명    칭: FindingForm_OnFisrtButtonClicked
기    능: 처음 버튼을 클릭했을 때 시나리오
*************************************************************************************************/
BOOL BusinessCardBinderForm_OnFirstButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	BusinessCardBinder* businessCardBinder;
	BusinessCard* index;
	IndexCardBinder* indexCardBinder;
	IndexCard* link;
	IndexCard(*indexCards);
	Long location;
	Long i;
	TVITEM item;
	HTREEITEM hRootItem;
	HTREEITEM hIndexCardItem;
	HTREEITEM hNameItem;
	TCHAR companyName[16] = "";

	if(HIWORD(wParam) == BN_CLICKED) {
		businessCardBinder = (BusinessCardBinder*)GetWindowLong(hWnd, GWL_USERDATA);
		//6.1. 명함철에서 첫 명함으로 이동한다.
		index = BusinessCardBinder_First(businessCardBinder);
		//6.2. 현재 명함을 쓴다.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_NAME), WM_SETTEXT,
			(WPARAM)0, (LPARAM)index->personal.name);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_RANK), WM_SETTEXT,
			(WPARAM)0, (LPARAM)index->personal.rank);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_CELLPHONENUMBER), WM_SETTEXT,
			(WPARAM)0, (LPARAM)index->personal.cellphoneNumber);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_EMAILADDRESS), WM_SETTEXT,
			(WPARAM)0, (LPARAM)index->personal.emailAddress);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANYNAME), WM_SETTEXT,
			(WPARAM)0, (LPARAM)index->company.name);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_ADDRESS), WM_SETTEXT,
			(WPARAM)0, (LPARAM)index->company.address);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_TELEPHONENUMBER), WM_SETTEXT,
			(WPARAM)0, (LPARAM)index->company.telephoneNumber);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_FAXNUMBER), WM_SETTEXT,
			(WPARAM)0, (LPARAM)index->company.faxNumber);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_URL), WM_SETTEXT,
			(WPARAM)0, (LPARAM)index->company.url);
		//6.3. 색인 카드철에서 찾는다.
		indexCardBinder = (IndexCardBinder*)GetProp(hWnd, "PROP_INDEXCARDBINDER");
		link = IndexCardBinder_Find(indexCardBinder, index->company.name);
		//6.4. 색인 카드에서 찾는다.
		location = IndexCard_Find(link, index);
		//6.5. 트리 뷰 컨트롤에서 상호 항목을 찾는다.
		hRootItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER),
			TVM_GETNEXTITEM, (WPARAM)TVGN_ROOT, (LPARAM)0);
		hIndexCardItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER),
			TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hRootItem);
		item.mask = TVIF_TEXT | TVIF_HANDLE;
		item.hItem = hIndexCardItem;
		item.pszText = companyName;
		item.cchTextMax = 16;
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER), TVM_GETITEM, (WPARAM)0, (LPARAM)&item);
		while (hIndexCardItem != NULL && strcmp(index->company.name, companyName) != 0) {
			hIndexCardItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER),
				TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)hIndexCardItem);
			item.hItem = hIndexCardItem;
			SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER), TVM_GETITEM, (WPARAM)0, (LPARAM)&item);
		}
		//6.6. 트리 뷰 컨트롤의 상호 항목을 펼친다.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER), TVM_EXPAND, (WPARAM)0, (LPARAM)hIndexCardItem);
		//6.7. 트리 뷰 컨트롤의 상호 항목에서 이동한다.
		hNameItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hIndexCardItem);
		i = 0;
		while (hNameItem != NULL && i < location) {
			hNameItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)hNameItem);
			i++;
		}
		//6.8. 트리뷰 컨트롤의 성명 항목을 선택한다.
		item.mask = TVIF_STATE | TVIF_HANDLE;
		item.stateMask = TVIS_SELECTED;
		item.state = TVIS_SELECTED;
		item.hItem = hNameItem;
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER), TVM_SETITEM, (WPARAM)0, (LPARAM)&item);
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER), TVM_SELECTITEM, (WPARAM)TVGN_CARET, (LPARAM)hNameItem);
	}
	return TRUE;
}
/************************************************************************************************
명    칭: FindingForm_OnPreviousButtonClicked
기    능: 이전 버튼을 클릭했을 때 시나리오
*************************************************************************************************/
BOOL BusinessCardBinderForm_OnPreviousButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	BusinessCardBinder* businessCardBinder;
	BusinessCard* index;
	IndexCardBinder* indexCardBinder;
	IndexCard* link;
	IndexCard(*indexCards);
	Long location;
	Long i;
	TVITEM item;
	HTREEITEM hRootItem;
	HTREEITEM hIndexCardItem;
	HTREEITEM hNameItem;
	TCHAR companyName[16] = "";

	if (HIWORD(wParam) == BN_CLICKED) {
		businessCardBinder = (BusinessCardBinder*)GetWindowLong(hWnd, GWL_USERDATA);
		//7.1. 명함철에서 이전 명함으로 이동한다.
		index = BusinessCardBinder_Previous(businessCardBinder);
		//7.2. 현재 명함을 쓴다.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_NAME), WM_SETTEXT,
			(WPARAM)0, (LPARAM)index->personal.name);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_RANK), WM_SETTEXT,
			(WPARAM)0, (LPARAM)index->personal.rank);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_CELLPHONENUMBER), WM_SETTEXT,
			(WPARAM)0, (LPARAM)index->personal.cellphoneNumber);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_EMAILADDRESS), WM_SETTEXT,
			(WPARAM)0, (LPARAM)index->personal.emailAddress);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANYNAME), WM_SETTEXT,
			(WPARAM)0, (LPARAM)index->company.name);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_ADDRESS), WM_SETTEXT,
			(WPARAM)0, (LPARAM)index->company.address);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_TELEPHONENUMBER), WM_SETTEXT,
			(WPARAM)0, (LPARAM)index->company.telephoneNumber);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_FAXNUMBER), WM_SETTEXT,
			(WPARAM)0, (LPARAM)index->company.faxNumber);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_URL), WM_SETTEXT,
			(WPARAM)0, (LPARAM)index->company.url);
		//7.3. 색인 카드철에서 찾는다.
		indexCardBinder = (IndexCardBinder*)GetProp(hWnd, "PROP_INDEXCARDBINDER");
		link = IndexCardBinder_Find(indexCardBinder, index->company.name);
		//7.4. 색인 카드에서 찾는다.
		location = IndexCard_Find(link, index);
		//7.5. 트리 뷰 컨트롤에서 상호 항목을 찾는다.
		hRootItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER),
			TVM_GETNEXTITEM, (WPARAM)TVGN_ROOT, (LPARAM)0);
		hIndexCardItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER),
			TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hRootItem);
		item.mask = TVIF_TEXT | TVIF_HANDLE;
		item.hItem = hIndexCardItem;
		item.pszText = companyName;
		item.cchTextMax = 16;
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER), TVM_GETITEM, (WPARAM)0, (LPARAM)&item);
		while (hIndexCardItem != NULL && strcmp(index->company.name, companyName) != 0) {
			hIndexCardItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER),
				TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)hIndexCardItem);
			item.hItem = hIndexCardItem;
			SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER), TVM_GETITEM, (WPARAM)0, (LPARAM)&item);
		}
		//7.6. 트리 뷰 컨트롤의 상호 항목을 펼친다.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER), TVM_EXPAND, (WPARAM)0, (LPARAM)hIndexCardItem);
		//7.7. 트리 뷰 컨트롤의 상호 항목에서 이동한다.
		hNameItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hIndexCardItem);
		i = 0;
		while (hNameItem != NULL && i < location) {
			hNameItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)hNameItem);
			i++;
		}
		//7.8. 트리뷰 컨트롤의 성명 항목을 선택한다.
		item.mask = TVIF_STATE | TVIF_HANDLE;
		item.stateMask = TVIS_SELECTED;
		item.state = TVIS_SELECTED;
		item.hItem = hNameItem;
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER), TVM_SETITEM, (WPARAM)0, (LPARAM)&item);
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER), TVM_SELECTITEM, (WPARAM)TVGN_CARET, (LPARAM)hNameItem);
	}
	return TRUE;
}
/************************************************************************************************
명    칭: FindingForm_OnNextButtonClicked
기    능: 다음 버튼을 클릭했을 때 시나리오
*************************************************************************************************/
BOOL BusinessCardBinderForm_OnNextButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	BusinessCardBinder* businessCardBinder;
	BusinessCard* index;
	IndexCardBinder* indexCardBinder;
	IndexCard* link;
	IndexCard(*indexCards);
	Long location;
	Long i;
	TVITEM item;
	HTREEITEM hRootItem;
	HTREEITEM hIndexCardItem;
	HTREEITEM hNameItem;
	TCHAR companyName[16] = "";

	if (HIWORD(wParam) == BN_CLICKED) {
		businessCardBinder = (BusinessCardBinder*)GetWindowLong(hWnd, GWL_USERDATA);
		//8.1. 명함철에서 다음 명함으로 이동한다.
		index = BusinessCardBinder_Next(businessCardBinder);
		//8.2. 현재 명함을 쓴다.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_NAME), WM_SETTEXT,
			(WPARAM)0, (LPARAM)index->personal.name);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_RANK), WM_SETTEXT,
			(WPARAM)0, (LPARAM)index->personal.rank);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_CELLPHONENUMBER), WM_SETTEXT,
			(WPARAM)0, (LPARAM)index->personal.cellphoneNumber);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_EMAILADDRESS), WM_SETTEXT,
			(WPARAM)0, (LPARAM)index->personal.emailAddress);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANYNAME), WM_SETTEXT,
			(WPARAM)0, (LPARAM)index->company.name);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_ADDRESS), WM_SETTEXT,
			(WPARAM)0, (LPARAM)index->company.address);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_TELEPHONENUMBER), WM_SETTEXT,
			(WPARAM)0, (LPARAM)index->company.telephoneNumber);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_FAXNUMBER), WM_SETTEXT,
			(WPARAM)0, (LPARAM)index->company.faxNumber);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_URL), WM_SETTEXT,
			(WPARAM)0, (LPARAM)index->company.url);
		//8.3. 색인 카드철에서 찾는다.
		indexCardBinder = (IndexCardBinder*)GetProp(hWnd, "PROP_INDEXCARDBINDER");
		link = IndexCardBinder_Find(indexCardBinder, index->company.name);
		//8.4. 색인 카드에서 찾는다.
		location = IndexCard_Find(link, index);
		//8.5. 트리 뷰 컨트롤에서 상호 항목을 찾는다.
		hRootItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER),
			TVM_GETNEXTITEM, (WPARAM)TVGN_ROOT, (LPARAM)0);
		hIndexCardItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER),
			TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hRootItem);
		item.mask = TVIF_TEXT | TVIF_HANDLE;
		item.hItem = hIndexCardItem;
		item.pszText = companyName;
		item.cchTextMax = 16;
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER), TVM_GETITEM, (WPARAM)0, (LPARAM)&item);
		while (hIndexCardItem != NULL && strcmp(index->company.name, companyName) != 0) {
			hIndexCardItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER),
				TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)hIndexCardItem);
			item.hItem = hIndexCardItem;
			SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER), TVM_GETITEM, (WPARAM)0, (LPARAM)&item);
		}
		//8.6. 트리 뷰 컨트롤의 상호 항목을 펼친다.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER), TVM_EXPAND, (WPARAM)0, (LPARAM)hIndexCardItem);
		//8.7. 트리 뷰 컨트롤의 상호 항목에서 이동한다.
		hNameItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hIndexCardItem);
		i = 0;
		while (hNameItem != NULL && i < location) {
			hNameItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)hNameItem);
			i++;
		}
		//8.8. 트리뷰 컨트롤의 성명 항목을 선택한다.
		item.mask = TVIF_STATE | TVIF_HANDLE;
		item.stateMask = TVIS_SELECTED;
		item.state = TVIS_SELECTED;
		item.hItem = hNameItem;
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER), TVM_SETITEM, (WPARAM)0, (LPARAM)&item);
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER), TVM_SELECTITEM, (WPARAM)TVGN_CARET, (LPARAM)hNameItem);
	}
	return TRUE;
}
/************************************************************************************************
명    칭: FindingForm_OnLastButtonClicked
기    능: 마지막 버튼을 클릭했을 때 시나리오
*************************************************************************************************/
BOOL BusinessCardBinderForm_OnLastButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	BusinessCardBinder* businessCardBinder;
	BusinessCard* index;
	IndexCardBinder* indexCardBinder;
	IndexCard* link;
	IndexCard(*indexCards);
	Long location;
	Long i;
	TVITEM item;
	HTREEITEM hRootItem;
	HTREEITEM hIndexCardItem;
	HTREEITEM hNameItem;
	TCHAR companyName[16] = "";

	if (HIWORD(wParam) == BN_CLICKED) {
		businessCardBinder = (BusinessCardBinder*)GetWindowLong(hWnd, GWL_USERDATA);
		//9.1. 명함철에서 마지막 명함으로 이동한다.
		index = BusinessCardBinder_Last(businessCardBinder);
		//9.2. 현재 명함을 쓴다.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_NAME), WM_SETTEXT,
			(WPARAM)0, (LPARAM)index->personal.name);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_RANK), WM_SETTEXT,
			(WPARAM)0, (LPARAM)index->personal.rank);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_CELLPHONENUMBER), WM_SETTEXT,
			(WPARAM)0, (LPARAM)index->personal.cellphoneNumber);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_EMAILADDRESS), WM_SETTEXT,
			(WPARAM)0, (LPARAM)index->personal.emailAddress);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANYNAME), WM_SETTEXT,
			(WPARAM)0, (LPARAM)index->company.name);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_ADDRESS), WM_SETTEXT,
			(WPARAM)0, (LPARAM)index->company.address);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_TELEPHONENUMBER), WM_SETTEXT,
			(WPARAM)0, (LPARAM)index->company.telephoneNumber);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_FAXNUMBER), WM_SETTEXT,
			(WPARAM)0, (LPARAM)index->company.faxNumber);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_URL), WM_SETTEXT,
			(WPARAM)0, (LPARAM)index->company.url);
		//9.3. 색인 카드철에서 찾는다.
		indexCardBinder = (IndexCardBinder*)GetProp(hWnd, "PROP_INDEXCARDBINDER");
		link = IndexCardBinder_Find(indexCardBinder, index->company.name);
		//9.4. 색인 카드에서 찾는다.
		location = IndexCard_Find(link, index);
		//9.5. 트리 뷰 컨트롤에서 상호 항목을 찾는다.
		hRootItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER),
			TVM_GETNEXTITEM, (WPARAM)TVGN_ROOT, (LPARAM)0);
		hIndexCardItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER),
			TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hRootItem);
		item.mask = TVIF_TEXT | TVIF_HANDLE;
		item.hItem = hIndexCardItem;
		item.pszText = companyName;
		item.cchTextMax = 16;
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER), TVM_GETITEM, (WPARAM)0, (LPARAM)&item);
		while (hIndexCardItem != NULL && strcmp(index->company.name, companyName) != 0) {
			hIndexCardItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER),
				TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)hIndexCardItem);
			item.hItem = hIndexCardItem;
			SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER), TVM_GETITEM, (WPARAM)0, (LPARAM)&item);
		}
		//9.6. 트리 뷰 컨트롤의 상호 항목을 펼친다.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER), TVM_EXPAND, (WPARAM)0, (LPARAM)hIndexCardItem);
		//9.7. 트리 뷰 컨트롤의 상호 항목에서 이동한다.
		hNameItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hIndexCardItem);
		i = 0;
		while (hNameItem != NULL && i < location) {
			hNameItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)hNameItem);
			i++;
		}
		//9.8. 트리뷰 컨트롤의 성명 항목을 선택한다.
		item.mask = TVIF_STATE | TVIF_HANDLE;
		item.stateMask = TVIS_SELECTED;
		item.state = TVIS_SELECTED;
		item.hItem = hNameItem;
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER), TVM_SETITEM, (WPARAM)0, (LPARAM)&item);
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER), TVM_SELECTITEM, (WPARAM)TVGN_CARET, (LPARAM)hNameItem);
	}
	return TRUE;
}
/************************************************************************************************
명    칭: BusinessCardBinderForm_OnNotify
기    능: WM_NOTIFY 메세지 발생 시 시나리오
*************************************************************************************************/
BOOL BusinessCardBinderForm_OnNotify(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	BOOL ret;

	switch (((LPNMHDR)lParam)->idFrom) {
	case IDC_TREE_INDEXCARDBINDER:
		ret = BusinessCardBinderForm_OnNameItemDoubleClicked(hWnd, wParam, lParam);
		break;
	default:
		ret = FALSE;
		break;
	}
	return ret;
}
/************************************************************************************************
명    칭: BusinessCardBinderForm_OnNameitemDoubleClicked
기    능: 트리뷰 컨트롤의 성명 항목을 더블클릭 했을때.
*************************************************************************************************/
BOOL BusinessCardBinderForm_OnNameItemDoubleClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	BusinessCardBinder* businessCardBinder;
	BusinessCard* index;
	IndexCardBinder* indexCardBinder;
	IndexCard* link;
	Long i;
	TVITEM item;
	HTREEITEM hSelectedItem;
	HTREEITEM hIndexCardItem;
	HTREEITEM hNameItem;
	TCHAR companyName[16];

	if ((((LPNMHDR)lParam)->code) == NM_DBLCLK) {
		//10.1. 트리 뷰 컨트롤의 선택된 성명 항목을 읽는다.
		hSelectedItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER), TVM_GETNEXTITEM, (WPARAM)TVGN_CARET, (LPARAM)0);
		//10.2. 트리 뷰 컨트롤의 상호 항목을 읽는다.
		hIndexCardItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER), TVM_GETNEXTITEM, (WPARAM)TVGN_PARENT, (LPARAM)hSelectedItem);
		item.mask = TVIF_TEXT | TVIF_HANDLE;
		item.hItem = hIndexCardItem;
		item.pszText = companyName;
		item.cchTextMax = 16;
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER), TVM_GETITEM, (WPARAM)0, (LPARAM)&item);
		//10.3. 트리 뷰 컨트롤의 상호 항목에서 이동한다.
		hNameItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hIndexCardItem);
		i = 0;
		while (hSelectedItem != hNameItem) {
			hNameItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)hNameItem);
			i++;
		}
		//10.4. 색인 카드철에서 찾는다.
		indexCardBinder = (IndexCardBinder*)GetProp(hWnd, "PROP_INDEXCARDBINDER");
		link = IndexCardBinder_Find(indexCardBinder, companyName);
		//10.5. 색인카드에서 가져온다.
		index = IndexCard_GetAt(link, i);
		//10.6. 명함카드철에서 이동한다.
		businessCardBinder = (BusinessCardBinder*)GetWindowLong(hWnd, GWL_USERDATA);
		BusinessCardBinder_Move(businessCardBinder, index);
		//10.7. 현재 명함을 쓴다.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_NAME), WM_SETTEXT,
			(WPARAM)0, (LPARAM)index->personal.name);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_RANK), WM_SETTEXT,
			(WPARAM)0, (LPARAM)index->personal.rank);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_CELLPHONENUMBER), WM_SETTEXT,
			(WPARAM)0, (LPARAM)index->personal.cellphoneNumber);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_EMAILADDRESS), WM_SETTEXT,
			(WPARAM)0, (LPARAM)index->personal.emailAddress);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANYNAME), WM_SETTEXT,
			(WPARAM)0, (LPARAM)index->company.name);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_ADDRESS), WM_SETTEXT,
			(WPARAM)0, (LPARAM)index->company.address);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_TELEPHONENUMBER), WM_SETTEXT,
			(WPARAM)0, (LPARAM)index->company.telephoneNumber);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_FAXNUMBER), WM_SETTEXT,
			(WPARAM)0, (LPARAM)index->company.faxNumber);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_URL), WM_SETTEXT,
			(WPARAM)0, (LPARAM)index->company.url);
	}
	return TRUE;
}
/************************************************************************************************
명    칭: BusinessCardBinderForm_OnClose
기    능: WM_CLOSE 메세지 발생 시 시나리오
*************************************************************************************************/
BOOL BusinessCardBinderForm_OnClose(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	BusinessCardBinder* businessCardBinder;
	IndexCardBinder* indexCardBinder;

	businessCardBinder = (BusinessCardBinder*)GetWindowLong(hWnd, GWL_USERDATA);
	if (businessCardBinder != NULL) {
		//11.1. 명함철에서 저장한다.
		Save(businessCardBinder);
		//11.2. 명함철을 없앤다.
		BusinessCardBinder_Destroy(businessCardBinder);
		free(businessCardBinder);
	}
	indexCardBinder = (IndexCardBinder*)GetProp(hWnd, "PROP_INDEXCARDBINDER");
	if (indexCardBinder != NULL) {
		//11.3. 색인카드철을 없앤다.
		IndexCardBinder_Destroy(indexCardBinder);
		free(indexCardBinder);
		RemoveProp(hWnd, "PROP_INDEXCARDBINDER");
	}
	//11.4. 윈도우를 닫는다.
	EndDialog(hWnd, 0);

	return TRUE;
}