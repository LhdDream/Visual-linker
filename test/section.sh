#!/bin/bash
cd ../test
./ld-new $1 $2 -T $3 |  awk '{ print $4" "$5}'