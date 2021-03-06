#include "stdafx.h"
#include "AccountSelectDlg.h"

CAccountSelectDlg::CAccountSelectDlg(IN OUT list<SELECTINFONODE>* pAccountNode):m_pAccountNode(pAccountNode)
{
}

CAccountSelectDlg::~CAccountSelectDlg()
{

	std::map<CButton*, string>::const_iterator cstIt = m_mapBtn.begin();
	for(; cstIt != m_mapBtn.end(); cstIt ++)
	{
		CButton* pButton = cstIt->first;
		if (pButton == NULL)
		{
			continue;
		}

		delete pButton;
		pButton = NULL;
	}

	std::list<CStatic*>::const_iterator cstStatic = m_listStatic.begin();
	while (m_listStatic.end() != cstStatic)
	{
		CStatic* pDel = *cstStatic;
		if (NULL != pDel)
		{
			delete pDel;
			pDel = NULL;
		}

		cstStatic ++;
	}

	m_mapBtn.clear();
	m_listStatic.clear();

}

LRESULT CAccountSelectDlg::OnCtlColorStatic(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
	HBRUSH hbr;
	HDC hdc = (HDC) wParam;
	HWND hwnd = (HWND) lParam;
	//获得当前控件的画刷，因为改变字体颜色后，需要返回自身的画刷才能生效
	hbr = (HBRUSH) GetCurrentObject(hdc, OBJ_BRUSH); 

	if(std::find(m_radiolist.begin(),m_radiolist.end(),hwnd) != m_radiolist.end()) //判断是否需要修改的控件
	{//如果是，修改字体颜色
		CDCHandle dc((HDC)wParam);
		dc.SetBkColor(RGB(61, 98, 123));
		//dc.SetBkMode(TRANSPARENT);

		return LRESULT(m_bkBrush2); 
	}
	else if(m_title == hwnd)
	{
		CDCHandle dc((HDC)wParam);
		SetTextColor((HDC)wParam,RGB(16,93,145));
		dc.SetBkColor(RGB(247, 252, 255));

		return LRESULT(m_bkBrush); 
	}
	else
	{//如果不是，返回主窗口默认画刷。
		CDCHandle dc((HDC)wParam);
		SetTextColor((HDC)wParam,RGB(16,93,145));

		dc.SetBkColor(RGB(255, 255, 255));

		return LRESULT(hbr); 
	}

}


LRESULT CAccountSelectDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	m_radiolist.clear();
	SetWindowText(_T("多卡号选择提示"));

	m_font.CreatePointFont(90,_T("Tahoma"));
	m_tFont.CreateFont(15, 0, 0, 0, FW_BOLD, 0, 0, 0, 0, 0, 0, 0, 0, _T("Tahoma"));
	CRect rectDlg;
	GetClientRect(&rectDlg);

	string strText = "检测到当前网银账号关联有多个账号，请选择要下载交易记录的账号";
	double dTextWidth = 16.0 + (59.0 / 9.0) * (strText.length() + 1);
	CStatic* pStatic = new CStatic();
	m_listStatic.push_back(pStatic);
	CRect rectTitle(rectDlg.Width() / 2 - (int)dTextWidth / 2, 3 * 80 / 4 - 8, rectDlg.Width() / 2 - (int)dTextWidth / 2 + dTextWidth, 3 * 80 / 4 + 8);
	pStatic->Create(m_hWnd, rectTitle, CA2W(strText.c_str()), WS_CHILD|WS_VISIBLE|SS_CENTER);
	pStatic->ShowWindow(true);
	pStatic->SetFont((HFONT)m_font);
	m_title = pStatic->m_hWnd;

	m_bkBrush = ::CreateSolidBrush(RGB(247,252,255));//暂时用取色的方式设置按钮的背景

	m_bkBrush2 = ::CreateSolidBrush(RGB(255,255,255));//暂时用取色的方式设置按钮的背景
	//int nXBegin = 30;
	int nXCurrent = 30;
	int nYCurrent = 110;
	//int nYTop = 110;
	//int nYBottom = 125;
	int nYSep = 10; // 行与行之间的间隔
	int nXSep1 = 100; // 第一列与第二列之间的间隔
	int nXSep2 = 55; // 第二列与第三列之间的间隔
	int nRBtn = 15; // Radio button的宽度

	if (NULL == m_pAccountNode)
		return 0;

	double dBigWidth;
	int nBigLen = 0;

	list<SELECTINFONODE>::const_iterator cstIt = m_pAccountNode->begin();
	while (cstIt != m_pAccountNode->end())
	{
		std::string str = (*cstIt).szNodeInfo;
		int nLen = str.length();
		if (nLen > nBigLen)
			nBigLen = nLen;

		cstIt ++;
		
	}

	dBigWidth = 16.0 + 7.5 * (nBigLen + 1);
	cstIt = m_pAccountNode->begin();

	while(m_pAccountNode->end() != cstIt)
	{
		nXCurrent = 30;
		CRect rectRBtn(nXCurrent, nYCurrent, nXCurrent + nRBtn, nYCurrent + nRBtn); 
		CButton *pButton = new CButton();
		pButton->Create(m_hWnd, rectRBtn, _T(""), WS_CHILD|WS_VISIBLE|BS_AUTORADIOBUTTON, 0, 7000);
		m_radiolist.push_back(pButton->m_hWnd);

		// X偏移
		nXCurrent += nRBtn;
		nXCurrent += nXSep1;


		strText = (*cstIt).szNodeInfo;
		//dTextWidth = 16.0 + (59.0 / 9.0) * (strText.length() + 1);
		pStatic = new CStatic();
		m_listStatic.push_back(pStatic);
		CRect rect22(nXCurrent, nYCurrent, nXCurrent + dBigWidth, nYCurrent + nRBtn);
		pStatic->Create(m_hWnd, rect22, CA2W(strText.c_str()), WS_CHILD|WS_VISIBLE|SS_CENTER);
		pStatic->ShowWindow(true);
		pStatic->SetFont((HFONT)m_font);
		m_mapBtn.insert(std::make_pair(pButton, strText));


		// X偏移
		nXCurrent += (int)dBigWidth;
		nXCurrent += nXSep2;

		strText = "信用卡";
		dTextWidth = 16.0 + (59.0 / 9.0) * (strText.length() + 1);
		pStatic = new CStatic();
		m_listStatic.push_back(pStatic);
		CRect rect3(nXCurrent, nYCurrent, nXCurrent + dTextWidth, nYCurrent + nRBtn);
		pStatic->Create(m_hWnd, rect3, CA2W(strText.c_str()), WS_CHILD|WS_VISIBLE|SS_CENTER);
		pStatic->ShowWindow(true);
		pStatic->SetFont((HFONT)m_font);

		// Y偏移
		nYCurrent += nRBtn;
		nYCurrent += nYSep;
		cstIt ++;
	}

	ApplyButtonSkin(IDOK);
	ApplyButtonSkin(IDCANCEL);

	int nTop = nYCurrent + 50;

	// 确定按钮定位
	CRect rectBtn;
	::GetClientRect(GetDlgItem(IDOK), &rectBtn);
	::MoveWindow(GetDlgItem(IDOK), rectDlg.Width() / 2 - rectBtn.Width() / 2 - 80, nYCurrent + 25 - (rectBtn.Height() / 4), rectBtn.Width(), rectBtn.Height(), true);//(nYBorderLeft / 2)


	::GetClientRect(GetDlgItem(IDCANCEL), &rectBtn);
	::MoveWindow(GetDlgItem(IDCANCEL), rectDlg.Width() / 2 - rectBtn.Width() / 2 + 80, nYCurrent + 25 - (rectBtn.Height() / 4), rectBtn.Width(), rectBtn.Height(), true);//(nYBorderLeft / 2)

	// 隐藏关闭按钮
	m_btnSysClose.ShowWindow(SW_HIDE);

	// 设备对话框大小
	MoveWindow(0, 0, rectDlg.Width(), nTop);

	CenterWindow(GetParent());
	return 0;
}

LRESULT CAccountSelectDlg::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CPaintDC dc(m_hWnd);

	CRect rectDlg;
	GetClientRect(&rectDlg);

   CPen penBlack;
   penBlack.CreatePen(PS_SOLID, 1, RGB(203, 207, 208));
   HGDIOBJ pOldPen = ::SelectObject(dc, HPEN(penBlack));
   int n = GetLastError();

   
	dc.RoundRect(20,80,rectDlg.right - 20, rectDlg.bottom - 40,20,20);


	CPen penRect;
	penRect.CreatePen(PS_SOLID, 1, RGB(233, 249, 254));

	::SelectObject(dc, penRect);
   ::DeleteObject(penBlack);
 
	CBrush brushBlue;
	brushBlue.CreateSolidBrush(RGB(233, 249, 254));
   HGDIOBJ pOldBrush = SelectObject(dc, brushBlue);

   CRect rectTitle(20, 80, rectDlg.right - 20, 103);
   rectTitle.DeflateRect(1, 1);
   dc.RoundRect(rectTitle, CPoint(20, 20));

   CRect rect2(20, 90, rectDlg.right - 20, 103);
   rect2.DeflateRect(1, 1);
   dc.Rectangle(rect2);
   
   dc.SetBkMode(TRANSPARENT);
   dc.SetTextColor(RGB(16, 93, 145));

   dc.SelectFont(m_tFont);

   dc.TextOut(200, 83, _T("卡号"));
   dc.TextOut(360, 83, _T("说明"));

   // put back the old objects
   ::SelectObject(dc, pOldBrush);
   ::DeleteObject(brushBlue);
   ::SelectObject(dc, pOldPen);
   ::DeleteObject(penRect);

	return 0;
}

LRESULT CAccountSelectDlg::OnCancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if(::MessageBox(NULL, L"确认取消导入账单！", L"导入账单", MB_OKCANCEL) != IDOK)
		return 0;
	EndDialog(IDCANCEL);
	return 0;
}
LRESULT CAccountSelectDlg::OnOK(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{

	std::map<CButton*, string>::const_iterator cstIt = m_mapBtn.begin();
	for(; cstIt != m_mapBtn.end(); cstIt ++)
	{
		CButton* pButton = cstIt->first;
		if (pButton->GetCheck() == BST_CHECKED)
		{
			string strTp = cstIt->second;
			list<SELECTINFONODE>::iterator itFind = m_pAccountNode->begin();//find((char*)strTp.c_str());
			for (; itFind != m_pAccountNode->end(); itFind ++)
			{
				string str1 = (*itFind).szNodeInfo;
				if (str1 == strTp)
				{
					(*itFind).dwVal = CHECKBOX_SHOW_CHECKED;
					EndDialog(IDOK);
					return 0;
				}
			
			}
			
		}
	}

	::MessageBox(NULL, L"您没有选择要导入的账号，请重新选择！", L"账号选择", MB_OK | MB_ICONINFORMATION);

	return 0;
}