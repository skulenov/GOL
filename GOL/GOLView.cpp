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

IMPLEMENT_DYNCREATE(CGOLView, CView)

BEGIN_MESSAGE_MAP(CGOLView, CView)
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
END_MESSAGE_MAP()

// CGOLView construction/destruction

CGOLView::CGOLView() : gridSize(50),
						cellSize(10),
						lineSize(1),
						speed(200),
						timer(0)
{
	grid = new Grid(gridSize);
	cellColor = RGB(63, 255, 31);
	lineColor = RGB(200, 200, 200);
	backColor = RGB(100, 100, 100);
	startStr.LoadStringW(IDS_BTNSTART);
	stopStr.LoadStringW(IDS_BTNSTOP);
	m_BtnStartText.LoadStringW(IDS_BTNSTART);
	gridRect = new CRect(0, 0, 0, 0);
	m_arrCells.SetSize(gridSize*gridSize);
}

CGOLView::~CGOLView()
{
	grid->~Grid();

	m_arrCells.RemoveAll();
	m_arrCells.~CArray();

}

BOOL CGOLView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CGOLView drawing

void CGOLView::OnDraw(CDC* pDC)
{
	CGOLDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// Save DC & start with double buffered drawing of the grid.
	// When done, restore saved DC.
		
	int pDCSave = pDC->SaveDC();
	GetClientRect(&clientRect);

	CDC dcBackBuffer;
	dcBackBuffer.CreateCompatibleDC(pDC);

	CBitmap bitmapBuffer;
	bitmapBuffer.CreateCompatibleBitmap(pDC, clientRect.Width(), clientRect.Height());
	CBitmap* oldBitBuffer = dcBackBuffer.SelectObject(&bitmapBuffer);
		
	CPoint oldShift = m_gridShift;
	m_gridShift = CPoint((clientRect.Width() - gridSize*cellSize) / 2, (clientRect.Height() - gridSize*cellSize) / 2);

	ComputeCoords();

	CBrush brush;
	brush.CreateStockObject(HOLLOW_BRUSH);
	CBrush* pBrOld = dcBackBuffer.SelectObject(&brush);
	COLORREF crCell;
		
	gridRect.SetRect(m_gridShift, m_gridShift + CPoint(gridSize*cellSize, gridSize*cellSize));

	for (int index = 0; index < gridSize*gridSize; ++index)
	{
		crCell = (grid->CellState(index)) ? cellColor : backColor;
		dcBackBuffer.FillSolidRect(&(m_arrCells.GetAt(index)), crCell);
		dcBackBuffer.Rectangle(&(m_arrCells.GetAt(index)));
	}

	pDC->BitBlt(0, 0, clientRect.Width(), clientRect.Height(), &dcBackBuffer, 0, 0, SRCCOPY);

	dcBackBuffer.SelectObject(oldBitBuffer);
	dcBackBuffer.DeleteDC();
	bitmapBuffer.DeleteObject();
	pDC->SelectObject(pBrOld);
	pDC->RestoreDC(pDCSave);
	brush.DeleteObject();
}

void CGOLView::ComputeCoords()
{
	for (int i = 0, x = 0, y = 0; i < gridSize*gridSize; ++i)
	{
		x = (i % gridSize) * cellSize + m_gridShift.x;
		y = (int)(i / gridSize) * cellSize + m_gridShift.y;
		m_arrCells[i].SetRect(x, y, x + cellSize, y + cellSize);
	}
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
	CView::AssertValid();
}

void CGOLView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
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
	if (m_BtnStartText==stopStr)
	{
		KillTimer(timer);
		m_BtnStartText = startStr;
	}
	else
	{
		m_BtnStartText = stopStr;
		SetTimer(0,speed,NULL);
	}
}

void CGOLView::OnBtrnd()
{
	grid->RandomGrid();
	Invalidate();
}

void CGOLView::OnTimer(UINT_PTR nIDEvent)
{
	timer = nIDEvent;
	grid->StepGeneration();
	Invalidate();
	CView::OnTimer(nIDEvent);
}

void CGOLView::OnUpdateBtstart(CCmdUI *pCmdUI)
{
	pCmdUI->SetText(m_BtnStartText);
}

BOOL CGOLView::OnEraseBkgnd(CDC* pDC)
{
	return true;
	// CView::OnEraseBkgnd(pDC);
}

void CGOLView::OnBtclr()
{
	KillTimer(timer);
	m_BtnStartText = startStr;
	grid->~Grid();
	grid = new Grid(gridSize);
	Invalidate();
}

void CGOLView::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (gridRect.PtInRect(point))
	{
		KillTimer(timer);
		m_BtnStartText = startStr;
		//The Grid expects zero based column(x) and row(y) indexes
		// so we transform them from client coords.: apply shift to align with the start of grid
		// and divide it by the cell size in pixels to get indexes
		// i = ((y-m_gridShift.y)/cellSize)*gridSize + (x-m_gridShift.x)/cellSize
		int index = (int)((point.y - m_gridShift.y) / cellSize)*gridSize + (int)((point.x - m_gridShift.x) / cellSize);
		if(grid->CellState(index)) grid->AlterCell(index, 0);
		else grid->AlterCell(index,1);
		Invalidate();
	}
	CView::OnLButtonDown(nFlags, point);
}

void CGOLView::OnMouseMove(UINT nFlags, CPoint point)
{
	CView::OnMouseMove(nFlags, point);
}