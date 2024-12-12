CC = gcc
FLAGS = -Wall -Werror -Wextra -std=c11
TARGET = s21_grep
File = s21_grep_test.sh
FileForTouch = touchfiles.sh
SRC = main.c flags_parser.c output.c
OBJ = $(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(FLAGS) -o $(TARGET) $(OBJ)

main.o: main.c s21_grep.h
	$(CC) $(FLAGS) -c main.c

flags_parser.o: flags_parser.c s21_grep.h
	$(CC) $(FLAGS) -c flags_parser.c

output.o: output.c s21_grep.h
	$(CC) $(FLAGS) -c output.c
check: $(TARGET)
	cppcheck --enable=all --suppress=missingIncludeSystem *.c *.h ./../common/*.*
style: $(TARGET)
	clang-format -style=Google -n *.c *.h
cleanallo:
	rm -f *.o
touchfile:
	sh $(FileForTouch)
testfile:
	sh $(File)

test: touchfile testfile cleanallo
clean:
	rm -f *.o *.txt $(TARGET)

