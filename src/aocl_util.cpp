#include "aocl_util.h"

#include "blis.h"

#include "Timer.h"

// #define DBG

int AmdBLISExample(int a_rows, int a_cols_b_rows, int b_cols) {
	num_t dt;
	dim_t m, n, k;
	inc_t rs, cs;
	side_t side;

	obj_t a, b, c;
	obj_t* alpha;
	obj_t* beta;

	// Create some matrix operands to work with.
	dt = BLIS_DOUBLE;
	m = a_rows;
    n = a_cols_b_rows;
    k = b_cols; 
    rs = 0; cs = 0;
	bli_obj_create( dt, m, n, rs, cs, &c );
	bli_obj_create( dt, m, k, rs, cs, &a );
	bli_obj_create( dt, k, n, rs, cs, &b );

	// Set the scalars to use.
	alpha = &BLIS_ONE;
	beta  = &BLIS_ONE;

	// Initialize the matrix operands.
	bli_randm( &a );
	bli_randm( &b );
	bli_setm( &BLIS_ZERO, &c );

#ifdef DBG
	bli_printm( strdup("a: randomized"), &a, strdup("%4.1f"), strdup("") );
	bli_printm( strdup("b: set to 1.0"), &b, strdup("%4.1f"), strdup("") );
	bli_printm( strdup("c: initial value"), &c, strdup("%4.1f"), strdup("") );
#endif
	Timer t(__FUNCTION__);
	// c := beta * c + alpha * a * b, where 'a', 'b', and 'c' are general.
	bli_gemm( alpha, &a, &b, beta, &c );
	t.stop();
	t.PrintTime();
#ifdef DBG
	bli_printm( strdup("c: after gemm"), &c, strdup("%4.1f"), strdup("") );
#endif
	// Free the objects.
	bli_obj_free( &a );
	bli_obj_free( &b );
	bli_obj_free( &c );

    return 0;
}
