/*---------------------------------------------------------------------------
Copyright (C) GeoLib.
This code is used under license from GeoLib (www.geolib.co.uk). This or
any modified versions of this cannot be resold to any other party.
---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------<BR>
\file IndexSet.cpp
\brief Implementation file for the CIndexSet class.
<P>---------------------------------------------------------------------------*/


#include "IndexSet.h"
#include "StdAfx.h"

/**--------------------------------------------------------------------------<BR>
\class CSetData
\brief Class to hold the index set data.
<P>---------------------------------------------------------------------------*/
class CSetData : public std::vector<unsigned int>
{

};


/**--------------------------------------------------------------------------<BR>
CIndexSet::CIndexSet <BR>
\brief Constructor.
<P>---------------------------------------------------------------------------*/
CIndexSet::CIndexSet(void)
{
	m_Data = new CSetData;
}

/**--------------------------------------------------------------------------<BR>
CIndexSet::~CIndexSet <BR>
\brief Destructor.
<P>---------------------------------------------------------------------------*/
CIndexSet::~CIndexSet(void)
{
	delete m_Data;
}

/**--------------------------------------------------------------------------<BR>
CIndexSet::Add <BR>
\brief Adds an index.
<P>---------------------------------------------------------------------------*/
void CIndexSet::Add(unsigned int nIndex)
{
	m_Data->push_back(nIndex);
}

/**--------------------------------------------------------------------------<BR>
CIndexSet::operator[] <BR>
\brief Data acccess.
<P>---------------------------------------------------------------------------*/
unsigned int& CIndexSet::operator[](unsigned int nIndex) const
{
	return (*m_Data)[nIndex];
}

/**--------------------------------------------------------------------------<BR>
CIndexSet::size <BR>
\brief size.
<P>---------------------------------------------------------------------------*/
unsigned int CIndexSet::size(void) const
{
	return m_Data->size();
}