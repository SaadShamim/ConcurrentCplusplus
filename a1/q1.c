// MUST USE FILE-NAME SUFFIX ".c" AND COMPILE WITH gcc
#include <stdio.h>

int main() {
    unsigned int frameno = 1;                           // main is the first frame

    void h( int p, int call ) {                         // nest routine
        int hv = 10;

        void g( int p, int call ) {                     // nest routine
            int gv = 100;

            void f( int p ) {                           // nest routine
                frameno += 1;
                printf( "%d\tf(%d)\t%p\t%p\t__\t__\n", frameno, p, &hv, &gv );
                if ( p != 0 ) f( p - 1 );
            } // f

            frameno += 1;
            printf( "%d\tg(%d,%d)\t%p\t%p\t__\n", frameno, p, call, &hv, &gv );
            if ( p != 0 ) g( p - 1, call );             // non-base case, recurse
            if ( p == 0 && call == 0 ) h( 1, 1 );       // base of recursion, call h
            if ( p == 0 && call == 1 ) f( 1 );          // base of recursion, call f
        } // g

        frameno += 1;
        printf( "%d\th(%d,%d)\t%p\t\t\t\n", frameno, p, call, &hv );
        if ( p != 0 ) h( p - 1, call );                 // non-base case, recurse
        if ( p == 0 && call == 0 ) g( 1, 0 );           // base of recursion, call g
        if ( p == 0 && call == 1 ) g( 1, 1 );           // base of recursion, call g
    } // h

    printf( "frame#\tcall\t&hv\t\t&gv\t\tfram:hv\tfram:gv\n"
            "==============================================\n" );
    h( 2, 0 );
} // main
