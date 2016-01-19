#ifndef FUZZYTERM_H
#define FUZZYTERM_H
//-----------------------------------------------------------------------------
//
//  Name:   FuzzyTerm.h
//
//  Author: angel
//
//  Desc:   abstract class to provide an interface for classes able to be
//          used as terms in a fuzzy if-then rule base.
//-----------------------------------------------------------------------------

class FuzzyTerm
{  
public:

  virtual ~FuzzyTerm(){}

  
  virtual FuzzyTerm* Clone()const = 0;

  
  virtual double      GetDOM()const=0;

 
  virtual void       ClearDOM()=0;

  
  virtual void       ORwithDOM(double val)=0;

   
};



#endif