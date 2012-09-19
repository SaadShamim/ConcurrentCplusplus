class q2Driver
{	
	public static void main (String [] args) throws InterruptedException {
	
	int seedVal = 3000;
	int P = 5; 
	int N = 30; 
	
	System.out.println(seedVal);
	q2PRNG.seed(seedVal);
	
	q2Philosopher[] allPhilosophers = new q2Philosopher[P];
	q2Printer p1 = new q2Printer(P);
	q2table t1 = new q2table(P, p1);

	for(int i=0; i<P; i++){
		allPhilosophers[i] = new q2Philosopher(i,N,t1,p1);
		allPhilosophers[i].start();
	}
	
	for(int i=0; i<P; i++){
		allPhilosophers[i].join();
	}   
	
	p1.finishPrinter();
   }
  
}
