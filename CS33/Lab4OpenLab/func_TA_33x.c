#include "func.h"

void Func1(int c[][TSIZE], int a[][TSIZE], int b[][TSIZE])
{
	int i, j, k;
	int r, sum;

	// ikj version
	#pragma omp parallel for private(i, j, k, r) 
	for (i=0; i<TSIZE; i++) {
		for (k=0; k<TSIZE; k++) 
		{ 
			r=a[i][k];
			for (j=0; j<TSIZE; j++) {
				c[i][j]+=r*b[k][j];
			}
		}
	}
}

void Func2(int d[][MSIZE], int c[][MSIZE])
{
	int i,j,ii, jj;
    int BSIZE=16;	// 16 shows the best performance Among 8, 16, 32, 128 Block size
	
    // Blocking
	#pragma omp parallel for private(jj, i, j) 
	for(ii=0; ii<MSIZE; ii+=BSIZE)
	for(jj=0; jj<MSIZE; jj+=BSIZE)
		for(i=ii; i<ii+BSIZE; i++) 
			for(j=jj; j<jj+BSIZE; j++)
				d[i][j]=c[j][i];
}

void Func3(int z[][MSIZE], int d[][MSIZE])
{
	int y, x, e, w, s, n;
	int near = 2;  	 // The weight of neighbor
	int itself = 42; // The weight of center

	// Removing "near*" by dividing z by 50
	// So, the value of itself is 42 instead of 84.
    #pragma omp parallel 
	{
	
	// Inner matrix has no conditions
	#pragma omp for private(x, e, w, s, n) //schedule(dynamic, 32)
	for (y=1; y<MSIZE-1; y++) {
		for (x=1; x<MSIZE-1; x++) {
            w = x-1;
			e = x+1;
			n = y-1;
			s = y+1;
			z[y][x] = 	(d[n][w]) + 	// nw
						(d[n][e]) +    // ne	
						(d[s][w]) +    // sw
						(d[s][e]) +    // se
						(d[y][w]) +      // w
						(d[y][e]) +      // e
	   					(d[n][x]) +    // n
	   					(d[s][x]) +    // s
						itself * d[y][x];        // c
			z[y][x]/=50;
		}
	}

	// leftmost column of the matrix
	#pragma omp for private(x, e, w, s, n) nowait
	for (y=0; y<MSIZE; y++) {
		x = 0;
        w = x;
		e = x+1;
		n = (y == 0) ? y : y-1;
		s = (y == MSIZE-1) ? y : y+1;
		z[y][x] = 	(d[n][w]) + 	// nw
	   				(d[n][e]) +    // ne	
	   				(d[s][w]) +    // sw
	   				(d[s][e]) +    // se
	   				(d[y][w]) +    // w
	   				(d[y][e]) +    // e
	   				(d[n][x]) +    // n
	   				(d[s][x]) +    // s
	   				itself * d[y][x];   // c
	   	z[y][x]/=50;
		}
    
	// rightmost column of the matrix
	#pragma omp for private(x, e, w, s, n) nowait
	for (y=0; y<MSIZE; y++) {
		x=MSIZE-1;
		w = x-1;
		e = x;
		n = (y == 0) ? y : y-1;
		s = (y == MSIZE-1) ? y : y+1;
		z[y][x] = 	(d[n][w]) + 	// nw
	   				(d[n][e]) +    // ne	
	   				(d[s][w]) +    // sw
	   				(d[s][e]) +    // se
	   				(d[y][w]) +      // w
	   				(d[y][e]) +      // e
	   				(d[n][x]) +    // n
	   				(d[s][x]) +    // s
	   				itself * d[y][x];        // c
	   	z[y][x]/=50;
		}
	
	// top row of the matrix except for 2 edges
	#pragma omp for private(x, y, e, w, s, n) nowait
	for (x=1; x<MSIZE-1; x++) {
		y=0;
		n=y;
		s=y+1;
		w=x-1;
		e=x+1;
		z[y][x] = 	(d[n][w]) + 	// nw
	   				(d[n][e]) +    // ne	
	   				(d[s][w]) +    // sw
	   				(d[s][e]) +    // se
	   				(d[y][w]) +    // w
	   				(d[y][e]) +    // e
	   				(d[n][x]) +    // n
	   				(d[s][x]) +    // s
	   				itself * d[y][x];   // c
	   	z[y][x]/=50;
	}
	
	// bottom row of the matrix except for 2 edges
	#pragma omp for private(x, y, e, w, s, n) nowait
	for (x=1; x<MSIZE-1; x++) {
		y=MSIZE-1;
		n=y-1;
		s=y;
		w=x-1;
		e=x+1;
		z[y][x] = 	(d[n][w]) + 	// nw
	   				(d[n][e]) +    // ne	
	   				(d[s][w]) +    // sw
	   				(d[s][e]) +    // se
	   				(d[y][w]) +    // w
	   				(d[y][e]) +    // e
	   				(d[n][x]) +    // n
	   				(d[s][x]) +    // s
	   				itself * d[y][x];   // c
	   	z[y][x]/=50;
	}
	
	}
}

void Func4(int b[], int a[])
{
	int chuck_size=MSIZE;	//	4096	 
	int array_size=VSIZE/chuck_size;	// 4096
	int chuck[chuck_size];
    int i, j;
	int j_array_size;
	int temp;
	
	// You can find some strength reduction comparing it with the original func.c 
	
	#pragma omp parallel private(i, j, j_array_size) 
	{
	#pragma omp for schedule(dynamic, 32)	// dynamic scheduling increase the performance 
	for(j=0; j<chuck_size; j++) {
		j_array_size=j<<12;
		b[j_array_size]=a[j_array_size];
		for (i=1; i<array_size; i++) {
			// There are dependencies between i iterations
			// but it does not cause an error since each thread takes the entire i iterations
			b[j_array_size+i]=b[j_array_size+i-1]+a[j_array_size+i];
		}
		chuck[j]=b[j_array_size+array_size-1];
	}
	
	//#pragma omp single 	// single works (or no pragma) but ordered better
	#pragma omp for ordered 
	for(j=1; j<chuck_size; j++) {
		#pragma omp ordered
		chuck[j]=chuck[j-1]+chuck[j];
	}

	#pragma omp for private(temp) schedule(dynamic, 32)	// dynimic scheduling increase the performance 
	for(j=1; j<chuck_size; j++) {
		j_array_size=j<<12;
		temp=chuck[j-1]/(j+1);
		for (i=0; i<array_size; i++) {
			b[j_array_size+i]+=temp;
		}
	}
	}

}

void Func5(int b[], int a[])
{
	int i, j,  stride, stride2, step, k;
    int temp=0;
	long log_N=log2(VSIZE);
	int i_1, k_1;
	int chunk[20];
	
	// You can find some strength reduction comparing it with the original func.c 
	
	#pragma omp parallel for 
	for(j=0; j<VSIZE; j+=2) {
		b[j]=a[j];
		b[j+1] = a[j] + a[j+1];
	}
	
	// Putting pragma here generates eror due to i=i<<1
	for(i=4; i<VSIZE; i=i<<1) { 
		k=i>>1;
		k_1=k-1;
		i_1=i-1;
		#pragma omp parallel for firstprivate(k_1, i_1)
		for(j=0; j<VSIZE; j+=i) {
				b[j+i_1] = b[j+k_1] + b[j+i_1];
		}
	}
	
	b[VSIZE-1]=0;
	// Putting pragma here generates incorrect result since there are dependency between i iterations
	for(i=(log_N-1); i>=0; i--) {
		{
		step=(2<<i);
		stride2=step-1;
		stride=(1<<i)-1;
		#pragma omp parallel for private(temp) firstprivate(stride, stride2)
		// pow() are revmoed.
		for(j=0; j<VSIZE; j+=step) {
                temp=b[j+stride];
			b[j+stride] = b[j+stride2];
			b[j+stride2] = temp+b[j+stride2];
		}
		}
	}
}
