#!/bin/bash

echo "Exp1 BF"
for FILE_ID in {25..30..1}
do
    echo -n "${FILE_ID},"
    for (( I=0; I<10; I++ ))
    do
        OUT="$(./BF < "exp1/t${FILE_ID}.i" 2>&1 >/dev/null)"
        echo -n "${OUT},"
    done
    echo ""
done

echo "Exp1 DP"
for FILE_ID in {25..30..1}
do
    echo -n "${FILE_ID},"
    for (( I=0; I<10; I++ ))
    do
        OUT="$(./DP < "exp1/t${FILE_ID}.i" 2>&1 >/dev/null)"
        echo -n "${OUT},"
    done
    echo ""
done

echo "Exp2"
for NUM_THREADS in {1..128..1}
do
    echo -n "${NUM_THREADS},"
    for (( I=0; I<10; I++ ))
    do
        OUT="$(./PDP ${NUM_THREADS} < "exp2/t100k.i" 2>&1 >/dev/null)"
        echo -n "${OUT},"
    done
    echo ""
done


