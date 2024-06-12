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

  double r_mcp1 = 19.1;
  double r_mcp2 = 14.5;
  
  std::vector<double> desired_gaps = {100.,725.,200.,725.,200.};

  g4rwgt::KnobFitter fitter(r_mcp1, r_mcp2, desired_gaps); 

  fitter.DoFit();
  fitter.PrintFitVals();
}

int main(){
	RunFit();
	return 0;
}
//...................................................................
