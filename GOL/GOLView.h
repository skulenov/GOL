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

// GOLView.h : interface of the CGOLView class
//

#pragma once
#include "Grid.h"
#include "atltypes.h"
#include "afxtempl.h"
#include "afxwin.h"

class CGOLView : public CView
{
protected: // create from serialization only
	CGOLView();
	DECLARE_DYNCREATE(CGOLView)

// Attributes
private:

public:
	CGOLDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CGOLView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnGridSzSlider();
	afx_msg void OnUpdateGridSzStatic(CCmdUI *pCmdUI);
	afx_msg void OnBtstart();
	afx_msg void OnBtrnd();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnUpdateBtstart(CCmdUI *pCmdUI);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBtclr();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSpeedSlider();
	afx_msg void OnUpdateSpeedIndStatic(CCmdUI *pCmdUI);
private:
	CString m_BtnStartText;
	CString startStr;
	CString stopStr;
	bool isRunning;
	UINT_PTR timer;
	int scrollPosY;
	int scrollPosX;
	int gridSize;
	int lineSize;
	int cellSize;
	int speed;
	bool draw;
	CRect clientRect;
	COLORREF cellColor;
	COLORREF lineColor;
	COLORREF backColor;
	Grid grid;
	int m_lastIndex;
	CPoint m_gridShift;
	CScrollBar m_vSBar;
	CScrollBar m_hSBar;
	SCROLLINFO h_SInfo;
	SCROLLINFO v_SInfo;
};

#ifndef _DEBUG  // debug version in GOLView.cpp
inline CGOLDoc* CGOLView::GetDocument() const
   { return reinterpret_cast<CGOLDoc*>(m_pDocument); }
#endif

