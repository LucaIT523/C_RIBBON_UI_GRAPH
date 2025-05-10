
#include "stdafx.h"
#include "afxdllx.h"

static AFX_EXTENSION_MODULE extensionDLL;

extern "C" int APIENTRY 
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		// Extension DLL one-time initialization 
		if (!AfxInitExtensionModule(
			extensionDLL, hInstance))
			return 0;

		LoadResource()

		// TODO: perform other initialization tasks here
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		// Extension DLL per-process termination
//		AfxTermExtensionModule(extensionDLL);

		// TODO: perform other cleanup tasks here
	}
	return 1;   // ok
}
