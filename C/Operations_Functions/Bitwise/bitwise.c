/* Demonstrate bit-wise operations usage for flags handling */
/* Set/Reset bit, Add/Combine, Reverse, Swap, Check */
#include <stdlib.h>
#include <stdio.h>

void AccountStatus( int Status, char *Name );

/* one bit flags */
#define ACTIVATED      0x8000
#define VIP            0x4000
#define PAST_DUE       0x2000
#define OVER_LIMIT     0x1000
#define STOLEN         0x0800
#define LOST           0x0400
#define PREFERRED_RATE 0x0200

int main (void)
{
    /* combine basic flags to initial values - OR */
	int Card1 = VIP | ACTIVATED | PREFERRED_RATE; 
	int Card2 = OVER_LIMIT | PAST_DUE | PREFERRED_RATE;
	char *Name1 = "Mark McKenzie";
	char *Name2 = "Marian Purue";

	printf( "\nBefore update\n" );
	AccountStatus( Card1, Name1 );
	Card1 &= ~(VIP | PREFERRED_RATE); /* clear VIP and pref rate bits */
	Card1 |= OVER_LIMIT;    /* set overlimit bit */
	Card1 ^= ACTIVATED;     /* reverse activated bit, whatever it was */
	printf( "\nAfter update\n" );
	AccountStatus( Card1, Name1 );

	printf( "\nBefore update\n" );
	AccountStatus( Card2, Name2 );
	Card2 &= ~PAST_DUE;   /* clear past due bit */
	Card2 |= LOST;        /* set lost bit */
	printf( "\nAfter update\n" );
	AccountStatus( Card2, Name2 );

	printf( "\nBefore swap\n" );
	AccountStatus( Card2, Name2 );
	AccountStatus( Card1, Name1 );
	Card1 ^= Card2;   // 3 xor's will exchange 2 values
	Card2 ^= Card1;
	Card1 ^= Card2;
	printf( "\nAfter swap\n" );
	AccountStatus( Card2, Name2 );
	AccountStatus( Card1, Name1 );

	//  or masks together to test for any of the set of conditions
	if ( Card1 & ( OVER_LIMIT | PAST_DUE ) )
        printf( "\n%s is a deadbeat\n", Name1 );
    
	return( 0 );
}

void AccountStatus( int Status, char *Name )
{
   printf( "Account status for %s\n%s%s%s%s%s%s%s\n", Name,
      ( Status & ACTIVATED )      ? "Activated\n"      : "Not Activated\n",
      ( Status & VIP )            ? "VIP\n"            : "",
      ( Status & PAST_DUE )       ? "Past Due\n"       : "",
      ( Status & OVER_LIMIT )     ? "Overlimit\n"      : "",
      ( Status & STOLEN )         ? "***Stolen***\n"   : "",
      ( Status & LOST )           ? "***Lost***\n"     : "",
      ( Status & PREFERRED_RATE ) ? "Preferred rate\n" : "Standard rate\n" );
}
