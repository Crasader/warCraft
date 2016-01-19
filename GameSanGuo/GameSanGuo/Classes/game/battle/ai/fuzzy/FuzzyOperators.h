#ifndef FUZZY_OPERATORS_H
#define FUZZY_OPERATORS_H
//-----------------------------------------------------------------------------
//
//  Name:   FuzzyOperators.h
//
//  Author:angel
//
//  Desc:   classes to provide the fuzzy AND and OR operators to be used in
//          the creation of a fuzzy rule base
//-----------------------------------------------------------------------------
#include <vector>
#include <cassert>
#include "misc/utils.h"
#include "FuzzyTerm.h"

///////////////////////////////////////////////////////////////////////////////
//
//  a fuzzy AND operator class
//
///////////////////////////////////////////////////////////////////////////////
class FzAND : public FuzzyTerm
{
private:

 
  std::vector<FuzzyTerm*> m_Terms;

 
  FzAND& operator=(const FzAND&);

public:

  ~FzAND();

 
  FzAND(const FzAND& fa);
   
 
  FzAND(FuzzyTerm& op1, FuzzyTerm& op2);
  FzAND(FuzzyTerm& op1, FuzzyTerm& op2, FuzzyTerm& op3);
  FzAND(FuzzyTerm& op1, FuzzyTerm& op2, FuzzyTerm& op3, FuzzyTerm& op4);

  
  FuzzyTerm* Clone()const{return new FzAND(*this);}
  
  double GetDOM()const;
  void  ClearDOM();
  void  ORwithDOM(double val);
};


///////////////////////////////////////////////////////////////////////////////
//
//  a fuzzy OR operator class
//
///////////////////////////////////////////////////////////////////////////////
class FzOR : public FuzzyTerm
{
private:

 
  std::vector<FuzzyTerm*> m_Terms;

 
  FzOR& operator=(const FzOR&);

public:

  ~FzOR();

  
  FzOR(const FzOR& fa);
   
  
  FzOR(FuzzyTerm& op1, FuzzyTerm& op2);
  FzOR(FuzzyTerm& op1, FuzzyTerm& op2, FuzzyTerm& op3);
  FzOR(FuzzyTerm& op1, FuzzyTerm& op2, FuzzyTerm& op3, FuzzyTerm& op4);

 
  FuzzyTerm* Clone()const{return new FzOR(*this);}
  
  double GetDOM()const;

  
  void ClearDOM(){assert(0 && "<FzOR::ClearDOM>: invalid context");}
  void ORwithDOM(double val){assert(0 && "<FzOR::ORwithDOM>: invalid context");}
};



#endif

