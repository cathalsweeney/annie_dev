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

#include "G4Rwgt/make_inputs/fitting/KnobFitter.h"
#include "TSystem.h"

//...................................................................
void RunFit() 
{

  double r_mcp1 = 39.9;
  double r_mcp2 = 8.71;
  
  std::vector<double> desired_gaps = {40., 950., 100., 950., 180.};

  g4rwgt::KnobFitter fitter(r_mcp1, r_mcp2, desired_gaps); 

  fitter.DoFit();
  fitter.PrintFitVals();
}


//...................................................................
