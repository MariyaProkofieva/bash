#!/bin/bash

# Путь к файлу с тестовыми случаями
test_cases_file="test_cases2.txt"

success_count=0
fail_count=0

LRED='\033[1;31m'
NORMAL='\033[0m'
GREEN='\033[0;32m'
# Число тестовых случаев
num_test_cases=0
echo test1
# Читаем тестовые случаи из файла
expected_output=""
while IFS= read -r line; do
    # Ищем метку тестового случая
    # echo $line
    
    if [[ $line == Test\ Case\ * ]]; then
        ((num_test_cases++))
        echo "Starting Test Case $num_test_cases:"
        # Сбрасываем ожидаемый вывод для нового тестового случая
        expected_output=""
    elif [[ $line == Command:* ]]; then
        # Извлекаем команду
        command_to_run=${line#*:}

    elif [[ $line == end_of_test_case* ]]; then
                # Выполняем команду и сохраняем вывод
        echo $command_to_run
        actual_output=$(eval "$command_to_run")
        actual_output+="\n"
        echo "$expected_output" > "expected_$num_test_cases"
        echo "$actual_output" > "actual_$num_test_cases"
        expected=$(diff expected_$num_test_cases actual_$num_test_cases)
        if cmp -s "expected_$num_test_cases" "actual_$num_test_cases"; then
            printf 'The file "%s" is the same as "%s"\n' "$expected_output" "$file2"
        else
            echo expected_output
            echo "$expected_output"
            echo actual_output
            echo "$actual_output"
        fi
        # Сравниваем фактический вывод с ожидаемым
        if [ "$expected" == "" ]; then
            echo "${GREEN} Success for Test Case $num_test_cases ${NORMAL}"
            ((success_count++)) 
        else
            echo "${LRED} Fail for Test Case $num_test_cases ${NORMAL}"
            ((fail_count++))
        fi

        rm expected_$num_test_cases
        rm actual_$num_test_cases
    else
        expected_output+=$"$line\n"
    fi
done < "$test_cases_file"
((test_amount=success_count+fail_count))   
echo "${GREEN} Count successed tests: $success_count/$test_amount ${NORMAL}"
echo "${LRED} Count failed tests: $fail_count/$test_amount ${NORMAL}"