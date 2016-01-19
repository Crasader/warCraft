#ifndef FUZZYSET_SINGLETON_H
#define FUZZYSET_SINGLETON_H
//-----------------------------------------------------------------------------
//
//  Name:   FuzzySet_Singleton.h
//
//  Author: angel
//
//  Desc:   This defines a fuzzy set that is a singleton (a range
//          over which the DOM is always 1.0)
//-----------------------------------------------------------------------------
#include "FuzzySet.h"
#include "misc/utils.h"



class FuzzySet_Singleton : public FuzzySet
{
private:

    
  double   m_dMidPoint;
  double   m_dLeftOffset;
  double   m_dRightOffset;

public:
  
  FuzzySet_Singleton(double       mid,
                     double       lft,
                     double       rgt):FuzzySet(mid),
                                      m_dMidPoint(mid),
                                      m_dLeftOffset(lft),
                                      m_dRightOffset(rgt)
  {}

 
  double     CalculateDOM(double val)const; 
};


#endif