import java.util.Random;
class q2Driver
{

	//throw usage error
	private static void usageError(){
		System.err.println("Usage: java q2Driver [ philosophers (> 1) [ noodles (> 0) [ Seed (> 0) ] ] ]");
		System.exit(1);
        }
	
	public static void main (String [] args) throws InterruptedException {
	
	Random rand = new Random();
	int seedVal = rand.nextInt(3000);
	int P = 5; 
	int N = 30; 
	
	//handle inputs
	switch (args.length) {
		case 3:
		    seedVal = Integer.parseInt(args[2]);
		    if (seedVal <= 0)
		        usageError();
		    // fall through
		case 2:
		    N = Integer.parseInt(args[1]);
		    if (N <= 0)
		        usageError();
		    // fall through
		case 1:
		    P = Integer.parseInt(args[0]);
		    if (P <= 1)
		        usageError();
		    // fall through
		case 0:
		    // use defaults
		    break;
		default:
		    usageError();
        }
	
	//seet seed
	System.out.println(seedVal);
	q2PRNG.seed(seedVal);
	
	//create printer, and tables
	q2Philosopher[] allPhilosophers = new q2Philosopher[P];
	q2Printer p1 = new q2Printer(P);
	q2table t1 = new q2table(P, p1);

	//create philosophers
	for(int i=0; i<P; i++){
		allPhilosophers[i] = new q2Philosopher(i,N,t1,p1);
		allPhilosophers[i].start();
	}
	
	//wait for all philosopher to finish
	for(int i=0; i<P; i++){
		allPhilosophers[i].join();
	}   
	
	p1.finishPrinter();
   }
  
}
