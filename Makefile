CC = g++-7
CC_TEST = g++
CFLAGS = -I include -Wall -Werror -pedantic-errors -Wextra -std=c++14 -ftrapv -fsanitize=undefined -O2
TFLAGS = -O0 -I include -g --coverage -std=c++11
NAME = pars
PARS_TEST_IN_PATH = tests/parser/input/
LEX_TEST_IN_PATH = tests/lexer/input/
PARS_TEST_OUT_PATH = tests/parser/output/
LEX_TEST_OUT_PATH = tests/lexer/output/
PARS_TEST_EXP_PATH = tests/parser/expected/
LEX_TEST_EXP_PATH = tests/lexer/expected/
PARS_TEST_NUM = 16
LEX_TEST_NUM = 3


all : $(NAME)

$(NAME) : lexer.o parser.o
	$(CC) lexer.o parser.o src/main.cpp $(CFLAGS) -o $(NAME)

lexer.o : src/lexer.cpp
	$(CC) src/lexer.cpp -c $(CFLAGS) -o lexer.o

parser.o : src/parser.cpp 
	$(CC) src/parser.cpp -c $(CFLAGS) -o parser.o

pars_test_build :
	$(CC_TEST) src/lexer.cpp src/parser.cpp src/main.cpp -o $(NAME)_pars_test $(TFLAGS) 

lex_test_build :
	$(CC_TEST) src/lexer.cpp src/test_lexer.cpp -o  $(NAME)_lex_test $(TFLAGS)

pars_test : pars_test_build
	touch tests/result
	num=1 ; while [[ $$num -le $(PARS_TEST_NUM) ]] ; do \
		./$(NAME)_pars_test < $(PARS_TEST_IN_PATH)test$$num > $(PARS_TEST_OUT_PATH)test$$num ; \
		diff $(PARS_TEST_OUT_PATH)test$$num $(PARS_TEST_EXP_PATH)test$$num > tests/result ; \
		[ -s tests/result ] && echo "\033[1;31mPARSER TEST № $$num FAILED\033[0m" || echo "\033[1;32mPARSER TEST № $$num PASSED\033[0m" ; \
		((num = num + 1)) ; \
	done

lex_test : lex_test_build
	touch tests/result
	num=1 ; while [[ $$num -le $(LEX_TEST_NUM) ]] ; do \
		./$(NAME)_lex_test < $(LEX_TEST_IN_PATH)test$$num > $(LEX_TEST_OUT_PATH)test$$num ; \
	    diff $(LEX_TEST_OUT_PATH)test$$num $(LEX_TEST_EXP_PATH)test$$num > tests/result ; \
	    [ -s tests/result ] && echo "\033[1;31mLEXER TEST № $$num FAILED\033[0m" || echo "\033[1;32mLEXER TEST № $$num PASSED\033[0m" ; \
		((num = num + 1)) ; \
	done

show_pars_tests :
	num=1 ; while [[ $$num -le $(PARS_TEST_NUM) ]] ; do \
		echo "\nPARSER TEST № $$num" ; \
		cat $(PARS_TEST_IN_PATH)test$$num ; \
		((num = num + 1)) ; \
	done

show_lex_tests :
	num=1 ; while [[ $$num -le $(LEX_TEST_NUM) ]] ; do \
		echo "\nLEXER TEST № $$num" ; \
		cat $(LEX_TEST_IN_PATH)test$$num ; \
		(( num = num + 1)) ; \
	done

cover : lcov lex_test pars_test
	mkdir cover
	mv *.gcda *.gcno cover
	cd cover
	lcov/bin/lcov --capture --directory cover --rc lcov_branch_coverage=1 --output-file cover/coverage.info
	lcov/bin/genhtml cover/coverage.info --branch-coverage
	cd ..

lcov :
	git clone https://github.com/linux-test-project/lcov.git

clean :
	rm -f *.o
	rm -f $(NAME) $(NAME)_pars_test $(NAME)_lex_test
	rm -rf $(PARS_TEST_OUT_PATH)/test*
	rm -rf $(LEX_TEST_OUT_PATH)/test*
	rm -rf cover
	rm -f *.info *.gcda *.gcno *.html *.css *.png 
	rm -rf *.dSYM Users usr v1
	clear
