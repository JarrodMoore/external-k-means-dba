# external-k-means-dba
Usage: \<executable\> \<data-file\> \<number of clusters\>

Data for the gatherData.sh script should be accessible from the local directory by ../data/<file-name>. All data should be stored as csv files.

Buffer size is set in the makefile as a makefile variable. The size may not exceed the size of the file analyzed.

Output from the gatherData.sh script should be scrubbed of centers (or center printing commented out) before buffer fill counts are processed by the python script. 

Require matplotlib and numpy to generate graphs.
