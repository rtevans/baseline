#include <stdio.h>
#include <stdlib.h>

// 25% is max

// IVB branch_miss%    m
//     24.83           100000
//     20.73           10000  
//     0.12            1000
//     0.00            100         

// SKX branch_miss%    m
//     24.95           100000
//     20.64           10000 
//     0.27%           1000
//     0.00%           100

// KNL branch_miss%    m
//     24.92           100000 
//     25.08           10000
//     23.27           1000
//     13.56           100
//     13.14           75
//     7.18            50
//     1.52            20
//     0.07            10

int main(int argc, char *argv[])
{

  int n = 1000000000;
  double c = 0;

  int m = atoi(argv[1]);
  int A[m];

  int j;
  for (j = 0; j < m; j++) {
    A[j]=rand()%2;
  }
  
  while (n--) {
    if (A[n%m] == 0)
      c++;
    else
      c--;
  }
  
  fprintf(stderr, "%f\n", c);
  printf("branch length %d\n", m);
  return 0;
}
