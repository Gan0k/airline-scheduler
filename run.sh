 #!/bin/bash

FILES=Benchmark/*.air

rm $2

for f in $FILES
do
	echo $f >> $2
	./$1 < $f >> $2
done
