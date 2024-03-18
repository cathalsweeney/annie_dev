// Script which uses KnobFitter class to determine G4Rwgt knob 
// value from fit to experimental data.
// We fit to carbon data as there is no scattering data
// for the NOvA soup (yet!)
//-----------------------------------------
// N.B. must pass "--lib G4RwgtFit" to cafe
//-----------------------------------------
// Cathal Sweeeney - csweeney@fnal.gov


#include <iostream>                                  
#include <string>                                    
#include <vector>                                    
#include <map>
                                                     
#include "Math/Factory.h" 
#include "Math/Functor.h" 
#include "Math/Minimizer.h"

#include "KnobFitter.h"
#include "TSystem.h"

//...................................................................
void RunFit() 
{

  double r_mcp1 = 7.22;
  double r_mcp2 = 2.32;
  
  std::vector<double> desired_gaps = {100.,950.,200.,950.,300.};

  g4rwgt::KnobFitter fitter(r_mcp1, r_mcp2, desired_gaps); 

  fitter.DoFit();
  fitter.PrintFitVals();
}

int main(){
	RunFit();
	return 0;
}
//...................................................................
