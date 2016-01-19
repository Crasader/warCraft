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
#include "utils.h"



class FuzzySet_Triangle : public FuzzySet
{
private:

  
  double   m_dMidPoint;
  double   m_dLeftOffset;
  double   m_dRightOffset;

public:
  
  FuzzySet_Triangle(std::string name,
                double mid,
                double lft,
                double rgt):FuzzySet(name),
                           m_dMidPoint(mid),
                           m_dLeftOffset(lft),
                           m_dRightOffset(rgt)
  {}

 
  inline double CalculateDOM(double val);

 
  double RepresentativeValue()const{return m_dMidPoint;}
  
};

///////////////////////////////////////////////////////////////////////////////

inline
double FuzzySet_Triangle::CalculateDOM(double val)
{
  
  if ( (isEqual(m_dRightOffset, 0.0) && (isEqual(m_dMidPoint, val))) ||
       (isEqual(m_dLeftOffset, 0.0) && (isEqual(m_dMidPoint, val))) )
  {
    return 1.0;
  }

  
  if ( (val <= m_dMidPoint) && (val > (m_dMidPoint - m_dLeftOffset)) )
  {
    double grad = 1.0 / m_dLeftOffset;

    return grad * (val - (m_dMidPoint - m_dLeftOffset));
  }
  
  else if ( (val > m_dMidPoint) && (val < (m_dMidPoint + m_dRightOffset)) )
  {
    double grad = 1.0 / -m_dRightOffset;

    return grad * (val - m_dMidPoint) + 1.0;
  }
  
  else
  {
    return 0.0;
  }

}

#endif