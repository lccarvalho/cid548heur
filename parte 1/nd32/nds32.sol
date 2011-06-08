Problem:    nd32
Rows:       16
Columns:    10 (10 integer, 0 binary)
Non-zeros:  37
Status:     INTEGER OPTIMAL
Objective:  objetivo = 56 (MINimum)

   No.   Row name        Activity     Lower bound   Upper bound
------ ------------    ------------- ------------- -------------
     1 capacidade[1,2]
                                   2                           4 
     2 capacidade[1,3]
                                   8                          10 
     3 capacidade[1,6]
                                   0                          10 
     4 capacidade[2,4]
                                   2                           4 
     5 capacidade[3,4]
                                   4                           4 
     6 capacidade[3,5]
                                   4                           6 
     7 capacidade[4,2]
                                   0                           2 
     8 capacidade[4,6]
                                   6                           8 
     9 capacidade[5,4]
                                   0                           2 
    10 capacidade[5,6]
                                   4                           4 
    11 inout[2]                    0            -0             = 
    12 inout[3]                    0            -0             = 
    13 inout[4]                    0            -0             = 
    14 inout[5]                    0            -0             = 
    15 demanda                    10            10               
    16 objetivo                   56                             

   No. Column name       Activity     Lower bound   Upper bound
------ ------------    ------------- ------------- -------------
     1 flow[1,2]    *              2             0               
     2 flow[1,3]    *              8             0               
     3 flow[1,6]    *              0             0               
     4 flow[2,4]    *              2             0               
     5 flow[3,4]    *              4             0               
     6 flow[3,5]    *              4             0               
     7 flow[4,2]    *              0             0               
     8 flow[4,6]    *              6             0               
     9 flow[5,4]    *              0             0               
    10 flow[5,6]    *              4             0               

Integer feasibility conditions:

KKT.PE: max.abs.err = 0.00e+00 on row 0
        max.rel.err = 0.00e+00 on row 0
        High quality

KKT.PB: max.abs.err = 0.00e+00 on row 0
        max.rel.err = 0.00e+00 on row 0
        High quality

End of output
