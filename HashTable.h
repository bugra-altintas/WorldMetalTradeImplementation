#ifndef __HASHTABLE__
#define __HASHTABLE__

#include "HashUtils.h"
#include "ItemNotFoundException.h"
#include <iostream>
using namespace std;
#define BUCKET_SIZE 2

template <class K, class T>
class HashTable {
    struct Entry {
        K Key;             // the key of the entry
        T Value;   // the value of the entry
        bool Deleted;        // flag indicating whether this entry is deleted
        bool Active;         // flag indicating whether this item is currently used

        Entry() : Key(), Value(), Deleted(false), Active(false) {}
    };

    struct Bucket {
        Entry entries[BUCKET_SIZE];
    };

    int _capacity; // indicates the total capacity of the table
    int _size; // indicates the total number of items in the table

    Bucket* _table; // the hash table

	int howIsBucket(Bucket &bucket,K key) const;
	

public:
    HashTable();
    HashTable(const HashTable<K, T>& rhs);
    HashTable<K, T>& operator=(const HashTable<K, T>& rhs);
    ~HashTable();
    void Insert(const K& key, const T& value);
    void Delete(const K& key);
    T& Get(const K& key) const;
    void Resize(int newCapacity);
    int Capacity() const;
    int Size() const;
    void getKeys(K* keys);
};

// Default constructor
template <class K, class T>
HashTable<K, T>::HashTable() {
	_capacity = NextCapacity(0);
	_size = 0;
	_table = new Bucket[_capacity];
}

// Copy constructor
template <class K, class T>
HashTable<K, T>::HashTable(const HashTable<K, T>& rhs) {
	_capacity = rhs.Capacity();
	_size = rhs.Size();
	_table = new Bucket[_capacity];
	int i,j;
	for(i=0; i<_capacity; i++){
		for(j=0; j<BUCKET_SIZE; j++){
			if(rhs._table[i].entries[j].Active){
				_table[i].entries[j].Key = rhs._table[i].entries[j].Key;
				_table[i].entries[j].Value = rhs._table[i].entries[j].Value;
				_table[i].entries[j].Active = true;
			}
		}
	}
}
 // Assignment operator overloading
template <class K, class T>
HashTable<K, T>& HashTable<K, T>::operator=(const HashTable<K, T>& rhs) {
	if(this->Capacity() != rhs.Capacity()){
		this->_capacity = rhs.Capacity();
		delete [] this->_table;
		this->_table = new Bucket[_capacity];
	}
	this->_size = rhs.Size();
	int i,j;
	for(i=0; i<_capacity; i++){
		for(j=0; j<BUCKET_SIZE; j++){
			if(rhs._table[i].entries[j].Active){
				this->_table[i].entries[j].Key = rhs._table[i].entries[j].Key;
				this->_table[i].entries[j].Value = rhs._table[i].entries[j].Value;
				this->_table[i].entries[j].Active = true;
			}
		}
	}
	
}

// Destructor
template <class K, class T>
HashTable<K, T>::~HashTable() {
    delete [] _table;
}

/*A function inserts the Entry in the hash table with the given Key & Value pair.
If the given key already exists, it updates the existing one with given value.
If the load factor of the table is greater than 0.6, the function resizes the table with
the next prime number which can be obtained from NextCapacity function.
*/
template <class K, class T>
void HashTable<K, T>::Insert(const K& key, const T& value) { 
	int h = Hash(key) % _capacity;
	int first = h;
	int next = 1;
	int situation = 4;
	float load;
    while(true){
    	situation = howIsBucket(_table[h],key);
    	if(situation == 2 || situation == 3){//update
    	    _table[h].entries[situation-2].Value = value;
    	    return;
    	}
    	else if(situation == -1){//probe
    	    h = (first + next*next) % _capacity;
    	    next++;
    	    continue;
    	}
    	else if(situation == 0 || situation == 1){//insert
    	    load = float(_size+1)/float(_capacity);
    	    if(load > 0.6){
				Resize(NextCapacity(_capacity));
				h = Hash(key) % Capacity();
				first = h;
				next = 1;
    	        continue;
    	    }
    	    else{
        	    _table[h].entries[situation].Key = key;
        	    _table[h].entries[situation].Value = value;
        	    _table[h].entries[situation].Active = true;
        	    _size++;
        	    break;
    	    }
			
    	}
    	
    }
}

/* A function deletes the Entry with given key.
If given key does not exist in the hash table, it just throws a ItemNotFoundException().
*/
template <class K, class T>
void HashTable<K, T>::Delete(const K& key) {
   	int h = Hash(key) % Capacity();
	int first = h;
	int next = 1;
	int situation = 4;
	while(true){
	    situation = howIsBucket(_table[h],key);
	    if(situation == 2 || situation == 3){//found -> delete
	        _table[h].entries[situation-2].Active = false;
	        _table[h].entries[situation-2].Deleted = true;
	        _size--;
	        return;
	    }
	    else if(situation == -1){//probe
	        h = (first + next*next) % _capacity;
	        next++;
	        continue;
	    }
	    else if(situation == 0 || situation == 1){//empty or deleted
	        if(_table[h].entries[situation].Deleted){//deleted slot, this key may be deleted
	            if(_table[h].entries[situation].Key == key){//this key is deleted
	                throw ItemNotFoundException();
	            }
	            else{//probe
	                h = (first + next*next) % _capacity;
	                next++;
    	            continue;
	            }
	        }
	        else{//empty -> ItemNotFound
	            throw ItemNotFoundException();
	        }
	    }
	}
	
}

/* A function returns the value that corresponds to given key.
If the key does not exist, it throws ItemNotFoundException().
*/
template <class K, class T>
T& HashTable<K, T>::Get(const K& key) const {
	
	int h = Hash(key) % Capacity();
	int first = h;
	int next = 1;
	int situation = 4;
	while(true){
	    situation = howIsBucket(_table[h],key);
	    if(situation == 2 || situation == 3){//found
	        return _table[h].entries[situation-2].Value;
	    }
	    else if(situation == -1){//probe
	        h = (first + next*next) % _capacity;
	        next++;
	        continue;
	    }
	    else if(situation == 0 || situation == 1){//empty or deleted
	        if(_table[h].entries[situation].Deleted){//deleted slot, this key may be deleted
	            if(_table[h].entries[situation].Key == key){//this key is deleted
					throw ItemNotFoundException();
	            }
	            else{//probe
	                h = (first + next*next) % _capacity;
	                next++;
    	            continue;
	            }
	        }
	        else{//empty -> ItemNotFound
	            throw ItemNotFoundException();
	        }
	    }
	}
}

/*A function resizes the capacity of hash table. Whenever the capacity changes,
all entries are rehashed.
*/
template <class K, class T>
void HashTable<K, T>::Resize(int newCapacity) {
	Bucket *new_table = new Bucket[newCapacity];
	K activeKeys[Size()];
	getKeys(activeKeys);
	for(int i=0; i<Size();i++){
		int h = Hash(activeKeys[i]) % newCapacity;
		int first = h;
		int next = 1;
		int situation = 4;
		while(true){
			situation = howIsBucket(new_table[h],activeKeys[i]);
			if(situation == -1){//probe
				h = (first + next*next) % newCapacity;
				next++;
				continue;
			}
			else if(situation == 0 || situation == 1){//insert
				new_table[h].entries[situation].Key = activeKeys[i];
        	    new_table[h].entries[situation].Value = Get(activeKeys[i]);
        	    new_table[h].entries[situation].Active = true;
				break;
			}
		}
	}
	_capacity = newCapacity;
	delete [] _table;
	_table = new_table;
}

// A function returns the capacity of hash table.
template <class K, class T>
int HashTable<K, T>::Capacity() const {
    return _capacity;
}

// A function returns the size of hash table.
template <class K, class T>
int HashTable<K, T>::Size() const {
    return _size;
}

// A function puts the active keys in hash table into the given parameter.
template <class K, class T>
void HashTable<K, T>::getKeys(K* keys) {
    int index = 0;
    for (int i = 0; i < _capacity; i++) {
        Bucket& bucket = _table[i];
        for (int j = 0; j < BUCKET_SIZE; j++) {
            if (bucket.entries[j].Active && !bucket.entries[j].Deleted) {
                keys[index++] = bucket.entries[j].Key;
            }
        }
    }
}

// A utility function checks the situation of given bucket with given key.
// It indicates the bucket cell is suitable for which operation.
// It is implemented for BUCKET_SIZE = 2
template <class K, class T>
int HashTable<K, T>::howIsBucket(Bucket& bucket,K key) const{
	
	if(bucket.entries[0].Active || bucket.entries[1].Active){ //Full or Update
		
		if(bucket.entries[0].Active && bucket.entries[0].Key == key){
			//update entries[0]
			return 2;
		}
		else if(bucket.entries[1].Active && bucket.entries[1].Key == key){
			//update entries[1]
			return 3;
		}
		else if(bucket.entries[0].Active && bucket.entries[1].Active){
			//Full
			return -1;
		}
	}
	
	//to insert
	if(!bucket.entries[0].Active){
		//insert entries[0]
		return 0;
	}
	else if(!bucket.entries[1].Active){
		//insert entries[1]
		return 1;
	}
	return -999; //Never happens
}



#endif
