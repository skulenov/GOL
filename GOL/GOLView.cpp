// This MFC Samples source code demonstrates using MFC Microsoft Office Fluent User Interface 
// (the "Fluent UI") and is provided only as referential material to supplement the 
// Microsoft Foundation Classes Reference and related electronic documentation 
// included with the MFC C++ library software.  
// License terms to copy, use or distribute the Fluent UI are available separately.  
// To learn more about our Fluent UI licensing program, please visit 
// http://go.microsoft.com/fwlink/?LinkId=238214.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// GOLView.cpp : implementation of the CGOLView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "GOL.h"
#endif

#include "GOLDoc.h"
#include "GOLView.h"

#include "Grid.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CGOLView

IMPLEMENT_DYNCREATE(CGOLView, CScrollView)

BEGIN_MESSAGE_MAP(CGOLView, CScrollView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_BTSTART, &CGOLView::OnBtstart)
	ON_COMMAND(ID_BTRND, &CGOLView::OnBtrnd)
	ON_WM_TIMER()
	ON_UPDATE_COMMAND_UI(ID_BTSTART, &CGOLView::OnUpdateBtstart)
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_BTCLR, &CGOLView::OnBtclr)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_CREATE()
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_SPEED_SLIDER, &CGOLView::OnSpeedSlider)
	ON_UPDATE_COMMAND_UI(IDC_SPEED_IND_STATIC, &CGOLView::OnUpdateSpeedIndStatic)
	ON_COMMAND(ID_GRID_SZ_SLIDER, &CGOLView::OnGridSzSlider)
	ON_UPDATE_COMMAND_UI(IDC_GRID_SZ_STATIC, &CGOLView::OnUpdateGridSzStatic)
	ON_COMMAND(ID_COLR_BTN, &CGOLView::OnColrBtn)
	ON_COMMAND(ID_CELL_COLR_BTN, &CGOLView::OnCellColrBtn)
	ON_COMMAND(ID_CELL_SZ_SLIDER, &CGOLView::OnCellSzSlider)
	ON_UPDATE_COMMAND_UI(IDC_CELL_SZ_STATIC, &CGOLView::OnUpdateCellSzStatic)
END_MESSAGE_MAP()

// CGOLView construction

CGOLView::CGOLView() : 
	gridSize(50),
	cellSize(10),
	lineSize(1),
	speed(500),
	timer(0),
	m_lastIndex(-42),
	grid(Grid(gridSize))
{
	cellColor = RGB(63, 255, 31);
	lineColor = RGB(200, 200, 200);
	backColor = RGB(100, 100, 100);
	startStr.LoadStringW(IDS_BTNSTART);
	stopStr.LoadStringW(IDS_BTNSTOP);
	m_BtnStartText.LoadStringW(IDS_BTNSTART);	
}

CGOLView::~CGOLView()
{
}

BOOL CGOLView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

void CGOLView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
	CMFCRibbonBar* pRibbon = ((CFrameWndEx*)AfxGetMainWnd())->GetRibbonBar();
	ASSERT_VALID(pRibbon);
	CMFCRibbonSlider* pSlider = DYNAMIC_DOWNCAST(CMFCRibbonSlider, pRibbon->FindByID(ID_CELL_SZ_SLIDER));
	pSlider->SetPos(5, true);
	OnCellSzSlider();
	pSlider = DYNAMIC_DOWNCAST(CMFCRibbonSlider, pRibbon->FindByID(ID_GRID_SZ_SLIDER));
	pSlider->SetPos(20, true);
	OnGridSzSlider();
	SetScrollSizes(MM_TEXT, CSize(grid.Size() * cellSize, grid.Size() * cellSize));
}

// CGOLView drawing
void CGOLView::OnDraw(CDC* pDC)
{
	CGOLDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	
	SetScrollSizes(MM_TEXT, CSize(grid.Size() * cellSize, grid.Size() * cellSize));
	scrollPosX = GetScrollPosition().x;
	scrollPosY = GetScrollPosition().y;
		
	GetClientRect(&clientRect);

	CRect rCell,vRect;
	vRect.SetRect(clientRect.TopLeft(), clientRect.BottomRight() + CPoint(scrollPosX, scrollPosY));
	
	CBitmap gridBitmap;
	gridBitmap.CreateCompatibleBitmap(pDC, vRect.Width(), vRect.Height());
	
	CDC dcBackBuffer;
	dcBackBuffer.CreateCompatibleDC(pDC);
	dcBackBuffer.SelectObject(&gridBitmap);
	
	CBrush brush;
	brush.CreateStockObject(HOLLOW_BRUSH);
	dcBackBuffer.SelectObject(&brush);
	
	m_gridShift.x = (clientRect.Width() <= grid.Size()*cellSize) ? 0 : (clientRect.Width() - grid.Size()*cellSize) / 2;
	m_gridShift.y = (clientRect.Height() <= grid.Size()*cellSize) ? 0 : (clientRect.Height() - grid.Size()*cellSize) / 2;

	for (int index = 0; index < grid.Length(); ++index)
	{
		int x = (index % grid.Size()) * cellSize;
		int y = (int)(index / grid.Size()) * cellSize;

		rCell.SetRect(x, y, x + cellSize, y + cellSize);
		rCell.OffsetRect(m_gridShift.x, m_gridShift.y);
		dcBackBuffer.FillSolidRect(rCell, (grid.CellState(index)) ? cellColor : backColor);
		dcBackBuffer.Rectangle(rCell);
	}
	pDC->BitBlt(0, 0, vRect.right, vRect.bottom, &dcBackBuffer, 0, 0, SRCCOPY);
}

void CGOLView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CGOLView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}

// CGOLView diagnostics

#ifdef _DEBUG
void CGOLView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CGOLView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CGOLDoc* CGOLView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGOLDoc)));
	return (CGOLDoc*)m_pDocument;
}
#endif //_DEBUG

// CGOLView message handlers

void CGOLView::OnBtstart()
{
	if (KillTimer(timer))
		m_BtnStartText = startStr;
	else
	{
		m_BtnStartText = stopStr;
		timer=SetTimer(timer,speed,NULL);
	}
}

void CGOLView::OnBtrnd()
{
	grid.RandomGrid();
	Invalidate();
}

void CGOLView::OnTimer(UINT_PTR nIDEvent)
{
	timer = nIDEvent;
	grid.StepGeneration();
	Invalidate();
	CScrollView::OnTimer(nIDEvent);
}

void CGOLView::OnUpdateBtstart(CCmdUI *pCmdUI)
{
	pCmdUI->SetText(m_BtnStartText);
}

BOOL CGOLView::OnEraseBkgnd(CDC* pDC)
{
	return true;
	//CScrollView::OnEraseBkgnd(pDC);
}

void CGOLView::OnBtclr()
{
	KillTimer(timer);
	m_BtnStartText = startStr;
	grid.Clear();
	Invalidate();
}

void CGOLView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CPoint pos = point;
	CDC * dc=this->GetDC();
	OnPrepareDC(dc);
	dc->DPtoLP(&pos);
	CSize size = GetTotalSize();
	CRect rGrid;
	rGrid.SetRect(m_gridShift, m_gridShift+CPoint(size));
	
	if (rGrid.PtInRect(pos))
	{
		SetCapture();
		int index = (int)((pos.y - m_gridShift.y) / cellSize)*grid.Size() + (int)((pos.x - m_gridShift.x) / cellSize);
		if(grid.CellState(index)) grid.AlterCell(index, 0);
		else grid.AlterCell(index,1);
		Invalidate();
	}
	CScrollView::OnLButtonDown(nFlags, point);
}

void CGOLView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (((nFlags&MK_LBUTTON) == MK_LBUTTON))
	{
		CPoint pos = point;
		CDC * dc = this->GetDC();
		OnPrepareDC(dc);
		dc->DPtoLP(&pos);
		CSize size = GetTotalSize();
		CRect rGrid;
		rGrid.SetRect(m_gridShift, m_gridShift + CPoint(size));

		if (rGrid.PtInRect(pos))
		{
			int index = (int)((pos.y - m_gridShift.y) / cellSize)*grid.Size() + (int)((pos.x - m_gridShift.x) / cellSize);
			if ((m_lastIndex != index) && !(grid.CellState(index)))
			{
				grid.AlterCell(index, 1);
				m_lastIndex = index;
				Invalidate();
			}
		}
	}
	CScrollView::OnMouseMove(nFlags, point);
}

void CGOLView::OnLButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();
	CScrollView::OnLButtonUp(nFlags, point);
}

void CGOLView::OnSpeedSlider()
{
	CMFCRibbonBar* pRibbon = ((CFrameWndEx*)AfxGetMainWnd())->GetRibbonBar();
	ASSERT_VALID(pRibbon);
	CMFCRibbonSlider* pSlider = DYNAMIC_DOWNCAST(CMFCRibbonSlider, pRibbon->FindByID(ID_SPEED_SLIDER));
	int position = pSlider->GetPos();
	speed = 500 / position;

	if (KillTimer(timer))	timer = SetTimer(timer, speed, NULL);
}

void CGOLView::OnUpdateSpeedIndStatic(CCmdUI *pCmdUI)
{
	CString str;
	str.Format(_T("Speed (ms): %d"), speed);
	pCmdUI->SetText(str);
}

void CGOLView::OnGridSzSlider()
{
	CMFCRibbonBar* pRibbon = ((CFrameWndEx*)AfxGetMainWnd())->GetRibbonBar();
	ASSERT_VALID(pRibbon);
	CMFCRibbonSlider* pSlider = DYNAMIC_DOWNCAST(CMFCRibbonSlider, pRibbon->FindByID(ID_GRID_SZ_SLIDER));
	int position = pSlider->GetPos();
	gridSize = position;
	grid.Resize(gridSize);
	Invalidate();
}

void CGOLView::OnUpdateGridSzStatic(CCmdUI *pCmdUI)
{
	CString str;
	str.Format(_T("Size : %d"), grid.Size());
	pCmdUI->SetText(str);
}

void CGOLView::OnCellSzSlider()
{
	CMFCRibbonBar* pRibbon = ((CFrameWndEx*)AfxGetMainWnd())->GetRibbonBar();
	ASSERT_VALID(pRibbon);
	CMFCRibbonSlider* pSlider = DYNAMIC_DOWNCAST(CMFCRibbonSlider, pRibbon->FindByID(ID_CELL_SZ_SLIDER));
	int position = pSlider->GetPos();
	cellSize = position;
	Invalidate();
}

void CGOLView::OnUpdateCellSzStatic(CCmdUI *pCmdUI)
{
	CString str;
	str.Format(_T("Size : %d"), cellSize);
	pCmdUI->SetText(str);
	Invalidate();
}

void CGOLView::OnColrBtn()
{
	CMFCColorDialog cDlg;
	if (cDlg.DoModal() == IDOK)
	{
		backColor = cDlg.GetColor();
	}
	Invalidate();
}

void CGOLView::OnCellColrBtn()
{
	CMFCColorDialog cDlg;
	if (cDlg.DoModal() == IDOK)
	{
		cellColor = cDlg.GetColor();
	}
	Invalidate();
}

