////////////////////////////////////////////////////////////////////////
// \file    KnobFitter.h
// \brief   Fitter to determine g4rwgt knob values from experimental data
// \author  $Author: csweeney $
// \date    $Date: 2021-02-26 $
////////////////////////////////////////////////////////////////////////
#ifndef MYFITTER_H
#define MYFITTER_H

#include <vector>
#include <iostream>
#include <map>
#include <string>

#include "TRandom3.h"
#include "Math/Minimizer.h"
#include "Math/Factory.h"
#include "Math/Functor.h"

namespace g4rwgt
{
  /// \brief Does fitting for geant4reweight knobs
  class KnobFitter
  {

  public:
    /// \name KnobFitter Constructor
    //@{
    KnobFitter(double r_mcp1, double r_mcp2, std::vector<double> desired_gaps);
    //@}

    virtual ~KnobFitter() = default;

    void DoFit();
    int GetFitStatus() {return fFitStatus;}

    void PrintFitVals();
    void PrintPars();
  private:

    void DefineFunc();

    double Chi2(const double *parVals);
    void CalcErrors();


    // Member variables
    static constexpr int fNPars = 5;
    std::unique_ptr<ROOT::Math::Minimizer> fMinimizer = NULL;

    int fFitStatus = 0;
    ROOT::Math::Functor fFunc;
    int fCallNum = 0; // way of keeping track of how many times Chi2 function has been called
    double fChi2;

    double fFitVals[fNPars];

    std::vector<double> fGaps;
    double fR_mcp1 = 0;
    double fR_mcp2 = 0;

    double fV_in;
  };


} // end namespace

#endif // MYFITTER_H
//////////////////////////////////////////////////////////////////////////////
