/*=============================================================================

    This file is part of FLINT.

    FLINT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    FLINT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with FLINT; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA

=============================================================================*/
/******************************************************************************

    Copyright (C) 2013 Mike Hansen

******************************************************************************/


#ifdef T

#include "templates.h"

int
main(void)
{
    int i, j;
    flint_rand_t state;
    flint_randinit(state);
    flint_printf("iterated_frobenius_preinv....");
    fflush(stdout);

    for (j = 0; j < 20 * flint_test_multiplier(); j++)
    {
        int result;
        fmpz_t q;
        slong n;
        
        TEMPLATE(T, ctx_t) ctx;
        TEMPLATE(T, poly_t) v, vinv, *h1, *h2;

        TEMPLATE(T, ctx_randtest)(ctx, state);

        fmpz_init(q);
        TEMPLATE(T, ctx_order)(q, ctx);
        
        TEMPLATE(T, poly_init)(v, ctx);
        TEMPLATE(T, poly_init)(vinv, ctx);

        TEMPLATE(T, poly_randtest_monic)(v, state, n_randint(state, 20) + 1, ctx);

        TEMPLATE(T, poly_reverse)(vinv, v, v->length, ctx);
        TEMPLATE(T, poly_inv_series_newton)(vinv, vinv, v->length, ctx);

        n = n_randint(state, 5) + 2;
        if (!(h1 = flint_malloc((2 *n) * sizeof(TEMPLATE(T, poly_struct)))))
        {
            flint_printf("Exception (t-fq_poly_iterated_frobenius_preinv):\n");
            flint_printf("Not enough memory.\n");
            abort();
        }
        h2 = h1 + n;

        for (i = 0; i < 2 * n; i++)
            TEMPLATE(T, poly_init)(h1[i], ctx);

        TEMPLATE(T, poly_iterated_frobenius_preinv)(h1, n, v, vinv, ctx);

        TEMPLATE(T, poly_gen)(h2[0], ctx);
        for (i = 1; i < n; i++)
            TEMPLATE(T, poly_powmod_fmpz_sliding_preinv)(h2[i], h2[i - 1], q, 0, v, vinv, ctx);

        result = 1;
        for (i = 0; i < n; i++)
            result = result && TEMPLATE(T, poly_equal)(h1[i], h2[i], ctx);
        
        
        if (!result)
        {
            flint_printf("FAIL (composition):\n");
            flint_printf("v:\n"); TEMPLATE(T, poly_print)(v, ctx); flint_printf("\n");
            abort();
        }

        TEMPLATE(T, poly_clear)(v, ctx);
        TEMPLATE(T, poly_clear)(vinv, ctx);

        for (i = 0; i < 2*n; i++)
            TEMPLATE(T, poly_clear)(h1[i], ctx);

        flint_free(h1);
        TEMPLATE(T, ctx_clear)(ctx);
        fmpz_clear(q);
    }
    
    flint_randclear(state);
    flint_cleanup();
    flint_printf("PASS\n");
    return 0;
}



#endif