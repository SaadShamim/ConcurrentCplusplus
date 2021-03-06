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

	//use t? gave error?
	vector<int> buff;
	int maxSize;

	public:

	BoundedBuffer( const unsigned int size = 10 ) {
		useBarge = false;
		maxSize = size;
		//buff.reserve(size);
	}

	void insert( T elem ){
#ifdef BUSY 

		mlk.acquire();

		while(buff.size() == maxSize) {
			pClk.wait(mlk);
		}

		assert(buff.size() != maxSize);
		buff.push_back(elem);

		//cerr << "prod added element, size is now: " << buff.size() << endl;

		cClk.signal();

		//cerr << " consumer signaled " << endl;

		mlk.release();

#endif // BUSY   

#ifdef NOBUSY 
		mlk.acquire();

		cerr << "acq" << endl;

		cerr << "insert: useBarge: " << useBarge << endl;

		if(useBarge){
			bClk.wait(mlk);
			useBarge = false;
		}

		cerr << "after barge" << endl;
		cerr << buff.size() << endl;
		if(buff.size() == maxSize){ 
			if(!bClk.empty()){
				useBarge = true;
				bClk.signal();
			}
		//	else if( bClk.empty() && useBarge)
		//		useBarge = false;	

			pClk.wait(mlk);
			useBarge = false;
	//		if(bClk.empty() && useBarge)
	//			useBarge = false;
		}	

		cerr << "after buff" << endl;

		assert(buff.size() != maxSize);
		buff.push_back(elem);
		cerr << "added val" << endl;
		if(!cClk.empty()){
			useBarge = true;
			cClk.signal();
			cerr << "signal consumer" << endl;
		}else{
			if(!bClk.empty()){
				useBarge = true;
				bClk.signal();
			}
		}

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
		cerr << "remove: useBarge: " << useBarge << endl;
		if(useBarge){
			bClk.wait(mlk);
			useBarge = false;
		}

		//cerr << "after barge" << endl;

		//	if(bClk.empty() && useBarge)
		//  		useBarge = false;

		if(buff.size() == 0){
			//if(arraySize == 0){
			//cerr << "buffersize is 0" << endl;
			if(!bClk.empty()){
				useBarge = true;
				bClk.signal();
			}
		//	else if( bClk.empty() && useBarge)
		//		useBarge = false;	

			cClk.wait(mlk);

			//if(bClk.empty() && useBarge)
				useBarge = false;
		}

		assert(!buff.empty());
		T x = buff.front();
		buff.erase(buff.begin());

		//	assert(arraySize != 0);
		//	T x = bufferArray[startIndex];
		//	arraySize--;
		/*	if(startIndex < buffSize-1)
			startIndex++;
			else
			startIndex = 0;*/

		if(!pClk.empty()){
			useBarge = true;
			pClk.signal();
		}else{
			if(!bClk.empty()){
				useBarge = true;
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
