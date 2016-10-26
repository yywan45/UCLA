// It compiles only with gcc 4.8

#include "func.h"
#include <stdio.h>
#define NUM_THREADS 20
#define BSIZE 16
#define VMSIZE 4096 //(VSIZE/MSIZE) for fun4
#define PART 4 // partition dimension for func2
#define MBSIZE 64 //((MSIZE/PART)/BSIZE) for func2

// split into # of tasks, L3 cache 25M
#define T1 20
#define T2 20
#define T3 120
#define T4 120
#define T5 120

// tasks have task_first and task_last variations to avoid conditional

void task1(int c[][TSIZE], int a[][TSIZE], int b[][TSIZE], int current_job, int ele_to_work)
{
  int i, j, k;
  // calculate starting and ending elements
  int first_ele = current_job*ele_to_work;
  int last_ele = first_ele+ele_to_work;
  // convert to starting and ending indices
  int first_i = first_ele/TSIZE;
  int last_i = last_ele/TSIZE;
  int first_k = first_ele - first_i*TSIZE;
  int last_k = last_ele - last_i*TSIZE;

  /* row major multiplication 
     -task can start and end anywhere
     -need to account for first and last */

  // first row
  i = first_i;
  for (k=first_k; k<TSIZE; k++)
    for (j=0; j<TSIZE; j++)
      c[i][j] += a[i][k] * b[k][j];

  // until before last row
  for (i=first_i+1; i<last_i; i++)
    for (k=0; k<TSIZE; k++)
      for (j=0; j<TSIZE; j++)
	c[i][j] += a[i][k] * b[k][j];

  // last row
  for (k=0; k<last_k; k++)
    for (j=0; j<TSIZE; j++)
      c[i][j] += a[i][k] * b[k][j];
}

void task1_last(int c[][TSIZE], int a[][TSIZE], int b[][TSIZE], int current_job, int ele_to_work)
{
  int i, j, k;
  // calculate starting element
  int first_ele = current_job*ele_to_work;
  int last_ele = first_ele+ele_to_work;
  // convert to starting indices
  int first_i = first_ele/TSIZE;
  int first_k = first_ele - first_i*TSIZE;

  /* row major multiplication 
     -task can start anywhere
     -need to account for first */

  // first row
  i = first_i;
  for (k=first_k; k<TSIZE; k++)
    for (j=0; j<TSIZE; j++)
      c[i][j] += a[i][k] * b[k][j];

  // until last row
  for (i=first_i+1; i<TSIZE; i++)
    for (k=0; k<TSIZE; k++)
      for (j=0; j<TSIZE; j++)
	c[i][j] += a[i][k] * b[k][j];
}
    
void Func1(int c[][TSIZE], int a[][TSIZE], int b[][TSIZE])
{
  omp_set_nested(1);
  omp_set_num_threads(NUM_THREADS);
  
#pragma omp parallel
#pragma omp single
  {
    // task id, elements per task
    int ele_to_work=1+(TSIZE*TSIZE-1)/(T1);
    int current_job;

    // create task in loop
    for(current_job = 0; current_job < T1-1; current_job++)
#pragma omp task
      task1(c, a, b, current_job, ele_to_work);

    // last task
#pragma omp task
    task1_last(c, a, b, current_job, ele_to_work);
  }
}

void task2(int d[][MSIZE], int c[][MSIZE], int current_job,  int b_to_work, int p_i, int p_j)
{
  int i, j, bi, bj;
  // calculate starting and ending blocks
  int first_b_ele = current_job*b_to_work;
  int last_b_ele = first_b_ele+b_to_work;
  // convert to starting and ending block indices
  int first_bi =  first_b_ele/MBSIZE;
  int last_bi = last_b_ele/MBSIZE;
  int first_bj = first_b_ele - first_bi*MBSIZE;
  int last_bj = last_b_ele - last_bi*MBSIZE;
  // offset by partition indices
  p_i = p_i*(4096/PART);
  p_j = p_j*(4096/PART);

  /* transpose with blocking
     i, j are indices inside each block.
     bi, bj are indices of a block.
     -task can start and end anywhere
     -need to account for first and last */

  // first block
  bi=first_bi;
  for(bj=first_bj; bj<MBSIZE; bj++)
    for(i=0; i<BSIZE; i++) 
      for(j=0; j<BSIZE; j++)
	d[p_i+bi*BSIZE+i][p_j+bj*BSIZE+j]=c[p_j+bj*BSIZE+j][p_i+bi*BSIZE+i];

  // until before last block
  for(bi=first_bi+1; bi<last_bi; bi++)
    for(bj=0; bj<MBSIZE; bj++)
      for(i=0; i<BSIZE; i++) 
	for(j=0; j<BSIZE; j++)
	  d[p_i+bi*BSIZE+i][p_j+bj*BSIZE+j]=c[p_j+bj*BSIZE+j][p_i+bi*BSIZE+i];

  // last block
  for(bj=0; bj<last_bj; bj++)
    for(i=0; i<BSIZE; i++) 
      for(j=0; j<BSIZE; j++)
	d[p_i+bi*BSIZE+i][p_j+bj*BSIZE+j]=c[p_j+bj*BSIZE+j][p_i+bi*BSIZE+i];
}

void task2_last(int d[][MSIZE], int c[][MSIZE], int current_job,  int b_to_work, int p_i, int p_j)
{
  int i, j, bi, bj;
  // calculate starting and ending blocks
  int first_b_ele = current_job*b_to_work;
  int last_b_ele = first_b_ele+b_to_work;
  // convert to starting and ending block indices
  int first_bi =  first_b_ele/MBSIZE;
  int first_bj = first_b_ele - first_bi*MBSIZE;
  // offset by partition indices
  p_i = p_i*(4096/PART);
  p_j = p_j*(4096/PART);

  /* transpose with blocking
     i, j are indices inside each block.
     bi, bj are indices of a block.
     -task can start anywhere
     -need to account for first */

  // first block
  bi=first_bi;
  for(bj=first_bj; bj<MBSIZE; bj++)
    for(i=0; i<BSIZE; i++) 
      for(j=0; j<BSIZE; j++)
	d[p_i+bi*BSIZE+i][p_j+bj*BSIZE+j]=c[p_j+bj*BSIZE+j][p_i+bi*BSIZE+i];

  // until last block
  for(bi=first_bi+1; bi<MBSIZE; bi++)
    for(bj=0; bj<MBSIZE; bj++)
      for(i=0; i<BSIZE; i++) 
	for(j=0; j<BSIZE; j++)
	  d[p_i+bi*BSIZE+i][p_j+bj*BSIZE+j]=c[p_j+bj*BSIZE+j][p_i+bi*BSIZE+i];
}

void Func2(int d[][MSIZE], int c[][MSIZE])
{
  omp_set_nested(1);
  omp_set_num_threads(NUM_THREADS);

#pragma omp parallel
#pragma omp single
  {
    // task id, blocks per task
    int b_to_work=1+((MBSIZE*MBSIZE)-1)/(T2);
    int current_job;
    /* break matrix into partitions */ 
    // partition index p_i, p_j
    int p_i, p_j;

    // for each partition
    for(p_i = 0; p_i < PART; p_i++)
      for(p_j = 0; p_j < PART; p_j++) {
	
	// create task in loop
	for(current_job = 0; current_job < T2-1; current_job++)
#pragma omp task
	  task2(d, c, current_job, b_to_work, p_i, p_j);

	// last task
#pragma omp task
	task2_last(d, c, current_job, b_to_work, p_i, p_j);
	
      }
  }
}

void task3_first(int z[][MSIZE], int d[][MSIZE], int ele_to_work)
{
  int y, x;
  // calculate ending element
  int last_ele = ele_to_work;
  // convert to ending indices
  int last_y = last_ele/MSIZE;
  int last_x = last_ele - last_y*MSIZE;
  
  /* iterate and sum
     account for first row
     account for first and last elements of a row
     -task can end anywhere
     -need to account for last */

  // first row
  y=0;
  x=0;
  z[y][x]=(45*d[y][x]+2*d[y][x+1]+2*d[y+1][x]+d[y+1][x+1])/50;
  for (x=1; x<MSIZE-1; x++)
    z[y][x]=(2*d[y][x-1]+43*d[y][x]+2*d[y][x+1]+d[y+1][x-1]+d[y+1][x]+d[y+1][x+1])/50;
  z[y][x]=(2*d[y][x-1]+45*d[y][x]+d[y+1][x-1]+2*d[y+1][x])/50;

  // until before last ele
  for(y=1; y<last_y; y++)
    {
      x=0;
      z[y][x]=(2*d[y-1][x]+d[y-1][x+1]+43*d[y][x]+d[y][x+1]+2*d[y+1][x]+d[y+1][x+1])/50;
      for (x=1; x<MSIZE-1; x++)
	z[y][x]=(d[y-1][x-1]+d[y-1][x]+d[y-1][x+1]+d[y][x-1]+42*d[y][x]+d[y][x+1]+d[y+1][x-1]+d[y+1][x]+d[y+1][x+1])/50;
      z[y][x]=(d[y-1][x-1]+2*d[y-1][x]+d[y][x-1]+43*d[y][x]+d[y+1][x-1]+2*d[y+1][x])/50;
    }

  // last ele
  x=0;
  z[y][x]=(2*d[y-1][x]+d[y-1][x+1]+43*d[y][x]+d[y][x+1]+2*d[y+1][x]+d[y+1][x+1])/50;
  for (x=1; x<last_x; x++)
    z[y][x]=(d[y-1][x-1]+d[y-1][x]+d[y-1][x+1]+d[y][x-1]+42*d[y][x]+d[y][x+1]+d[y+1][x-1]+d[y+1][x]+d[y+1][x+1])/50;
}
		 
void task3(int z[][MSIZE], int d[][MSIZE], int current_job, int ele_to_work)
{
  int y, x;
  // calculate starting and ending elements
  int first_ele = current_job*ele_to_work;
  int last_ele = first_ele+ele_to_work;
  // convert to starting and ending indices
  int first_y = first_ele/MSIZE;
  int last_y = last_ele/MSIZE;
  int first_x = first_ele - first_y*MSIZE;
  int last_x = last_ele - last_y*MSIZE;

  /* iterate and sum
     account for first and last elements of a row
     -task can start and end anywhere
     -need to account for first and last */

  // first ele
  y=first_y;
  for (x=first_x; x<MSIZE-1; x++)
    z[y][x]=(d[y-1][x-1]+d[y-1][x]+d[y-1][x+1]+d[y][x-1]+42*d[y][x]+d[y][x+1]+d[y+1][x-1]+d[y+1][x]+d[y+1][x+1])/50;
  z[y][x]=(d[y-1][x-1]+2*d[y-1][x]+d[y][x-1]+43*d[y][x]+d[y+1][x-1]+2*d[y+1][x])/50;

  // until before last ele
  for (y=first_y+1; y<last_y; y++)
    {
      x=0;
      z[y][x]=(2*d[y-1][x]+d[y-1][x+1]+43*d[y][x]+d[y][x+1]+2*d[y+1][x]+d[y+1][x+1])/50;
      for (x=1; x<MSIZE-1; x++)
	z[y][x]=(d[y-1][x-1]+d[y-1][x]+d[y-1][x+1]+d[y][x-1]+42*d[y][x]+d[y][x+1]+d[y+1][x-1]+d[y+1][x]+d[y+1][x+1])/50;
      z[y][x]=(d[y-1][x-1]+2*d[y-1][x]+d[y][x-1]+43*d[y][x]+d[y+1][x-1]+2*d[y+1][x])/50;
    }

  // last ele
  x=0;
  z[y][x]=(2*d[y-1][x]+d[y-1][x+1]+43*d[y][x]+d[y][x+1]+2*d[y+1][x]+d[y+1][x+1])/50;
  for (x=1; x<last_x; x++)
    z[y][x]=(d[y-1][x-1]+d[y-1][x]+d[y-1][x+1]+d[y][x-1]+42*d[y][x]+d[y][x+1]+d[y+1][x-1]+d[y+1][x]+d[y+1][x+1])/50;
}
		 
void task3_last(int z[][MSIZE], int d[][MSIZE], int current_job, int ele_to_work)
{
  int y, x;
  // calculate starting elements
  int first_ele = current_job*ele_to_work;
  // convert to starting indices
  int first_y = first_ele/MSIZE;
  int first_x = first_ele - first_y*MSIZE;

  // first ele
  y=first_y;
  for (x=first_x; x<MSIZE-1; x++)
    z[y][x]=(d[y-1][x-1]+d[y-1][x]+d[y-1][x+1]+d[y][x-1]+42*d[y][x]+d[y][x+1]+d[y+1][x-1]+d[y+1][x]+d[y+1][x+1])/50;
  z[y][x]=(d[y-1][x-1]+2*d[y-1][x]+d[y][x-1]+43*d[y][x]+d[y+1][x-1]+2*d[y+1][x])/50;

  // until last row
  for (y=first_y+1; y<MSIZE-1; y++)
    {
      x=0;
      z[y][x]=(2*d[y-1][x]+d[y-1][x+1]+43*d[y][x]+d[y][x+1]+2*d[y+1][x]+d[y+1][x+1])/50;
      for (x=1; x<MSIZE-1; x++)
	z[y][x]=(d[y-1][x-1]+d[y-1][x]+d[y-1][x+1]+d[y][x-1]+42*d[y][x]+d[y][x+1]+d[y+1][x-1]+d[y+1][x]+d[y+1][x+1])/50;
      z[y][x]=(d[y-1][x-1]+2*d[y-1][x]+d[y][x-1]+43*d[y][x]+d[y+1][x-1]+2*d[y+1][x])/50;
    }

  // last row
  x=0;
  z[y][x]=(2*d[y-1][x]+d[y-1][x+1]+45*d[y][x]+2*d[y][x+1])/50;
  for (x=1; x<MSIZE-1; x++)
    z[y][x]=(d[y-1][x-1]+d[y-1][x]+d[y-1][x+1]+2*d[y][x-1]+43*d[y][x]+2*d[y][x+1])/50;
  z[y][x]=(d[y-1][x-1]+2*d[y-1][x]+2*d[y][x-1]+45*d[y][x])/50;
}
		 
void Func3(int z[][MSIZE], int d[][MSIZE])
{
  omp_set_nested(1);
  omp_set_num_threads(NUM_THREADS);
  
#pragma omp parallel
#pragma omp single
  {
    // task id, elements per task
    int ele_to_work=1+(MSIZE*MSIZE-1)/(T3);
    int current_job;

    // first task
#pragma omp task
    task3_first(z, d, ele_to_work);

    // create task in loop
    for(current_job = 1; current_job < T3-1; current_job++)
#pragma omp task
      task3(z, d, current_job, ele_to_work);

    // last task
#pragma omp task
    task3_last(z, d, current_job, ele_to_work);
  }
}

void task4_first(int b[], int a[], int rows_to_work, int *flag, int *sum)
{
  int j, i, tmp=0, tmp2=0;
  // calculate ending row
  int last_row = rows_to_work;

  /*
    1. sum each row, and also sum all rows
    3. pass total sum to next task
    4. increment each row with the right sum
    (sum of each row is its last element)
  */

  // 1
  for(j=0; j<last_row; j++) {
    b[j*VMSIZE] = a[j*VMSIZE];
    for(i=1; i<VMSIZE-1; i++)
      b[j*VMSIZE+i] = b[j*VMSIZE+i-1]+a[j*VMSIZE+i];
    b[j*VMSIZE+i] = b[j*VMSIZE+i-1]+a[j*VMSIZE+i];
    tmp += b[j*VMSIZE+i]; // sum of rows in this task
  }

  // 3
  (*sum) += tmp; // add to total sum
  (*flag)++; // signal next task

  // 4
  tmp2 = b[VMSIZE-1];
  tmp = tmp2/2;
  for(j = 1; j<last_row; j++) {
    for(i = 0; i<VMSIZE-1; i++)
      b[j*MSIZE+i]+=tmp;
    tmp2 += b[j*MSIZE+i]; // re-sum of rows in this task
    b[j*MSIZE+i] += tmp;
    tmp = tmp2/(j+2);
  }
}

void task4(int b[], int a[], int current_job, int rows_to_work, int *flag, int *sum)
{
  int j, i, tmp=0, tmp2=0, flg_tmp=0, sum_tmp=0;
  // calculate starting and ending row
  int first_row = current_job*rows_to_work;
  int last_row = first_row+rows_to_work;

  /*
    1. sum each row, and also sum all rows
    2. wait for prev task to sum prev rows
    3. pass total sum to next task
    4. increment each row with the right sum
    (sum of each row is its last element)
  */

  // 1
  for(j=first_row; j<last_row; j++) {
    b[j*VMSIZE] = a[j*VMSIZE];
    for(i=1; i<VMSIZE-1; i++)
      b[j*VMSIZE+i] = b[j*VMSIZE+i-1]+a[j*VMSIZE+i];
    b[j*VMSIZE+i] = b[j*VMSIZE+i-1]+a[j*VMSIZE+i];
    tmp += b[j*VMSIZE+i]; // sum of rows in this task
  }
  
  // 2
  while (flg_tmp != current_job) {
#pragma omp atomic read
    flg_tmp= (*flag); // wait for prev tasks to finish step 1-3
  }
  
  // 3
  sum_tmp = (*sum); // read sum from prev task
  (*sum) += tmp; // add to total sum from prev tasks
  (*flag)++; // signal next task

  // 4
  tmp = (sum_tmp)/(first_row+1);
  for(j = first_row; j<last_row; j++) {
    for(i = 0; i<VMSIZE-1; i++)
      b[j*MSIZE+i]+=tmp;
    tmp2 += b[j*MSIZE+i]; // re-sum of rows in this task
    b[j*MSIZE+i] += tmp;
    tmp = (sum_tmp+tmp2)/(j+2);
  }
}

void task4_last(int b[], int a[], int current_job, int rows_to_work, int *flag, int *sum)
{
  int j, i, tmp=0, tmp2=0, flg_tmp=0, sum_tmp=0;
  // calculate starting row
  int first_row = current_job*rows_to_work;

  /*
    1. sum each row, and also sum all rows
    2. wait for prev task to sum prev rows
    4. increment each row with the right sum
    (sum of each row is its last element)
  */

  // 1
  for(j=first_row; j<MSIZE; j++) {
    b[j*VMSIZE] = a[j*VMSIZE];
    for(i=1; i<VMSIZE; i++)
      b[j*VMSIZE+i] = b[j*VMSIZE+i-1]+a[j*VMSIZE+i];
  }

  // 2
  while (flg_tmp != current_job) {
#pragma omp atomic read
    flg_tmp= (*flag); // wait for prev tasks to finish step 1-3
  }
  sum_tmp = (*sum); // read sum from prev task

  // 4
  tmp = (sum_tmp)/(first_row+1);
  for(j = first_row; j<MSIZE; j++) {
    for(i = 0; i<VMSIZE-1; i++)
      b[j*MSIZE+i]+=tmp;
    tmp2 += b[j*MSIZE+i]; // re-sum of rows in this task
    b[j*MSIZE+i] += tmp;
    tmp = (sum_tmp+tmp2)/(j+2);
  }
}

void Func4(int b[], int a[])
{
  omp_set_nested(1);
  omp_set_num_threads(NUM_THREADS);

#pragma omp parallel
#pragma omp single
  {
    // task id, rows per task
    int rows_to_work=1+(MSIZE-1)/(T4);
    int current_job;
     // shared data for sync
    int flag=0, sum=0;

    // first task
#pragma omp task shared(flag, sum)
    task4_first(b, a, rows_to_work, &flag, &sum);

    // create task in loop
    for(current_job = 1; current_job < T4-1; current_job++)
#pragma omp task shared(flag, sum)
      task4(b, a, current_job, rows_to_work, &flag, &sum);

    // last task
#pragma omp task shared(flag, sum)
    task4_last(b, a, current_job, rows_to_work, &flag, &sum);
  }
}

void task5_first(int b[], int a[], int ele_to_work, int *flag, int *sum)
{
  int j;
  // calculate ending element
  int last_ele = ele_to_work;

  /*
    1. increment each element with sum from prev elements in this task
    3. pass total sum to next task
   */

  // 1
  b[0]=0;
  for(j=1; j<last_ele; j++)
    b[j]=b[j-1]+a[j-1];

  // 3
  (*sum) += b[j-1]; // add to total sum
  (*flag)++; // signal next task
}

void task5(int b[], int a[], int current_job, int ele_to_work, int *flag, int *sum)
{
  int j, flg_tmp=0, sum_tmp=0;
  // calculate starting and ending element
  int first_ele = current_job*ele_to_work;
  int last_ele = first_ele+ele_to_work;

  /*
    1. increment each element with sum from prev elements in this task
    2. wait for prev task to sum prev elements
    3. pass total sum to next task
    4. increment each element with sum from prev elements in prev task
  */

  // 1
  b[first_ele]=a[first_ele-1];
  for(j=first_ele+1; j<last_ele; j++)
    b[j]=b[j-1]+a[j-1];

  // 2
  while (flg_tmp != current_job) {
#pragma omp atomic read
    flg_tmp= (*flag); // wait for prev task to finish step 1-3
  }
  // 3
  sum_tmp = (*sum); // read sum from prev task
  (*sum) += b[j-1]; // add to total sum from prev tasks
  (*flag)++; // signal next task

  // 4
  for(j=first_ele; j<last_ele; j++)
    b[j]+=sum_tmp;  
}

void task5_last(int b[], int a[], int current_job, int ele_to_work, int *flag, int *sum)
{
  int j, flg_tmp=0, sum_tmp=0;
  // calculate ending element
  int first_ele = current_job*ele_to_work;
  
  /*
    1. increment each element with sum from prev elements in this task
    2. wait for prev task to sum prev elements
    4. increment each element with sum from prev elements in prev task
  */

  // 1
  b[first_ele]=a[first_ele-1];
  for(j=first_ele+1; j<VSIZE; j++)
    b[j]=b[j-1]+a[j-1];

  // 2
  while (flg_tmp != current_job) {
#pragma omp atomic read
    flg_tmp= (*flag); // wait for prev tasks to finish step 1-3
  }
  sum_tmp = (*sum); // read sum from prev task

  // 4
  for(j=first_ele; j<VSIZE; j++)
    b[j]+=sum_tmp;  
}

void Func5(int b[], int a[])
{
  omp_set_nested(1);
  omp_set_num_threads(NUM_THREADS);

#pragma omp parallel
#pragma omp single
  {
    // task id, elements per task
    int ele_to_work=1+(MSIZE*MSIZE-1)/(T5);
    int current_job;
    // shared data for sync
    int flag=0, sum=0;

    // first task
#pragma omp task shared(flag, sum)
    task5_first(b, a, ele_to_work, &flag, &sum);

    // create task in loop
    for(current_job = 1; current_job < T5-1; current_job++)
#pragma omp task shared(flag, sum)
      task5(b, a, current_job, ele_to_work, &flag, &sum);

    // last task
#pragma omp task shared(flag, sum)
    task5_last(b, a, current_job, ele_to_work, &flag, &sum);
  }
}

