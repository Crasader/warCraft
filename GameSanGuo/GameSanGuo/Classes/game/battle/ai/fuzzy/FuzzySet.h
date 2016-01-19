#ifndef FUZZYSET_H
#define FUZZYSET_H
//-----------------------------------------------------------------------------
//
//  Name:   FuzzySet.h
//
//  Author: angel
//
//  Desc:   class to define an interface for a fuzzy set
//-----------------------------------------------------------------------------
#include <string>
#include <cassert>


class FuzzySet
{
protected:
  
  
  double        m_dDOM;

  
  double        m_dRepresentativeValue;

public:

  FuzzySet(double RepVal):m_dDOM(0.0), m_dRepresentativeValue(RepVal){}

 
  virtual double      CalculateDOM(double val)const = 0;

 
  void               ORwithDOM(double val){if (val > m_dDOM) m_dDOM = val;}

 
  double             GetRepresentativeVal()const{return m_dRepresentativeValue;}
  
  void               ClearDOM(){m_dDOM = 0.0;}  
  double             GetDOM()const{return m_dDOM;}
  void               SetDOM(double val)
  {
    assert ((val <=1) && (val >= 0) && "<FuzzySet::SetDOM>: invalid value");
    m_dDOM = val;
  }
};


#endif