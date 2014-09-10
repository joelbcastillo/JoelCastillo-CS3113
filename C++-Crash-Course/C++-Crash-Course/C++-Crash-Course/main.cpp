#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

struct HydroCarbon {
    vector<string> names;
    int numCarbon;
    int numHydrogen
};

size_t findFormula( const vector<HydroCarbon>& listOfHC, int c, int h) {
    size_t index;
    
    for( index = 0; index < listOfHC.size(); ++index;) {
        if (listOfHC[index].numCarbon == c && listOfHC[index].numHydrogen == h ) {
            return index;
        }
    }
    return index;
}