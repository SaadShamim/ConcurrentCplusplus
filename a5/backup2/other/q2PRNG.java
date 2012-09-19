import java.util.Random;

public class q2PRNG {
    private static Random random = new Random();
  
   public static void seed(int  s ) {				// reset seed
	random.setSeed(s);
    }
    
     public synchronized static int getRand(int u ) {			// [0,u]
	return random.nextInt(u + 1);			// call operator()()
    }
    
     public synchronized static int getRand(int l, int u ) {			// [0,u]
	return random.nextInt(u - l + 1) + l;
    }

}
