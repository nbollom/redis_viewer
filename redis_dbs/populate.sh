#!/bin/sh


# Strings
for i in {1..100}
do
    redis-cli set strings:key$i value$i;
done


# Lists
for i in {1..10}
do
    for j in {1..100}
    do
        redis-cli rpush lists:key$i value$j
    done
done


# Sets
for i in {1..10}
do
    for j in {1..100}
    do
        redis-cli sadd sets:key$i value$j
    done
done


# ZSets
for i in {1..10}
do
    for j in {1..100}
    do
        redis-cli zadd zsets:key$i $j value$j
    done
done

# Hashes
for i in {1..10}
do
    for j in {1..10}
    do
        for k in {1..100}
        do
            redis-cli hset hashes:key$i key$j value$k
        done
    done
done
