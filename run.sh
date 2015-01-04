 #!/bin/bash

FILES=Benchmark/*.air

rm $1

for f in $FILES
do
	echo $f >> $1
	./practica < $f >> $1
done
