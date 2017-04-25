FILE=3D_spatial_network
ITERATIONS=1000
CLUSTERS=3
rm randInit_$FILE.out
rm firstBlock_$FILE.out
rm blockAverage_$FILE.out
for i in `seq 1 $ITERATIONS`; do ./extKmeansRandInit  ../data/$FILE.csv $CLUSTERS >> randInit_$FILE.out ; done
for i in `seq 1 $ITERATIONS`; do ./extKmeansClusterFirstBlockInit ../data/$FILE.csv $CLUSTERS >> firstBlock_$FILE.out ; done
for i in `seq 1 $ITERATIONS`; do ./extKmeansClusterBlocksAveraging ../data/$FILE.csv $CLUSTERS >> blockAverage_$FILE.out ; done
