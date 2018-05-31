# Tiny Warehouse
__Description__:mini application used for keeping information about supermarket customers and posing queries to extract information.

### Compilation
There is a Makefile available for automatic compilation:
```sh
$ make
```
After compilation a  `twa`  executable is being created
### Execution
The program can be executed in the following way:
```sh
$./twa -b <bucket_entries> -f <load_factor> -l <data_file> -p <operation_file>
```
* __bucket_entry__: (necessary) number of records that can be stored in one bucket of the hashtable.
* __load_factor__: (necessary) percentage of how full the hash table is allowed to get.
* __data_file__: (optional) input file that contains records to be inserted in the hashtable. Each line represents one record and should be in the following format: 
``` <customer_id> <first_name> <last_name> <street> <number> <city> <post_code> <amount> ``` 
* __operation_file__: (optional) input file containing commands to be applied on the structures and data. Each line represents one of the commands explained below.

### User menu
The program begings creating all the necessary structures, inserting records of load_file (if given) in them and executing commands of operarion_file (if given). After that, the program waits for user's command. The user may type one of the following commands:
* ``> l file_name`` or ``> load file_name`` : Adds records of file in the hashtable.
*  `> t N` or `> top N`  : displays top N records in hashtable.
*  `> b N` or `> bottom N`: displays bottom N records in hashtable.
*  `> a` or `> average` : displays the average amount of money among all records.
*  `> ο file_name` or `> operarions file_name`: executes commands found in file given.
*  `> p ID` or `> percentile ID`: displays the percentage of records that have less amount of money than the record with id ID.
*  `> q ID` or `> query ID`: displays all information for record with id ID.
*  `> r ID1 ID2` or `> range ID1 ID2`: displays all records with amount between that of user with id ID1 and the one of user with id ID2 in ascending order.
*  `> i ID last_name first_name street number p_code city amount` or `> insert ID last_name first_name street number p_code city amount`: creates one new record with the details given.
*  `> e` or `> exit`: exits application. 

### Input Files Available
* __recs150.txt__: sample input file with 150 records
* __ops_recs150_20__: sample operations file for records in recs150.txt file.
