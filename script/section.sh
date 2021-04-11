#!/bin/bash
cd ../script
./ld-new $1 $2 -T $3 |  awk '{ print $4" "$5}'