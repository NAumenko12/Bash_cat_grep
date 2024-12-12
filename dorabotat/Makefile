all: s21_cat s21_grep

s21_cat:
	$(MAKE) -C cat

s21_grep:
	$(MAKE) -C grep
	
clang:
	$(MAKE) -C cat clang
	$(MAKE) -C grep clang
	
clean:
	$(MAKE) -C cat clean
	$(MAKE) -C grep clean

test:
	$(MAKE) -C cat test
	$(MAKE) -C grep test

	
CC = gcc
FLAGS = -Wall -Werror -Wextra -std=c11
TARGET = s21_grep
File = grep/s21_grep_test.sh
FileForTouch = grep/touchfiles.sh
SRC = grep/main.c grep/flags_parser.c grep/output.c
OBJ = $(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(FLAGS) -o $(TARGET) $(OBJ)

main.o: grep/main.c grep/s21_grep.h
	$(CC) $(FLAGS) -c grep/main.c -o main.o

flags_parser.o: grep/flags_parser.c grep/s21_grep.h
	$(CC) $(FLAGS) -c grep/flags_parser.c -o flags_parser.o

output.o: grep/output.c grep/s21_grep.h
	$(CC) $(FLAGS) -c grep/output.c -o output.o

check: $(TARGET)
	cppcheck --enable=all --suppress=missingIncludeSystem grep/*.c grep/*.h

style: $(TARGET)
	clang-format -style=Google -n grep/*.c grep/*.h

touchfile:
	sh $(FileForTouch)

testfile:
	sh $(File)

test: touchfile testfile cleanallo

cleanallo:
	rm -f *.o

clean:
	rm -f *.o *.txt $(TARGET)