#include "WeatherStation.h"

#include <ostream>

using namespace std;

void
ClearSky::AtmosphericConditions::ice_print(ostream& os) const
{
    os << "{ Temperature = " << temperature << "°C, Humidity = " << humidity << "% }";
}
