#ifndef FUZZYSET_TRIANGLE_H
#define FUZZYSET_TRIANGLE_H
//-----------------------------------------------------------------------------
//
//  Name:   FuzzySetTriangle.h
//
//  Author: angel
//
//  Desc:   This is a simple class to define fuzzy sets that have a triangular 
//          shape and can be defined by a mid point, a left displacement and a
//          right displacement. 
//-----------------------------------------------------------------------------
#include "FuzzySet.h"
#include "misc/utils.h"



class FuzzySet_Triangle : public FuzzySet
{
private:

  
  double   m_dPeakPoint;
  double   m_dLeftOffset;
  double   m_dRightOffset;

public:
  
  FuzzySet_Triangle(double mid,
                    double lft,
                    double rgt):FuzzySet(mid), 
                               m_dPeakPoint(mid),
                               m_dLeftOffset(lft),
                               m_dRightOffset(rgt)
  {}

 
  double CalculateDOM(double val)const;
};



#endif