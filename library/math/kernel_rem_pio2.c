/*
 * $Id: math_kernel_rem_pio2.c,v 1.7 2006-11-13 09:51:53 clib2devs Exp $
*/

#ifndef _MATH_HEADERS_H
#include "math_headers.h"
#endif /* _MATH_HEADERS_H */

static const int init_jk[] = {2, 3, 4, 6}; /* initial value for jk */
static const double PIo2[] = {
	1.57079625129699707031e+00, /* 0x3FF921FB, 0x40000000 */
	7.54978941586159635335e-08, /* 0x3E74442D, 0x00000000 */
	5.39030252995776476554e-15, /* 0x3CF84698, 0x80000000 */
	3.28200341580791294123e-22, /* 0x3B78CC51, 0x60000000 */
	1.27065575308067607349e-29, /* 0x39F01B83, 0x80000000 */
	1.22933308981111328932e-36, /* 0x387A2520, 0x40000000 */
	2.73370053816464559624e-44, /* 0x36E38222, 0x80000000 */
	2.16741683877804819444e-51, /* 0x3569F31D, 0x00000000 */
};

static const double
	zero = 0.00000000000000000000e+00,	 /* 0x00000000, 0x00000000 */
	half = 5.00000000000000000000e-01,	 /* 0x3FE00000, 0x00000000 */
	one = 1.00000000000000000000e+00,	 /* 0x3FF00000, 0x00000000 */
	two24 = 1.67772160000000000000e+07,	 /* 0x41700000, 0x00000000 */
	twon24 = 5.96046447753906250000e-08; /* 0x3E700000, 0x00000000 */

INLINE STATIC int kernel_rem_pio2(double *x, double *y, int e0, int nx, int prec, const int *ipio2)
{
	int jz, jx, jv, jp, jk, carry, n, iq[20], i, j, k, m, q0, ih;
	double z, fw, f[20], fq[20], q[20];

	/* initialize jk*/
	jk = init_jk[prec];
	jp = jk;

	/* determine jx,jv,q0, note that 3>q0 */
	jx = nx - 1;
	jv = (e0 - 3) / 24;
	if (jv < 0)
		jv = 0;
	q0 = e0 - 24 * (jv + 1);

	/* set up f[0] to f[jx+jk] where f[jx+jk] = ipio2[jv+jk] */
	j = jv - jx;
	m = jx + jk;
	for (i = 0; i <= m; i++, j++)
		f[i] = (j < 0) ? zero : (double)ipio2[j];

	/* compute q[0],q[1],...q[jk] */
	for (i = 0; i <= jk; i++)
	{
		for (j = 0, fw = 0.0; j <= jx; j++)
			fw += x[j] * f[jx + i - j];
		q[i] = fw;
	}

	jz = jk;
recompute:
	/* distill q[] into iq[] reversingly */
	for (i = 0, j = jz, z = q[jz]; j > 0; i++, j--)
	{
		fw = (double)((int)(twon24 * z));
		iq[i] = (int)(z - two24 * fw);
		z = q[j - 1] + fw;
	}

	/* compute n */
	z = scalbn(z, (int)q0);		 /* actual value of z */
	z -= 8.0 * floor(z * 0.125); /* trim off integer >= 8 */
	n = (int)z;
	z -= (double)n;
	ih = 0;
	if (q0 > 0)
	{ /* need iq[jz-1] to determine n */
		i = (iq[jz - 1] >> (24 - q0));
		n += i;
		iq[jz - 1] -= i << (24 - q0);
		ih = iq[jz - 1] >> (23 - q0);
	}
	else if (q0 == 0)
		ih = iq[jz - 1] >> 23;
	else if (z >= 0.5)
		ih = 2;

	if (ih > 0)
	{ /* q > 0.5 */
		n += 1;
		carry = 0;
		for (i = 0; i < jz; i++)
		{ /* compute 1-q */
			j = iq[i];
			if (carry == 0)
			{
				if (j != 0)
				{
					carry = 1;
					iq[i] = 0x1000000 - j;
				}
			}
			else
				iq[i] = 0xffffff - j;
		}
		if (q0 > 0)
		{ /* rare case: chance is 1 in 12 */
			switch (q0)
			{
			case 1:
				iq[jz - 1] &= 0x7fffff;
				break;
			case 2:
				iq[jz - 1] &= 0x3fffff;
				break;
			}
		}
		if (ih == 2)
		{
			z = one - z;
			if (carry != 0)
				z -= scalbn(one, (int)q0);
		}
	}

	/* check if recomputation is needed */
	if (z == zero)
	{
		j = 0;
		for (i = jz - 1; i >= jk; i--)
			j |= iq[i];
		if (j == 0)
		{ /* need recomputation */
			for (k = 1; iq[jk - k] == 0; k++)
				; /* k = no. of terms needed */

			for (i = jz + 1; i <= jz + k; i++)
			{ /* add q[jz+1] to q[jz+k] */
				f[jx + i] = (double)ipio2[jv + i];
				for (j = 0, fw = 0.0; j <= jx; j++)
					fw += x[j] * f[jx + i - j];
				q[i] = fw;
			}
			jz += k;
			goto recompute;
		}
	}

	/* chop off zero terms */
	if (z == 0.0)
	{
		jz -= 1;
		q0 -= 24;
		while (iq[jz] == 0)
		{
			jz--;
			q0 -= 24;
		}
	}
	else
	{ /* break z into 24-bit if necessary */
		z = scalbn(z, -(int)q0);
		if (z >= two24)
		{
			fw = (double)((int)(twon24 * z));
			iq[jz] = (int)(z - two24 * fw);
			jz += 1;
			q0 += 24;
			iq[jz] = (int)fw;
		}
		else
			iq[jz] = (int)z;
	}

	/* convert integer "bit" chunk to floating-point value */
	fw = scalbn(one, (int)q0);
	for (i = jz; i >= 0; i--)
	{
		q[i] = fw * (double)iq[i];
		fw *= twon24;
	}

	/* compute PIo2[0,...,jp]*q[jz,...,0] */
	for (i = jz; i >= 0; i--)
	{
		for (fw = 0.0, k = 0; k <= jp && k <= jz - i; k++)
			fw += PIo2[k] * q[i + k];
		fq[jz - i] = fw;
	}

	/* compress fq[] into y[] */
	switch (prec)
	{
	case 0:
		fw = 0.0;
		for (i = jz; i >= 0; i--)
			fw += fq[i];
		y[0] = (ih == 0) ? fw : -fw;
		break;
	case 1:
	case 2:
		fw = 0.0;
		for (i = jz; i >= 0; i--)
			fw += fq[i];
		y[0] = (ih == 0) ? fw : -fw;
		fw = fq[0] - fw;
		for (i = 1; i <= jz; i++)
			fw += fq[i];
		y[1] = (ih == 0) ? fw : -fw;
		break;
	case 3: /* painful */
		for (i = jz; i > 0; i--)
		{
			fw = fq[i - 1] + fq[i];
			fq[i] += fq[i - 1] - fw;
			fq[i - 1] = fw;
		}
		for (i = jz; i > 1; i--)
		{
			fw = fq[i - 1] + fq[i];
			fq[i] += fq[i - 1] - fw;
			fq[i - 1] = fw;
		}
		for (fw = 0.0, i = jz; i >= 2; i--)
			fw += fq[i];
		if (ih == 0)
		{
			y[0] = fq[0];
			y[1] = fq[1];
			y[2] = fw;
		}
		else
		{
			y[0] = -fq[0];
			y[1] = -fq[1];
			y[2] = -fw;
		}
	}
	return n & 7;
}

static const int two_over_pi[] = {
	0xA2F983,
	0x6E4E44,
	0x1529FC,
	0x2757D1,
	0xF534DD,
	0xC0DB62,
	0x95993C,
	0x439041,
	0xFE5163,
	0xABDEBB,
	0xC561B7,
	0x246E3A,
	0x424DD2,
	0xE00649,
	0x2EEA09,
	0xD1921C,
	0xFE1DEB,
	0x1CB129,
	0xA73EE8,
	0x8235F5,
	0x2EBB44,
	0x84E99C,
	0x7026B4,
	0x5F7E41,
	0x3991D6,
	0x398353,
	0x39F49C,
	0x845F8B,
	0xBDF928,
	0x3B1FF8,
	0x97FFDE,
	0x05980F,
	0xEF2F11,
	0x8B5A0A,
	0x6D1F6D,
	0x367ECF,
	0x27CB09,
	0xB74F46,
	0x3F669E,
	0x5FEA2D,
	0x7527BA,
	0xC7EBE5,
	0xF17B3D,
	0x0739F7,
	0x8A5292,
	0xEA6BFB,
	0x5FB11F,
	0x8D5D08,
	0x560330,
	0x46FC7B,
	0x6BABF0,
	0xCFBC20,
	0x9AF436,
	0x1DA9E3,
	0x91615E,
	0xE61B08,
	0x659985,
	0x5F14A0,
	0x68408D,
	0xFFD880,
	0x4D7327,
	0x310606,
	0x1556CA,
	0x73A8C9,
	0x60E27B,
	0xC08C6B,
};

static const int npio2_hw[] = {
	0x3FF921FB,
	0x400921FB,
	0x4012D97C,
	0x401921FB,
	0x401F6A7A,
	0x4022D97C,
	0x4025FDBB,
	0x402921FB,
	0x402C463A,
	0x402F6A7A,
	0x4031475C,
	0x4032D97C,
	0x40346B9C,
	0x4035FDBB,
	0x40378FDB,
	0x403921FB,
	0x403AB41B,
	0x403C463A,
	0x403DD85A,
	0x403F6A7A,
	0x40407E4C,
	0x4041475C,
	0x4042106C,
	0x4042D97C,
	0x4043A28C,
	0x40446B9C,
	0x404534AC,
	0x4045FDBB,
	0x4046C6CB,
	0x40478FDB,
	0x404858EB,
	0x404921FB,
};

static const double
	invpio2 = 6.36619772367581382433e-01, /* 0x3FE45F30, 0x6DC9C883 */
	pio2_1 = 1.57079632673412561417e+00,  /* 0x3FF921FB, 0x54400000 */
	pio2_1t = 6.07710050650619224932e-11, /* 0x3DD0B461, 0x1A626331 */
	pio2_2 = 6.07710050630396597660e-11,  /* 0x3DD0B461, 0x1A600000 */
	pio2_2t = 2.02226624879595063154e-21, /* 0x3BA3198A, 0x2E037073 */
	pio2_3 = 2.02226624871116645580e-21,  /* 0x3BA3198A, 0x2E000000 */
	pio2_3t = 8.47842766036889956997e-32; /* 0x397B839A, 0x252049C1 */

int __rem_pio2(double x, double *y)
{
	double z, w, t, r, fn;
	double tx[3];
	int i, j, n, ix, hx;
	int e0, nx;
	unsigned int low;

	GET_HIGH_WORD(hx, x); /* high word of x */
	ix = hx & 0x7fffffff;
	if (ix <= 0x3fe921fb) /* |x| ~<= pi/4 , no need for reduction */
	{
		y[0] = x;
		y[1] = 0;
		return 0;
	}
	if (ix < 0x4002d97c)
	{ /* |x| < 3pi/4, special case with n=+-1 */
		if (hx > 0)
		{
			z = x - pio2_1;
			if (ix != 0x3ff921fb)
			{ /* 33+53 bit pi is good enough */
				y[0] = z - pio2_1t;
				y[1] = (z - y[0]) - pio2_1t;
			}
			else
			{ /* near pi/2, use 33+33+53 bit pi */
				z -= pio2_2;
				y[0] = z - pio2_2t;
				y[1] = (z - y[0]) - pio2_2t;
			}
			return 1;
		}
		else
		{ /* negative x */
			z = x + pio2_1;
			if (ix != 0x3ff921fb)
			{ /* 33+53 bit pi is good enough */
				y[0] = z + pio2_1t;
				y[1] = (z - y[0]) + pio2_1t;
			}
			else
			{ /* near pi/2, use 33+33+53 bit pi */
				z += pio2_2;
				y[0] = z + pio2_2t;
				y[1] = (z - y[0]) + pio2_2t;
			}
			return -1;
		}
	}
	if (ix <= 0x413921fb)
	{ /* |x| ~<= 2^19*(pi/2), medium size */
		t = fabs(x);
		n = (int)(t * invpio2 + half);
		fn = (double)n;
		r = t - fn * pio2_1;
		w = fn * pio2_1t; /* 1st round good to 85 bit */
		if (n < 32 && ix != npio2_hw[n - 1])
		{
			y[0] = r - w; /* quick check no cancellation */
		}
		else
		{
			unsigned int high;
			j = ix >> 20;
			y[0] = r - w;
			GET_HIGH_WORD(high, y[0]);
			i = j - ((high >> 20) & 0x7ff);
			if (i > 16)
			{ /* 2nd iteration needed, good to 118 */
				t = r;
				w = fn * pio2_2;
				r = t - w;
				w = fn * pio2_2t - ((t - r) - w);
				y[0] = r - w;
				GET_HIGH_WORD(high, y[0]);
				i = j - ((high >> 20) & 0x7ff);
				if (i > 49)
				{		   /* 3rd iteration need, 151 bits acc */
					t = r; /* will cover all possible cases */
					w = fn * pio2_3;
					r = t - w;
					w = fn * pio2_3t - ((t - r) - w);
					y[0] = r - w;
				}
			}
		}
		y[1] = (r - y[0]) - w;
		if (hx < 0)
		{
			y[0] = -y[0];
			y[1] = -y[1];
			return -n;
		}
		else
			return n;
	}
	/* 
     * all other (large) arguments
     */
	if (ix >= 0x7ff00000)
	{ /* x is inf or NaN */
		y[0] = y[1] = x - x;
		return 0;
	}
	/* keep the compiler happy */
	z = 0;
	/* set z = scalbn(|x|,ilogb(x)-23) */
	GET_LOW_WORD(low, x);
	SET_LOW_WORD(z, low);
	e0 = (int)((ix >> 20) - 1046); /* e0 = ilogb(z)-23; */
	SET_HIGH_WORD(z, ix - ((int)e0 << 20));
	for (i = 0; i < 2; i++)
	{
		tx[i] = (double)((int)(z));
		z = (z - tx[i]) * two24;
	}
	tx[2] = z;
	nx = 3;
	while (tx[nx - 1] == zero)
		nx--; /* skip zero term */
	n = kernel_rem_pio2(tx, y, e0, nx, 2, two_over_pi);
	if (hx < 0)
	{
		y[0] = -y[0];
		y[1] = -y[1];
		return -n;
	}
	return n;
}
