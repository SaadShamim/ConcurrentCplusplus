#include <uC++.h>
#include <vector>
#include <iostream>
using namespace std;

template<typename T> class BoundedBuffer {

  int buffSize;
  bool useBarge;

  uOwnerLock mlk; //mutual exclusive lock
  uCondLock pClk;
  uCondLock cClk;
  uCondLock bClk; //barge condition lock

  //use t? gave error?
  vector<int> buff;
  
  int *bufferArray;
  int startIndex;
  int arraySize;


  public:

    BoundedBuffer( const unsigned int size = 10 ): bufferArray(new int[size]) {
	buffSize = size;
	useBarge = false;
	startIndex =0;
	arraySize =0;
	//entryIndex=0;
    }
    
    ~BoundedBuffer() {
    	delete[] bufferArray;
    }	

    void insert( T elem ){
    	#ifdef BUSY 

    	mlk.acquire();
    	
    	while(buff.size() == (int)buffSize){
    		pClk.wait(mlk);
    	}
    	
	
	//cerr << " prod active " << endl;
	
	assert(buff.size() != buffSize);
    	buff.push_back(elem);
    	
    	//cerr << "prod added element, size is now: " << buff.size() << endl;

    	cClk.signal();

	//cerr << " consumer signaled " << endl;

    	mlk.release();
    	
    	#endif // BUSY   
    		
    	#ifdef NOBUSY 
    	

    	mlk.acquire();
    	
    	//cerr << "useBarge " << useBarge << endl;
    	if(useBarge)
    		bClk.wait(mlk);

//	if(bClk.empty() && useBarge)
  //  		useBarge = false;
    		
    	if(buff.size() == (int)buffSize){ 
    //	if(arraySize == buffSize){
    	    	if(!bClk.empty())
    			bClk.signal();
		else if( bClk.empty() && useBarge)
			useBarge = false;	
    		
    		pClk.wait(mlk);
		if(bClk.empty() && useBarge)
    			useBarge = false;

	}	
	
	
	
	//cerr << " prod active " << endl;

	assert(buff.size() != buffSize);
    	buff.push_back(elem);

// /   	assert(arraySize != buffSize)
// /   	bufferArray[((startIndex+arraySize)%buffSize)] = elem;
// /   	arraySize++;
//    	if(entryIndex < buffSize-1)
//    		entryIndex++;
 //   	else
 //   		entryIndex=0;
    	
    	//cerr << "prod added element, size is now: " << buff.size() << endl;

	if(!cClk.empty()){
		useBarge = true;
	    	cClk.signal();
    	}else{
		if(!bClk.empty()){
			bClk.signal();
		}
	
	}

	//cerr << " consumer signaled " << endl;

    	mlk.release();
    	
    	#endif // NOBUSY 

    	
    	
    }

    T remove(){
	#ifdef BUSY 

	mlk.acquire();

	while(buff.size() == 0){
		cClk.wait(mlk);
	}

	assert(!buff.empty());
	T x = buff.at(0);
	//cerr << "cons poped of value: " << x << endl;
	buff.erase(buff.begin());

	pClk.signal();

	mlk.release();
	
	return x;
	
	#endif // BUSY 
	
	#ifdef NOBUSY 
	
	//cerr << "acquiring lock" << endl;
	mlk.acquire();
	//cerr << "acquired lock" << endl;
	
	if(useBarge)
    		bClk.wait(mlk);

	//cerr << "after barge" << endl;

//	if(bClk.empty() && useBarge)
  //  		useBarge = false;
	
	if(buff.size() == 0){
	//if(arraySize == 0){
		//cerr << "buffersize is 0" << endl;
	    	if(!bClk.empty())
    			bClk.signal();
		else if( bClk.empty() && useBarge)
			useBarge = false;	
		
		cClk.wait(mlk);
		if(bClk.empty() && useBarge)
    			useBarge = false;
	}


	assert(!buff.empty());
	T x = buff.at(0);
	//cerr << "cons poped of value: " << x << endl;
	buff.erase(buff.begin());
	
//	assert(arraySize != 0);
//	T x = bufferArray[startIndex];
//	arraySize--;
	if(startIndex < buffSize-1)
		startIndex++;
	else
		startIndex = 0;

	if(!pClk.empty()){
		useBarge = true;
		pClk.signal();
	}else{
		if(!bClk.empty()){
			bClk.signal();
		}
	}

	mlk.release();
	
	//cerr << "********* use barge set at: " << useBarge << " barge empty?: " << bClk.empty() << endl;
	return x;
	
	#endif // NOBUSY 
    }
};

_Task Producer {
    BoundedBuffer<int> *buffer;
    const int Produce;
    const int Delay;
    
    void main();
  public:
    Producer( BoundedBuffer<int> &buffer, const int Produce, const int Delay ):buffer(&buffer), Produce(Produce),Delay(Delay) {}
};

_Task Consumer {
    BoundedBuffer<int> *buffer;
    const int Delay;
    int Sentinel;
    int *sum;
    
    void main();
  public:
    Consumer( BoundedBuffer<int> &buffer, const int Delay, const int Sentinel, int &sum ):buffer(&buffer),Delay(Delay),Sentinel(Sentinel),sum(&sum){}
};
