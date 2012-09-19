template<typename T> _Task QuickSort {
	T *array;
	int low; //low value
	int high; //high value 
	unsigned int depth; //depth value

	//partioning function
	int partition(T sequence[], int low, int high,int pivot){
		//put pivot at end of array	
		swap(sequence[pivot], sequence[high]);
		
		int entryIndex = low;
		//sort values from low to high
		for(int i=low; i<high; i++){
			if(sequence[i] < sequence[high]){
				swap(sequence[entryIndex], sequence[i]);
				entryIndex++;
			}
		}
		//put pivot back in place
		swap(sequence[entryIndex], sequence[high]);
		
		return entryIndex;	
	}

	void runQuickSort(T sequence[], int low, int high, unsigned int depth){
		int newPivot;
		
		if(low < high){
			//calculate pivot point
			int pivot = low +(high-low)/2;
			//partition
			newPivot = partition(sequence,low,high, pivot);
			
			//sort through recursion or thread, depending on depth
			if(depth > 0){
				QuickSort q1(sequence, low, newPivot-1, depth-1);
				QuickSort q2(sequence, newPivot + 1, high, depth-1);
			}else{
				runQuickSort(sequence, low, newPivot-1, 0);
				runQuickSort(sequence, newPivot + 1, high, 0);
			}
		}
	}
	
	void main() {
		runQuickSort(array, low, high, depth);
	}
	
  public:
    //constructor
    QuickSort(T array[], int low, int high, unsigned int depth ): array(array), low(low), high(high), depth(depth){}
};

//swaps values at two locations
void swap(int &a, int &b){
	int tmp = a;
	a = b;
	b = tmp;
}

unsigned int uDefaultStackSize() {
return 512 * 1000;
// set task stack-size to 512K
}



