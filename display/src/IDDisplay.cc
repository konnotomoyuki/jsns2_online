#include "IDDisplay.hh"

#include <TMath.h>

#include <fstream>

using namespace JSNS2;

const double IDDisplay::g_scale    = 12000;      // Overall scaling factor
const double IDDisplay::g_radius   = 1849.04;// / g_scale;     //Radius of the acrylic tank
const double IDDisplay::g_height   = 1470;// / g_scale;         //Half of height of the acrylic tank
const double IDDisplay::g_rPMT     = 101.0 * 1.2;// / g_scale;  //radius of the PMT

