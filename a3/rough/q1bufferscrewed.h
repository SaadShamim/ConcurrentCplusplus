#include <uC++.h>
#include <vector>
#include <iostream>
using namespace std;

template<typename T> class BoundedBuffer {

	bool useBarge;

	uOwnerLock mlk; //mutual exclusive lock
	uCondLock pClk; //producer lock (insert)
	uCondLock cClk; //consumer lock (remove)
	uCondLock bClk; //barge condition lock

	//buffer
	vector<int> buff;
	
	
	int *bufferArray;
	
	int startIndex;
	int entryIndex;
	int arraySize;
	int buffSize;

	public:

	BoundedBuffer( const unsigned int size = 10 ):bufferArray(new int[size]) {
		useBarge = false;
		buff.reserve(size);
		cerr << "bf: " << buff.size() << " bx: " << buff.capacity() << endl;
		buffSize = 10;
		
		startIndex =0;
		entryIndex = 0;
		arraySize =0;
	}

	void insert( T elem ){
#ifdef BUSY 

		mlk.acquire(); //get mutal lock

		//if buffer full, busy wait
		while(buff.size() == buff.max_size()) {
			pClk.wait(mlk);
		}

		assert(buff.size() != buff.max_size());
		buff.push_back(elem); //add element

		cClk.signal(); //signal consumer

		mlk.release(); //release mutual lock

#endif // BUSY   

#ifdef NOBUSY 
		mlk.acquire(); //acquire mutex

		//barge acts as buffer to prevent race conditions
		if(useBarge){
		cerr << "bargeinsert: a: " << arraySize << " b: " << buffSize << " v: " << buff.size() << "bempty: " << bClk.empty() << "cempty: " << cClk.empty() << "pempty: " << pClk.empty() << endl;
			bClk.wait(mlk); //maybe false right after?
		}

		//if nothing more can be added
		//if(arraySize == buffSize){
		if(buff.size() == buff.capacity()){ 
			if(!cClk.empty()){
				cClk.signal();
			}else if(!bClk.empty() && cClk.empty()){
				bClk.signal(); //signal barge
			}
			
			if(useBarge && pClk.empty() && cClk.empty()){
				useBarge = false; //if no one barge, set to false.
			}

cerr << "insert wait: a: " << arraySize << " b: " << buffSize << " v: " << buff.size() << "bempty: " << bClk.empty() << "cempty: " << cClk.empty() << "pempty: " << pClk.empty() << endl;
			pClk.wait(mlk);//sleep

			//reset barge
			if(pClk.empty() && cClk.empty() && useBarge)
				useBarge = false;
		}	


		assert(buff.size() != buff.capacity());
		buff.push_back(elem); //add elements to buffer 
		
	//	cerr << "a: " << arraySize << " b: " << buffSize << " v: " << buff.size() << "bempty: " << bClk.empty() << "cempty: " << cClk.empty() << "pempty: " << pClk.empty() endl; 
		assert(arraySize != buffSize)
		bufferArray[entryIndex] = elem;
		arraySize++; 
		entryIndex++;
		entryIndex = entryIndex%buffSize;
		
		
		
		
		
		//either signal: consumer, if waiting, or barge, if waiting; do nothing if no one waiting
		if(!cClk.empty()){
			useBarge = true;
			cClk.signal();
		}else{
			if(pClk.empty() && cClk.empty()){
				useBarge = false;
			}
				
			if(!bClk.empty()){
				bClk.signal();
			}else{
				pClk.signal();
			}
		}

		//release mutex
		mlk.release();

#endif // NOBUSY 
	}

	T remove(){
#ifdef BUSY 
		
		//acquire mutex
		mlk.acquire();

		//busy wait, if buffer is empty
		while(buff.size() == 0){
			cClk.wait(mlk);
		}

		assert(!buff.empty());
		T x = buff.at(0); //store return value
		buff.erase(buff.begin()); //remove elements

		pClk.signal();//signal producer

		mlk.release();//release mutex lock

		return x;

#endif // BUSY 

#ifdef NOBUSY 

		//acquire mutex lock
		mlk.acquire();
		
		//barge prevents race condition on mutex lock
		if(useBarge){
		cerr << "remove barge: a: " << arraySize << " b: " << buffSize << " v: " << buff.size() << "bempty: " << bClk.empty() << "cempty: " << cClk.empty() << "pempty: " << pClk.empty() << endl;
			bClk.wait(mlk);
		}

		//if buff empty, wait
		if(buff.size() == 0){
	//	if(arraySize == 0){
			if(!pClk.empty()){
				pClk.signal();
			} else if(!bClk.empty() && pClk.empty()){
				bClk.signal(); //release threads in barge
			}
			
			if(pClk.empty() && cClk.empty() && useBarge){
				useBarge = false; //reset barge variable	
			}
				
cerr << "remove wait a: " << arraySize << " b: " << buffSize << " v: " << buff.size() << "bempty: " << bClk.empty() << "cempty: " << cClk.empty() << "pempty: " << pClk.empty() << endl;
			cClk.wait(mlk); //wait

			if(pClk.empty() && cClk.empty() && useBarge)
				useBarge = false; //reset barge variable
		}

		assert(!buff.empty());
		T x = buff.at(0); //store for return
		buff.erase(buff.begin()); //remove buff element 
		
		assert(arraySize != 0);
		//T x = bufferArray[startIndex];
		arraySize--;
		if(startIndex < buffSize-1)
			startIndex++;
		else
			startIndex = 0; 
			
		

		//signal a producer if waiting, else barge if waiting
		if(!pClk.empty()){
			useBarge = true;
			pClk.signal();
		}else{
			if(pClk.empty() && cClk.empty())
				useBarge = false;
			//release any threads that may be left in barge
			if(!bClk.empty()){
				bClk.signal();
			}
		}

		mlk.release();

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
