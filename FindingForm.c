//FindingForm.c
/************************************************************************************************
파일명칭: FindingForm.c
기    능: 명함철 찾기 윈도우 관리
작 성 자: 임다니엘
작성일자: 2022/05/30
*************************************************************************************************/
#include "FindingForm.h"
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
명    칭: FindingFormProc
기    능: 찾기윈도우 프로시져함수
*************************************************************************************************/
BOOL CALLBACK FindingFormProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	BOOL ret;
	
	switch (message) {
	case WM_COMMAND:
		ret = FindingForm_OnCommand(hWnd, wParam, lParam);
		break;
	case WM_CLOSE:
		ret = FindingForm_OnClose(hWnd, wParam, lParam);
		break;
	default:
		ret = FALSE;
		break;
	}
	return ret;
}
/************************************************************************************************
명    칭: FindingForm_OnCommand
기    능: WM_COMMAND 메세지 발생 시 시나리오
*************************************************************************************************/
BOOL FindingForm_OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	BOOL ret;

	switch (LOWORD(wParam)) {
	case IDC_BUTTON_FIND:
		ret = FindingForm_OnFindButtonClicked(hWnd, wParam, lParam);
		break;
	case IDC_BUTTON_SELECT:
		ret = FindingForm_OnSelectButtonClicked(hWnd, wParam, lParam);
		break;
	case IDC_BUTTON_FIRST:
		ret = FindingForm_OnFirstButtonClicked(hWnd, wParam, lParam);
		break;
	case IDC_BUTTON_PREVIOUS:
		ret = FindingForm_OnPreviousButtonClicked(hWnd, wParam, lParam);
		break;
	case IDC_BUTTON_NEXT:
		ret = FindingForm_OnNextButtonClicked(hWnd, wParam, lParam);
		break;
	case IDC_BUTTON_LAST:
		ret = FindingForm_OnLastButtonClicked(hWnd, wParam, lParam);
		break;
	default:
		ret = FALSE;
		break;
	}
	return ret;
}
/************************************************************************************************
명    칭: FindingForm_OnFindButtonClicked
기    능: 찾기 버튼을 클릭했을 때 시나리오
*************************************************************************************************/
BOOL FindingForm_OnFindButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	HWND hBusinessCardBinderWnd;
	BusinessCardBinder* businessCardBinder;
	BusinessCard* (*indexes) = NULL;
	BusinessCard* index = NULL;
	Long count;
	Long i;
	TCHAR name[11];

	if (HIWORD(wParam) == BN_CLICKED) {
		//1.1. 성명을 읽는다.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_KEY), WM_GETTEXT, (WPARAM)11, (LPARAM)name);
		//1.2. 명함철 윈도우를 찾는다.
		hBusinessCardBinderWnd = FindWindow("#32770", "명함철");
		//1.3. 명함철윈도우의 명함철에서 성명으로 찾는다.
		businessCardBinder = (BusinessCardBinder*)GetWindowLong(hBusinessCardBinderWnd, GWL_USERDATA);
		indexes = (BusinessCard * (*))GetWindowLong(hWnd, GWL_USERDATA);
		if (indexes != NULL) {
			free(indexes);
		}
		Find(businessCardBinder, name, &indexes, &count);
		SetWindowLong(hWnd, GWL_USERDATA, (LONG)indexes);
		SetProp(hWnd, "PROP_COUNT", (HANDLE)count);
		//1.4. 찾은 명함들 중 첫번째 명함을 쓴다.
		if (count > 0) {
			i = 0;
			index = indexes[i];
			SetProp(hWnd, "PROP_CURRENT", (HANDLE)i);
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
	}
	return TRUE;
}
/************************************************************************************************
명    칭: FindingForm_OnSelectButtonClicked
기    능: 선택 버튼을 클릭했을 때 시나리오
*************************************************************************************************/
BOOL FindingForm_OnSelectButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	HWND hBusinessCardBinderWnd;
	BusinessCardBinder* businessCardBinder;
	BusinessCard* (*indexes) = NULL;
	BusinessCard* index = NULL;
	IndexCardBinder* indexCardBinder;
	IndexCard* link;
	Long count;
	Long location;
	Long i;
	TVITEM item;
	HTREEITEM hRootItem;
	HTREEITEM hIndexCardItem;
	HTREEITEM hNameItem;
	TCHAR companyName[16] = "";

	if (HIWORD(wParam) == BN_CLICKED) {
		//2.1. 명함철 윈도우를 찾는다.
		hBusinessCardBinderWnd = FindWindow("#32770", "명함철");
		//2.2. 명함철 윈도우의 명함철에서 이동한다.
		businessCardBinder = (BusinessCardBinder*)GetWindowLong(hBusinessCardBinderWnd, GWL_USERDATA);
		indexes = (BusinessCard * (*))GetWindowLong(hWnd, GWL_USERDATA);
		i = (Long)GetProp(hWnd, "PROP_CURRENT");
		index = indexes[i];
		BusinessCardBinder_Move(businessCardBinder, index);
		//2.3. 명함철윈도우에 현재 명함을 쓴다.
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
		//2.4. 명함철 윈도우의 색인 카드철에서 찾는다.
		indexCardBinder = (IndexCardBinder*)GetProp(hBusinessCardBinderWnd, "PROP_INDEXCARDBINDER");
		link = IndexCardBinder_Find(indexCardBinder, index->company.name);
		//2.5. 명함철 윈도우의 색인카드에서 찾는다.
		location = IndexCard_Find(link, index);
		//2.6. 명함철 윈도우의 트리 뷰 컨트롤에서 상호 항목을 찾는다.
		hRootItem = (HTREEITEM)SendMessage(GetDlgItem(hBusinessCardBinderWnd, IDC_TREE_INDEXCARDBINDER),
			TVM_GETNEXTITEM, (WPARAM)TVGN_ROOT, (LPARAM)0);
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
		//2.7. 명함철 윈도우 트리 뷰 컨트롤의 상호 항목을 펼친다.
		SendMessage(GetDlgItem(hBusinessCardBinderWnd, IDC_TREE_INDEXCARDBINDER), TVM_EXPAND, (WPARAM)0, (LPARAM)hIndexCardItem);
		//2.8. 명함철 윈도우 트리 뷰 컨트롤의 상호 항목에서 이동한다.
		hNameItem = (HTREEITEM)SendMessage(GetDlgItem(hBusinessCardBinderWnd, IDC_TREE_INDEXCARDBINDER),
			TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hIndexCardItem);
		i = 0;
		while (hNameItem != NULL && i < location) {
			hNameItem = (HTREEITEM)SendMessage(GetDlgItem(hBusinessCardBinderWnd, IDC_TREE_INDEXCARDBINDER),
				TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)hNameItem);
			i++;
		}
		//2.9. 명함철 윈도우 트리 뷰 컨트롤의 성명 항목을 선택한다.
		item.mask = TVIF_STATE | TVIF_HANDLE;
		item.stateMask = TVIS_SELECTED;
		item.state = TVIS_SELECTED;
		item.hItem = hNameItem;
		SendMessage(GetDlgItem(hBusinessCardBinderWnd, IDC_TREE_INDEXCARDBINDER), TVM_SETITEM, (WPARAM)0, (LPARAM)&item);
		SendMessage(GetDlgItem(hBusinessCardBinderWnd, IDC_TREE_INDEXCARDBINDER), TVM_SELECTITEM, (WPARAM)TVGN_CARET, (LPARAM)hNameItem);
		//2.10. 찾기 윈도우를 닫는다.
		if (indexes != NULL) {
			free(indexes);
		}
		RemoveProp(hWnd, "PROP_COUNT");
		RemoveProp(hWnd, "PROP_CURRENT");
		EndDialog(hWnd, 0);
	}
	return TRUE;
}
/************************************************************************************************
명    칭: FindingForm_OnFisrtButtonClicked
기    능: 처음 버튼을 클릭했을 때 시나리오
*************************************************************************************************/
BOOL FindingForm_OnFirstButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	BusinessCard* (*indexes) = NULL;
	BusinessCard* index = NULL;
	Long i;

	if (HIWORD(wParam) == BN_CLICKED) {
		//3.1. 찾은 명함들 중 첫번째 명함으로 이동한다.
		i = 0;
		SetProp(hWnd, "PROP_CURRENT", (HANDLE)i);
		indexes = (BusinessCard * (*))GetWindowLong(hWnd, GWL_USERDATA);
		index = indexes[i];
		//3.2. 명함을 쓴다.
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
명    칭: FindingForm_OnPreviousButtonClicked
기    능: 이전 버튼을 클릭했을 때 시나리오
*************************************************************************************************/
BOOL FindingForm_OnPreviousButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	BusinessCard* (*indexes) = NULL;
	BusinessCard* index = NULL;
	Long i;

	if (HIWORD(wParam) == BN_CLICKED) {
		//4.1. 찾은 명함들 중 직전 번째 명함으로 이동한다,
		i = (Long)GetProp(hWnd, "PROP_CURRENT");
		i--;
		if (i < 0) {
			i = 0;
		}
		SetProp(hWnd, "PROP_CURRENT", (HANDLE)i);
		indexes = (BusinessCard * (*))GetWindowLong(hWnd, GWL_USERDATA);
		index = indexes[i];
		//4.2. 명함을 쓴다.
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
명    칭: FindingForm_OnNextButtonClicked
기    능: 이전 버튼을 클릭했을 때 시나리오
*************************************************************************************************/
BOOL FindingForm_OnNextButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	BusinessCard* (*indexes) = NULL;
	BusinessCard* index = NULL;
	Long i;
	Long count;

	if (HIWORD(wParam) == BN_CLICKED) {
		//5.1. 찾은 명함들 중 다음 명함으로 이동한다,
		i = (Long)GetProp(hWnd, "PROP_CURRENT");
		count = (Long)GetProp(hWnd, "PROP_COUNT");
		i++;
		if (i >= count) {
			i = count - 1;
		}
		SetProp(hWnd, "PROP_CURRENT", (HANDLE)i);
		indexes = (BusinessCard * (*))GetWindowLong(hWnd, GWL_USERDATA);
		index = indexes[i];
		//5.2. 명함을 쓴다.
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
명    칭: FindingForm_OnLastButtonClicked
기    능: 이전 버튼을 클릭했을 때 시나리오
*************************************************************************************************/
BOOL FindingForm_OnLastButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	BusinessCard* (*indexes) = NULL;
	BusinessCard* index = NULL;
	Long i;
	Long count;

	if (HIWORD(wParam) == BN_CLICKED) {
		//6.1. 찾은 명함들 중 다음 명함으로 이동한다,
		count = (Long)GetProp(hWnd, "PROP_COUNT");
		i = count - 1;
		SetProp(hWnd, "PROP_CURRENT", (HANDLE)i);
		indexes = (BusinessCard * (*))GetWindowLong(hWnd, GWL_USERDATA);
		index = indexes[i];
		//6.2. 명함을 쓴다.
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
명    칭: FindingForm_OnClose
기    능: WM_CLOSE 메세지 발생 시 시나리오
*************************************************************************************************/
BOOL FindingForm_OnClose(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	BusinessCard* (*indexes) = NULL;

	//7.1. 찾기윈도우를 닫는다.
	indexes = (BusinessCard * (*))GetWindowLong(hWnd, GWL_USERDATA);
	if (indexes != NULL) {
		free(indexes);
	}
	RemoveProp(hWnd, "PROP_COUNT");
	RemoveProp(hWnd, "PROP_CURRENT");
	EndDialog(hWnd, 0);

	return TRUE;
}