
SUCCESS=0
FAIL=0
COUNTER=0
DIFF_RES=""

GREEN=$(tput setaf 2)
RED=$(tput setaf 1)
RESET=$(tput sgr0)


declare -a tests=(
"s ./tests/test_0_grep.txt VAR"
"for s21_grep.c grep.h Makefile VAR"
"for s21_grep.c VAR"
"-e for -e ^int s21_grep.c grep.h Makefile VAR"
"-e for -e ^int s21_grep.c VAR"
"-e while -e void s21_grep.c Makefile VAR -f ./tests/test_ptrn_grep.txt"
)

declare -a extra=(
"-n for ./tests/test_1_grep.txt ./tests/test_2_grep.txt"
"-n for ./tests/test_1_grep.txt"
"-n -e ^\} ./tests/test_1_grep.txt"
"-c -e \/ ./tests/test_1_grep.txt"
"-ce ^int ./tests/test_1_grep.txt ./tests/test_2_grep.txt"
"-e ^int ./tests/test_1_grep.txt"
"-nivh = ./tests/test_1_grep.txt ./tests/test_2_grep.txt"
"-e"
"-ie INT ./tests/test_5_grep.txt"
"-echar ./tests/test_1_grep.txt ./tests/test_2_grep.txt"
"-ne = -e out test_5_grep.txt"
"-iv int ./tests/test_5_grep.txt"
"-in int ./tests/test_5_grep.txt"
"-c -l aboba ./tests/test_1_grep.txt ./tests/test_5_grep.txt"
"-v ./tests/test_1_grep.txt -e ank"
"-noe ) ./tests/test_5_grep.txt"
"-l for ./tests/test_1_grep.txt test_2_grep.txt"
"-e = -e out ./tests/test_5_grep.txt"
"-e ing -e as -e the -e not -e is ./tests/test_6_grep.txt"
"-l for no_file.txt ./tests/test_2_grep.txt"
)

testing()
{
    t=$(echo $@ | sed "s/VAR/$var/")
    ./s21_grep $t > test_s21_grep.log
    grep $t > test_sys_grep.log
    DIFF_RES="$(diff -s test_s21_grep.log test_sys_grep.log)"
    (( COUNTER++ ))
    if [ "$DIFF_RES" == "Files test_s21_grep.log and test_sys_grep.log are identical" ]
    then
      (( SUCCESS++ ))
      echo ${GREEN}SUCCESS${RESET}
      echo FAIL:$FAIL SUCCESS:$SUCCESS ALL:$COUNTER $t
    else
      (( FAIL++ ))
      echo ${RED}FAIL${RESET}
      echo FAIL:$FAIL SUCCESS:$SUCCESS ALL:$COUNTER $t
    fi
    rm test_s21_grep.log test_sys_grep.log
}

# специфические тесты
for i in "${extra[@]}"
do
    var="-"
    testing $i
done

# 1 параметр
for var1 in v c l n h o
do
    for i in "${tests[@]}"
    do
        var="-$var1"
        testing $i
    done
done

# 2 параметра
for var1 in v c l n h o
do
    for var2 in v c l n h o
    do
        if [ $var1 != $var2 ]
        then
            for i in "${tests[@]}"
            do
                var="-$var1 -$var2"
                testing $i
            done
        fi
    done
done

# 2 сдвоенных параметра
for var1 in v c l n h o
do
    for var2 in v c l n h o
    do
        if [ $var1 != $var2 ]
        then
            for i in "${tests[@]}"
            do
                var="-$var1$var2"
                testing $i
            done
        fi
    done
done

echo ${RED}FAIL: $FAIL${RESET}
echo ${GREEN}SUCCESS: $SUCCESS${RESET}
echo ALL: $COUNTER
