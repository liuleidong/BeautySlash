/*---------------------------------------------------------------------------
Copyright (C) GeoLib.
This code is used under license from GeoLib (www.geolib.co.uk). This or
any modified versions of this cannot be resold to any other party.
---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------<BR>
\file Sort.h
\brief File for the sorting templates, all of which use the quick sort algorithm.

File for a number of sorting algothithms, all of which sort items using the 
quick sort algorithm. Main algorithms as follows:
1. Simple sort which sorts items in ascending order.
2. Parallel sort which sorts items according to one array whilst sorting another
in Parallel.
3. A sort which uses a function call of the class being sorted e.g. GetLength().
4. A sort which uses a function to determine which item out of 2 should come
first in the array.

Any type of array can be sorted using the functions which ask to specify the
limits of the sort (normally between the 0 and max element) but for convienience, 
there are wrapper functions for all which sort between the 0 element and that 
given by the size() function of the array less 1. 
<P>---------------------------------------------------------------------------*/


#ifndef _GEOLIB_SORT_H 
#define _GEOLIB_SORT_H



/**--------------------------------------------------------------------------<BR>
\namespace GeoSort
\brief Namespace for a number of sorting algothithms, all of which sort items 
using variations of the quick sort algorithm.
<P>---------------------------------------------------------------------------*/
namespace GeoSort
{


/**--------------------------------------------------------------------------<BR>
GeoSort::ReverseOrder
\brief Reverses the order of an array.
<P>---------------------------------------------------------------------------*/
template<class ARRAY_TYPE, class TYPE>  
void ReverseOrder(ARRAY_TYPE& Array) 
{
	TYPE Temp;
	unsigned int nCount = (unsigned int) Array.size();
	unsigned int nHalfWay =  nCount / 2;

	for (unsigned int i = 0; i < nHalfWay ; i++)
	{
		Temp = Array[i];
		Array[i] = Array[nCount - 1 - i];
		Array[nCount - 1 - i] = Temp;
	}	
}

	
/**--------------------------------------------------------------------------<BR>
GeoSort::FQuickSort
\brief Wrapper for the function call sort.
<P>---------------------------------------------------------------------------*/
template<class ARRAY_TYPE, class TYPE, typename COMPTYPE>  
void FQuickSort(ARRAY_TYPE& Array, COMPTYPE (TYPE::*pFunctionCall)(void) const) 
{
	FQuickSort<ARRAY_TYPE, TYPE, COMPTYPE>(Array,  pFunctionCall , (int) 0, (int)(Array.size() - 1 ));
}

/**--------------------------------------------------------------------------<BR>
GeoSort::FQuickSort
\brief Sorts an array by calling a function for each object in the array e.g.
GetLength().
<P>---------------------------------------------------------------------------*/
template<class ARRAY_TYPE, class TYPE, typename COMPTYPE>  
void FQuickSort(ARRAY_TYPE& Array, COMPTYPE (TYPE::*pFunctionCall)(void) const , int lo0, int hi0)
{
	int lo = lo0;
	int hi = hi0;
	if (lo >= hi)  return;

    else if( lo == hi - 1 )
	{
            // sort a two element list by swapping if necessary 
            if ((Array[lo].*pFunctionCall)() > (Array[hi].*pFunctionCall)()) 
			{
                TYPE T = Array[lo];
                Array[lo] = Array[hi];
                Array[hi] = T;
            }
            return;
	}

    //  Pick a pivot and move it out of the way

	TYPE pivot = Array[(lo + hi) / 2];
        Array[(lo + hi) / 2] = Array[hi];
        Array[hi] = pivot;

   while( lo < hi ) 
   {
        //  Search forward from a[lo] until an element is found that
        //  is greater than the pivot or lo >= hi 
            
		while ((Array[lo].*pFunctionCall)() <= (pivot.*pFunctionCall)() && lo < hi) 
		{
			lo++;
		}
		// Search backward from a[hi] until element is found that
		//  is less than the pivot, or lo >= hi
		     
		while ((pivot.*pFunctionCall)() <= (Array[hi].*pFunctionCall)() && lo < hi ) 
		{
			hi--;
		}
        //  Swap elements a[lo] and a[hi]
		if( lo < hi ) 
		{
			TYPE T = Array[lo];
			Array[lo] = Array[hi];
			Array[hi] = T;
		}
	}

    //  Put the median in the "center" of the list

	Array[hi0] = Array[hi];
	Array[hi] = pivot;

  	//Recursive calls, elements a[lo0] to a[lo-1] are less than or
	//equal to pivot, elements a[hi+1] to a[hi0] are greater than
	//pivot.
         
	FQuickSort<ARRAY_TYPE, TYPE>(Array, pFunctionCall, lo0, lo-1);
	FQuickSort<ARRAY_TYPE, TYPE>(Array, pFunctionCall, hi+1, hi0);
}


/**--------------------------------------------------------------------------<BR>
GeoSort::FQuickSort
\brief Wrapper for the function based sort.
<P>---------------------------------------------------------------------------*/
template<class ARRAY_TYPE, class TYPE>  
void FQuickSort(ARRAY_TYPE& Array, bool (*pSwitchIfTrue)(TYPE&, TYPE&)) 
{
	//  if pComparision returns true, the first element should be ordered after the second.
	FQuickSort<ARRAY_TYPE, TYPE>(Array,  pSwitchIfTrue , (int) 0, (int)(Array.size() - 1 ));
}


/**--------------------------------------------------------------------------<BR>
GeoSort::FQuickSort
\brief Sorts an array by calling a function which takes 2 objects in the array 
and returns true if the first should be ordered after the second.
<P>---------------------------------------------------------------------------*/
template<class ARRAY_TYPE, class TYPE>  
void FQuickSort(ARRAY_TYPE& Array, bool (*pSwitchIfTrue)(TYPE&, TYPE&), int lo0, int hi0) 
{
	//  if pComparision returns true, the first element should be ordered after the second.
	int lo = lo0;
	int hi = hi0;
	if (lo >= hi)  return;

    else if( lo == hi - 1 )
	{
            // sort a two element list by swapping if necessary 
            if ( pSwitchIfTrue(Array[lo], Array[hi] )) /*Array[lo] > Array[hi])*/ 
			{
                TYPE T = Array[lo];
                Array[lo] = Array[hi];
                Array[hi] = T;
            }
            return;
	}

    //  Pick a pivot and move it out of the way

	TYPE pivot = Array[(lo + hi) / 2];
        Array[(lo + hi) / 2] = Array[hi];
        Array[hi] = pivot;

   while( lo < hi ) 
   {
        //  Search forward from a[lo] until an element is found that
        //  is greater than the pivot or lo >= hi 
            
		while ( !pSwitchIfTrue(Array[lo], pivot)   /*Array[lo] <= pivot*/ && lo < hi) 
		{
			lo++;
		}
		// Search backward from a[hi] until element is found that
		//  is less than the pivot, or lo >= hi
		     
		while (!pSwitchIfTrue(pivot, Array[hi]) /*pivot <= Array[hi]*/ && lo < hi ) 
		{
			hi--;
		}
        //  Swap elements a[lo] and a[hi]
		if( lo < hi ) 
		{
			TYPE T = Array[lo];
			Array[lo] = Array[hi];
			Array[hi] = T;
		}
	}

    //  Put the median in the "center" of the list

	Array[hi0] = Array[hi];
	Array[hi] = pivot;

  	//Recursive calls, elements a[lo0] to a[lo-1] are less than or
	//equal to pivot, elements a[hi+1] to a[hi0] are greater than
	//pivot.
         
	FQuickSort<ARRAY_TYPE, TYPE>(Array,  pSwitchIfTrue , lo0, lo-1);
	FQuickSort<ARRAY_TYPE, TYPE>(Array, pSwitchIfTrue  , hi+1, hi0);
}


/**--------------------------------------------------------------------------<BR>
GeoSort::SQuickSort
\brief Wrapper for a simple quicksort.
<P>---------------------------------------------------------------------------*/
template<class ARRAY_TYPE, class TYPE>  
void SQuickSort(ARRAY_TYPE& Array)
{
	SQuickSort<ARRAY_TYPE, TYPE>(Array,(int) 0, (int)(Array.size() - 1 ));
}


/**--------------------------------------------------------------------------<BR>
GeoSort::SQuickSort
\brief Straight forward quicksort.
<P>---------------------------------------------------------------------------*/
template<class ARRAY_TYPE, class TYPE>
void SQuickSort(ARRAY_TYPE& Array, int lo0, int hi0)
{
	int lo = lo0;
	int hi = hi0;
	if (lo >= hi)  return;

    else if( lo == hi - 1 )
	{
            // sort a two element list by swapping if necessary 
            if (Array[lo] > Array[hi]) 
			{
                TYPE T = Array[lo];
                Array[lo] = Array[hi];
                Array[hi] = T;
            }
            return;
	}

    //  Pick a pivot and move it out of the way

	TYPE pivot = Array[(lo + hi) / 2];
        Array[(lo + hi) / 2] = Array[hi];
        Array[hi] = pivot;

   while( lo < hi ) 
   {
        //  Search forward from a[lo] until an element is found that
        //  is greater than the pivot or lo >= hi 
            
		while (Array[lo] <= pivot && lo < hi) 
		{
			lo++;
		}
		// Search backward from a[hi] until element is found that
		//  is less than the pivot, or lo >= hi
		     
		while (pivot <= Array[hi] && lo < hi ) 
		{
			hi--;
		}
        //  Swap elements a[lo] and a[hi]
		if( lo < hi ) 
		{
			TYPE T = Array[lo];
			Array[lo] = Array[hi];
			Array[hi] = T;
		}
	}

    //  Put the median in the "center" of the list

	Array[hi0] = Array[hi];
	Array[hi] = pivot;

  	//Recursive calls, elements a[lo0] to a[lo-1] are less than or
	//equal to pivot, elements a[hi+1] to a[hi0] are greater than
	//pivot.
         
	SQuickSort<ARRAY_TYPE, TYPE>(Array, lo0, lo-1);
	SQuickSort<ARRAY_TYPE, TYPE>(Array, hi+1, hi0);
}


/**--------------------------------------------------------------------------<BR>
GeoSort::SQuickSort
\brief Parallel quicksort which sorts the first array by its elements whilst
also sorting the second in the same way.
<P>---------------------------------------------------------------------------*/
template<class ARRAY_TYPE, class TYPE, class PARRAY_TYPE, class PTYPE>
void PQuickSort(ARRAY_TYPE& Array, PARRAY_TYPE& ParArray, int lo0, int hi0)
{
    int lo = lo0;
    int hi = hi0;
    if (lo >= hi)  return;

    else if( lo == hi - 1 )
    {
            // sort a two element list by swapping if necessary
            if (Array[lo] > Array[hi])
            {
                TYPE T = Array[lo];
                Array[lo] = Array[hi];
                Array[hi] = T;

                PTYPE PAR = ParArray[lo];
                ParArray[lo] = ParArray[hi];
                ParArray[hi] = PAR;
            }
            return;
    }

    //  Pick a pivot and move it out of the way

    TYPE pivot = Array[(lo + hi) / 2];
        Array[(lo + hi) / 2] = Array[hi];
        Array[hi] = pivot;

    PTYPE ParPivot = ParArray[(lo + hi) / 2];
        ParArray[(lo + hi) / 2] = ParArray[hi];
        ParArray[hi] = ParPivot;


   while( lo < hi )
   {
        //  Search forward from a[lo] until an element is found that
        //  is greater than the pivot or lo >= hi

        while (Array[lo] <= pivot && lo < hi)
        {
            lo++;
        }
        // Search backward from a[hi] until element is found that
        //  is less than the pivot, or lo >= hi

        while (pivot <= Array[hi] && lo < hi )
        {
            hi--;
        }
        //  Swap elements a[lo] and a[hi]
        if( lo < hi )
        {
            TYPE T = Array[lo];
            Array[lo] = Array[hi];
            Array[hi] = T;

            PTYPE PAR = ParArray[lo];
            ParArray[lo] = ParArray[hi];
            ParArray[hi] = PAR;

        }
    }

    //  Put the median in the "center" of the list

    Array[hi0] = Array[hi];
    Array[hi] = pivot;

    ParArray[hi0] = ParArray[hi];
    ParArray[hi] = ParPivot;

    //Recursive calls, elements a[lo0] to a[lo-1] are less than or
    //equal to pivot, elements a[hi+1] to a[hi0] are greater than
    //pivot.

    PQuickSort< ARRAY_TYPE, TYPE, PARRAY_TYPE, PTYPE>(Array, ParArray, lo0, lo-1);
    PQuickSort<ARRAY_TYPE, TYPE, PARRAY_TYPE, PTYPE>(Array, ParArray, hi+1, hi0);
}


/**--------------------------------------------------------------------------<BR>
GeoSort::SQuickSort
\brief Wrapper for a Parallel quicksort.
<P>---------------------------------------------------------------------------*/
template<class ARRAY_TYPE, class TYPE, class PARRAY_TYPE, class PTYPE> 
void PQuickSort(ARRAY_TYPE& Array, PARRAY_TYPE& ParallelArray)
{
	PQuickSort<ARRAY_TYPE, TYPE, PARRAY_TYPE, PTYPE>(Array, ParallelArray, (int) 0, (int)(Array.size() - 1 ));
}




}


#endif




