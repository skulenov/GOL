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
END_MESSAGE_MAP()

// CGOLView construction/destruction

CGOLView::CGOLView() : gridSize(10),draw(false),cellSize(5),lineSize(2),speed(50)
{
	// TODO: add construction code here
	grid = new Grid(gridSize);
	cellColor = RGB(255, 63, 31);
	lineColor = RGB(200, 200, 200);
	backColor = RGB(50, 80, 100);
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
	if (draw)
	{
		
		int pDCSave = pDC->SaveDC();
		CRect recClient;
		GetClientRect(&recClient);
		pDC->FillSolidRect(&recClient, backColor);

		CBrush brush;
		brush.CreateStockObject(HOLLOW_BRUSH);
		CBrush* pBrOld = pDC->SelectObject(&brush);

		COLORREF crCell;

		for(int row = 0; row < gridSize; ++row)
			for (int col = 0; col < gridSize; ++col)
			{
				CRect recCell = {col * cellSize, row * cellSize, col * cellSize + cellSize, row * cellSize + cellSize };
				crCell = (grid->CellState(col, row)) ? cellColor : backColor;
				pDC->FillSolidRect(&recCell, crCell);
				pDC->Rectangle(&recCell);
			}

		pDC->RestoreDC(pDCSave);
		brush.DeleteObject();
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
	draw = true;
	Invalidate();
}
