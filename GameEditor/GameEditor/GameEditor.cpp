// GameEditor.cpp : Defines the entry point for the console application.
//

// Including SDKDDKVer.h defines the highest available Windows platform.

// If you wish to build your application for a previous Windows platform, include WinSDKVer.h and
// set the _WIN32_WINNT macro to the platform you wish to support before including SDKDDKVer.h.

#include "main.h"
#include "../Base/include/tabfile.h"

int _tmain(int argc, _TCHAR* argv[])
{
	// ��ȡ�� tab �ļ���תΪ variant
	KTabFile file;
	file.InitWithPath("test.txt");

	// variant ��ֵ��һ�� cardhelper ���󣬲����䴴��һ�� card ����

	// ʹ�� cardhelper ���󣬱������ݿ�

	return 0;
}

