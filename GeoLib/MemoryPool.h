/**--------------------------------------------------------------------------<BR>
\file MemoryPool.h
\brief Declaration file for the CMemoryPool class.

Declaration file for the CMemoryPool class which allocates large chuncks on the
heap to speed things up.
<P>---------------------------------------------------------------------------*/

#pragma once


#include <vector>


#define _MEMORY_POOL_DECLARATION_PURE	virtual void* operator new(unsigned int) = 0;\
										virtual void* operator new(unsigned int, const char*,int) = 0;\
										virtual void operator delete(void* p) = 0;

#define _MEMORY_POOL_DECLARATION void* operator new(unsigned int);\
								 void* operator new(unsigned int, const char*,int);\
								 void operator delete(void* p);

#define _MEMORY_POOL_IMPLEMENATION(_TYPE) void* _TYPE::operator new(unsigned int) \
											{return CMemoryPool<_TYPE>::Allocate();} \
										  void _TYPE::operator delete(void* p) \
											{CMemoryPool<_TYPE>::Deallocate(p);} \
										  void* _TYPE::operator new(unsigned int, const char*,int) \
											{return CMemoryPool<_TYPE>::Allocate();}

#define _MEMORY_SIMPLE_IMPLEMENATION(_TYPE) void* _TYPE::operator new(unsigned int) \
											{return ::new _TYPE;} \
										  void _TYPE::operator delete(void* p) \
											{::delete p;} \
										  void* _TYPE::operator new(unsigned int, const char*,int) \
											{return ::new _TYPE;}


#define _BLOCK_SIZE 1000 // = 10kb / byte


template <class TYPE>
class CMemoryPool
{
public:
	/// Constructor
	CMemoryPool(void);
	/// Destructor
	~CMemoryPool(void);
	/// Allocated
	static void* Allocate(void);
	/// Deallocate
	static void Deallocate(void* pData);


//	static CMemoryPool<TYPE>& GetInstance(void);
private:

	void Clear(void);

	void* PAllocate(void);
	/// Deallocate
	void PDeallocate(void* pData);

	struct sList
	{
		TYPE cObject;
		sList* pNext;
	};

	sList* m_pList;

	std::vector<char*> m_Blocks;

	static CMemoryPool<TYPE>* m_spInstance;

	static unsigned int m_snCount;
};

template<class TYPE>
CMemoryPool<TYPE>* CMemoryPool<TYPE>::m_spInstance = 0;

template<class TYPE>
unsigned int CMemoryPool<TYPE>::m_snCount = 0;

template<class TYPE>
/**--------------------------------------------------------------------------<BR>
CMemoryPool<TYPE>::CMemoryPool <BR>
Constructor.
<P>---------------------------------------------------------------------------*/
CMemoryPool<TYPE>::CMemoryPool(void)
{
    m_pList = 0;
}


template<class TYPE>
/**--------------------------------------------------------------------------<BR>
CMemoryPool<TYPE>::CMemoryPool <BR>
Constructor.
<P>---------------------------------------------------------------------------*/
CMemoryPool<TYPE>::~CMemoryPool(void)
{
	Clear();
}


template<class TYPE>
/**--------------------------------------------------------------------------<BR>
CMemoryPool<TYPE>::Clear <BR>
Clear.
<P>---------------------------------------------------------------------------*/
void CMemoryPool<TYPE>::Clear(void)
{
	for (unsigned int i = 0; i < m_Blocks.size(); i++)
	{
		delete[] m_Blocks[i];
	}

	m_Blocks.clear();

    m_pList = 0;
}


template<class TYPE>
/**--------------------------------------------------------------------------<BR>
CMemoryPool<TYPE>::Allocate <BR>
Allocates memory.
<P>---------------------------------------------------------------------------*/
void* CMemoryPool<TYPE>::Allocate(void)
{
    if (m_spInstance == 0)
		m_spInstance = new CMemoryPool<TYPE>;

	m_snCount++;

	return m_spInstance->PAllocate();
}

template<class TYPE>
/**--------------------------------------------------------------------------<BR>
CMemoryPool<TYPE>::Allocate <BR>
Allocates memory.
<P>---------------------------------------------------------------------------*/
void CMemoryPool<TYPE>::Deallocate(void* pData)
{
	if (m_spInstance)
	{
		m_spInstance->PDeallocate(pData);
		m_snCount--;
		if (m_snCount == 0)
		{
			delete m_spInstance;
            m_spInstance = 0;
		}
	}
}


template<class TYPE>
/**--------------------------------------------------------------------------<BR>
CMemoryPool<TYPE>::Allocate <BR>
Allocates memory.
<P>---------------------------------------------------------------------------*/
void* CMemoryPool<TYPE>::PAllocate(void)
{

	// If we have no data then create some
    if (m_pList == 0)
	{
		// Create a load of items - just allocate the memory
	//	sList* pData = new sList[_BLOCK_SIZE];

		char* pData = new char[sizeof(sList) * _BLOCK_SIZE];
		// Record this for later deletion
		m_Blocks.push_back(pData);
		// Create a linked list for allocation
		sList* pList = (sList*) pData;
		// Initialise the head
		m_pList = pList;
		// Create the linked list
		for (unsigned int i = 1; i < _BLOCK_SIZE; i++)
		{
		//	pData++;
			pData += sizeof(sList);
			pList->pNext = (sList*) pData;
			pList = pList->pNext;
		}
		// Terminate the list
        pList->pNext = 0;
	}
	// Get the return value
	sList* pList = m_pList;
	// Take it off the top of the list
	m_pList = m_pList->pNext;
	// Return it.
	return pList;
}


template<class TYPE>
/**--------------------------------------------------------------------------<BR>
CMemoryPool<TYPE>::Allocate <BR>
Deallocates/recycles memory.
<P>---------------------------------------------------------------------------*/
void CMemoryPool<TYPE>::PDeallocate(void* pData)
{
	// Cast this to a list entry
	sList* pList = (sList*)pData;
	// Insert it for reallocation.
	pList->pNext = m_pList;

	m_pList = pList;
}
