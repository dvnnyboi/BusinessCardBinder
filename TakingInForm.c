//TakingInForm.c
/************************************************************************************************
���ϸ�Ī: TakingInForm.c
��    ��: ����ö ����� ������ ����
�� �� ��: �ӴٴϿ�
�ۼ�����: 2022/05/30
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
��    Ī: TakingInFormProc
��    ��: ����������� ���ν����Լ�
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
��    Ī: TakingInForm_OnInitDialog
��    ��: WM_INITDIALOG �޼��� �߻� �� �ó�����
*************************************************************************************************/
BOOL TakingInForm_OnInitDialog(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	Long i;
	TCHAR ranks[][20] = { "���", "����", "�븮", "����", "����", "����", "�̻�", "��", "����", "����" };
	TCHAR domains[][16] = { "naver.com", "gmail.com", "hanmail.com" };

	//1.1. ���� ����Ʈ�ڽ��� �׸���� �߰��Ѵ�.

	for (i = 0; i < (sizeof(ranks) / sizeof(ranks[0])); i++) {
		SendMessage(GetDlgItem(hWnd, IDC_COMBO_RANK), CB_ADDSTRING, (WPARAM)0, (LPARAM)ranks[i]);
	}
	//1.2. �̸����ּ� ����Ʈ�ڽ��� �׸���� �߰��Ѵ�.
	for (i = 0; i < (sizeof(domains) / sizeof(domains[0])); i++) {
		SendMessage(GetDlgItem(hWnd, IDC_COMBO_EMAILADDRESS), CB_ADDSTRING, (WPARAM)0, (LPARAM)domains[i]);
	}
}
/************************************************************************************************
��    Ī: TakingInForm_OnCommand
��    ��: WM_COMMAND �޼��� �߻� �� �ó�����
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
��    Ī: TakingInForm_OnTakeInButtonClicked
��    ��: ����� ��ư�� Ŭ������ �� �ó�����
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
		//2.1. ������ �д´�.
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
		//2.2. ����ö �����츦 ã�´�.
		hBusinessCardBinderWnd = FindWindow("#32770", "����ö");
		//2.3. ����ö �������� ����ö���� �����.
		businessCardBinder = (BusinessCardBinder*)GetWindowLong(hBusinessCardBinderWnd, GWL_USERDATA);
		index = TakeIn(businessCardBinder, businessCard);
		//2.4. ����ö �����쿡 ���� ������ ����.
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
		//2.5. ����ö �������� ����ī��ö���� �����.
		indexCardBinder = (IndexCardBinder*)GetProp(hBusinessCardBinderWnd, "PROP_INDEXCARDBINDER");
		link = IndexCardBinder_TakeIn(indexCardBinder, index);
		//2.6. ����ö �������� ����ī��ö���� �����Ѵ�.
		IndexCardBinder_Arrange(indexCardBinder);
		//2.7. ����ö ��������  ���� ī��ö���� ����� �����.
		MakeList(indexCardBinder, &indexCards, &count);
		//2.8. ����ö ������ Ʈ�� �� ��Ʈ���� ��� �׸���� �����.
		SendMessage(GetDlgItem(hBusinessCardBinderWnd, IDC_TREE_INDEXCARDBINDER), TVM_DELETEITEM, (WPARAM)0, (LPARAM)TVI_ROOT);
		//2.9. ����ö ������ Ʈ�� �� ��Ʈ�ѿ� ��Ʈ �׸��� �߰��Ѵ�.
		item.mask = TVIF_TEXT | TVIF_CHILDREN;
		item.pszText = "ȸ���";
		item.cChildren = 1;
		insertItem.hParent = NULL;
		insertItem.hInsertAfter = (HTREEITEM)TVI_FIRST;
		insertItem.item = item;
		hRootItem = (HTREEITEM)SendMessage(GetDlgItem(hBusinessCardBinderWnd, IDC_TREE_INDEXCARDBINDER),
			TVM_INSERTITEM, (WPARAM)0, (LPARAM)&insertItem);
		//2.10. ������ŭ �ݺ��Ѵ�.
		i = 0;
		while (i < count) {
			//2.10.1. ����ö ������ Ʈ�� �� ��Ʈ�ѿ� ��ȣ �׸��� �߰��Ѵ�.
			link = &(indexCards[i]);
			item.mask = TVIF_TEXT | TVIF_CHILDREN;
			item.pszText = link->companyName;
			item.cChildren = 1;
			insertItem.hParent = hRootItem;
			insertItem.hInsertAfter = hIndexCardItem;
			insertItem.item = item;
			hIndexCardItem = (HTREEITEM)SendMessage(GetDlgItem(hBusinessCardBinderWnd, IDC_TREE_INDEXCARDBINDER),
				TVM_INSERTITEM, (WPARAM)0, (LPARAM)&insertItem);
			//2.10.2. ������ ������ŭ �ݺ��Ѵ�.
			j = 0;
			while (j < link->length) {
				//2.10.2.1. ����ö ������ Ʈ�� �� ��Ʈ�ѿ� ���� �׸��� �߰��Ѵ�.
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
		//2.11. ����ö �������� ����ö���� ������ġ�� �д´�.
		index = businessCardBinder->current;
		//2.12. ����ö �������� ���� ī��ö���� ã�´�.
		link = IndexCardBinder_Find(indexCardBinder, index->company.name);
		//2.13. ����ö �������� ���� ī�忡�� ã�´�.
		location = IndexCard_Find(link, index);
		//2.14. ����ö ������ Ʈ�� �� ��Ʈ���� ��Ʈ �׸��� ��ģ��.
		SendMessage(GetDlgItem(hBusinessCardBinderWnd, IDC_TREE_INDEXCARDBINDER), TVM_EXPAND, (WPARAM)0, (LPARAM)hRootItem);
		//2.15. ����ö ������ Ʈ�� �� ��Ʈ�ѿ��� ��ȣ �׸��� ã�´�.
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
		//2.16. ����ö ������ Ʈ�� �� ��Ʈ���� ��ȣ �׸��� ��ģ��.
		SendMessage(GetDlgItem(hBusinessCardBinderWnd, IDC_TREE_INDEXCARDBINDER), TVM_EXPAND, (WPARAM)0, (LPARAM)hIndexCardItem);
		//2.17. ����ö ������ Ʈ�� �� ��Ʈ���� ��ȣ �׸񿡼� �̵��Ѵ�.
		hNameItem = (HTREEITEM)SendMessage(GetDlgItem(hBusinessCardBinderWnd, IDC_TREE_INDEXCARDBINDER),
			TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hIndexCardItem);
		i = 0;
		while (hNameItem != NULL && i < location) {
			hNameItem = (HTREEITEM)SendMessage(GetDlgItem(hBusinessCardBinderWnd, IDC_TREE_INDEXCARDBINDER),
				TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)hNameItem);
			i++;
		}
		//2.18. ����ö ������ Ʈ���� ��Ʈ���� ���� �׸��� �����Ѵ�.
		item.mask = TVIF_STATE | TVIF_HANDLE;
		item.stateMask = TVIS_SELECTED;
		item.state = TVIS_SELECTED;
		item.hItem = hNameItem;
		SendMessage(GetDlgItem(hBusinessCardBinderWnd, IDC_TREE_INDEXCARDBINDER), TVM_SETITEM, (WPARAM)0, (LPARAM)&item);
		SendMessage(GetDlgItem(hBusinessCardBinderWnd, IDC_TREE_INDEXCARDBINDER), TVM_SELECTITEM, (WPARAM)TVGN_CARET, (LPARAM)hNameItem);
		
		//2.19. ����� �����츦 �ݴ´�.
		EndDialog(hWnd, 0);
	}
	return TRUE;
}
/************************************************************************************************
��    Ī: TakingInForm_OnCompanyNameKillFocus
��    ��: ��ȣ�� ��Ŀ���� �Ҿ��� �� �ó�����
*************************************************************************************************/
BOOL TakingInForm_OnCompanyNameKillFocus(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	HWND hBusinessCardBinderWnd;
	BusinessCardBinder* businessCardBinder;
	BusinessCard* index;
	TCHAR companyName[16];

	if (HIWORD(wParam) == EN_KILLFOCUS) {
		//3.1. ��ȣ�� �д´�.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANYNAME), WM_GETTEXT, (WPARAM)16, (LPARAM)companyName);
		//3.2. ����ö �����츦 ã�´�.
		hBusinessCardBinderWnd = FindWindow("#32770", "����ö");
		//3.3. ����ö �������� ����ö���� ��ȣ�� ã�´�.
		businessCardBinder = (BusinessCardBinder*)GetWindowLong(hBusinessCardBinderWnd, GWL_USERDATA);
		index = FindByCompanyName(businessCardBinder, companyName);
		//3.4. ��ȣ�� ������
		if (index != NULL) {
			//3.4.1. ȸ�翡 ����.
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
��    Ī: TakingInForm_OnClose
��    ��: WM_CLOSE �޼��� �߻� �� �ó�����
*************************************************************************************************/
BOOL TakingInForm_OnClose(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	//4.1. ����� �����츦 �ݴ´�.
	EndDialog(hWnd, 0);

	return TRUE;
}