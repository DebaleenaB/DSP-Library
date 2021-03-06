#include "dsp_lib.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "error.h"

/***********************************************
Function name: bavrfftf

Function description: 
The forward real-to-complex FFT of the real input vector is computed and stored in the complex
output vector.

Parameters: 
1. rv1 (inp) -  Real input vector
2. inc1 (inp) - Input vector increment
3. cvo (out) - Complex output vector
4. inco (inp) - Output vector increment
5. rso (out) - Nyquist value
6. n (inp) - FFT size (number of complex points)
7. p (inp) - pointer to FFT setup previously generated by calling avfftwts

Return type: integer error code

 ***********************************************/
void swap(float *in, CV_COMPLEX *out);

int bavrfftf( float *rv1, long inc1, float *cout, long inco, long n, long *p) 
{
	int incflag = 0, bufflag = 0, vecflag = 0, cflag = 0;
	float *cv2;
	float *cbuff;
	int s = 0;
	int q = 0, r = 0;
	FILE *inputFd, *outputFd;
	
	vec_float v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17,\
	v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32,l1, l2, l3, l4,zero;
	
	vec_float l1, l2, l3, l4;
	zero.AsVector = (vector float){0, 0, 0, 0};
	vec_Uchar perm_v1, perm_v2;
	
	
    perm_v1.AsVector = (vector unsigned char){0, 1, 2, 3, 16, 17, 18, 19, 4, 5, 6, 7, 16, 17, 18, 19};
	perm_v2.AsVector = (vector unsigned char){8, 9, 10, 11, 16, 17, 18, 19, 12, 13, 14, 15, 16, 17, 18, 19};
	 
	int prefetch_const = 0x10010100;
  if (inc1 == 1  && inco == 2 && (n%64 == 0))
	{
	 if (rv1 != cout)	 {
		int i = 0, j = 0;
		bufflag = 1;
		cbuff = calloc (n*inc1*2, sizeof (float));
		
		for (s = 0,i = 0; s < (n*inc1)/4, i < (2*n*inc1)/4; s+=4,i+=8)
		{
			vec_dst(&rv1[s*4], prefetch_const, 0);

			l1.AsVector = vec_ld(0, &rv1[s*4]);
			l2.AsVector = vec_ld(0, &rv1[(s+1)*4]);
			l3.AsVector = vec_ld(0, &rv1[(s+2)*4]);
			l4.AsVector = vec_ld(0, &rv1[(s+3)*4]);
			
			
		    v1.AsVector = vec_perm(l1.AsVector, zero.AsVector, perm_v1.AsVector); 
		    v2.AsVector = vec_perm(l1.AsVector, zero.AsVector, perm_v2.AsVector);  
		    v3.AsVector = vec_perm(l2.AsVector, zero.AsVector, perm_v1.AsVector);  
		    v4.AsVector = vec_perm(l2.AsVector, zero.AsVector, perm_v2.AsVector);                        
                    v5.AsVector = vec_perm(l3.AsVector, zero.AsVector, perm_v1.AsVector); 
		    v6.AsVector = vec_perm(l3.AsVector, zero.AsVector, perm_v2.AsVector); 
		    v7.AsVector = vec_perm(l4.AsVector, zero.AsVector, perm_v1.AsVector); 
		    v8.AsVector = vec_perm(l4.AsVector, zero.AsVector, perm_v2.AsVector); 
			
			
			vec_st(v1.AsVector, 0, &cbuff[i*4]);
			vec_st(v2.AsVector, 0, &cbuff[(i+1)*4]);
			vec_st(v3.AsVector, 0, &cbuff[(i+2)*4]);
			vec_st(v4.AsVector, 0, &cbuff[(i+3)*4]);
			vec_st(v5.AsVector, 0, &cbuff[(i+4)*4]);
			vec_st(v6.AsVector, 0, &cbuff[(i+5)*4]);
			vec_st(v7.AsVector, 0, &cbuff[(i+6)*4]);
			vec_st(v8.AsVector, 0, &cbuff[(i+7)*4]);

		}
		vec_dss(0);			
	}
	
		
		

	if ((inc1 != 2))
	{
		incflag = 1;
		cv2 = calloc(n*2, sizeof(float));
		
		
	if ((inc1 != 2))
	{
		incflag = 1;
		cv2 = vec_calloc(n*2, sizeof(float));
		for(s=0, q = 0;  s < (n*inc1)/4, q < (n*2)/4; s+=4, q += 8)
		{								
			if (bufflag == 1)
			{
				vec_dst(&cbuff[r*4], prefetch_const, 0);

				v1.AsVector = vec_ld(0, &cbuff[r*4]);
				v2.AsVector = vec_ld(0, &cbuff[(r+1)*4]);
				v3.AsVector = vec_ld(0, &cbuff[(r+2)*4]);
				v4.AsVector = vec_ld(0, &cbuff[(r+3)*4]);
				v5.AsVector = vec_ld(0, &cbuff[(r+4)*4]);
				v6.AsVector = vec_ld(0, &cbuff[(r+5)*4]);
				v7.AsVector = vec_ld(0, &cbuff[(r+6)*4]);
				v8.AsVector = vec_ld(0, &cbuff[(r+7)*4]);	

				vec_st(v1.AsVector, 0, &cv2[q*4]);
				vec_st(v2.AsVector, 0, &cv2[(q+1)*4]);
				vec_st(v3.AsVector, 0, &cv2[(q+2)*4]);
				vec_st(v4.AsVector, 0, &cv2[(q+3)*4]);
				vec_st(v5.AsVector, 0, &cv2[(q+4)*4]);
				vec_st(v6.AsVector, 0, &cv2[(q+5)*4]);
				vec_st(v7.AsVector, 0, &cv2[(q+6)*4]);
				vec_st(v8.AsVector, 0, &cv2[(q+7)*4]);

			}
			else
			{
				vec_dst(&rv1[s*4], prefetch_const, 0);

			    l1.AsVector = vec_ld(0, &rv1[s*4]);
			    l2.AsVector = vec_ld(0, &rv1[(s+1)*4]);
			    l3.AsVector = vec_ld(0, &rv1[(s+2)*4]);
			    l4.AsVector = vec_ld(0, &rv1[(s+3)*4]);
			    
			    
		        v1.AsVector = vec_perm(l1.AsVector, zero.AsVector, perm_v1.AsVector); 
		        v2.AsVector = vec_perm(l1.AsVector, zero.AsVector, perm_v2.AsVector);  
		        v3.AsVector = vec_perm(l2.AsVector, zero.AsVector, perm_v1.AsVector);  
		        v4.AsVector = vec_perm(l2.AsVector, zero.AsVector, perm_v2.AsVector);                        
                        v5.AsVector = vec_perm(l3.AsVector, zero.AsVector, perm_v1.AsVector); 
		        v6.AsVector = vec_perm(l3.AsVector, zero.AsVector, perm_v2.AsVector); 
		        v7.AsVector = vec_perm(l4.AsVector, zero.AsVector, perm_v1.AsVector); 
		        v8.AsVector = vec_perm(l4.AsVector, zero.AsVector, perm_v2.AsVector); 
			    
			    
			    vec_st(v1.AsVector, 0, &cv2[q*4]);
			    vec_st(v2.AsVector, 0, &cv2[(q+1)*4]);
			    vec_st(v3.AsVector, 0, &cv2[(q+2)*4]);
			    vec_st(v4.AsVector, 0, &cv2[(q+3)*4]);
			    vec_st(v5.AsVector, 0, &cv2[(q+4)*4]);
			    vec_st(v6.AsVector, 0, &cv2[(q+5)*4]);
			    vec_st(v7.AsVector, 0, &cv2[(q+6)*4]);
			    vec_st(v8.AsVector, 0, &cv2[(q+7)*4]);
			
			}
			r = r+(inc1);
			vec_dss(0);
		}
		inc1 = 2;
	}	
		
	
	if ((inc1 == 2) && n >=16)
	{
		vecflag = 1;
		const vector float vcfzero = (vector float){ 0., 0., 0., 0.};
		const vector float vcfnegeven = (vector float){-0., 0.,-0., 0.};
		const vector float vcfnegodd = (vector float){ 0.,-0., 0.,-0.};
		const vector float vcppnn = (vector float){ 1., 1.,-1.,-1.};
		const vector float vcpnnp = (vector float){ 1.,-1.,-1., 1.};
		const vector unsigned char vcprm1032 = (vector unsigned char){ 4,5,6,7, 0,1,2,3, 12,13,14,15, 8,9,10,11 };
		const vector unsigned char vcprm0022 = (vector unsigned char){ 0,1,2,3, 0,1,2,3, 8,9,10,11, 8,9,10,11 };
		const vector unsigned char vcprm1133 = (vector unsigned char){ 4,5,6,7, 4,5,6,7, 12,13,14,15, 12,13,14,15 };
		const vector unsigned char vcprm2301 = (vector unsigned char){ 8,9,10,11,12,13,14,15, 0,1,2,3,4,5,6,7 };
		const vector unsigned char vcprm0101 = (vector unsigned char){ 0,1,2,3,4,5,6,7, 0,1,2,3,4,5,6,7 };
		const vector unsigned char vcprm3232 = (vector unsigned char){ 12,13,14,15,8,9,10,11, 12,13,14,15,8,9,10,11 };
		const vector unsigned char vcaiarbibr = (vector unsigned char){ 4,5,6,7, 0,1,2,3, 20,21,22,23, 16,17,18,19 };
		const vector unsigned char vcaraibrbi = (vector unsigned char){ 0,1,2,3,4,5,6,7, 16,17,18,19,20,21,22,23 };
		vector float vtf10,vtf11,vtf12,vtf13,vtf14,vtf15,
		vtf20,vtf21,vtf22,vtf23,vtf24,vtf25,
		vtf31,vtf32,vtf33,vtf34,vtf35,
		vtf41,vtf42,vtf43,vtf44,vtf45,
		vtfw0,vtfw1,vtfw2,vtfw3;
		vector float* pvf;
		unsigned int pa,pb,qa,qb,stride,edirts,iw;
		unsigned int stage,block,j; 
		unsigned int log_n,i;

		log_n = log(n)/log(2);
		vector float *pw = (vector float *)(((fft_vector*)p)->cplx_arr);

		/*	vector float *pw = (vector float *)p;*/
		if (incflag == 1)
			pvf = (vector float *)cv2;
		else if (bufflag == 1)
			pvf = (vector float *)cbuff;
		else
			pvf = (vector float *)rv1;


		/*
		printf ("The twiddle values passed are \n\n");

		for (i = 0; i < 32; i++)
		{
			printf ("W[] = %f\n", ((float *)pw)[i]);
		}

		printf("\n\n");

		for (i = 0; i < 32; i++)
		{
			printf ("W[] = %f\n", ((float *)pvf)[i]);
		}
		 */


		stride = n/2;  /*in stage 0 stride = n/2 */
		for( j=0; j<stride/4; j++ ) {
			vtfw1 = vec_perm( pw[j], pw[j], vcprm1032 );
			vtfw2 = vec_xor( vtfw1, vcfnegeven );
			vtfw3 = vec_xor( vtfw1, vcfnegodd );
			vtf10 = vec_add( pvf[j], pvf[n/4+j] );
			vtf11 = vec_sub( pvf[j], pvf[n/4+j] );
			pvf[j] = vtf10;
			vtf20 = vec_add( pvf[n/8+j], pvf[n/8+n/4+j] );
			vtf21 = vec_sub( pvf[n/8+j], pvf[n/8+n/4+j] );
			pvf[n/8+j] = vtf20;
			vtf12 = vec_perm( vtf11, vtf11, vcprm0022 );
			vtf12 = vec_madd( vtf12, pw[j], vcfzero );
			vtf13 = vec_perm( vtf11, vtf11, vcprm1133 );
			pvf[n/4+j] = vec_madd( vtfw2, vtf13, vtf12 );
			vtf22 = vec_perm( vtf21, vtf21, vcprm1133 );
			vtf22 = vec_madd( vtf22, pw[j], vcfzero ); 
			vtf23 = vec_perm( vtf21, vtf21, vcprm0022 ); 
			pvf[n/4+n/8+j] = vec_madd( vtfw3, vtf23, vtf22 );
		}
		stride = n/4;  /*in stage 1 stride = n/4*/ 
		edirts = 1;
		for( stage=1; stage<log_n-2; stage++ ) {
			for( block=0; block<n/2; block+=stride ) {
				pa = block;
				pb = block + stride/4;
				qa = block + stride/2;
				qb = block + stride/4 + stride/2;
				iw = 0;
				for( j=0; j<stride/4; j++ ) {
					vtfw0 = vec_perm( pw[j*2*edirts],
							pw[j*2*edirts+edirts],
							vcaraibrbi );
					vtfw1 = vec_perm( pw[j*2*edirts],
							pw[j*2*edirts+edirts],
							vcaiarbibr );
					vtfw2 = vec_xor( vtfw1, vcfnegeven );
					vtfw3 = vec_xor( vtfw1, vcfnegodd );
					vtf10 = vec_add( pvf[pa+j], pvf[qa+j] );
					vtf11 = vec_sub( pvf[pa+j], pvf[qa+j] );
					pvf[pa+j] = vtf10;
					vtf20 = vec_add( pvf[pb+j], pvf[qb+j] );
					vtf21 = vec_sub( pvf[pb+j], pvf[qb+j] );
					pvf[pb+j] = vtf20;
					vtf12 = vec_perm( vtf11, vtf11, vcprm0022 );
					vtf12 = vec_madd( vtf12, vtfw0, vcfzero );
					vtf13 = vec_perm( vtf11, vtf11, vcprm1133 );
					pvf[qa+j] = vec_madd( vtfw2, vtf13, vtf12 );
					vtf22 = vec_perm( vtf21, vtf21, vcprm1133 );
					vtf22 = vec_madd( vtf22, vtfw0, vcfzero ); 
					vtf23 = vec_perm( vtf21, vtf21, vcprm0022 ); 
					pvf[qb+j] = vec_madd( vtfw3, vtf23, vtf22 ); 
				}

			}
			stride = stride >> 1;
			edirts = edirts << 1;
		}
		for( block=0; block<n/2; block+=8 ) { 
			vtf13 = vec_sub( pvf[block ], pvf[block+1] );
			vtf11 = vec_add( pvf[block ], pvf[block+1] );
			vtf23 = vec_sub( pvf[block+2], pvf[block+3] );
			vtf21 = vec_add( pvf[block+2], pvf[block+3] );

			/*This block makes sure N > 8 or at least 16*/
			vtf33 = vec_sub( pvf[block+4], pvf[block+5] );
			vtf31 = vec_add( pvf[block+4], pvf[block+5] );
			vtf43 = vec_sub( pvf[block+6], pvf[block+7] );
			vtf41 = vec_add( pvf[block+6], pvf[block+7] );
			/*This block makes sure N > 8 or at least 16*/

			vtf15 = vec_perm( vtf13, vtf13, vcprm3232 );
			vtf14 = vec_perm( vtf13, vtf13, vcprm0101 );
			vtf12 = vec_perm( vtf11, vtf11, vcprm2301 );
			vtf25 = vec_perm( vtf23, vtf23, vcprm3232 );
			vtf24 = vec_perm( vtf23, vtf23, vcprm0101 );
			vtf22 = vec_perm( vtf21, vtf21, vcprm2301 );

			/*This block corresponding to the blocks which make sure N is atleat 16*/
			vtf35 = vec_perm( vtf33, vtf33, vcprm3232 );
			vtf34 = vec_perm( vtf33, vtf33, vcprm0101 );
			vtf32 = vec_perm( vtf31, vtf31, vcprm2301 );
			vtf45 = vec_perm( vtf43, vtf43, vcprm3232 );
			vtf44 = vec_perm( vtf43, vtf43, vcprm0101 );
			vtf42 = vec_perm( vtf41, vtf41, vcprm2301 );
			/*This block corresponding to the blocks which make sure N is atleat 16*/

			pvf[block] = vec_madd( vtf11, vcppnn, vtf12 );
			pvf[block+1] = vec_madd( vtf15, vcpnnp, vtf14 );
			pvf[block+2] = vec_madd( vtf21, vcppnn, vtf22 );
			pvf[block+3] = vec_madd( vtf25, vcpnnp, vtf24 );

			/*This block corresponding to the blocks which make sure N is atleat 16*/
			pvf[block+4] = vec_madd( vtf31, vcppnn, vtf32 );
			pvf[block+5] = vec_madd( vtf35, vcpnnp, vtf34 );
			pvf[block+6] = vec_madd( vtf41, vcppnn, vtf42 );
			pvf[block+7] = vec_madd( vtf45, vcpnnp, vtf44 );
			/*This block corresponding to the blocks which make sure N is atleat 16*/
		}
	 }
	}
	else if ((inc1 == 2))
	{
	
	  cflag = 1;
	  CV_COMPLEX *pfs;
	  unsigned int stage,block,j,i,iw=0;
	  unsigned int pa,pb,qa,qb;
	  unsigned int stride,edirts;
	  CV_COMPLEX ft1a,ft1b,ft2a,ft2b,ft3a,ft3b;
	  unsigned int log_n;
      
	  log_n = log(n)/log(2);
      
	  if (incflag == 1)
	  	pfs = (CV_COMPLEX *)cv2;
	  else if (bufflag == 1)
	  	pfs = (CV_COMPLEX *)cbuff;
	  else
	  	pfs = (float *)rv1;
      
	  CV_COMPLEX *pfw = ((fft_vector*)p)->cplx_arr;
      
	  /*//INIT*/
	  stride = n/2;
	  edirts = 1;
	  /*//DIF FFT*/

	for( stage=0; stage<log_n-2; stage++ ) 
	{
		for( block=0; block<n; block+=stride*2 ) 
		{
			pa = block;
			pb = block + stride/2;
			qa = block + stride;
			qb = block + stride/2 + stride;
			iw = 0;
			for( j=0; j<stride/2; j++ ) 
			{
				/*//2bufflies/loop
					//add*/
				ft1a.real = pfs[pa+j].real + pfs[qa+j].real;
				ft1a.im = pfs[pa+j].im + pfs[qa+j].im;
				ft1b.real = pfs[pb+j].real + pfs[qb+j].real;
				ft1b.im = pfs[pb+j].im + pfs[qb+j].im;
				/*//sub*/
				ft2a.real = pfs[pa+j].real - pfs[qa+j].real;
				ft2a.im = pfs[pa+j].im - pfs[qa+j].im;
				ft2b.real = pfs[pb+j].real - pfs[qb+j].real;
				ft2b.im = pfs[pb+j].im - pfs[qb+j].im;
				pfs[pa+j] = ft1a;
				/*//store adds*/
				pfs[pb+j] = ft1b;
				/*//cmul*/
				pfs[qa+j].real = ft2a.real * pfw[iw].real -
				ft2a.im * pfw[iw].im;
				pfs[qa+j].im = ft2a.real * pfw[iw].im +
				ft2a.im * pfw[iw].real;
				/*//twiddled cmul*/
				pfs[qb+j].real = ft2b.real * pfw[iw].im +
				ft2b.im * pfw[iw].real;
				pfs[qb+j].im = -ft2b.real * pfw[iw].real +
				ft2b.im * pfw[iw].im;
				iw += edirts;
			}
		}
		stride = stride>>1;
		edirts = edirts<<1;
	}
	/*//last two stages*/
	for( j=0; j<n; j+=4 ) 
	{
		/*//upper two*/
		ft1a.real = pfs[j ].real + pfs[j+2].real;
		ft1a.im = pfs[j ].im + pfs[j+2].im;
		ft1b.real = pfs[j+1].real + pfs[j+3].real;
		ft1b.im = pfs[j+1].im + pfs[j+3].im;
		ft2a.real = ft1a.real + ft1b.real;
		ft2a.im = ft1a.im + ft1b.im;
		ft2b.real = ft1a.real - ft1b.real;
		ft2b.im = ft1a.im - ft1b.im;
		/*//lower two
			//notwiddle*/
		ft3a.real = pfs[j].real - pfs[j+2].real;
		ft3a.im = pfs[j].im - pfs[j+2].im;
		/*//twiddle*/
		ft3b.real = pfs[j+1].im - pfs[j+3].im;
		ft3b.im = -pfs[j+1].real + pfs[j+3].real;
		/*//store*/
		pfs[j ]	= ft2a;
		pfs[j+1]= ft2b;
		pfs[j+2].real = ft3a.real + ft3b.real;
		pfs[j+2].im = ft3a.im + ft3b.im;
		pfs[j+3].real = ft3a.real - ft3b.real;
		pfs[j+3].im = ft3a.im - ft3b.im;
	}	
	
	if ((incflag == 1) && (bufflag == 0))
	{
		q = 0;
		r = 0;
		
		for(r = 0; r < (n*2)/4; r += 8)
		{

			vec_dst(&cv2[r*4], prefetch_const, 0);

			v1.AsVector = vec_ld(0, &cv2[r*4]);
			v2.AsVector = vec_ld(0, &cv2[(r+1)*4]);
			v3.AsVector = vec_ld(0, &cv2[(r+2)*4]);
			v4.AsVector = vec_ld(0, &cv2[(r+3)*4]);
			v5.AsVector = vec_ld(0, &cv2[(r+4)*4]);
			v6.AsVector = vec_ld(0, &cv2[(r+5)*4]);
			v7.AsVector = vec_ld(0, &cv2[(r+6)*4]);
			v8.AsVector = vec_ld(0, &cv2[(r+7)*4]);	

			vec_st(v1.AsVector, 0, &rv1[q*4]);
			vec_st(v2.AsVector, 0, &rv1[(q+1)*4]);
			vec_st(v3.AsVector, 0, &rv1[(q+2)*4]);
			vec_st(v4.AsVector, 0, &rv1[(q+3)*4]);
			vec_st(v5.AsVector, 0, &rv1[(q+4)*4]);
			vec_st(v6.AsVector, 0, &rv1[(q+5)*4]);
			vec_st(v7.AsVector, 0, &rv1[(q+6)*4]);
			vec_st(v8.AsVector, 0, &rv1[(q+7)*4]);

			q = q + inco;
		}
		vec_dss(0);
	}
		
		
		
	else if ((incflag == 1) && (bufflag == 1))
	{
		q = 0;
		r = 0;
		
		for(r = 0; r < (n*2)/4; r += 8)
		{
			vec_dst(&cv2[r*4], prefetch_const, 0);

			v1.AsVector = vec_ld(0, &cv2[r*4]);
			v2.AsVector = vec_ld(0, &cv2[(r+1)*4]);
			v3.AsVector = vec_ld(0, &cv2[(r+2)*4]);
			v4.AsVector = vec_ld(0, &cv2[(r+3)*4]);
			v5.AsVector = vec_ld(0, &cv2[(r+4)*4]);
			v6.AsVector = vec_ld(0, &cv2[(r+5)*4]);
			v7.AsVector = vec_ld(0, &cv2[(r+6)*4]);
			v8.AsVector = vec_ld(0, &cv2[(r+7)*4]);	

			vec_st(v1.AsVector, 0, &cvo[q*4]);
			vec_st(v2.AsVector, 0, &cvo[(q+1)*4]);
			vec_st(v3.AsVector, 0, &cvo[(q+2)*4]);
			vec_st(v4.AsVector, 0, &cvo[(q+3)*4]);
			vec_st(v5.AsVector, 0, &cvo[(q+4)*4]);
			vec_st(v6.AsVector, 0, &cvo[(q+5)*4]);
			vec_st(v7.AsVector, 0, &cvo[(q+6)*4]);
			vec_st(v8.AsVector, 0, &cvo[(q+7)*4]);

			q = q + inco;
		}
		vec_dss(0);
	}
		
		
	
	
	else if ((incflag == 0) && (bufflag == 1))
	{
		q = 0;
		r = 0;
		
		for(r = 0; r < (n*2)/4; r += 8)			
		{

			vec_dst(&cbuff[r*4], prefetch_const, 0);

			v1.AsVector = vec_ld(0, &cbuff[r*4]);
			v2.AsVector = vec_ld(0, &cbuff[(r+1)*4]);
			v3.AsVector = vec_ld(0, &cbuff[(r+2)*4]);
			v4.AsVector = vec_ld(0, &cbuff[(r+3)*4]);
			v5.AsVector = vec_ld(0, &cbuff[(r+4)*4]);
			v6.AsVector = vec_ld(0, &cbuff[(r+5)*4]);
			v7.AsVector = vec_ld(0, &cbuff[(r+6)*4]);
			v8.AsVector = vec_ld(0, &cbuff[(r+7)*4]);	

			vec_st(v1.AsVector, 0, &cvo[q*4]);
			vec_st(v2.AsVector, 0, &cvo[(q+1)*4]);
			vec_st(v3.AsVector, 0, &cvo[(q+2)*4]);
			vec_st(v4.AsVector, 0, &cvo[(q+3)*4]);
			vec_st(v5.AsVector, 0, &cvo[(q+4)*4]);
			vec_st(v6.AsVector, 0, &cvo[(q+5)*4]);
			vec_st(v7.AsVector, 0, &cvo[(q+6)*4]);
			vec_st(v8.AsVector, 0, &cvo[(q+7)*4]);

		}

		q = q + inco;
	}
		
		
		
	if (bufflag == 1)
	{
		free (cbuff);
	}
	if (incflag == 1)
	{
		free (cv2);
	}
	return 0;
}
/*-----------------------------------c code --------------------------------------------*/
  else
    {
    	  if (rv1 != cout)
    	{
    		int i = 0, j = 0;
    		bufflag = 1;
    		cbuff = calloc (n*inc1*2, sizeof (float));
		while ( i < n*inc1*2 )
		{
			cbuff[i] = rv1[j];
			i = i + 2;
			j = j + 1;
		}
		inc1 = 2 * inc1;
	
		outputFd = fopen("fft_gap.txt", "w+");
		for (i = 0; i < 128*1024; i++)
		{
			fprintf(outputFd, "%f\n", *((float*)cbuff+i));
		}
		fclose(outputFd);
	
		for (i = 0; i < 10; i++)
		{
			printf("cbuff[%d]:%f\n", i, cbuff[i]);
			printf("%d\n", inc1);
		}
	
	   }
	
	
	if ((inc1 != 2))
	{
		incflag = 1;
		cv2 = calloc(n*2, sizeof(float));
		
		
	if ((inc1 != 2))
	{
		incflag = 1;
		cv2 = vec_calloc(n*2, sizeof(float));
		
		while ( q < n*2 )
		{
			if (bufflag == 1)
			{
				cv2[q] = cbuff[r];
				cv2[q+1] = cbuff[r+1];
			}
			else
			{
				cv2[q] = rv1[r];
				cv2[q+1] = rv1[r+1];
			}
			q = q+2;
			r = r+inc1;
		}
		inc1 = 2;
	}
	
	if ((inc1 == 2) && n >=16)
	{
		vecflag = 1;
		const vector float vcfzero = (vector float){ 0., 0., 0., 0.};
		const vector float vcfnegeven = (vector float){-0., 0.,-0., 0.};
		const vector float vcfnegodd = (vector float){ 0.,-0., 0.,-0.};
		const vector float vcppnn = (vector float){ 1., 1.,-1.,-1.};
		const vector float vcpnnp = (vector float){ 1.,-1.,-1., 1.};
		const vector unsigned char vcprm1032 = (vector unsigned char){ 4,5,6,7, 0,1,2,3, 12,13,14,15, 8,9,10,11 };
		const vector unsigned char vcprm0022 = (vector unsigned char){ 0,1,2,3, 0,1,2,3, 8,9,10,11, 8,9,10,11 };
		const vector unsigned char vcprm1133 = (vector unsigned char){ 4,5,6,7, 4,5,6,7, 12,13,14,15, 12,13,14,15 };
		const vector unsigned char vcprm2301 = (vector unsigned char){ 8,9,10,11,12,13,14,15, 0,1,2,3,4,5,6,7 };
		const vector unsigned char vcprm0101 = (vector unsigned char){ 0,1,2,3,4,5,6,7, 0,1,2,3,4,5,6,7 };
		const vector unsigned char vcprm3232 = (vector unsigned char){ 12,13,14,15,8,9,10,11, 12,13,14,15,8,9,10,11 };
		const vector unsigned char vcaiarbibr = (vector unsigned char){ 4,5,6,7, 0,1,2,3, 20,21,22,23, 16,17,18,19 };
		const vector unsigned char vcaraibrbi = (vector unsigned char){ 0,1,2,3,4,5,6,7, 16,17,18,19,20,21,22,23 };
		vector float vtf10,vtf11,vtf12,vtf13,vtf14,vtf15,
		vtf20,vtf21,vtf22,vtf23,vtf24,vtf25,
		vtf31,vtf32,vtf33,vtf34,vtf35,
		vtf41,vtf42,vtf43,vtf44,vtf45,
		vtfw0,vtfw1,vtfw2,vtfw3;
		vector float* pvf;
		unsigned int pa,pb,qa,qb,stride,edirts,iw;
		unsigned int stage,block,j; 
		unsigned int log_n,i;

		log_n = log(n)/log(2);
		vector float *pw = (vector float *)(((fft_vector*)p)->cplx_arr);

		/*	vector float *pw = (vector float *)p;*/
		if (incflag == 1)
			pvf = (vector float *)cv2;
		else if (bufflag == 1)
			pvf = (vector float *)cbuff;
		else
			pvf = (vector float *)rv1;


		/*
		printf ("The twiddle values passed are \n\n");

		for (i = 0; i < 32; i++)
		{
			printf ("W[] = %f\n", ((float *)pw)[i]);
		}

		printf("\n\n");

		for (i = 0; i < 32; i++)
		{
			printf ("W[] = %f\n", ((float *)pvf)[i]);
		}
		 */


		stride = n/2;  /*in stage 0 stride = n/2 */
		for( j=0; j<stride/4; j++ ) {
			vtfw1 = vec_perm( pw[j], pw[j], vcprm1032 );
			vtfw2 = vec_xor( vtfw1, vcfnegeven );
			vtfw3 = vec_xor( vtfw1, vcfnegodd );
			vtf10 = vec_add( pvf[j], pvf[n/4+j] );
			vtf11 = vec_sub( pvf[j], pvf[n/4+j] );
			pvf[j] = vtf10;
			vtf20 = vec_add( pvf[n/8+j], pvf[n/8+n/4+j] );
			vtf21 = vec_sub( pvf[n/8+j], pvf[n/8+n/4+j] );
			pvf[n/8+j] = vtf20;
			vtf12 = vec_perm( vtf11, vtf11, vcprm0022 );
			vtf12 = vec_madd( vtf12, pw[j], vcfzero );
			vtf13 = vec_perm( vtf11, vtf11, vcprm1133 );
			pvf[n/4+j] = vec_madd( vtfw2, vtf13, vtf12 );
			vtf22 = vec_perm( vtf21, vtf21, vcprm1133 );
			vtf22 = vec_madd( vtf22, pw[j], vcfzero ); 
			vtf23 = vec_perm( vtf21, vtf21, vcprm0022 ); 
			pvf[n/4+n/8+j] = vec_madd( vtfw3, vtf23, vtf22 );
		}
		stride = n/4;  /*in stage 1 stride = n/4*/ 
		edirts = 1;
		for( stage=1; stage<log_n-2; stage++ ) {
			for( block=0; block<n/2; block+=stride ) {
				pa = block;
				pb = block + stride/4;
				qa = block + stride/2;
				qb = block + stride/4 + stride/2;
				iw = 0;
				for( j=0; j<stride/4; j++ ) {
					vtfw0 = vec_perm( pw[j*2*edirts],
							pw[j*2*edirts+edirts],
							vcaraibrbi );
					vtfw1 = vec_perm( pw[j*2*edirts],
							pw[j*2*edirts+edirts],
							vcaiarbibr );
					vtfw2 = vec_xor( vtfw1, vcfnegeven );
					vtfw3 = vec_xor( vtfw1, vcfnegodd );
					vtf10 = vec_add( pvf[pa+j], pvf[qa+j] );
					vtf11 = vec_sub( pvf[pa+j], pvf[qa+j] );
					pvf[pa+j] = vtf10;
					vtf20 = vec_add( pvf[pb+j], pvf[qb+j] );
					vtf21 = vec_sub( pvf[pb+j], pvf[qb+j] );
					pvf[pb+j] = vtf20;
					vtf12 = vec_perm( vtf11, vtf11, vcprm0022 );
					vtf12 = vec_madd( vtf12, vtfw0, vcfzero );
					vtf13 = vec_perm( vtf11, vtf11, vcprm1133 );
					pvf[qa+j] = vec_madd( vtfw2, vtf13, vtf12 );
					vtf22 = vec_perm( vtf21, vtf21, vcprm1133 );
					vtf22 = vec_madd( vtf22, vtfw0, vcfzero ); 
					vtf23 = vec_perm( vtf21, vtf21, vcprm0022 ); 
					pvf[qb+j] = vec_madd( vtfw3, vtf23, vtf22 ); 
				}

			}
			stride = stride >> 1;
			edirts = edirts << 1;
		}
		for( block=0; block<n/2; block+=8 ) { 
			vtf13 = vec_sub( pvf[block ], pvf[block+1] );
			vtf11 = vec_add( pvf[block ], pvf[block+1] );
			vtf23 = vec_sub( pvf[block+2], pvf[block+3] );
			vtf21 = vec_add( pvf[block+2], pvf[block+3] );

			/*This block makes sure N > 8 or at least 16*/
			vtf33 = vec_sub( pvf[block+4], pvf[block+5] );
			vtf31 = vec_add( pvf[block+4], pvf[block+5] );
			vtf43 = vec_sub( pvf[block+6], pvf[block+7] );
			vtf41 = vec_add( pvf[block+6], pvf[block+7] );
			/*This block makes sure N > 8 or at least 16*/

			vtf15 = vec_perm( vtf13, vtf13, vcprm3232 );
			vtf14 = vec_perm( vtf13, vtf13, vcprm0101 );
			vtf12 = vec_perm( vtf11, vtf11, vcprm2301 );
			vtf25 = vec_perm( vtf23, vtf23, vcprm3232 );
			vtf24 = vec_perm( vtf23, vtf23, vcprm0101 );
			vtf22 = vec_perm( vtf21, vtf21, vcprm2301 );

			/*This block corresponding to the blocks which make sure N is atleat 16*/
			vtf35 = vec_perm( vtf33, vtf33, vcprm3232 );
			vtf34 = vec_perm( vtf33, vtf33, vcprm0101 );
			vtf32 = vec_perm( vtf31, vtf31, vcprm2301 );
			vtf45 = vec_perm( vtf43, vtf43, vcprm3232 );
			vtf44 = vec_perm( vtf43, vtf43, vcprm0101 );
			vtf42 = vec_perm( vtf41, vtf41, vcprm2301 );
			/*This block corresponding to the blocks which make sure N is atleat 16*/

			pvf[block] = vec_madd( vtf11, vcppnn, vtf12 );
			pvf[block+1] = vec_madd( vtf15, vcpnnp, vtf14 );
			pvf[block+2] = vec_madd( vtf21, vcppnn, vtf22 );
			pvf[block+3] = vec_madd( vtf25, vcpnnp, vtf24 );

			/*This block corresponding to the blocks which make sure N is atleat 16*/
			pvf[block+4] = vec_madd( vtf31, vcppnn, vtf32 );
			pvf[block+5] = vec_madd( vtf35, vcpnnp, vtf34 );
			pvf[block+6] = vec_madd( vtf41, vcppnn, vtf42 );
			pvf[block+7] = vec_madd( vtf45, vcpnnp, vtf44 );
			/*This block corresponding to the blocks which make sure N is atleat 16*/
		}
	}
	else if ((inc1 == 2))
	{
	
	  cflag = 1;
	  CV_COMPLEX *pfs;
	  unsigned int stage,block,j,i,iw=0;
	  unsigned int pa,pb,qa,qb;
	  unsigned int stride,edirts;
	  CV_COMPLEX ft1a,ft1b,ft2a,ft2b,ft3a,ft3b;
	  unsigned int log_n;
      
	  log_n = log(n)/log(2);
      
	  if (incflag == 1)
	  	pfs = (CV_COMPLEX *)cv2;
	  else if (bufflag == 1)
	  	pfs = (CV_COMPLEX *)cbuff;
	  else
	  	pfs = (float *)rv1;
      
	  CV_COMPLEX *pfw = ((fft_vector*)p)->cplx_arr;
      
	  /*//INIT*/
	  stride = n/2;
	  edirts = 1;
	  /*//DIF FFT*/

	for( stage=0; stage<log_n-2; stage++ ) 
	{
		for( block=0; block<n; block+=stride*2 ) 
		{
			pa = block;
			pb = block + stride/2;
			qa = block + stride;
			qb = block + stride/2 + stride;
			iw = 0;
			for( j=0; j<stride/2; j++ ) 
			{
				/*//2bufflies/loop
					//add*/
				ft1a.real = pfs[pa+j].real + pfs[qa+j].real;
				ft1a.im = pfs[pa+j].im + pfs[qa+j].im;
				ft1b.real = pfs[pb+j].real + pfs[qb+j].real;
				ft1b.im = pfs[pb+j].im + pfs[qb+j].im;
				/*//sub*/
				ft2a.real = pfs[pa+j].real - pfs[qa+j].real;
				ft2a.im = pfs[pa+j].im - pfs[qa+j].im;
				ft2b.real = pfs[pb+j].real - pfs[qb+j].real;
				ft2b.im = pfs[pb+j].im - pfs[qb+j].im;
				pfs[pa+j] = ft1a;
				/*//store adds*/
				pfs[pb+j] = ft1b;
				/*//cmul*/
				pfs[qa+j].real = ft2a.real * pfw[iw].real -
				ft2a.im * pfw[iw].im;
				pfs[qa+j].im = ft2a.real * pfw[iw].im +
				ft2a.im * pfw[iw].real;
				/*//twiddled cmul*/
				pfs[qb+j].real = ft2b.real * pfw[iw].im +
				ft2b.im * pfw[iw].real;
				pfs[qb+j].im = -ft2b.real * pfw[iw].real +
				ft2b.im * pfw[iw].im;
				iw += edirts;
			}
		}
		stride = stride>>1;
		edirts = edirts<<1;
	}
	/*//last two stages*/
	for( j=0; j<n; j+=4 ) 
	{
		/*//upper two*/
		ft1a.real = pfs[j ].real + pfs[j+2].real;
		ft1a.im = pfs[j ].im + pfs[j+2].im;
		ft1b.real = pfs[j+1].real + pfs[j+3].real;
		ft1b.im = pfs[j+1].im + pfs[j+3].im;
		ft2a.real = ft1a.real + ft1b.real;
		ft2a.im = ft1a.im + ft1b.im;
		ft2b.real = ft1a.real - ft1b.real;
		ft2b.im = ft1a.im - ft1b.im;
		/*//lower two
			//notwiddle*/
		ft3a.real = pfs[j].real - pfs[j+2].real;
		ft3a.im = pfs[j].im - pfs[j+2].im;
		/*//twiddle*/
		ft3b.real = pfs[j+1].im - pfs[j+3].im;
		ft3b.im = -pfs[j+1].real + pfs[j+3].real;
		/*//store*/
		pfs[j ]	= ft2a;
		pfs[j+1]= ft2b;
		pfs[j+2].real = ft3a.real + ft3b.real;
		pfs[j+2].im = ft3a.im + ft3b.im;
		pfs[j+3].real = ft3a.real - ft3b.real;
		pfs[j+3].im = ft3a.im - ft3b.im;
	}	
	
	if ((incflag == 1) && (bufflag == 0))
	{
		q = 0;
		r = 0;
		
			
		while (r < n*2)
		{
			rv1[q] = cv2[r];
			rv1[q+1] = cv2[r+1];

			q = q + inco;
			r = r + 2;
		}
	}
	
	
	
	else if ((incflag == 1) && (bufflag == 1))
	{
		q = 0;
		r = 0;
				
		while (r < n*2)
		{
			cout[q] = cv2[r];
			cout[q+1] = cv2[r+1];

			q = q + inco;
			r = r + 2;
		}
	}
	
	
	
	
	else if ((incflag == 0) && (bufflag == 1))
	{
		q = 0;
		r = 0;
		
		
		while (r < n*2)
		{
			cout[q] = cbuff[r];
			cout[q+1] = cbuff[r+1];

			q = q + inco;
			r = r + 2;
		}
	}
	if (bufflag == 1)
	{
		free (cbuff);
	}
	if (incflag == 1)
	{
		free (cv2);
	}
	return 0;
  }
 }
}


unsigned int reverseBits(unsigned int num, unsigned int n)
{
	unsigned int  NO_OF_BITS = log(n)/log(2);	
	unsigned int reverse_num = 0, i, temp;
	for (i = 0; i < NO_OF_BITS; i++)
	{ 
		temp = (num & (1 << i));
		if(temp)
			reverse_num |= (1 << ((NO_OF_BITS - 1) - i));

	}
	/*printf("input number=%d, reversed no.=%d\n", num, reverse_num);*/
	return reverse_num;
}

int rearrange_fft( float *inp, unsigned int n)
{
	CV_COMPLEX *pfs = (CV_COMPLEX *)inp;

	int i=0, j=0;
	for (i=0; i<n; i++)
	{
		j = reverseBits(i,n);
		if (i < j)
		{
			swap(&pfs[i],&pfs[j]);
		}
	}
}

void swap(CV_COMPLEX *in, CV_COMPLEX *out)
{
	CV_COMPLEX temp;

	temp.real = out->real;
	temp.im = out->im;

	out->real = in->real;
	out->im = in->im;

	in->real = temp.real;
	in->im = temp.im;

}
/***********************************************
Functdtion name: bavfftwts

Function description: 
bavfftwts creates a vector capable of holding real or complex data.
These functions return a pointer to the newly created vector header.
The created data areas meet all padding and alignment requirements.

Parameters: 
1. pointer to pointer to vector
2. complex length 
3. real length

Return type: error integer code

************************************************/

int bavfftwts(long **p, long m, long n);

int bavfftwts(long **p, long m, long n)
{
	volatile int k = 0,i = 0,j = 0;
	volatile float cplx_ang;
	volatile float real_ang;
	volatile vec_float vec_cplx_ang, vec_real_ang;
	volatile vec_float v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12,\
	v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, \
	v26, v27, v28, v29, v30, v31, v32;

	volatile float *arr_cnt;
	const vector float kMinusZero = vec_neg_zero();
	volatile vector float zero = (vector float) vec_splat_u8(0);
	volatile float *tw;

	if(((*p) = (fft_vector*)vec_calloc(1, sizeof(fft_vector))) == NULL)
	{
		fprintf(stderr, "Failed to allocate memory\n");
		exit (EXIT_FAILURE);
	}

	if (n == 0)
	{
		if (m >= 16)
		{
			arr_cnt = (float *)vec_calloc ((m), sizeof(float));
			for(i = 0; i < m; i++) 
			{
				arr_cnt[i] = i;
			}

			cplx_ang = (2*PI)/m;
			vec_cplx_ang.AsVector = (vector float) {cplx_ang, cplx_ang, cplx_ang, cplx_ang}; 

			if((((fft_vector*)*p)->cplx_arr = (CV_COMPLEX*)vec_calloc((m), sizeof(CV_COMPLEX))) == NULL)
			{
				fprintf(stderr, "Failed to allocate memory for complex vector\n");
				exit (EXIT_FAILURE);
			}
			((fft_vector*)*p)->c_size = m;
			((fft_vector*)*p)->real_arr = NULL;

			tw = (float*)((fft_vector*)*p)->cplx_arr;

			for (i = 0; i < (m/4); i += 4) 
			{
				v1.AsVector = vec_ld (0, &arr_cnt[i*4]);
				v2.AsVector = vec_ld (0, &arr_cnt[(i+1)*4]);
				v3.AsVector = vec_ld (0, &arr_cnt[(i+2)*4]);
				v4.AsVector = vec_ld (0, &arr_cnt[(i+3)*4]);

				v5.AsVector = vec_madd (v1.AsVector, vec_cplx_ang.AsVector, kMinusZero);
				v6.AsVector = vec_madd (v2.AsVector, vec_cplx_ang.AsVector, kMinusZero);
				v7.AsVector = vec_madd (v3.AsVector, vec_cplx_ang.AsVector, kMinusZero);
				v8.AsVector = vec_madd (v4.AsVector, vec_cplx_ang.AsVector, kMinusZero);

				sin_cos(v5.AsVector, &v9.AsVector, &v13.AsVector);
				sin_cos(v6.AsVector, &v10.AsVector, &v14.AsVector);
				sin_cos(v7.AsVector, &v11.AsVector, &v15.AsVector);
				sin_cos(v8.AsVector, &v12.AsVector, &v16.AsVector);

				/*negate sine*/
				v17.AsVector = vec_sub (zero, v13.AsVector);
				v18.AsVector = vec_sub (zero, v14.AsVector);
				v19.AsVector = vec_sub (zero, v15.AsVector);
				v20.AsVector = vec_sub (zero, v16.AsVector);

				v21.AsVector = vec_mergeh (v9.AsVector, v17.AsVector);
				v22.AsVector = vec_mergel (v9.AsVector, v17.AsVector);
				v23.AsVector = vec_mergeh (v10.AsVector, v18.AsVector);
				v24.AsVector = vec_mergel (v10.AsVector, v18.AsVector);
				v25.AsVector = vec_mergeh (v11.AsVector, v19.AsVector);
				v26.AsVector = vec_mergel (v11.AsVector, v19.AsVector);
				v27.AsVector = vec_mergeh (v12.AsVector, v20.AsVector);
				v28.AsVector = vec_mergel (v12.AsVector, v20.AsVector);

				vec_st (v21.AsVector, 0, &tw[(i*2)*4]);
				vec_st (v22.AsVector, 0, &tw[((i*2)+1)*4]);
				vec_st (v23.AsVector, 0, &tw[((i*2)+2)*4]);
				vec_st (v24.AsVector, 0, &tw[((i*2)+3)*4]);
				vec_st (v25.AsVector, 0, &tw[((i*2)+4)*4]);
				vec_st (v26.AsVector, 0, &tw[((i*2)+5)*4]);
				vec_st (v27.AsVector, 0, &tw[((i*2)+6)*4]);
				vec_st (v28.AsVector, 0, &tw[((i*2)+7)*4]);

			}
		}
		else
		{
			if((((fft_vector*)*p)->cplx_arr = (CV_COMPLEX*)vec_calloc((m), sizeof(CV_COMPLEX))) == NULL)
			{
				fprintf(stderr, "Failed to allocate memory for complex vector\n");
				exit (EXIT_FAILURE);
			}
			((fft_vector*)*p)->c_size = m;
			((fft_vector*)*p)->real_arr = NULL;


			while(k < (m))
			{
				((fft_vector*)*p)->cplx_arr[j].real = cos(((2 * PI * k ) / m ));
				((fft_vector*)*p)->cplx_arr[j].im =  - sin(((2 * PI * k ) / m));
				k = k + 1;
				j += 1;
			}

		}
	}
	else if(m == 0)
	{	
		if (n >= 16)
		{
			arr_cnt = (float *)vec_calloc ((n), sizeof(float));
			for(i = 0; i < n; i++) 
			{
				arr_cnt[i] = i;
			}

			real_ang = (2*PI)/n;
			vec_real_ang.AsVector = (vector float) {real_ang, real_ang, real_ang, real_ang}; 

			if((((fft_vector*)*p)->real_arr = (CV_COMPLEX*)vec_calloc((n), sizeof(CV_COMPLEX))) == NULL)
			{
				fprintf(stderr, "Failed to allocate memory for real vector\n");
				exit (EXIT_FAILURE);
			}
			((fft_vector*)*p)->r_size = n;
			((fft_vector*)*p)->cplx_arr = NULL;

			tw = (float*)((fft_vector*)*p)->real_arr;
			for (i = 0; i < (n/4); i += 4) 
			{
				v1.AsVector = vec_ld (0, &arr_cnt[i*4]);
				v2.AsVector = vec_ld (0, &arr_cnt[(i+1)*4]);
				v3.AsVector = vec_ld (0, &arr_cnt[(i+2)*4]);
				v4.AsVector = vec_ld (0, &arr_cnt[(i+3)*4]);

				v5.AsVector = vec_madd (v1.AsVector, vec_real_ang.AsVector, kMinusZero);
				v6.AsVector = vec_madd (v2.AsVector, vec_real_ang.AsVector, kMinusZero);
				v7.AsVector = vec_madd (v3.AsVector, vec_real_ang.AsVector, kMinusZero);
				v8.AsVector = vec_madd (v4.AsVector, vec_real_ang.AsVector, kMinusZero);

				sin_cos(v5.AsVector, &v9.AsVector, &v13.AsVector);
				sin_cos(v6.AsVector, &v10.AsVector, &v14.AsVector);
				sin_cos(v7.AsVector, &v11.AsVector, &v15.AsVector);
				sin_cos(v8.AsVector, &v12.AsVector, &v16.AsVector);

				/*			negate sine*/
				v17.AsVector = vec_sub (zero, v13.AsVector);
				v18.AsVector = vec_sub (zero, v14.AsVector);
				v19.AsVector = vec_sub (zero, v15.AsVector);
				v20.AsVector = vec_sub (zero, v16.AsVector);

				v21.AsVector = vec_mergeh (v9.AsVector, v17.AsVector);
				v22.AsVector = vec_mergel (v9.AsVector, v17.AsVector);
				v23.AsVector = vec_mergeh (v10.AsVector, v18.AsVector);
				v24.AsVector = vec_mergel (v10.AsVector, v18.AsVector);
				v25.AsVector = vec_mergeh (v11.AsVector, v19.AsVector);
				v26.AsVector = vec_mergel (v11.AsVector, v19.AsVector);
				v27.AsVector = vec_mergeh (v12.AsVector, v20.AsVector);
				v28.AsVector = vec_mergel (v12.AsVector, v20.AsVector);

				vec_st (v21.AsVector, 0, &tw[(i*2)*4]);
				vec_st (v22.AsVector, 0, &tw[((i*2)+1)*4]);
				vec_st (v23.AsVector, 0, &tw[((i*2)+2)*4]);
				vec_st (v24.AsVector, 0, &tw[((i*2)+3)*4]);
				vec_st (v25.AsVector, 0, &tw[((i*2)+4)*4]);
				vec_st (v26.AsVector, 0, &tw[((i*2)+5)*4]);
				vec_st (v27.AsVector, 0, &tw[((i*2)+6)*4]);
				vec_st (v28.AsVector, 0, &tw[((i*2)+7)*4]);
			}
		}
		else
		{
			if((((fft_vector*)*p)->real_arr = (CV_COMPLEX*)vec_calloc((n), sizeof(CV_COMPLEX))) == NULL)
			{
				fprintf(stderr, "Failed to allocate memory for real vector\n");
				exit (EXIT_FAILURE);
			}
			((fft_vector*)*p)->r_size = n;
			((fft_vector*)*p)->cplx_arr = NULL;

			while(k < (n))
			{
				((fft_vector*)*p)->real_arr[j].real = cos(((2 * PI * k ) / n));
				((fft_vector*)*p)->real_arr[j].im =  - sin(((2 * PI * k ) / n));
				k = k + 1;
				j += 1;
			}
		}
	}
	return (SUCCESS);
}

/***********************************************
Function name: bavfreewts

Function description: 
bavfreewts deallocates the  memory used by operand. bvfree also frees up memory allocated by 
vmalloc or vcalloc. Finally, vfree can also deallocate memory allocated by any of the FIR 
initialization functions.

Parameters: pointer to pointer to vector

Return type: error code

************************************************/

int bavfreewts(long **p);

int bavfreewts(long **p)
{
	if (((fft_vector*)*p)->cplx_arr != NULL) 
	{
		free (((fft_vector*)*p)->cplx_arr);
		(((fft_vector*)*p)->cplx_arr) = NULL;
		free(((fft_vector*)*p));
		(*p) = NULL; 
	}
	else if (((fft_vector*)*p)->real_arr != NULL)
	{
		free (((fft_vector*)*p)->real_arr);
		(((fft_vector*)*p)->real_arr) = NULL;
		free(((fft_vector*)*p));
		(*p) = NULL; 
	}
	return (SUCCESS);
}

