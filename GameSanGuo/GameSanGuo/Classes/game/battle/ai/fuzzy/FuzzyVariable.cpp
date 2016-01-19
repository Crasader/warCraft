#include "fuzzyvariable.h"
#include "fuzzyoperators.h"
#include "FuzzySet_triangle.h"
#include "FuzzySet_LeftShoulder.h"
#include "FuzzySet_RightShoulder.h"
#include "FuzzySet_Singleton.h"
#include "FzSet.h"
#include "misc/utils.h"
#include <cassert>
#include <iostream>
#include <algorithm>


FuzzyVariable::~FuzzyVariable()
{
  MemberSets::iterator it;
  for (it = m_MemberSets.begin(); it != m_MemberSets.end(); ++it)
  {
      if(it->second)
        delete it->second;
  }
}


void FuzzyVariable::Fuzzify(double val)
{    
  
  assert ( (val >= m_dMinRange) && (val <= m_dMaxRange) && 
           "<FuzzyVariable::Fuzzify>: value out of range");

  
  MemberSets::const_iterator curSet;
  for (curSet = m_MemberSets.begin(); curSet != m_MemberSets.end(); ++curSet)
  {
    curSet->second->SetDOM(curSet->second->CalculateDOM(val));
  }
}



double FuzzyVariable::DeFuzzifyMaxAv()const
{
  double bottom = 0.0;
  double top    = 0.0;

  MemberSets::const_iterator curSet;
  for (curSet = m_MemberSets.begin(); curSet != m_MemberSets.end(); ++curSet)
  {
    bottom += curSet->second->GetDOM();

    top += curSet->second->GetRepresentativeVal() * curSet->second->GetDOM();
  }

  
  if (isEqual(0, bottom)) return 0.0;

  return top / bottom;   
}


double FuzzyVariable::DeFuzzifyCentroid(int NumSamples)const
{
 
  double StepSize = (m_dMaxRange - m_dMinRange)/(double)NumSamples;

  double TotalArea    = 0.0;
  double SumOfMoments = 0.0;

  
  for (int samp=1; samp<=NumSamples; ++samp)
  {
   
    MemberSets::const_iterator curSet = m_MemberSets.begin();
    for (curSet; curSet != m_MemberSets.end(); ++curSet)
    {
      double contribution = 
          MinOf(curSet->second->CalculateDOM(m_dMinRange + samp * StepSize),
                curSet->second->GetDOM());

      TotalArea += contribution;

      SumOfMoments += (m_dMinRange + samp * StepSize)  * contribution;
    }
  }

  
  if (isEqual(0, TotalArea)) return 0.0;
  
  return (SumOfMoments / TotalArea);
}


FzSet FuzzyVariable::AddTriangularSet(std::string name,
                                     double       minBound,
                                     double       peak,
                                     double       maxBound)
{
  m_MemberSets[name] = new FuzzySet_Triangle(peak,
                                             peak-minBound,
                                             maxBound-peak);
  
  AdjustRangeToFit(minBound, maxBound);

  return FzSet(*m_MemberSets[name]);
}


FzSet FuzzyVariable::AddLeftShoulderSet(std::string name,
                                        double       minBound,
                                        double       peak,
                                        double       maxBound)
{
  m_MemberSets[name] = new FuzzySet_LeftShoulder(peak, peak-minBound, maxBound-peak);

 
  AdjustRangeToFit(minBound, maxBound);

  return FzSet(*m_MemberSets[name]);
}



FzSet FuzzyVariable::AddRightShoulderSet(std::string name,
                                         double       minBound,
                                         double       peak,
                                         double       maxBound)
{
  m_MemberSets[name] = new FuzzySet_RightShoulder(peak, peak-minBound, maxBound-peak);

 
  AdjustRangeToFit(minBound, maxBound);

  return FzSet(*m_MemberSets[name]);
}



FzSet FuzzyVariable::AddSingletonSet(std::string name,
                                    double       minBound,
                                    double       peak,
                                    double       maxBound)
{
  m_MemberSets[name] = new FuzzySet_Singleton(peak,
                                              peak-minBound,
                                              maxBound-peak);

  AdjustRangeToFit(minBound, maxBound);

  return FzSet(*m_MemberSets[name]);
}


void FuzzyVariable::AdjustRangeToFit(double minBound, double maxBound)
{
  if (minBound < m_dMinRange) m_dMinRange = minBound;
  if (maxBound > m_dMaxRange) m_dMaxRange = maxBound;
}


std::ostream& FuzzyVariable::WriteDOMs(std::ostream& os)
{
  MemberSets::iterator it;
  for (it = m_MemberSets.begin(); it != m_MemberSets.end(); ++it)
  {

    os << "\n" << it->first << " is " << it->second->GetDOM();
  }

  os << "\nMin Range: " << m_dMinRange << "\nMax Range: " << m_dMaxRange;

    return os;
  
}