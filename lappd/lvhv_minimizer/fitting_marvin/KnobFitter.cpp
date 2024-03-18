#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>

#include "Math/Factory.h"
#include "Math/Functor.h"
#include "Math/Minimizer.h"

#include "KnobFitter.h"

 
namespace g4rwgt
{
  KnobFitter::KnobFitter(double r_mcp1, double r_mcp2, std::vector<double> desired_gaps)
  {

    fR_mcp1 = r_mcp1;
    fR_mcp2 = r_mcp2;
    fGaps = desired_gaps;
    if(fGaps.size() != fNPars){
      std::cerr << "Expected exactly five gaps, but given " << fGaps.size() << "\n ";
      exit(1);
    }
    for(int i=0; i<fNPars; i++) fV_in += fGaps[i];

    std::cout << "Supplying " << fV_in << " volts in \n";
                                  
    fMinimizer = std::unique_ptr<ROOT::Math::Minimizer>
      ( ROOT::Math::Factory::CreateMinimizer( "Minuit2", "Migrad" ) );

    fMinimizer->SetMaxFunctionCalls(2000);
    fMinimizer->SetTolerance( 0.1 );
    fMinimizer->SetPrintLevel(1);

    fMinimizer->SetVariable( 0, "r_a", 0.2181, 0.001 );
    fMinimizer->SetVariableLimits( 0, 0.05, 0.8 );

    fMinimizer->SetVariable( 1, "r_b", 2.979, 0.01 );
    fMinimizer->SetVariableLimits( 1, 1.0, 8.0 );

    fMinimizer->SetVariable( 2, "r_c", 0.4334, 0.01 );
    fMinimizer->SetVariableLimits( 2, 0.1, 4.0 );

    fMinimizer->SetFixedVariable( 3, "r_d", 1e10);

    fMinimizer->SetVariable( 4, "r_e", 0.3173, 0.01 );
    fMinimizer->SetVariableLimits( 4, 0.1, 4.0 );


  }

  //......................................................................

  //ROOT::Math::Functor
  void KnobFitter::DefineFunc()
  {
    fFunc = ROOT::Math::Functor(
      [&](double const *coeffs) {
        double ret = Chi2(coeffs);
        return ret;
      },
      fNPars);
  }
  double KnobFitter::Chi2(const double *parVals)
  {
    double chi2 = 0.0;

    double r_mcp1_eff = 1./(1./parVals[1]  + 1./fR_mcp1);
    double r_mcp2_eff = 1./(1./parVals[3]  + 1./fR_mcp2);

    double r_total = parVals[0] + parVals[2] + parVals[4] +
      r_mcp1_eff + r_mcp2_eff;


    for(int i=0; i<fNPars; i++){
      double r;
      if(i == 1) r = r_mcp1_eff; 
      else if(i == 3) r = r_mcp2_eff; 
      else r = parVals[i];

      double desired = fGaps[i];
      double actual = (r/r_total)*fV_in;
      chi2 += std::abs(actual-desired) / desired;      
      if(fCallNum%1000 == 0){
	std::cout << actual << " - " << desired << "\n";
      }
    }

    fCallNum++;
    fChi2 = chi2;
    if(fCallNum%100 == 0){ 
      std::cout << "---" << fCallNum << "--- \n";
      std::cout<<"parameters: ";
      for(int i=0; i<fNPars; i++) std::cout << parVals[i] << " , ";
      std::cout << "\n";
      std::cout << "chi2 "<<fChi2 << "\n";
    }
    return chi2;
  }


  //......................................................................
  void KnobFitter::DoFit(){

    DefineFunc();
    fMinimizer->SetFunction(fFunc);
    fFitStatus = fMinimizer->Minimize();

    std::cout << "FitStatus: " << fFitStatus << "\n";

    if(fFitStatus){
      for(int i=0; i<fNPars; i++){
        fFitVals[i] = fMinimizer->X()[i];
      }// end for(fNPars)
    }// end if(fFitStatus)

  }

  //......................................................................

  void KnobFitter::PrintFitVals()
  {
    if(!fFitStatus){
      std::cout << "Haven't run succesful fit yet \n";
      std::cout << fCallNum << " calls \n";
      return;
    }
    for(int i=0; i<fNPars; i++){
      std::cout << fMinimizer->X()[i] << "\n";
    }

    std::cout << "chi2  : " << fChi2  << "\n";
    std::cout << "calls  : " << fCallNum  << "\n";
  }
  //......................................................................


}// end namespace

