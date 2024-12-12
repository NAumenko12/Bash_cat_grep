COLOR_RESET="\033[0m"
COLOR_RED="\033[1;31m"
COLOR_GREEN="\033[1;32m"
COLOR_YELLOW="\033[1;33m"
COLOR_BLUE="\033[0;34m"
COLOR_MAGENTA="\033[0;35m"
COLOR_CYAN="\033[0;36m"

filename1="test_file_1.txt"
filename2="test_file_2.txt"
filename3="test_file_3.txt"
patterns_file="test_file_4.txt"
pattern="Little"
flags=("-i" "-v" "-c" "-l" "-n" "-h" "-s")
num_flags=${#flags[@]}

program="./s21_grep"
my_program="grep"


my_testfile="s21_grep.txt"
testfileorig="grep.txt"

echo "${COLOR_MAGENTA} GREP TEST${COLOR_MAGENTA}"


diff_files() {
    flags="$1"
    echo "${COLOR_CYAN}Флаги: ${flags:-}${COLOR_RESET}"
    "$my_program" $flags "$pattern" "$filename1" "$filename2" "$filename3">> "$my_testfile"
    "$program" $flags "$pattern" "$filename1" "$filename2" "$filename3">> "$testfileorig"

    if diff "$my_testfile" "$testfileorig"; then
        echo  "${COLOR_GREEN}Файлы идентичны. УСПЕХ.${COLOR_RESET}"
    else
        echo  "${COLOR_RED}Файлы различаются. ПРОВАЛ.${COLOR_RESET}"
    fi
    rm "$my_testfile" "$testfileorig"
}

diff_files_patternfile() {
    flags="$1"
    echo "${COLOR_CYAN}Флаги: ${flags:-}${COLOR_RESET}"
    "$my_program" $flags -f "$patterns_file" "$filename1" "$filename2" "$filename3">> "$my_testfile"
    "$program" $flags -f "$patterns_file" "$filename1" "$filename2" "$filename3">> "$testfileorig"

    if diff "$my_testfile" "$testfileorig"; then
        echo  "${COLOR_GREEN}Файлы идентичны. УСПЕХ.${COLOR_RESET}"
    else
        echo  "${COLOR_RED}Файлы различаются. ПРОВАЛ.${COLOR_RESET}"
    fi
    rm "$my_testfile" "$testfileorig"
}

echo  "${COLOR_YELLOW}ОДИНОЧНЫЕ ФЛАГИ С ЗАДАННЫМ ШАБЛОНОМ:${COLOR_RESET}"
for flag in "${flags[@]}"; do
    diff_files "$flag"
done

echo  "${COLOR_YELLOW}КОМБИНАЦИИ ФЛАГОВ С ЗАДАННЫМ ШАБЛОНОМ:${COLOR_RESET}"
for ((i = 0; i < num_flags; i++)); do
    for ((j = i + 1; j < num_flags; j++)); do
        diff_files "${flags[i]} ${flags[j]}"
    done
done

echo  "${COLOR_YELLOW}ОДИНОЧНЫЕ ФЛАГИ С ШАБЛОНАМИ ИЗ ФАЙЛА:${COLOR_RESET}"
for flag in "${flags[@]}"; do
    diff_files_patternfile "$flag"
done

echo  "${COLOR_YELLOW}КОМБИНАЦИИ ФЛАГОВ С ШАБЛОНАМИ ИЗ ФАЙЛА:${COLOR_RESET}"
for ((i = 0; i < num_flags; i++)); do
    for ((j = i + 1; j < num_flags; j++)); do
        diff_files_patternfile "${flags[i]} ${flags[j]}"
    done
done
