/*
    Copyright (C) 2017 Daniel Schultz

    This file is part of FLINT.

    FLINT is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include "flint.h"
#include "fmpz.h"
#include "fmpz_mpoly.h"
#include "ulong_extras.h"

int
main(void)
{
    slong i;
    FLINT_TEST_INIT(state);

    flint_printf("print_parse....");
    fflush(stdout);

    {
        ordering_t ord;
        slong nvars, len1, exp_bound1, coeff_bits;
        fmpz_mpoly_ctx_t ctx;
        fmpz_mpoly_t f, f1;
        char * str;
        const char * vars[] = {"x","y","z","w","u","v"};

        for (i = 0; i < flint_test_multiplier(); i++)
        {
            ord = mpoly_ordering_randtest(state);
            nvars = n_randint(state, 6) + 1;

            fmpz_mpoly_ctx_init(ctx, nvars, ord);
            fmpz_mpoly_init(f, ctx);
            fmpz_mpoly_init(f1, ctx);

            for (len1 = 3; len1 < 1000; len1 += len1/2)
            {
                exp_bound1 = 1000;
                coeff_bits = 10;
                fmpz_mpoly_randtest(f, state, len1, exp_bound1, coeff_bits, ctx);

                str = fmpz_mpoly_get_str_pretty(f, vars, ctx);
                fmpz_mpoly_set_str_pretty(f1, str, vars, ctx);
                flint_free(str);

                if (!fmpz_mpoly_equal(f, f1, ctx))
                {
                    flint_printf("FAIL\n");
                    flint_abort();
                }
            }

            fmpz_mpoly_clear(f, ctx);
            fmpz_mpoly_clear(f1, ctx);
        }
    }

    printf("PASS\n");
    FLINT_TEST_CLEANUP(state);

    return 0;
}

