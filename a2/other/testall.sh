#!/bin/bash

input="20 10 30"
./regress ./tokenring "${input}" ./tokenring_64 "${input}" ''

for stations in `seq 2 100`; do
	for failure in `seq 2 100 `; do
		for seed in `seq 2 100`; do
			./regress ./tokenring "${stations} ${failure} ${seed}" \
				./tokenring_64 "${stations} ${failure} ${seed}" ''
			if [ $? -eq 1 ]; then
				exit 1;
				#continue
			fi
		done
	done
done

