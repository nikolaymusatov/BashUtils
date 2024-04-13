#!/bin/bash

PASSED=0
FAILED=0
COUNTER=0
DIFF_RES=""

declare -a tests=(
"s test_grep/test_0_grep.txt FLAGS"
"for grep_functions.c grep_functions.h Makefile FLAGS"
"for grep_functions.c FLAGS"
"grep_functions.c grep_functions.h Makefile FLAGS -e for -e ^int"
"grep_functions.c FLAGS -e for -e ^int"
"grep_functions.c FLAGS -f test_grep/test_ptrn_grep.txt -e regex -e ^print"
"grep_functions.c Makefile FLAGS -f test_grep/test_ptrn_grep.txt -e while -e void"
)

declare -a extra=(
"-n for test_grep/test_1_grep.txt test_grep/test_2_grep.txt"
"-n for test_grep/test_1_grep.txt"
"-n -e ^\} test_grep/test_1_grep.txt"
"-c -e /\ test_grep/test_1_grep.txt"
"-ce ^int test_grep/test_1_grep.txt test_grep/test_2_grep.txt"
"-e ^int test_grep/test_1_grep.txt"
"-nivh = test_grep/test_1_grep.txt test_grep/test_2_grep.txt"
"-e"
"-ie INT test_grep/test_5_grep.txt"
"-echar test_grep/test_1_grep.txt test_grep/test_2_grep.txt"
"test_grep/test_5_grep.txt -ne = -e out"
"-iv int test_grep/test_5_grep.txt"
"-in int test_grep/test_5_grep.txt"
"-c -l aboba test_grep/test_1_grep.txt test_grep/test_5_grep.txt"
"-v test_grep/test_1_grep.txt -e ank"
"-noe ) test_grep/test_5_grep.txt"
"-l for test_grep/test_1_grep.txt test_grep/test_2_grep.txt"
"-o -e int test_grep/test_4_grep.txt"
"test_grep/test_5_grep.txt -e = -e out"
"-noe ing -e as -e the -e not -e is test_grep/test_6_grep.txt"
"test_grep/test_6_grep.txt -e ing -e as -e the -e not -e is"
"-c -e . test_grep/test_1_grep.txt -e '.'"
"-l for test_grep/no_file.txt test_grep/test_2_grep.txt"
"-f test_grep/test_3_grep.txt test_grep/test_5_grep.txt"
)

test()
{
    template=$(echo $@ | sed "s/FLAGS/$flags/")
    ./grep $template > test_my_grep.log
    grep $template > test_sys_grep.log
    DIFF_RES="$(diff -s test_my_grep.log test_sys_grep.log)"
    (( COUNTER++ ))
    if [ "$DIFF_RES" == "Files test_my_grep.log and test_sys_grep.log are identical" ]
    then
      (( PASSED++ ))
        echo "TEST #$COUNTER PASSED! Operation: grep $template"
    else
      (( FAILED++ ))
        echo "TEST #$COUNTER FAILED! Operation: grep $template"
    fi
}

# 1 флаг
for flag1 in v c l n h o
do
    for test in "${tests[@]}"
    do
        flags="-$flag1"
        test $test
    done
done

# 2 флага
for flag1 in v c l n h o
do
    for flag2 in v c l n h o
    do
        if [ $flag1 != $flag2 ]
        then
            for test in "${tests[@]}"
            do
                flags="-$flag1$flag2"
                test $test
            done
        fi
    done
done

# 3 флага
for flag1 in v c l n h o
do
    for flag2 in v c l n h o
    do
        for flag3 in v c l n h o
        do
            if [ $flag1 != $flag2 ] && [ $flag2 != $flag3 ] && [ $flag1 != $flag3 ]
            then
                for test in "${tests[@]}"
                do
                    flags="-$flag1$flag2$flag3"
                    test $test
                done
            fi
        done
    done
done

# дополнительные тесты 
for test in "${extra[@]}"
do
    test $test
done

echo "___________________________________________________________"
echo "$COUNTER tests DONE. Result: $PASSED passed, $FAILED failed"
rm test_my_grep.log test_sys_grep.log