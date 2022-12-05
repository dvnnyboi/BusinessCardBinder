//BusinessCardBinderForm.c
/************************************************************************************************
���ϸ�Ī: BusinessCardBinderForm.c
��    ��: ����ö ���������� ����
�� �� ��: �ӴٴϿ�
�ۼ�����: 2022/05/30
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

//WinMain �Լ� ����
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	int response;
	response = DialogBox(hInstance, MAKEINTRESOURCE(IDD_BUSINESSCARDBINDERFORM), NULL, BusinessCardBinderFormProc);

	return response;
}
/************************************************************************************************
��    Ī: BusinessCardBinderFormProc
��    ��: ���������� ���ν����Լ�
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
��    Ī: BusinessCardBinderForm_OnInitDialog
��    ��: WM_INITDIALOG �޼��� �߻� �� �ó�����
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

	//1.1.����ö�� �����.
	businessCardBinder = (BusinessCardBinder*)malloc(sizeof(BusinessCardBinder));
	BusinessCardBinder_Create(businessCardBinder);
	SetWindowLong(hWnd, GWL_USERDATA, (LONG)businessCardBinder);
	//1.2. ����ī��ö�� �����.
	indexCardBinder = (IndexCardBinder*)malloc(sizeof(IndexCardBinder));
	IndexCardBinder_Create(indexCardBinder);
	SetProp(hWnd, "PROP_INDEXCARDBINDER", (HANDLE)indexCardBinder);
	//1.3. Ʈ�� �� ��Ʈ�ѿ� ��Ʈ �׸��� �߰��Ѵ�.
	item.mask = TVIF_TEXT | TVIF_CHILDREN;
	item.pszText = "ȸ���";
	item.cChildren = 1;
	insertItem.hParent = NULL;
	insertItem.hInsertAfter = (HTREEITEM)TVI_FIRST;
	insertItem.item = item;
	hRootItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER), TVM_INSERTITEM, (WPARAM)0, (LPARAM)&insertItem);
	//1.4. ����ö���� �����Ѵ�.
	count = Load(businessCardBinder);
	if (count > 0) {
		//1.5. ������ŭ �ݺ��Ѵ�.
		index = BusinessCardBinder_First(businessCardBinder);
		i = 0;
		while (i < count) {
			//1.5.1. ����ī��ö���� ������ �����.
			link = IndexCardBinder_TakeIn(indexCardBinder, index);
			index = BusinessCardBinder_Next(businessCardBinder);
			i++;
		}
		//1.6. ����ī��ö���� �����Ѵ�.
		IndexCardBinder_Arrange(indexCardBinder);
		//1.7. ����ī��ö���� ����� �����.
		MakeList(indexCardBinder, &indexCards, &count);
		//1.8. ������ŭ �ݺ��Ѵ�.
		i = 0;
		while (i < count) {
			//1.8.1. Ʈ�� �� ��Ʈ�ѿ� ��ȣ �׸��� �߰��Ѵ�.
			link = &(indexCards[i]);
			item.mask = TVIF_TEXT | TVIF_CHILDREN;
			item.pszText = link->companyName;
			item.cChildren = 1;
			insertItem.hParent = hRootItem;
			insertItem.hInsertAfter = hIndexCardItem;
			insertItem.item = item;
			hIndexCardItem= (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER), TVM_INSERTITEM, (WPARAM)0, (LPARAM)&insertItem);
			//1.8.2. ������ ������ŭ �ݺ��Ѵ�.
			j = 0;
			while (j < link->length) {
				//1.8.2.1. Ʈ�� �� ��Ʈ�ѿ� ���� �׸��� �߰��Ѵ�.
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
		//1.9. ����ö���� ù �������� �̵��Ѵ�.
		index = BusinessCardBinder_First(businessCardBinder);
		//1.10. ���� ������ ����.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_NAME), WM_SETTEXT, (WPARAM)0, (LPARAM)index->personal.name);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_RANK), WM_SETTEXT, (WPARAM)0, (LPARAM)index->personal.rank);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_CELLPHONENUMBER), WM_SETTEXT, (WPARAM)0, (LPARAM)index->personal.cellphoneNumber);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_EMAILADDRESS), WM_SETTEXT, (WPARAM)0, (LPARAM)index->personal.emailAddress);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANYNAME), WM_SETTEXT, (WPARAM)0, (LPARAM)index->company.name);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_ADDRESS), WM_SETTEXT, (WPARAM)0, (LPARAM)index->company.address);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_TELEPHONENUMBER), WM_SETTEXT, (WPARAM)0, (LPARAM)index->company.telephoneNumber);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_FAXNUMBER), WM_SETTEXT, (WPARAM)0, (LPARAM)index->company.faxNumber);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_URL), WM_SETTEXT, (WPARAM)0, (LPARAM)index->company.url);
		//1.11. ���� ī��ö���� ã�´�.
		link = IndexCardBinder_Find(indexCardBinder, index->company.name);
		//1.12. ���� ī�忡�� ã�´�.
		location = IndexCard_Find(link, index);
		//1.13. Ʈ�� �� ��Ʈ���� ��Ʈ �׸��� ��ģ��.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER), TVM_EXPAND, (WPARAM)0, (LPARAM)hRootItem);
		//1.14. Ʈ�� �� ��Ʈ�ѿ��� ��ȣ �׸��� ã�´�.
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
		//1.15. Ʈ�� �� ��Ʈ���� ��ȣ �׸��� ��ģ��.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER), TVM_EXPAND, (WPARAM)0, (LPARAM)hIndexCardItem);
		//1.16. Ʈ�� �� ��Ʈ���� ��ȣ �׸񿡼� �̵��Ѵ�.
		hNameItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hIndexCardItem);
		i = 0;
		while (hNameItem != NULL && i < location) {
			hNameItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)hNameItem);
			i++;
		}
		//1.17. Ʈ���� ��Ʈ���� ���� �׸��� �����Ѵ�.
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
��    Ī: BusinessCardBinderForm_OnCommand
��    ��: WM_COMMAND �޼��� �߻� �� �ó�����
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
��    Ī: BusinessCardBinderForm_OnTakeInBunttonClicked
��    ��: ����� ��ư�� Ŭ������ �� �ó�����
*************************************************************************************************/
BOOL BusinessCardBinderForm_OnTakeInButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	//2.1. ����� �����츦 ����Ѵ�.
	if (HIWORD(wParam) == BN_CLICKED) {
		DialogBox((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDD_TAKINGINFORM), NULL, TakingInFormProc);
	}
	return TRUE;
}
/************************************************************************************************
��    Ī: BusinessCardBinderForm_OnFindBunttonClicked
��    ��: ã�� ��ư�� Ŭ������ �� �ó�����
*************************************************************************************************/
BOOL BusinessCardBinderForm_OnFindButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	//3.1. ã�� �����츦 ����Ѵ�.
	if (HIWORD(wParam) == BN_CLICKED) {
		DialogBox((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDD_FINDINGFORM), NULL, FindingFormProc);
	}
	return TRUE;
}
/************************************************************************************************
��    Ī: BusinessCardBinderForm_OnTakeOutBunttonClicked
��    ��: ���� ��ư�� Ŭ������ �� �ó�����
*************************************************************************************************/
BOOL BusinessCardBinderForm_OnTakeOutButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	BusinessCardBinder* businessCardBinder;

	businessCardBinder = (BusinessCardBinder*)GetWindowLong(hWnd, GWL_USERDATA);
	if (businessCardBinder->length > 0) {
		//4.1. ���� �����츦 ����Ѵ�.
		if (HIWORD(wParam) == BN_CLICKED) {
			DialogBox((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDD_TAKINGOUTFORM), NULL, TakingOutFormProc);
		}
	}
	return TRUE;
}
/************************************************************************************************
��    Ī: BusinessCardBinderForm_OnArrangeBunttonClicked
��    ��: �����ϱ� ��ư�� Ŭ������ �� �ó�����
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
		//5.1. ����ī��ö���� �����Ѵ�.
		indexCardBinder = (IndexCardBinder*)GetProp(hWnd, "PROP_INDEXCARDBINDER");
		IndexCardBinder_Arrange(indexCardBinder);
		//5.2. ���� ī��ö���� ����� �����.
		MakeList(indexCardBinder, &indexCards, &count);
		//5.3. Ʈ�� �� ��Ʈ���� ��� �׸���� �����.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER), TVM_DELETEITEM, (WPARAM)0, (LPARAM)TVI_ROOT);
		//5.4. Ʈ�� �� ��Ʈ�ѿ� ��Ʈ �׸��� �߰��Ѵ�.
		item.mask = TVIF_TEXT | TVIF_CHILDREN;
		item.pszText = "ȸ���";
		item.cChildren = 1;
		insertItem.hParent = NULL;
		insertItem.hInsertAfter = (HTREEITEM)TVI_FIRST;
		insertItem.item = item;
		hRootItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER), TVM_INSERTITEM, (WPARAM)0, (LPARAM)&insertItem);
		//5.5. ������ŭ �ݺ��Ѵ�.
		i = 0;
		while (i < count) {
			//5.5.1. Ʈ�� �� ��Ʈ�ѿ� ��ȣ �׸��� �߰��Ѵ�.
			link = &(indexCards[i]);
			item.mask = TVIF_TEXT | TVIF_CHILDREN;
			item.pszText = link->companyName;
			item.cChildren = 1;
			insertItem.hParent = hRootItem;
			insertItem.hInsertAfter = hIndexCardItem;
			insertItem.item = item;
			hIndexCardItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER), TVM_INSERTITEM, (WPARAM)0, (LPARAM)&insertItem);
			//5.5.2. ������ ������ŭ �ݺ��Ѵ�.
			j = 0;
			while (j < link->length) {
				//5.5.2.1. Ʈ�� �� ��Ʈ�ѿ� ���� �׸��� �߰��Ѵ�.
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
		//5.6. ����ö���� ó������ �̵��Ѵ�.
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
		//5.7. ���� ī��ö���� ã�´�.
		link = IndexCardBinder_Find(indexCardBinder, index->company.name);
		//5.8. ���� ī�忡�� ã�´�.
		location = IndexCard_Find(link, index);
		//5.9. Ʈ�� �� ��Ʈ���� ��Ʈ �׸��� ��ģ��.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER), TVM_EXPAND, (WPARAM)0, (LPARAM)hRootItem);
		//5.10. Ʈ�� �� ��Ʈ�ѿ��� ��ȣ �׸��� ã�´�.
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
		//5.11. Ʈ�� �� ��Ʈ���� ��ȣ �׸��� ��ģ��.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER), TVM_EXPAND, (WPARAM)0, (LPARAM)hIndexCardItem);
		//5.12. Ʈ�� �� ��Ʈ���� ��ȣ �׸񿡼� �̵��Ѵ�.
		hNameItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hIndexCardItem);
		i = 0;
		while (hNameItem != NULL && i < location) {
			hNameItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)hNameItem);
			i++;
		}
		//5.13. Ʈ���� ��Ʈ���� ���� �׸��� �����Ѵ�.
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
��    Ī: FindingForm_OnFisrtButtonClicked
��    ��: ó�� ��ư�� Ŭ������ �� �ó�����
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
		//6.1. ����ö���� ù �������� �̵��Ѵ�.
		index = BusinessCardBinder_First(businessCardBinder);
		//6.2. ���� ������ ����.
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
		//6.3. ���� ī��ö���� ã�´�.
		indexCardBinder = (IndexCardBinder*)GetProp(hWnd, "PROP_INDEXCARDBINDER");
		link = IndexCardBinder_Find(indexCardBinder, index->company.name);
		//6.4. ���� ī�忡�� ã�´�.
		location = IndexCard_Find(link, index);
		//6.5. Ʈ�� �� ��Ʈ�ѿ��� ��ȣ �׸��� ã�´�.
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
		//6.6. Ʈ�� �� ��Ʈ���� ��ȣ �׸��� ��ģ��.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER), TVM_EXPAND, (WPARAM)0, (LPARAM)hIndexCardItem);
		//6.7. Ʈ�� �� ��Ʈ���� ��ȣ �׸񿡼� �̵��Ѵ�.
		hNameItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hIndexCardItem);
		i = 0;
		while (hNameItem != NULL && i < location) {
			hNameItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)hNameItem);
			i++;
		}
		//6.8. Ʈ���� ��Ʈ���� ���� �׸��� �����Ѵ�.
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
��    Ī: FindingForm_OnPreviousButtonClicked
��    ��: ���� ��ư�� Ŭ������ �� �ó�����
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
		//7.1. ����ö���� ���� �������� �̵��Ѵ�.
		index = BusinessCardBinder_Previous(businessCardBinder);
		//7.2. ���� ������ ����.
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
		//7.3. ���� ī��ö���� ã�´�.
		indexCardBinder = (IndexCardBinder*)GetProp(hWnd, "PROP_INDEXCARDBINDER");
		link = IndexCardBinder_Find(indexCardBinder, index->company.name);
		//7.4. ���� ī�忡�� ã�´�.
		location = IndexCard_Find(link, index);
		//7.5. Ʈ�� �� ��Ʈ�ѿ��� ��ȣ �׸��� ã�´�.
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
		//7.6. Ʈ�� �� ��Ʈ���� ��ȣ �׸��� ��ģ��.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER), TVM_EXPAND, (WPARAM)0, (LPARAM)hIndexCardItem);
		//7.7. Ʈ�� �� ��Ʈ���� ��ȣ �׸񿡼� �̵��Ѵ�.
		hNameItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hIndexCardItem);
		i = 0;
		while (hNameItem != NULL && i < location) {
			hNameItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)hNameItem);
			i++;
		}
		//7.8. Ʈ���� ��Ʈ���� ���� �׸��� �����Ѵ�.
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
��    Ī: FindingForm_OnNextButtonClicked
��    ��: ���� ��ư�� Ŭ������ �� �ó�����
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
		//8.1. ����ö���� ���� �������� �̵��Ѵ�.
		index = BusinessCardBinder_Next(businessCardBinder);
		//8.2. ���� ������ ����.
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
		//8.3. ���� ī��ö���� ã�´�.
		indexCardBinder = (IndexCardBinder*)GetProp(hWnd, "PROP_INDEXCARDBINDER");
		link = IndexCardBinder_Find(indexCardBinder, index->company.name);
		//8.4. ���� ī�忡�� ã�´�.
		location = IndexCard_Find(link, index);
		//8.5. Ʈ�� �� ��Ʈ�ѿ��� ��ȣ �׸��� ã�´�.
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
		//8.6. Ʈ�� �� ��Ʈ���� ��ȣ �׸��� ��ģ��.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER), TVM_EXPAND, (WPARAM)0, (LPARAM)hIndexCardItem);
		//8.7. Ʈ�� �� ��Ʈ���� ��ȣ �׸񿡼� �̵��Ѵ�.
		hNameItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hIndexCardItem);
		i = 0;
		while (hNameItem != NULL && i < location) {
			hNameItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)hNameItem);
			i++;
		}
		//8.8. Ʈ���� ��Ʈ���� ���� �׸��� �����Ѵ�.
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
��    Ī: FindingForm_OnLastButtonClicked
��    ��: ������ ��ư�� Ŭ������ �� �ó�����
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
		//9.1. ����ö���� ������ �������� �̵��Ѵ�.
		index = BusinessCardBinder_Last(businessCardBinder);
		//9.2. ���� ������ ����.
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
		//9.3. ���� ī��ö���� ã�´�.
		indexCardBinder = (IndexCardBinder*)GetProp(hWnd, "PROP_INDEXCARDBINDER");
		link = IndexCardBinder_Find(indexCardBinder, index->company.name);
		//9.4. ���� ī�忡�� ã�´�.
		location = IndexCard_Find(link, index);
		//9.5. Ʈ�� �� ��Ʈ�ѿ��� ��ȣ �׸��� ã�´�.
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
		//9.6. Ʈ�� �� ��Ʈ���� ��ȣ �׸��� ��ģ��.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER), TVM_EXPAND, (WPARAM)0, (LPARAM)hIndexCardItem);
		//9.7. Ʈ�� �� ��Ʈ���� ��ȣ �׸񿡼� �̵��Ѵ�.
		hNameItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hIndexCardItem);
		i = 0;
		while (hNameItem != NULL && i < location) {
			hNameItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)hNameItem);
			i++;
		}
		//9.8. Ʈ���� ��Ʈ���� ���� �׸��� �����Ѵ�.
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
��    Ī: BusinessCardBinderForm_OnNotify
��    ��: WM_NOTIFY �޼��� �߻� �� �ó�����
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
��    Ī: BusinessCardBinderForm_OnNameitemDoubleClicked
��    ��: Ʈ���� ��Ʈ���� ���� �׸��� ����Ŭ�� ������.
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
		//10.1. Ʈ�� �� ��Ʈ���� ���õ� ���� �׸��� �д´�.
		hSelectedItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER), TVM_GETNEXTITEM, (WPARAM)TVGN_CARET, (LPARAM)0);
		//10.2. Ʈ�� �� ��Ʈ���� ��ȣ �׸��� �д´�.
		hIndexCardItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER), TVM_GETNEXTITEM, (WPARAM)TVGN_PARENT, (LPARAM)hSelectedItem);
		item.mask = TVIF_TEXT | TVIF_HANDLE;
		item.hItem = hIndexCardItem;
		item.pszText = companyName;
		item.cchTextMax = 16;
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER), TVM_GETITEM, (WPARAM)0, (LPARAM)&item);
		//10.3. Ʈ�� �� ��Ʈ���� ��ȣ �׸񿡼� �̵��Ѵ�.
		hNameItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hIndexCardItem);
		i = 0;
		while (hSelectedItem != hNameItem) {
			hNameItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBINDER), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)hNameItem);
			i++;
		}
		//10.4. ���� ī��ö���� ã�´�.
		indexCardBinder = (IndexCardBinder*)GetProp(hWnd, "PROP_INDEXCARDBINDER");
		link = IndexCardBinder_Find(indexCardBinder, companyName);
		//10.5. ����ī�忡�� �����´�.
		index = IndexCard_GetAt(link, i);
		//10.6. ����ī��ö���� �̵��Ѵ�.
		businessCardBinder = (BusinessCardBinder*)GetWindowLong(hWnd, GWL_USERDATA);
		BusinessCardBinder_Move(businessCardBinder, index);
		//10.7. ���� ������ ����.
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
��    Ī: BusinessCardBinderForm_OnClose
��    ��: WM_CLOSE �޼��� �߻� �� �ó�����
*************************************************************************************************/
BOOL BusinessCardBinderForm_OnClose(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	BusinessCardBinder* businessCardBinder;
	IndexCardBinder* indexCardBinder;

	businessCardBinder = (BusinessCardBinder*)GetWindowLong(hWnd, GWL_USERDATA);
	if (businessCardBinder != NULL) {
		//11.1. ����ö���� �����Ѵ�.
		Save(businessCardBinder);
		//11.2. ����ö�� ���ش�.
		BusinessCardBinder_Destroy(businessCardBinder);
		free(businessCardBinder);
	}
	indexCardBinder = (IndexCardBinder*)GetProp(hWnd, "PROP_INDEXCARDBINDER");
	if (indexCardBinder != NULL) {
		//11.3. ����ī��ö�� ���ش�.
		IndexCardBinder_Destroy(indexCardBinder);
		free(indexCardBinder);
		RemoveProp(hWnd, "PROP_INDEXCARDBINDER");
	}
	//11.4. �����츦 �ݴ´�.
	EndDialog(hWnd, 0);

	return TRUE;
}