#include <uC++.h>
#include <vector>
#include <iostream>
using namespace std;

template<typename T> class BoundedBuffer {

	bool useBarge;

	uOwnerLock mlk; //mutual exclusive lock
	uCondLock pClk;
	uCondLock cClk;
	uCondLock bClk; //barge condition lock

	vector<int> buff;
	int maxSize;
	
	//buffer variables
	int *bufferArray;
	int startIndex;
	int arraySize;

	public:
	
	//set initial values
	BoundedBuffer( const unsigned int size = 10 ):bufferArray(new int[size]) {
		useBarge = false;
		maxSize = size;
		
		startIndex =0;
		arraySize =0;
	}
	
	//free mem
	~BoundedBuffer(){
		delete[] bufferArray;
	}

	void insert( T elem ){
#ifdef BUSY 
		//get mutex lock
		mlk.acquire();
		//busy loop for mutex
		while(arraySize == maxSize) {
			pClk.wait(mlk);
		}
		
		//add elem to array
		assert(arraySize != maxSize)
		bufferArray[(startIndex+arraySize)%maxSize] = elem;
		arraySize++; 

		//call consumer
		cClk.signal();

		mlk.release();

#endif // BUSY   

#ifdef NOBUSY 
		mlk.acquire();	

		//buffer to prevent barging
		if(useBarge){
			bClk.wait(mlk);
			useBarge = false;
		}

		//buffer full
		if(arraySize == maxSize){
			if(!bClk.empty()){
				useBarge = true; //prevent race cond.
				bClk.signal();
			}


			pClk.wait(mlk);
			useBarge = false; //doesn't matter, will be set to true if cond. lock is signaled

		}	

		//add elem into buffer
		assert(arraySize != maxSize)
		bufferArray[(startIndex+arraySize)%maxSize] = elem;
		arraySize++; 
			
		if(!cClk.empty()){
			useBarge = true;
			cClk.signal(); //call consumer
		}else{
			if(!bClk.empty()){
				useBarge = true;
				bClk.signal(); //call barge
			}
		}

		mlk.release();

#endif // NOBUSY 
	}

	T remove(){
#ifdef BUSY 

		mlk.acquire();

		//busy wait for mutex
		while(arraySize == 0){
			cClk.wait(mlk);
		}
			//remove elements from array
			assert(arraySize != 0);
			T x = bufferArray[startIndex];
			arraySize--;
			if(startIndex < maxSize-1)
				startIndex++;
			else
				startIndex = 0;

		//call producer
		pClk.signal();

		mlk.release();

		return x;

#endif // BUSY 

#ifdef NOBUSY 


		mlk.acquire();
		
		//buffer to prevent race condition (barging)
		if(useBarge){
			bClk.wait(mlk);
			useBarge = false;
		}

			//array is empty
			if(arraySize == 0){

				if(!bClk.empty()){
					useBarge = true;
					bClk.signal();
				}

				//sleep
				cClk.wait(mlk);

				useBarge = false;
			}

			//remove lements from array
			assert(arraySize != 0);
			T x = bufferArray[startIndex];
			arraySize--;
			if(startIndex < maxSize-1)
				startIndex++;
			else
				startIndex = 0;
			

		if(!pClk.empty()){
			useBarge = true; //prevent race condition
			pClk.signal(); // call producer
		}else{
			if(!bClk.empty()){
				useBarge = true;
				bClk.signal(); //call barge
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
