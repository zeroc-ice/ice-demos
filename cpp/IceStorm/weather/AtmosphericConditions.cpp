#include "WeatherStation.h"

#include <ostream>

using namespace std;

// Implementation of the ice_print method for the AtmosphericConditions class.
void
ClearSky::AtmosphericConditions::ice_print(ostream& os) const
{
    os << "{ Temperature = " << temperature << "°C, Humidity = " << humidity << "% }";
}
