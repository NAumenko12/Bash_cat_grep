CC = gcc
FLAGS = -Wall -Werror -Wextra -std=c11
TARGET = s21_cat
File_test = test.sh
File_txt = touchfiles.sh
SRC = main.c flags_parser.c output.c
OBJ = $(SRC:.c=.o)

all: $(TARGET) cleanallo

$(TARGET): $(OBJ)
	$(CC) $(FLAGS) -o $(TARGET) $(OBJ)

main.o: main.c s21_cat.h
	$(CC) $(FLAGS) -c main.c

flags_parser.o: flags_parser.c s21_cat.h
	$(CC) $(FLAGS) -c flags_parser.c

output.o: output.c s21_cat.h
	$(CC) $(FLAGS) -c output.c

touchfiles:
	sh $(File_txt)
testfile:
	sh $(File_test)

test: touchfiles testfile cleanallo

check: $(TARGET)
	cppcheck --enable=all --suppress=missingIncludeSystem *.c *.h ./../common/*.*

style: $(TARGET)
	clang-format -style=Google -n *.c *.h

cleantxt:
	rm -f *.o *.txt

cleanallo:
	rm -f *.o *.o

clean:
	rm -f *.o *.txt $(TARGET)
