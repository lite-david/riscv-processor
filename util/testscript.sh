build="./benchmarks/build"
ref="./benchmarks/reference_output"
mkdir -p logs
log="./logs"

./catapult.sim $build/qsort.out > $log/qsort.log
python verify_simulation.py $ref/qsort.ref $log/qsort.log

./catapult.sim $build/multiply.out > $log/multiply.log
python verify_simulation.py $ref/multiply.ref $log/multiply.log

./catapult.sim $build/towers.out > $log/towers.log
python verify_simulation.py $ref/towers.ref $log/towers.log

./catapult.sim $build/median.out > $log/median.log
python verify_simulation.py $ref/median.ref $log/median.log

./catapult.sim $build/vvadd.out > $log/vvadd.log
python verify_simulation.py $ref/vvadd.ref $log/vvadd.log
