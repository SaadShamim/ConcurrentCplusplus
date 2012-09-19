class q2Driver
{	
	public static void main (String [] args) throws InterruptedException {
	
	//int seedVal = getpid();
	int P = 5; 
	int N = 30; 
	
	System.out.println(seedVal);
	myPrng.seed(seedVal);
	
	Philosophers[] allPhilosophers = new Philosopher[P];
	Printer p1 = new Printer(P);
	Table t1 = new Table(P, p1);

	for(int i=0; i<P; i++){
		allPhilosophers[i] = new Philosopher(i,N,t1,p1);
		allPhilosophers[i].start();
	}
	
	for(int i=0; i<P; i++){
		allPhilosophers[i].join();
	}   
   }
  
}
