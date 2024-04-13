#!/bin/bash

PASSED=0
FAILED=0
COUNTER=0
DIFF_RES=0 

declare -a tests=(
"FLAGS test_cat/test_case_cat.txt"
)

declare -a extra=(
"-s test_cat/test_1_cat.txt"
"-b -e -n -s -t -v test_cat/test_1_cat.txt"
"-t test_cat/test_3_cat.txt"
"-n test_cat/test_2_cat.txt"
"no_file.txt"
"-n -b test_cat/test_1_cat.txt"
"-s -n -e test_cat/test_4_cat.txt"
"test_cat/test_1_cat.txt -n"
"-n test_cat/test_1_cat.txt"
"-n test_cat/test_1_cat.txt test_cat/test_2_cat.txt"
"-v test_cat/test_5_cat.txt"
)

test()
{
    template=$(echo $@ | sed "s/FLAGS/$flags/")
    ./cat $template > test_my_cat.log
    cat $template > test_sys_cat.log
    DIFF_RES="$(diff -s test_my_cat.log test_sys_cat.log)"
    (( COUNTER++ ))
    if [ "$DIFF_RES" == "Files test_my_cat.log and test_sys_cat.log are identical" ]
    then
      (( PASSED++ ))
        echo "TEST #$COUNTER PASSED! Operation: cat $template"
    else
      (( FAILED++ ))
        echo "TEST #$COUNTER FAILED! Operation: cat $template"
    fi
}

# 1 флаг
for flag1 in b e n s t v
do
    for test in "${tests[@]}"
    do
        flags="-$flag1"
        test $test
    done
done

# 3 флага
for flag1 in b e n s t v
do
    for flag2 in b e n s t v
    do
        for flag3 in b e n s t v
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
rm test_my_cat.log test_sys_cat.log