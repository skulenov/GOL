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
END_MESSAGE_MAP()

// CGOLView construction/destruction

CGOLView::CGOLView() : gridSize(50),
						draw(true),
						cellSize(10),
						lineSize(1),
						speed(200),
						isRunning(false),
						timer(0)
{
	// TODO: add construction code here
	grid = new Grid(gridSize);
	cellColor = RGB(63, 255, 31);
	lineColor = RGB(200, 200, 200);
	backColor = RGB(100, 100, 100);
	startStr.LoadStringW(IDS_BTNSTART);
	stopStr.LoadStringW(IDS_BTNSTOP);
	m_BtnStartText = startStr;
	gridRect = new CRect(0, 0, 0, 0);
}

CGOLView::~CGOLView()
{
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

	// TODO: add draw code for native data here
		
		int pDCSave = pDC->SaveDC();
		GetClientRect(&clientRect);
		pDC->FillSolidRect(&clientRect, backColor);

		CDC dcBackBuffer;
		dcBackBuffer.CreateCompatibleDC(pDC);
		CBitmap bitmapBuffer;
		bitmapBuffer.CreateCompatibleBitmap(pDC, clientRect.Width(), clientRect.Height());
		CBitmap* oldBitBuffer = dcBackBuffer.SelectObject(&bitmapBuffer);

		int xMove = (clientRect.Width() - gridSize*cellSize) / 2;
		int yMove = (clientRect.Height() - gridSize*cellSize) / 2;
		

		CBrush brush;
		brush.CreateStockObject(HOLLOW_BRUSH);
		CBrush* pBrOld = dcBackBuffer.SelectObject(&brush);

		COLORREF crCell;

		for(int row = 0; row < gridSize; ++row)
			for (int col = 0; col < gridSize; ++col)
			{
				cellRect = {col * cellSize+xMove, row * cellSize+yMove, col * cellSize + cellSize+xMove, row * cellSize + cellSize + yMove};
				crCell = (grid->CellState(col, row)) ? cellColor : backColor;
				dcBackBuffer.FillSolidRect(&cellRect, crCell);
				dcBackBuffer.Rectangle(&cellRect);
			}
		pDC->BitBlt(0, 0, clientRect.Width(), clientRect.Height(), &dcBackBuffer, 0, 0, SRCCOPY);
		dcBackBuffer.SelectObject(oldBitBuffer);
		dcBackBuffer.DeleteDC();
		bitmapBuffer.DeleteObject();
		pDC->SelectObject(pBrOld);
		pDC->RestoreDC(pDCSave);
		brush.DeleteObject();
		// now that Grid is known...
		if (cellRect.BottomRight() != gridRect.BottomRight())
		{
			CPoint lastCellPoint = CPoint(cellRect.BottomRight());
			CPoint firstCellPoint = CPoint(lastCellPoint.x - gridSize*cellSize, lastCellPoint.y - gridSize*cellSize);
			gridRect = new CRect(firstCellPoint, lastCellPoint);
		}
}

void CGOLView::ResizeWindow()
{
	CRect recClient, recWindow;
	GetClientRect(&recClient);
	GetParentFrame()->GetWindowRect(&recWindow);
	int deltaX = recWindow.Width() - recClient.Width();
	int deltaY = recWindow.Height() - recClient.Height();
	recWindow.right = recWindow.left + gridSize*cellSize + deltaX;
	recWindow.bottom = recWindow.top + gridSize*cellSize + deltaY;
	GetParentFrame()->MoveWindow(&recWindow);
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
	// TODO: Add your command handler code here
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
	// TODO: Add your command handler code here
	grid->RandomGrid();
	Invalidate();
}


void CGOLView::OnTimer(UINT_PTR nIDEvent)
{
	timer = nIDEvent;
	// TODO: Add your message handler code here and/or call default
	grid->StepGeneration();
	Invalidate();
	CView::OnTimer(nIDEvent);
}


void CGOLView::OnUpdateBtstart(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetText(m_BtnStartText);
}


BOOL CGOLView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return true;// CView::OnEraseBkgnd(pDC);
}


void CGOLView::OnBtclr()
{
	// TODO: Add your command handler code here
	KillTimer(timer);
	m_BtnStartText = startStr;

	grid = new Grid(gridSize);
	Invalidate();
}


void CGOLView::OnLButtonDown(UINT nFlags, CPoint point)
{
	
	if (gridRect.PtInRect(point))
	{
		KillTimer(timer);
		m_BtnStartText = startStr;
		int xmove = (clientRect.Width() - gridRect.Width()) / 2;
		int ymove = (clientRect.Height() - gridRect.Height()) / 2;
		CPoint gridPoint = CPoint(point.x - xmove, point.y - ymove);

		int cellX = (int)((gridPoint.x) / cellSize);
		int cellY = (int)((gridPoint.y) / cellSize);
		if(grid->CellState(cellX,cellY)) grid->AlterCell(cellX, cellY, 0);
		else grid->AlterCell(cellX,cellY,1);
		Invalidate();
	}
	// TODO: Add your message handler code here and/or call default

	CView::OnLButtonDown(nFlags, point);
}
