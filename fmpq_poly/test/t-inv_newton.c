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

    Copyright (C) 2009 William Hart
    Copyright (C) 2010 Sebastian Pancratz

******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <mpir.h>
#include "flint.h"
#include "fmpz.h"
#include "fmpq_poly.h"
#include "fmpq_poly.h"
#include "ulong_extras.h"

int
main(void)
{
    int i, result;
    fmpz_randstate_t state;

    printf("inv_newton....");
    fflush(stdout);

    fmpq_poly_randinit(state);

    /* Check aliasing of a and c */
    for (i = 0; i < 500; i++)
    {
        fmpq_poly_t a, b;
        long n = n_randint(50) + 1;

        fmpq_poly_init(a);
        fmpq_poly_init(b);
        fmpq_poly_randtest_not_zero(a, state, n_randint(50) + 1, 50);
        fmpz_randtest_not_zero(a->coeffs, state, 50);
        fmpq_poly_canonicalise(a);

        fmpq_poly_inv_newton(b, a, n);
        fmpq_poly_inv_newton(a, a, n);

        result = (fmpq_poly_equal(a, b));
        if (!result)
        {
            printf("FAIL:\n");
            fmpq_poly_print(a), printf("\n\n");
            fmpq_poly_print(b), printf("\n\n");
            abort();
        }

        fmpq_poly_clear(a);
        fmpq_poly_clear(b);
    }

    /* Check Q^{-1} * Q is congruent 1 mod t^n */
    for (i = 0; i < 1000; i++)
    {
        fmpq_poly_t a, b, c, one;
        long n = n_randint(80) + 1;

        fmpq_poly_init(a);
        fmpq_poly_init(b);
        fmpq_poly_init(c);
        fmpq_poly_init(one);

        fmpq_poly_randtest_not_zero(a, state, n_randint(80) + 1, 80);
        fmpz_randtest_not_zero(a->coeffs, state, 80);
        fmpq_poly_canonicalise(a);

        fmpq_poly_fit_length(one, 1);
        fmpz_set_ui(one->coeffs, 1);
        one->length = 1;

        fmpq_poly_inv_newton(b, a, n);
        fmpq_poly_mul(c, a, b);
        fmpq_poly_truncate(c, n);

        result = (fmpq_poly_equal(c, one));
        if (!result)
        {
            printf("FAIL:\n");
            printf("a = "), fmpq_poly_print(a), printf("\n\n");
            printf("b = "), fmpq_poly_print(b), printf("\n\n");
            printf("c = "), fmpq_poly_print(c), printf("\n\n");
            abort();
        }

        fmpq_poly_clear(a);
        fmpq_poly_clear(b);
        fmpq_poly_clear(c);
        fmpq_poly_clear(one);
    }

    fmpq_poly_randclear(state);
    _fmpz_cleanup();
    printf("PASS\n");
    return 0;
}
