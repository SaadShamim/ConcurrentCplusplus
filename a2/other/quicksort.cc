using namespace std;
#include <iostream>

void swap(int &a, int &b){
	int tmp = a;
	a = b;
	b = tmp;
}

int partition(int sequence[], int low, int high,int pivot){
    int temp;

	//partition
	swap(sequence[pivot], sequence[high]);

	int entryIndex = low;

	for(int i=low; i<high-1; i++){
		if(sequence[i] < sequence[high]){
			swap(sequence[entryIndex], sequence[i]);
			entryIndex++;
		}
	}
	
	swap(sequence[entryIndex], sequence[high]);
	
	return entryIndex;	
}

void quickSort2(int sequence[], int low, int high){
	int newPivot;

	if(low < high){
		int pivot = low +(high-low)/2;
		newPivot = partition(sequence,low,high, pivot);
		quickSort2(sequence, low, newPivot-1);
		quickSort2(sequence, newPivot + 1, high);
	}
}


int main(){
	int a[] = {3,7,8,5,2,1,9,5,4};

	quickSort2(a, 0, 8);

	for(int i = 0; i < 9; i++) cout<<a[i] << " ";
	cout << endl;
}
