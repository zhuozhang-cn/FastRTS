# FastRTS
## Part 1: Experimental Environment

All of the experiments were conducted on a machine equipped with an Intel Xeon(R) W-2145 CPU @ 3.70GHz and 100GB RAM running Ubuntu 20.04.3.

Please compile the source code by gcc 9.3.0 with flag O3 turned on. A compiled program has been placed in the "build" folder.  

## Part 2: DataGenerator

Options:

-qsn {integer} number of queries when starting the system  
-qin {integer} number of queries to be inserted later  
-lf {string}  file generate location  
-s   {float}   scale of query  
-dp  {float}   proportion of queries that live to maturity  
-U   {integer} universe size  
-t   {integer} threshold of query  
-w   {integer} mean value of weight of element  
-d   {integer} dimension of data space  
-real{0 or 1}  synthetic or real dataset  
-rf {string}  real data file location (optional)  



For more information, please refer to ./DataGenerator/start.sh  
This bash script can generate command lines in batches.  

The format of generated file is as follows:  
-First line: dimension qsn pin qin qdn U  
-The following qsn lines: query_id query_range (2\*dim) threshold  
-The following lines: op_id op_content  
----op_id:1 insert_element_id element_point(dim) weight  
----op_id:2 insert_query_id query_range(2\*dim) threshold  
----op_id:3 delete_query_id  
----op_id:-1 the end of the file


About real data file:

Please down load real data file from https://drive.google.com/file/d/19_7bgVfCUnaWDi85acDpHovp9J6yReDT/view?usp=sharing  and put the file in the current fold ./

This file is obtained by preprocessing the transaction snapshot records of two stocks. Each line, the information includes: line_number, bid_bolume, bid_price_stock1, bid_price_stock2, average_price_stock1, average_price_stock2.   

The original transaction snapshot data is here:  
https://drive.google.com/file/d/19ZxP8mrtbY9rCrlxfK716AO-m36im50y/view?usp=sharing  
https://drive.google.com/file/d/1kbMdPZ2lsQ31ueTzpE2OpiBhitZ4nV6J/view?usp=sharing  


## Part 3: Our Module

The compiled file is placed in the “build” folder of the corresponding model.  
When running the program, the command line passes in at least two parameters: data file location (generated by datagenerator) and target file location (save the time of query maturity)  
If necessary, specify the approximation parameter epsilon with the third parameter (default value is 0.05).  

For more information, please refer to ./FastRTS/build/start.sh  
This bash script can generate command lines in batches.

The program records the running time by logging the system time, and obtains the memory consumption by reading the system file.

When the program finishes, useful information will be logged at result.txt


