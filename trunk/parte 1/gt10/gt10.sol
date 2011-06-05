Problem:    Unknown
Rows:       8
Columns:    3 (3 integer, 3 binary)
Non-zeros:  16
Status:     INTEGER OPTIMAL
Objective:  objetivo = 1 (MINimum)

   No.   Row name        Activity     Lower bound   Upper bound
------ ------------    ------------- ------------- -------------
     1 vizinhoA[1]                 0                           1 
     2 vizinhoA[2]                 1                           1 
     3 vizinhoA[3]                 1                           1 
     4 vizinhoA[4]                 0                           1 
     5 maximal[1,2]                1             1               
     6 maximal[2,3]                1             1               
     7 maximal[3,4]                1             1               
     8 objetivo                    1                             

   No. Column name       Activity     Lower bound   Upper bound
------ ------------    ------------- ------------- -------------
     1 x[1,2]       *              0             0             1 
     2 x[2,3]       *              1             0             1 
     3 x[3,4]       *              0             0             1 

Integer feasibility conditions:

KKT.PE: max.abs.err = 0.00e+00 on row 0
        max.rel.err = 0.00e+00 on row 0
        High quality

KKT.PB: max.abs.err = 0.00e+00 on row 0
        max.rel.err = 0.00e+00 on row 0
        High quality

End of output
