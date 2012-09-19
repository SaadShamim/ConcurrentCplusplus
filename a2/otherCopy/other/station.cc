#include "station.h"

void Station::main() {
        try {
            _Enable {                         // allow delivery of nonlocal exceptions
                suspend();                    // establish starter for termination
            } // _Enable
        } _CatchResume( Election &election ) {
        } // try
}
