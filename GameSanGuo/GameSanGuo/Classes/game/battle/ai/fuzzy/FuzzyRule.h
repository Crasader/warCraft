#ifndef FUZZY_RULE_H
#define FUZZY_RULE_H
//-----------------------------------------------------------------------------
//
//  Name:   FuzzyRule.h
//
//  Author: angel
//
//  Desc:   This class implements a fuzzy rule of the form:
//  
//          IF fzVar1 AND fzVar2 AND ... fzVarn THEN fzVar.c
//
//          
//-----------------------------------------------------------------------------
#include <vector>
#include "FuzzySet.h"
#include "FuzzyOperators.h"
#include "misc/utils.h"


class FuzzyRule
{ 
private:

  
  const FuzzyTerm*  m_pAntecedent;

  
  FuzzyTerm*        m_pConsequence;

  
  FuzzyRule(const FuzzyRule&);
  FuzzyRule& operator=(const FuzzyRule&);


public:

  FuzzyRule(const FuzzyTerm& ant,
            const FuzzyTerm& con):m_pAntecedent(ant.Clone()),
                                  m_pConsequence(con.Clone())
  {}

  ~FuzzyRule(){delete m_pAntecedent; delete m_pConsequence;}

  void SetConfidenceOfConsequentToZero(){m_pConsequence->ClearDOM();}

  
  void Calculate()
  {
    m_pConsequence->ORwithDOM(m_pAntecedent->GetDOM());
  }
};

#endif