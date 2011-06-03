Problem:    gt10
Rows:       22
Columns:    5 (5 integer, 5 binary)
Non-zeros:  58
Status:     INTEGER OPTIMAL
Objective:  objetivo = 1 (MINimum)

   No.   Row name        Activity     Lower bound   Upper bound
------ ------------    ------------- ------------- -------------
     1 vizinhoA[1,2,4]
                                   0                           1 
     2 vizinhoA[1,2,3]
                                   1                           1 
     3 vizinhoA[1,4,2]
                                   0                           1 
     4 vizinhoA[1,4,3]
                                   1                           1 
     5 vizinhoA[1,3,2]
                                   1                           1 
     6 vizinhoA[1,3,4]
                                   1                           1 
     7 vizinhoB[2,3,1]
                                   1                           1 
     8 vizinhoB[3,4,1]
                                   0                           1 
     9 vizinhoB[1,4,3]
                                   0                           1 
    10 vizinhoB[1,3,2]
                                   1                           1 
    11 vizinhoC[2,3,1]
                                   0                           1 
    12 vizinhoC[3,4,2]
                                   0                           1 
    13 vizinhoC[3,4,1]
                                   1                           1 
    14 vizinhoD[1,2,3]
                                   0                           1 
    15 vizinhoD[2,3,4]
                                   0                           1 
    16 vizinhoD[1,3,4]
                                   1                           1 
    17 maximal[1,2]                1             1               
    18 maximal[2,3]                1             1               
    19 maximal[3,4]                1             1               
    20 maximal[1,4]                1             1               
    21 maximal[1,3]                3             1               
    22 objetivo                    1                             

   No. Column name       Activity     Lower bound   Upper bound
------ ------------    ------------- ------------- -------------
     1 x[1,4]       *              0             0             1 
     2 x[1,2]       *              0             0             1 
     3 x[1,3]       *              1             0             1 
     4 x[2,3]       *              0             0             1 
     5 x[3,4]       *              0             0             1 

Integer feasibility conditions:

KKT.PE: max.abs.err = 0.00e+00 on row 0
        max.rel.err = 0.00e+00 on row 0
        High quality

KKT.PB: max.abs.err = 0.00e+00 on row 0
        max.rel.err = 0.00e+00 on row 0
        High quality

End of output
