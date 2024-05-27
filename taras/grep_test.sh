#!/bin/bash

# Путь к утилите
utility_path="./a.out"

# Путь к файлу с тестовыми случаями
test_cases_file="test_cases.txt"

# Число тестовых случаев
num_test_cases=0

# Читаем тестовые случаи из файла
while IFS= read -r line; do
    # Ищем метку тестового случая
    if [[ $line == Test\ Case\ * ]]; then
        ((num_test_cases++))
        echo "Starting Test Case $num_test_cases:"
    elif [[ $line == Command:* ]]; then
        # Извлекаем команду
        command_to_run=${line#*:}
        # Выполняем команду и сохраняем вывод
        actual_output=$(eval "$command_to_run")
        # Сбрасываем ожидаемый вывод для нового тестового случая
        expected_output=""
        # Читаем ожидаемые результаты
        while IFS= read -r line; do
            if [[ $line!= Command:* ]] && [[ $line!= Test\ Case\ * ]]; then
                # Добавляем строку к ожидаемому выводу
                expected_output+=$'\n'"$line"
            fi
        done < "$test_cases_file"
        # Сравниваем фактический вывод с ожидаемым
        if [[ $actual_output == $expected_output ]]; then
            echo "Success for Test Case $num_test_cases"
        else
            echo "Fail for Test Case $num_test_cases"
        fi
    fi
done < "$test_cases_file"