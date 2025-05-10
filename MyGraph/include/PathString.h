
class CPathString
{
public:
	CPathString();
	~CPathString();

	void setPath(LPCTSTR pchPath);
	CString getDrive() const;
	CString getDir() const;
	CString getFileName() const;
	CString getExt() const;

	void initFromExe();
	void initZero();
	
	void split();
	void make();

private:
	TCHAR m_pchPath[_MAX_PATH];
	TCHAR m_pchDrive[_MAX_DRIVE];
	TCHAR m_pchDir[_MAX_DIR];
	TCHAR m_pchFname[_MAX_FNAME];
	TCHAR m_pchExt[_MAX_EXT];
};
