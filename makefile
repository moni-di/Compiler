# Group 10
# Name: Hemant Singh Sisodiya ID: 2019A7PS0070P
# Name: Akash S Revankar      ID: 2019A7PS0294P
# Name: Harsh Butani          ID: 2019A7PS0022P
# Name: Siddharth Upadhyay    ID: 2019A7PS0033P
# Name: Mohit Sharma          ID: 2019A7PS0100P


all: compile

compile:
	gcc -o compiler driver.c parser.c lexer.c hashtable.c set.c stack.c tree.c ast.c semantic_analyzer.c

clean:
	rm -rf compiler
