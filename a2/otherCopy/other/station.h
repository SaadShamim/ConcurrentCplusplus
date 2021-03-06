#include <uC++.h>
#include <vector>

_Coroutine Station {
    _Event Election {
      public:
        Station &candidate;
        Election( Station &candidate ) : candidate( candidate ) {}
    };
    static unsigned int value;                // global variable, increasing transmission value
    static std::vector<Station *> active;     // global variable, active stations
    Station *remove( Station *victim );       // delete failed object and return new partner
    void vote( Station &candidate );          // perform election voting

    int idx;
    int failurex;

     void main(); 
  
public:
    static Station *coordinator;              // global variable, shared among all instances
    struct Frame {};
    Station( unsigned int id, unsigned int failure ): idx(1),failurex(10) {}
    void start( Station *partner );           // supply partner
    void transmit( const Frame &f );          // pass frame
    unsigned int id();                        // station id
    Station *partner();                       // station partner
    bool failed();                            // station status
};
