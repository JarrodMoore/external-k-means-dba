constants = -DmemBlock='1024'
all: randInit clusterFirstBlock clusterBlocksAveraging
randInit: externalKmeans-RandInit.c
	gcc -O2 -Wall -pedantic $(constants) externalKmeans-RandInit.c -o extKmeansRandInit -lm
clusterFirstBlock: externalKmeans-ClusterFirstBlockInit.c
	gcc -O2 -Wall -pedantic $(constants) externalKmeans-ClusterFirstBlockInit.c -o extKmeansClusterFirstBlockInit -lm
clusterBlocksAveraging: externalKmeans-ClusterBlocksAveraging.c
	gcc -O2 -Wall -pedantic $(constants) externalKmeans-ClusterBlocksAveraging.c -o extKmeansClusterBlocksAveraging -lm
