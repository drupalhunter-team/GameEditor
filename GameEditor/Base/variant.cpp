#include "include/variant.h"
#include <string>
#include <assert.h>

KVariant::KVariant()
{
	m_uType = eVT_STRING;
	m_Content.Init();
	m_IndexContent.Init();
}

KVariant::~KVariant()
{
	// ���� tree �ӽڵ�
	std::map<const char*,KTreeNode*>::iterator it = m_pChildren.begin();
	for (; it != m_pChildren.end(); ++it)
	{
		if (it->second) {
			delete it->second;
		}
	}

	// ���� list �ӽڵ�
	KListNode* pNode = m_pContainer;
	while (pNode)
	{
		KListNode* pNext = pNode->GetNext();
		delete pNode;
		pNode = pNext;
	}

	// �����Լ��е� content
	if (m_uType == eVT_STRING)
	{
		m_Content.Uninit();
	}
	// ���� index content
	m_IndexContent.Uninit();
}

KVariant& KVariant::operator[](int nIndex)
{
	KVariant* pNode = (KVariant*)m_pContainer;
	int nPos = 0;
	while (pNode)
	{
		if (pNode->m_nIndexNumer != 0)
			nPos = pNode->m_nIndexNumer;

		if (nPos == nIndex)
		{
			break;
		}
		pNode = (KVariant*)pNode->GetNext();
		nPos++;
	}

	if (pNode == NULL)
	{
		pNode = new KVariant;
		pNode->m_nIndexNumer = nIndex;

		AddTail(pNode);
	}

	return *pNode;
}

KVariant& KVariant::operator[](const char* szIndex)
{
	KVariant* pNode = (KVariant*)m_pChildren[szIndex];
	if (pNode == NULL)
	{
		pNode = new KVariant;
		m_pChildren[szIndex] = pNode;
		
		pNode->m_pParent = this;
		pNode->m_IndexContent.CopyString(szIndex);
	}

	return *pNode;
}

KVariant& KVariant::operator=(int nValue)
{
	SetNumber(nValue);
	return *this;
}

KVariant& KVariant::operator=(char* szValue)
{
	SetString(szValue);
	return *this;
}

KVariant& KVariant::operator=(const char* szValue)
{
	SetString(szValue);
	return *this;
}

KVariant& KVariant::operator=(KVariant& sVariant)
{
	// �ȿ��� content ����
	switch (sVariant.GetType())
	{
	case eVT_BOOL:
		SetBool(sVariant.GetBool());
		break;
	case eVT_FLOAT:
		SetFloat(sVariant.GetFloat());
		break;
	case eVT_LONG_NUMBER:
		SetLongNumber(sVariant.GetLongNumber());
		break;
	case eVT_NUMBER:
		SetNumber(sVariant.GetNumber());
		break;
	case eVT_STRING:
		SetString(sVariant.GetString());
		break;
	}

	// �ٿ��� index content
	if (sVariant.GetIndexName())
	{
		m_IndexContent.CopyString(sVariant.GetIndexName());
	}

	// Ȼ�󿽱� list
	KListNode* pVar = sVariant.GetHeader();
	while (pVar != NULL)
	{
		KVariant* pNewVariant = new KVariant;
		*pNewVariant = *((KVariant*)pVar);
		AddTail(pNewVariant);

		pVar = pVar->GetNext();
	}

	// ����� tree
	//std::map<const char*, KTreeNode*> vChildren = sVariant.GetTree();
	std::map<const char*,KTreeNode*>::iterator it = sVariant.m_pChildren.begin();
	for (; it != sVariant.m_pChildren.end(); ++it)
	{
		if (it->second) {
			KVariant* pVar = (KVariant*) it->second;
			KVariant* pNewVariant = new KVariant;
			*pNewVariant = *pVar;
			m_pChildren[pVar->GetIndexName()] = pNewVariant;

			pNewVariant->m_pParent = this;
			// pNewVariant->m_IndexContent.CopyString(pVar->GetIndexName());
		}
	}

	return *this;
}

/************************************************************************/
/* Set & Get                                                            */
/************************************************************************/
void KVariant::SetNumber(int nNumber)
{
	if (m_uType == eVT_STRING)
	{
		m_Content.Uninit();
	}

	m_uType = eVT_NUMBER;
	m_nNumber = nNumber;
}

int KVariant::GetNumber()
{
	if (m_uType == eVT_NUMBER)
	{
		return m_nNumber;
	}

	return 0;
}

void KVariant::SetLongNumber(long long number)
{
	if (m_uType == eVT_STRING)
	{
		m_Content.Uninit();
	}

	m_uType = eVT_LONG_NUMBER;
	m_llNumber = number;
}


long long KVariant::GetLongNumber()
{
	if (m_uType == eVT_LONG_NUMBER)
	{
		return m_llNumber;
	}

	return 0;
}

void KVariant::SetFloat(float fNumber)
{
	if (m_uType == eVT_STRING)
	{
		m_Content.Uninit();
	}

	m_uType = eVT_FLOAT;
	m_fNumber = fNumber;
}

float KVariant::GetFloat()
{
	if (m_uType == eVT_FLOAT)
	{
		return m_fNumber;
	}

	return 0;
}

void KVariant::SetBool(bool bValue)
{
	if (m_uType == eVT_BOOL)
	{
		m_Content.Uninit();
	}

	m_uType = eVT_BOOL;
	m_bValue = bValue;
}

bool KVariant::GetBool()
{
	if (m_uType == eVT_BOOL)
	{
		return m_bValue;
	}

	return false;
}

void KVariant::SetString(const char* szContent, int nSize)
{
	if (m_uType == eVT_STRING)
	{
		m_Content.Uninit();
	}

	m_uType = eVT_STRING;
	if (szContent)
	{
		m_Content.CopyString(szContent, nSize);
	}
}

char* KVariant::GetString()
{
	if (m_uType == eVT_STRING)
	{
		return m_Content.GetBuffer();
	}

	return NULL;
}

/************************************************************************/
/* Add                                                                     */
/************************************************************************/
void KVariant::AddNumber(int nNumber)
{
	KVariant* pNewVariant = new KVariant;
	pNewVariant->SetNumber(nNumber);

	AddTail(pNewVariant);
}

void KVariant::AddLongNumber(long long number)
{
	KVariant* pNewVariant = new KVariant;
	pNewVariant->SetLongNumber(number);

	AddTail(pNewVariant);
}

void KVariant::AddFloat(float fValue)
{
	KVariant* pNewVariant = new KVariant;
	pNewVariant->SetFloat(fValue);

	AddTail(pNewVariant);
}

void KVariant::AddBool(bool bValue)
{
	KVariant* pNewVariant = new KVariant;
	pNewVariant->SetBool(bValue);

	AddTail(pNewVariant);	
}

void KVariant::AddString(const char* szContent, int nSize /* = 0 */)
{
	KVariant* pNewVariant = new KVariant;
	pNewVariant->SetString(szContent, nSize);

	AddTail(pNewVariant);
}

/************************************************************************/
/* to                                                                     */
/************************************************************************/
int KVariant::ToNumber()
{
	assert(0);
	return 1;
}

long long KVariant::ToLongNumber()
{
	assert(0);
	return 1;
}

float KVariant::ToFloat()
{
	assert(0);
	return 0.0;
}

bool KVariant::ToBool()
{
	assert(0);
	return false;
}

const char* KVariant::ToString()
{
	assert(0);
	return (char *)"";
}