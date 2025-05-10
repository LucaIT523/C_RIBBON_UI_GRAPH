#include "stdafx.h"

#include "..\include\PathString.h"

CPathString::CPathString()
{
	initZero();
}

CPathString::~CPathString()
{
}

void CPathString::initFromExe()
{
	::GetModuleFileName(NULL, m_pchPath, sizeof(m_pchPath));
	split();
}

void CPathString::initZero()
{
	ZeroMemory(m_pchPath, sizeof(m_pchPath));
	ZeroMemory(m_pchDrive, sizeof(m_pchDrive));
	ZeroMemory(m_pchDir, sizeof(m_pchDir));
	ZeroMemory(m_pchFname, sizeof(m_pchFname));
	ZeroMemory(m_pchExt, sizeof(m_pchExt));
}

void CPathString::split()
{
	_tsplitpath(m_pchPath, m_pchDrive, m_pchDir, m_pchFname, m_pchExt);
}

void CPathString::make()
{
}

void CPathString::setPath(LPCTSTR pchPath)
{
	_tcscpy(m_pchPath, pchPath);
	split();
}

CString CPathString::getDrive() const
{
	return m_pchDrive;
}

CString CPathString::getDir() const
{
	return m_pchDir;
}

CString CPathString::getFileName() const
{
	return m_pchFname;
}

CString CPathString::getExt() const
{
	return m_pchExt;
}
