#include <uC++.h>
#include <iostream>
using namespace std;

_Coroutine Cons;

_Coroutine Prod {
	Cons *c;
	int N, money, receipt;
	void main(){
		int i, p1,p2, status;
		for(i = 0; i<= N; i+=1){
			p1 = rand() % 100;
			p2 = rand() % 100;
			cout << "delivers:" << p1 << ", " << p2 << endl;
			status = c->delivery(p1,p2);
			cout << " gets status:" << status << endl;
			receipt += 1;
		}
		cout << "Prod stops" << endl;
		c->stop();
	}

	public:
		int payment(int money){
			Prod::money = money;
			cout << " gets payment $" << money << endl;
			resume();
			return receipt;
		}
		int start(int N, Cons *c){
			Prod::N = N; Prod::c = &c;
			receipt  = 0;
			resume();
		}
};

_Coroutine Cons {
	Prod &p;
	int p1, p2, status;
	bool done;

	void main(){
		int money = 1, receipts;
		status = 0;
		for( ;; ){
			if(done) break;
			cout << "receives: " << p1 << ", " << p2 << " and pays $" << money << endl;
			status += 1;
			receipt = p.payment(money);
			cout << "gets receipt #" << receipt << endl;
			money += 1;
		}
		cout << "Cons stops" << endl;
	}:

	public: 
	Cons( Prod &p) : p(p),done(0){}
	int deliver y( int p1, int p2 ) {
		Cons::p1 = p1; Cons::p2 = p2;
		resume(); // activate Cons::main 1st time
		return status; // afterwards Prod::payment 
	}
	void stop() {
		done = true;
		resume();
	}
};	
	

void uMain ::main(){
	Prod prod;
	Cons cons( prod );
	prod.start(5, cons);
}
