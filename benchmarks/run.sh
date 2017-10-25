make
make -C ../simulator/
simulator=../simulator/bin/simRISCV
mkdir -p simulator_output
for f in ./build/*.out; do
	echo "begining RISCV simulation for "$f
	$simulator -v -f $f &> "./simulator_output/"`basename $f`".debug" &
	pid=$!
	spin[0]="-"
	spin[1]="\\"
	spin[2]="|"
	spin[3]="/"
	
	echo -n "[executing...] ${spin[0]}"
	while kill -0 $pid 2>/dev/null
	do
		for i in "${spin[@]}"
		do
			echo -ne "\b$i"
			sleep 0.1
		done
	done
	echo "done.."
done
