#ifndef FLV_H
#define FLV_H
#pragma warning (disable:4786)
//-----------------------------------------------------------------------------
//
//  Name:   FuzzyVariable.h
//
//  Author: angel
//
//  Desc:   Class to define a fuzzy linguistic variable (FLV).
//
//          An FLV comprises of a number of fuzzy sets  
//
//-----------------------------------------------------------------------------
#include <map>
#include <iosfwd>
#include <string>

class FuzzySet;
class FzSet;
class FuzzyModule;


class FuzzyVariable
{
private:
  
  typedef std::map<std::string, FuzzySet*>  MemberSets;
    
public:
  
 
  FuzzyVariable(const FuzzyVariable&);
  FuzzyVariable& operator=(const FuzzyVariable&);

private:
 
 
  MemberSets   m_MemberSets;

 
  double        m_dMinRange;
  double        m_dMaxRange;
  

  
  void AdjustRangeToFit(double min, double max);

  
  ~FuzzyVariable();

  friend class FuzzyModule;


public:

  FuzzyVariable():m_dMinRange(0.0),m_dMaxRange(0.0){}
  
  
  FzSet  AddLeftShoulderSet(std::string name, double minBound, double peak, double maxBound);

  FzSet  AddRightShoulderSet(std::string name, double minBound, double peak, double maxBound);

  FzSet  AddTriangularSet(std::string name,
                             double       minBound,
                             double       peak,
                             double       maxBound);

  FzSet  AddSingletonSet(std::string name,
                            double       minBound,
                            double       peak,
                            double       maxBound);
  
  
  
  void        Fuzzify(double val);

  
  double       DeFuzzifyMaxAv()const;

 
  double       DeFuzzifyCentroid(int NumSamples)const;



  std::ostream& WriteDOMs(std::ostream& os);
};




          
#endif