#ifndef _STATION_H_
#define _STATION_H_

#include <uC++.h>
#include <vector>
using namespace std;

//enum frameType {ACK,NACK,DF,TOKEN};

_Coroutine Station {
    _Event Election {
      public:
        Station &candidate;
        Election( Station &candidate ) : candidate( candidate ) {}
    };
    static unsigned int value;                // global variable, increasing transmission value
    static std::vector<Station *> active;     // global variable, active stations
    Station *remove( Station *victim );       // delete failed object and return new partner
    void vote();          // perform election voting


  
public:
    static Station *coordinator;              // global variable, shared among all instances
    struct Frame {
	enum frameType {ACK,NACK,DF,TOKEN};
	frameType type;	
	int value;
	int srcId;
	int dstId;
    };
    Station( unsigned int id, unsigned int failure ); 
    void start( Station *partner );           // supply partner
    void transmit( const Frame &f );          // pass frame
    unsigned int id();                        // station id
    Station *partner();                       // station partner
    bool failed();                            // station status

private:
    Frame::frameType fFrameType;
    Frame localFrame;
    bool check(int);
	
    void main(); 
    int idx; //curent stations id
    int failurex; //current stations Failure value
    bool iFailed; //stations failed status
    Station *myPartner; //stations partner
    int holdCounter; //keeps track of whether station can hold token
};

#endif
