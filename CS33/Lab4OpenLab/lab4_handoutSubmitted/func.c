#include "func.h"
#include <omp.h>

void Func1(int c[][TSIZE], int a[][TSIZE], int b[][TSIZE])
{
  int i, j, k;
  
  omp_set_nested(2);
#pragma omp parallel for num_threads(30) private(i, j, k)

  for (i=0; i<TSIZE; i++)
    {
      int *arrayA = a[i];
      int *arrayC = c[i];
      for (k=0; k<TSIZE; k++)
	{
	  int Ak = arrayA[k];
	  int *arrayB = b[k];
	  for (j=0; j<TSIZE; j++)
	      arrayC[j]+=Ak*arrayB[j];
	    
	}
    }
}

void Func2(int d[][MSIZE], int c[][MSIZE])
{
  int i,j,x,y,a,b;
  omp_set_nested(3);
#pragma omp parallel for num_threads(30) firstprivate(c, d) private(i, j, x, y, a, b)
	for(i=0; i<MSIZE; i+=32)
	  for(j=0; j<MSIZE; j+=32)
	    for(x=i; x<i+32; x++)
	      for(y=j; y<j+32; y++)
		d[x][y]=c[y][x];
}

void Func3(int z[][MSIZE], int d[][MSIZE])
{
  int y, x;
  int near = 2;  // The weight of neighbor
  int itself = 84; // The weight of center
  omp_set_nested(1);
#pragma omp parallel for num_threads(30) firstprivate(near, itself, d, z) private(x, y)

  for (y=0; y<MSIZE; y++) {
    int *DY = d[y];
    int *DY1 = d[y+1];
    int *DYneg1 = d[y-1];
    int *ZY = z[y];
    for (x=0; x<MSIZE; x++) {
      int DYX = DY[x];
      int DY1X = DY1[x];
      int DYneg1X = DYneg1[x];

      int DYX1 = DY[x+1];
      int DY1X1 = DY1[x+1];
      int DYneg1X1 = DYneg1[x+1];

      int DYXneg1 = DY[x-1];
      int DY1Xneg1 = DY1[x-1];
      int DYneg1Xneg1 = DYneg1[x-1];

      if (y==0) {
	if (x==0) {
	  z[y][x] = near * (DYX + DYX1 + DY1X + DY1X1 + DYX + DYX1 + DYX + DY1X) + itself * DYX;
	} else if (x==MSIZE-1) {
	  z[y][x] = near * (DYXneg1 + DYX + DY1Xneg1 + DY1X + DYXneg1 + DYX + DYX + DY1X) + itself * DYX;
	} else {
	  z[y][x] = near * (DYXneg1 + DYX1 + DY1Xneg1 + DY1X1 + DYXneg1 + DYX1 + DYX + DY1X) + itself * DYX;
	}
      } else if (y==MSIZE-1) {
	if (x==0) {
	  z[y][x] = near * (DYneg1X + DYneg1X1 + DYX + DYX1 + DYX + DYX1 + DYneg1X + DYX) + itself * DYX;
	} else if (x==MSIZE-1) {
	  z[y][x] = near * (DYneg1Xneg1 + DYneg1X + DYXneg1 + DYX + DYXneg1 + DYX + DYneg1X + DYX) + itself * DYX;
	} else {
	  z[y][x] = near * (DYneg1Xneg1 + DYneg1X1 + DYXneg1 + DYX1 + DYXneg1 + DYX1 + DYneg1X + DYX) + itself * DYX;
	}
      } else {
	if (x==0) {
	  z[y][x] = near * (DYneg1X + DYneg1X1 + DY1X + DY1X1 + DYX + DYX1 + DYneg1X + DY1X) +itself * DYX;
	} else if (x==MSIZE-1) {
	  z[y][x] = near * (DYneg1Xneg1 + DYneg1X + DY1Xneg1 + DY1X + DYXneg1 + DYX + DYneg1X + DY1X) + itself * DYX;
	} else {
	  z[y][x] = near * (DYneg1Xneg1 + DYneg1X1 + DY1Xneg1 + DY1X1 + DYXneg1 + DYX1 + DYneg1X + DY1X) + itself * DYX;
	}
      }
      ZY[x]/=100;
    }
  }
}
						
void Func4(int b[], int a[])
{
	int chuck_size=MSIZE;	 
	int array_size=VSIZE/chuck_size;
	int chuck[chuck_size];
    int i, j;

    omp_set_nested(1);
#pragma omp parallel for num_threads(30) private(i,j)
	for(j=0; j<chuck_size; j++) {
		b[j*array_size]=a[j*array_size];
		int temp = j*array_size;
		int temp1 = temp-1;
		for (i=1; i<VSIZE/chuck_size; i++) {
			b[temp+i]=b[temp1+i]+a[temp+i];
		}
		chuck[j]=b[(j+1)*array_size-1];
	}

	for(j=1; j<chuck_size; j++) {
		chuck[j]=chuck[j-1]+chuck[j];
	}
	omp_set_nested(1);
#pragma omp parallel for num_threads(30) private(i,j)

	
	for(j=1; j<chuck_size; j++)
	  {
	    int temp = chuck[j-1]/(j+1);
	    int temp2 = j*array_size;
		for (i=0; i<array_size; i++)
		  {
			b[temp2+i]+=temp;
		}
	}
}

void Func5(int b[], int a[])
{
	int i=0, j, stride, stride2, step;
	int temp, temp2;
	long log_N=log2(VSIZE);
	omp_set_nested(1);
#pragma omp parallel for num_threads(30) private(i,j)
	
	for(j=0; j<VSIZE; j+=2)
	  {
	    int AJ = a[j];
	    b[j]=AJ;
	    b[j+1] = AJ + a[j+1];
	  }

	for(i=4; i<VSIZE; i*=2)
	  {
	    int ineg1 = i-1;
	    int idiv2neg1 = i/2-1;
		for(j=0; j<VSIZE; j+=i)
		  {
				b[j+ineg1] += b[j+idiv2neg1];
		  }
	  }
	b[VSIZE-1]=0;

	for(i=(log_N-1); i>=0; i--)
	  {
		stride2=(2<<i)-1;
		stride=(1<<i)-1;
		step=stride2+1;

		int pow1 = (pow(2, i))-1;
		int pow2 = (pow(2, i+1))-1;
		int pow3 = pow2+1;

		
		for(j=0; j<VSIZE; j+=pow3)
		  {
		    temp=b[j+pow1];
		    temp2=b[j+pow2];
		    b[j+pow1] = temp2;
		    b[j+pow2] = temp+temp2;
		  }
	  }
}
