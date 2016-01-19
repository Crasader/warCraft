#ifndef FUZZYSET_LEFTSHOULDER_H
#define FUZZYSET_LEFTSHOULDER_H
//-----------------------------------------------------------------------------
//
//  Name:   FuzzySet_LeftShoulder.h
//
//  Author: angel
//
//  Desc:   definition of a fuzzy set that has a left shoulder shape. (the
//          minimum value this variable can accept is *any* value less than the
//          midpoint.
//-----------------------------------------------------------------------------
#include "FuzzySet.h"
#include "misc/utils.h"



class FuzzySet_LeftShoulder : public FuzzySet
{
private:

 
  double   m_dPeakPoint;
  double   m_dRightOffset;
  double   m_dLeftOffset;

public:
  
  FuzzySet_LeftShoulder(double peak,
                        double LeftOffset,
                        double RightOffset):  
  
                  FuzzySet( ((peak - LeftOffset) + peak) / 2),
                  m_dPeakPoint(peak),
                  m_dLeftOffset(LeftOffset),
                  m_dRightOffset(RightOffset)
  {}

 
  double CalculateDOM(double val)const;  
};



#endif