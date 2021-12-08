# WorldMetalTradeImplementation
Metal trade implementation around the world with a trade dataset by using hash table and graph.

## World Metal Trade Dataset
The dataset contains information on trade miscellaneous manufactures of metal among 80 countries in 1994. All countries with entries in the paper version of the Commodity Trade Statistics published by the United Nations were included, but for some countries, the 1993 data (Austria, Seychelles, Bangladesh, Croatia, and Barbados) or 1995 data (South Africa and Ecuador) were used because they were not available for 1994. You can read more about the dataset at [World Metal Trade Dataset](http://vlado.fmf.uni-lj.si/pub/networks/data/esna/metalWT.htm).

**Node** class represents countries with its additional information such as the country id as **vid**, country name as **country**, **continent**, and gross domestic product as **gdp**. 
**Edge** class represents imports by one country from another for the class of commodities designated
as 'miscellaneous manufactures of metal', which represents high technology products or heavy manufacture. The absolute value of imports (in 1000 US$) is used but imports with values less than 1% of the country's total imports were omitted. There are two fields in **Edge** class under **private** section. **tailNode** represents the country that imports the metal. **import** represents the import value. 
There is no self-loop within the dataset. Each field is separated by tab characters in the dataset.

## Hash Table Part
**HashTable** class implements the hash table structure. In this **hash table**, every possible place in the table is able to store at most 2 values, called a **Bucket**. **Bucket** structures have an array of 2 **Entry**s. If the **bucket** for a **key** is used, then your **hash table** should use the **standard open addressing (quadratic probing)** to find the next **bucket** for that **key**.

**HashUtils** class contains two **hash functions** for the keys of type **string** and **int** respectively, and a function for finding the **next appropriate size** for the hash table. Implementations of these functions can be found in *HashUtils.cpp*.

**Entry** struct is implemented in *HashTable.h*. Each **Entry** object stores information regarding a single key-value pair in the **hash table**. **Key** variable of type **K** should store the original **key** variable given in the **Insert** method of the entry. **Value** variables of type **T** store the value given in the **Insert** method. **Active** variable of type **bool** denotes that the key-value pair stored in this entry is valid. For example, the 0th entry of the 0th bucket in the example above is not valid, however, 0th entries of the 1st, 2nd, 3rd, and 4th bucket are valid and their **Active** fields are set as true. Initially, **Active** variable of each **Entry** object is set to **false**. **Deleted** variable stores whether this entry has been **deleted** before. Initially, every **Entry** object has this variable as **false**.

There are some hash table methods. Implementation and explanations of these methods are included in *HashTable.h*. Also, inspecting the details of **HashTable** class for a better understanding of the **Bucket** and **Entry** structures would be nice.

## Graph Part
**Graph** class constructs the trade representation. Each **vertex** represents a **country**, each **directed edge** represents **imports** by one country from another. The edge should be directed towards the country that imports the commodity. Each node has a country id, country name, continent, and gdp. Each edge has an import value in the US dollar which you should consider as the edge weight. The adjacency list of the graph is stored in a hash table as follows:
**HashTable< string; list < Edge >> adjList**. In this hash table, the country name of a node is used as a key, but since the country id's are also unique, the hash table could be:
**HashTable< int; list < Edge >> adjList**.
There are several methods such as the shortest path algorithm, cycle detection algorithm, BFS search algorithm in **Graph** class. Implementations and explanations of them can be found in *Graph.cpp*. Inspecting *Graph.h* would be nice for better understanding.


