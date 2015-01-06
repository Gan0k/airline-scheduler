 #!/bin/bash

FILES=Benchmark/*.air

rm -f $2

for f in `ls Benchmark/*.air`
do
	echo ${f##*/} >> $2
	./$1 -v2 < $f >> $2
done
