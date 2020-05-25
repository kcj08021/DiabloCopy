#include "stdafx.h"
#include "Item.h"


void CItem::UpdateRect(){
		D3DXVECTOR3 vScreenPos = m_tInfo.vPos - m_pScrollMgr->GetScroll();
		m_tRect.left = static_cast<LONG>(vScreenPos.x) - (m_ptRectSize.x >> 1);
		m_tRect.top = static_cast<LONG>(vScreenPos.y) - (m_ptRectSize.y >> 1);

		m_tRect.right = static_cast<LONG>(vScreenPos.x) + (m_ptRectSize.x >> 1);
		m_tRect.bottom = static_cast<LONG>(vScreenPos.y) + (m_ptRectSize.y >> 1);
}

CItem::CItem(): m_iWidth(0), m_iHeight(0), m_bIsField(false), m_bIsSoundOut(false), m_iIndex(0){
}


CItem::~CItem(){
}