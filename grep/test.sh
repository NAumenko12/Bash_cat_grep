COLOR_RESET="\033[0m"
COLOR_RED="\033[1;31m"
COLOR_GREEN="\033[1;32m"
COLOR_YELLOW="\033[1;33m"
COLOR_BLUE="\033[0;34m"
COLOR_MAG="\033[0;35m"
COLOR_CYAN="\033[0;36m"

filename1="test_file_1.txt"
filename2="test_file_2.txt"

flags=("-n" "-b" "-s" "-v" "-t" "-e")
#long_flags=("--number" "--number-nonblank" "--squeeze-blank" "--show-nonprinting" "--show-tabs" "--show-ends")
num_flags=${#flags[@]}

program="./s21_cat"
my_program="cat"


my_testfile="s21_cat.txt"
testfileorig="cat.txt"

echo "${COLOR_MAG}  CAT TEST${COLOR_MAG}"


diff_files() {
    flags="$1"
    echo "${COLOR_CYAN}Флаги: ${flags:-}${COLOR_RESET}"
    "$my_program" $flags "$filename1"   >> "$my_testfile"
    "$program" $flags "$filename1"  >> "$testfileorig"

    if diff "$my_testfile" "$testfileorig"; then
        echo  "${COLOR_GREEN}Файлы идентичны. УСПЕХ.${COLOR_RESET}"
    else
        echo  "${COLOR_RED}Файлы различаются. ПРОВАЛ.${COLOR_RESET}"
    fi
    rm "$my_testfile" "$testfileorig"
}


echo  "${COLOR_YELLOW}ОДИНОЧНЫЕ ФЛАГИ:${COLOR_RESET}"
for flag in "${flags[@]}"; do
    diff_files "$flag"
done
