// GameEditor.cpp : Defines the entry point for the console application.
//

// Including SDKDDKVer.h defines the highest available Windows platform.

// If you wish to build your application for a previous Windows platform, include WinSDKVer.h and
// set the _WIN32_WINNT macro to the platform you wish to support before including SDKDDKVer.h.

#include "main.h"
#include "../Base/include/tabfile.h"
#include "../Base/include/variant.h"

int _tmain(int argc, _TCHAR* argv[])
{
	// ��ȡ�� tab �ļ���תΪ variant
	KTabFile file;
	file.InitWithPath("../../Configure/card.txt");

	KVariant var;
	list<vector<char*>>& content = file.GetContent();
	list<vector<char*>>::iterator it = content.begin();
	vector<char*>& lstTitle = file.GetTitle();
	int i = 0;
	for (; it != content.end(); it++)
	{
		KVariant temp;
		for (int n = 0; n < lstTitle.size(); n ++)
		{
			temp[lstTitle[n]] = (*it)[n];
		}
		
		var[i] = temp;
		i++;
	}

	// variant ��ֵ��һ�� cardhelper ���󣬲����䴴��һ�� card ����
	var.ShowVariant();

	// ʹ�� cardhelper ���󣬱������ݿ�

	return 0;
}

