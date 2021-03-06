/**
 * A hash set.
 * Stub written by James Daly, 8 Oct 2014
 * Completed by Michael Wright
 * Colaborated with Joe Dinkha and Ian Ferguson 
 * for whiteboard ideas to implement methods
 */
#pragma once

#ifndef HASH_SET_H
#define HASH_SET_H

#include <algorithm>
#include <functional>
#include <iostream>
#include <vector>

using namespace std;
using std::vector;

template<class T>
class HashSet
{
    public:
        HashSet(std::function<unsigned int(const T&)> hash, double loadFactor);
        HashSet(HashSet<T> &other);
        virtual ~HashSet();

        // Supplied Methods
        bool IsEmpty() const { return Size() == 0; }
        unsigned int Size() const { return size; }
        double GetLoadFactor() const { return loadFactor; }
        void DisplayItems() const { ForEach([](const T& item) { std::cout << item << std::endl; }); }
        double GetLoad() const { return ((double)Size()) / (double)NumBuckets(); }

        // Methods that you must complete
        unsigned int NumBuckets() const { return mTableSize; }
        bool Contains(const T& item) const;
        bool Insert(const T &item);
        unsigned int Index(const T &item);
        bool Remove(const T &item);
        void Clear();
        void ForEach(std::function<void (const T&)> func) const;
        unsigned int MaxBucketSize() const;
        double PercentEmptyBuckets() const;

    private:
        unsigned int size;
        double loadFactor;
        std::function<unsigned int (const T&)> hash;
        vector<T> mNulVec;
        // You need some data source to store items

        // Size of default table
        unsigned int mTableSize = 10;

        // The underlying set structure
        vector<vector<T>> mTable; 

        // Temporary vector used for resizing
        vector<T> resizeVec;

        // You can put any helper methods here



};

/**
 * Default constructor
 */
template<class T>
HashSet<T>::HashSet(std::function<unsigned int(const T&)> hash, double loadFactor):
    size(0),
    loadFactor(loadFactor),
    hash(hash)
{
    // Empty: You can put other initialization information here
    for(auto i = 0; i < mTableSize; i++)
    {
        mTable.push_back(mNulVec);
    }
}

/**
 * Copy Constructor
 * Uses uniform instantiation to initialize itself
 * and then copies all of the items from the other set
 */
template<class T>
HashSet<T>::HashSet(HashSet<T> &other) :
    HashSet(other.hash, other.loadFactor)
{
    auto insertItem = [=](const T& x){ this->Insert(x); };
    other.ForEach(insertItem);
}

/**
 * Destructor
 * Any cleanup you need to do can be done here
 */
    template<class T>
HashSet<T>::~HashSet()
{
    // Empty: You can put any steps necessary for cleanup here
}

/**
 * Returns true iff the item is in the set
 */
template<class T>
bool HashSet<T>::Contains(const T &item) const
{
    unsigned int hashed = hash(item);
    unsigned int bucket = hashed % NumBuckets();
    for(auto i = 0; i != mTable[bucket].size(); i++)
    {
        if(mTable[bucket][i] == item)
        {
            return true;
        }
    }
    return false;
}

/**
 * Inserts the item into the set.
 * Only one copy can exist in the set at a time.
 * Returns true iff the item was successfully added.
 */
    template<class T>
bool HashSet<T>::Insert(const T &item)
{
    // after insert, if the load is too big, call rehash
    // make helper method hashset
    // load factor is mean number of items (items/buckets)
    if (! Contains(item))
    {
        unsigned int hashed = hash(item);
        unsigned int bucket = hashed % mTableSize;
        size++;
        mTable[bucket].push_back(item);

        // Resizing the hashtable
        if (GetLoad() > GetLoadFactor())
        {
            
            //this code was taken from James Daly
            auto pushingLambda = [&](const T&item)
            {
                resizeVec.push_back(item);
            };

            ForEach(pushingLambda);

            //clear the set
            Clear();            

            //add new buckets
            for(auto i = 0; i < mTableSize; i++)
            {
                mTable.push_back(mNulVec);
            }

            //resize the set
            mTableSize = (NumBuckets() * 2);

            // recursize call to insert
            for(auto i = 0; i < resizeVec.size(); i++)
            {
                Insert(resizeVec[i]);
            }
        }
        return true;
    }
    else
        return false;
}

/**
 * Removes the item from the set.
 * Returns true iff the item was successfully removed.
 */
    template<class T>
bool HashSet<T>::Remove(const T &item)
{
    for(auto bucket = 0; bucket != NumBuckets(); bucket++)
    {
        for(auto i= 0; i != mTable[bucket].size(); i++)
        {
            if(item == mTable[bucket][i])
            {
                //decrement the size
                size--;
                //find and delete our item
                mTable[bucket].erase(std::remove(mTable[bucket].begin(), 
                            mTable[bucket].end(), item), mTable[bucket].end());
                return true;
            }
        }
    }
    return false;
}

/**
 * Removes all elements from the set.
 */
    template<class T>
void HashSet<T>::Clear()
{
    for (auto i = 0; i != NumBuckets(); i++)
    {
        mTable[i].clear();
        size = 0;
    } 
}

/**
 * Invokes the function once on each item in the set.
 * The exact order is undefined.
 */
template<class T>
void HashSet<T>::ForEach(std::function<void (const T&)> func) const
{
    for(auto bucket = 0; bucket != NumBuckets(); bucket++)
    {
        for(auto item = 0; item != mTable[bucket].size(); item++)
        {
            func(mTable[bucket][item]);
        }
    }
}

/**
 * Finds the maximum number of items in a bucket.
 */
template<class T>
unsigned int HashSet<T>::MaxBucketSize() const
{
    double highestBucket = 0, currBucket = 0;

    for(auto bucket = 0; bucket != NumBuckets(); bucket++)
    {
        currBucket = mTable[bucket].size();
        if(currBucket > highestBucket)
        {
            highestBucket = currBucket;
        }
    }     
    return highestBucket; 
}

/**
 * Finds which fraction of the buckets are empty
 * The result is returned as a percent
 */
template<class T>
double HashSet<T>::PercentEmptyBuckets() const
{
    double numFull = 0;
    for(auto i = 0; i != NumBuckets(); i++)
    {
        if(mTable[i].size() != 0)
        {
            numFull++;
        }
    }     
    return (((NumBuckets()-numFull)/NumBuckets())*100);
}

/*
   template<class T>
   unsigned int HashSet<T>::Index(const T &item)
   {
   unsigned int hashed = hash(item);
   unsigned int bucket = hashed % mTableSize;
   return bucket;
   }
   */

#endif
