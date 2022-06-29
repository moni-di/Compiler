/*
Group 10
Name: Hemant Singh Sisodiya ID: 2019A7PS0070P
Name: Akash S Revankar      ID: 2019A7PS0294P
Name: Harsh Butani          ID: 2019A7PS0022P
Name: Siddharth Upadhyay    ID: 2019A7PS0033P
Name: Mohit Sharma          ID: 2019A7PS0100P
*/



#include "driver.h"
#include "lexer.h"
#include "parser.h"
#include "semantic_analyzer.h"
#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
// #include <string.h>

// void printpt(nAryTreeNode* ast){
// 	if(ast==NULL)
// 	 	return;
// 	printf("%s-",ast->sym.isTerminal?keyToToken[ast->sym.t]:keyToNT[ast->sym.nt]);
// 	if(ast->parent){
// 		printf("%s\n",ast->parent->sym.isTerminal?keyToToken[ast->parent->sym.t]:keyToNT[ast->parent->sym.nt]);
// 	}
// 	else{
// 		printf("no parent\n");
// 	}
// 	ast = ast->leftMostChild;
// 	while(ast){
// 		printpt(ast);
// 		ast = ast->brother;
// 	}
// }
void printMenu(){
	printf("\033[0;32m");
	printf("\tPress option for the defined task\n");
	printf("\033[0m");
	printf("0: Exit\n");
	// printf("1: Remove Comments\n");
	printf("1: Print Token list\n");
	printf("2: Parse\n");
	printf("3: Generate AST\n");
	printf("4: Calculate memory usage\n");
	printf("5: Print Symbol Table\n");
	printf("6: Print Global Variables\n");
	printf("7: Print Activation record sizes\n");
	printf("8: Print Record types and sizes\n");
	printf("9: Type checking and Semantic Analysis\n");
	printf("10: Code generation and dynamic type checking\n");
	printf("-------------------------------------\n");
	printf("Enter your choice:");
}

void runRemoveComments(char* sourceFile, char* outputFile){
		FILE *source, *output;
	source = fopen(sourceFile, "r");
	output = fopen(outputFile, "w");
	if(source==NULL || output==NULL){
		printf("Error opening file\n");
		return ;
	}
	initializeLexer(source);
	removeComments(sourceFile);
	fclose(source);
	fclose(output);
}


void runLexer(char* sourceFile, char* outputFile){
		FILE *source, *output, *fp;
	source = fopen(sourceFile, "r");
	output = fopen(outputFile, "w");
	if(source==NULL||output==NULL){
		printf("Error opening file\n");
		return;
	}
	initializeLexer(source);
	getStream(source);
	fclose(source);
	fclose(output);
}

void runParser(char* sourceFile, char* outputFile){
		FILE *source, *output, *fp;
	source = fopen(sourceFile, "r");
	output = fopen(outputFile, "w");
	if(source==NULL||output==NULL){
		printf("Error opening file\n");
		return ;
	}
	initializeLexer(source);
	initializeParser();
	fp = fopen("grammar.txt", "r");
	if(fp == NULL){
		perror("fopen");
	}
	populateGrammar(fp);
	computeFirstSets();
	computeFollowSets();
	generateParseTable();
	nAryTreeNode* pt = parseInputSourceCode(source);
	// printf("Parse Tree\n");
	char prefix[10000] = "";
	// printParseTreePreorder(prefix, ast, true);
	if(pt==NULL){
		printf("Empty parse tree\n");
	}
	// printf("\033[0;36m");
	// printf("\nPrinting Pre-Order traversal of parse tree on console\n");
	// printf("\033[0m");
	// printParseTreePreorder(prefix, pt, true);
	// printf("AST Print hua");
	printf("\033[0;36m");
	printf("\nPrinting In-Order traversal of parse tree\n");
	printf("\033[0m");
	char col1[] = "Lexeme CurrentNode";
	char col2[] = "Line no";
	char col3[] = "Token name";
	char col4[] = "ValueIfNumber";
	char col5[] = "ParentNodeSymbol";
	char col6[] = "IsLeafNode";
	char col7[] = "NodeSymbol";
	fprintf(stdout, "%-30s %-30s %-30s %-30s %-30s %-30s %-30s\n", col1, col2, col3, col4, col5, col6, col7);
	// // printParseTreePreorder(prefix1, ast, true);
	printParseTree(pt, output);//printParsetree me dikka
	// printf("AST File me dala");
	freeGrammar();
	fclose(fp);
	fclose(source);
	fclose(output);
}

void runTimer(char* sourceFile, char* outputFile){
		FILE *source, *output, *fp;
	clock_t start_time, end_time;
					double total_CPU_time, total_CPU_time_in_seconds;
					start_time = clock();
					source = fopen(sourceFile, "r");
					output = fopen(outputFile, "w");
					if(source==NULL||output==NULL){
						printf("Error opening file\n");
						return ;
					}
					initializeLexer(source);
					initializeParser();

					fp = fopen("grammar.txt", "r");
					if(fp == NULL){
						perror("fopen");
					}

					populateGrammar(fp);
					computeFirstSets();
					computeFollowSets();


					generateParseTable();
					nAryTreeNode* pt = parseInputSourceCode(source);
					if(pt==NULL){
						printf("Empty parse tree\n");
					}

					freeGrammar();
					fclose(fp);
					fclose(source);
					fclose(output);
					end_time = clock();
					total_CPU_time = (double) (end_time - start_time);
					total_CPU_time_in_seconds = total_CPU_time / CLOCKS_PER_SEC;
					printf("total CPU time = %f \ntotal CPU time in seconds = %f\n",total_CPU_time,total_CPU_time_in_seconds);
}

void runAST(char* sourceFile, char* outputFile){
		FILE *source, *output, *fp;
	source = fopen(sourceFile, "r");
	output = fopen(outputFile, "w");
	if(source==NULL||output==NULL){
		printf("Error opening file\n");
		return ;
	}
	initializeLexer(source);
	initializeParser();
	fp = fopen("grammar.txt", "r");
	if(fp == NULL){
		perror("fopen");
	}
	populateGrammar(fp);
	computeFirstSets();
	computeFollowSets();
	generateParseTable();
	nAryTreeNode* pt = parseInputSourceCode(source);
	// printf("Parse Tree\n");
	char prefix[10000] = "";
	// printParseTreePreorder(prefix, ast, true);
	if(pt==NULL){
		printf("Empty parse tree\n");
	}
	nAryTreeNode* ast = constructAST(pt);
	printf("\033[0;36m");
	printf("\nPrinting Pre-Order traversal of abstract syntax tree on console\n");
	printf("\033[0m");
	printParseTreePreorder(prefix, ast, true);
	// printf("AST Print hua");
	// printf("\033[0;36m");
	// printf("\nStoring In-Order traversal of abstract syntax tree in %s\n",outputFile);
	// printf("\033[0m");
	// char col1[] = "Lexeme CurrentNode";
	// char col2[] = "Line no";
	// char col3[] = "Token name";
	// char col4[] = "ValueIfNumber";
	// char col5[] = "ParentNodeSymbol";
	// char col6[] = "IsLeafNode";
	// char col7[] = "NodeSymbol";
	// fprintf(output, "%-30s %-30s %-30s %-30s %-30s %-30s %-30s\n", col1, col2, col3, col4, col5, col6, col7);
	// // printParseTreePreorder(prefix1, ast, true);
	// printParseTree(ast, output);//printParsetree me dikka
	// printf("AST File me dala");
	freeGrammar();
	fclose(fp);
	fclose(source);
	fclose(output);
}

void runSemanticAnalyzer(char* sourceFile, char* outputFile){
	FILE *source, *output, *fp;
source = fopen(sourceFile, "r");
output = fopen(outputFile, "w");
if(source==NULL||output==NULL){
	printf("Error opening file\n");
	return ;
}
clock_t start_time, end_time;
					double total_CPU_time, total_CPU_time_in_seconds;
					start_time = clock();
initializeLexer(source);
initializeParser();
fp = fopen("grammar.txt", "r");
if(fp == NULL){
	perror("fopen");
}
populateGrammar(fp);
computeFirstSets();
computeFollowSets();
generateParseTable();
nAryTreeNode* pt = parseInputSourceCode(source);
// printf("Parse Tree\n");
// char prefix[10000] = "";
// printParseTreePreorder(prefix, ast, true);
if(pt==NULL){
	printf("Empty parse tree\n");
}
nAryTreeNode* ast = constructAST(pt);
// freeParser(pt);
// printpt(ast);
initTable(ruidToNode);
rootSymbolTablePtr = initSymbolTable();
currentSymbolTablePtr=rootSymbolTablePtr;
genConstrTypetoNodeMap(ruidToNode, ast, NULL);

// fnReDefn = false;
constructSymbolTable(ast, NULL);

thirdASTPass(ast);
if(errorsDetected==false)
	printf(GREEN"Code compiles successfully....\n"RESET);

end_time = clock();
total_CPU_time = (double) (end_time - start_time);
total_CPU_time_in_seconds = total_CPU_time / CLOCKS_PER_SEC;
printf("total CPU time = %f \ntotal CPU time in seconds = %f\n",total_CPU_time,total_CPU_time_in_seconds);
// printf("third ast completed\n");
freeGrammar();
fclose(fp);
fclose(source);
fclose(output);
}

void calc(nAryTreeNode* node, int *cnt, long long *size){
	if(node==NULL){
		return;
	}
	*cnt = *cnt + 1;
	*size = *size+sizeof(node);
	nAryTreeNode* child = node->leftMostChild;
	while(child){
		calc(child, cnt, size);
		child = child->brother;
	}
}

void calculateMemory(char* sourceFile, char* outputFile){
		FILE *source, *output, *fp;
	source = fopen(sourceFile, "r");
	output = fopen(outputFile, "w");
	if(source==NULL||output==NULL){
		printf("Error opening file\n");
		return ;
	}
	initializeLexer(source);
	initializeParser();
	fp = fopen("grammar.txt", "r");
	if(fp == NULL){
		perror("fopen");
	}
	populateGrammar(fp);
	computeFirstSets();
	computeFollowSets();
	generateParseTable();
	nAryTreeNode* pt = parseInputSourceCode(source);
	// printf("Parse Tree\n");
	char prefix[10000] = "";
	// printParseTreePreorder(prefix, ast, true);
	if(pt==NULL){
		printf("Empty parse tree\n");
	}
	int ptNodes = 0;
	long long ptMemory = 0;
	int astNodes = 0;
	long long astMemory = 0;
	nAryTreeNode* ast = constructAST(pt);
	calc(pt,&ptNodes,&ptMemory);
	calc(ast,&astNodes,&astMemory);
	double comprPer = ((double)ptMemory-astMemory)/ptMemory;
	comprPer*=100;
	printf("Parse tree Number of nodes = %d, Allocated Memory = %lld bytes\n",ptNodes, ptMemory);
	printf("AST Number of nodes = %d, Allocated Memory = %lld bytes\n",astNodes, astMemory);
	printf("Compression percentage = %lf %%\n", comprPer);
	// printf("\033[0;36m");
	// printf("\nPrinting Pre-Order traversal of abstract syntax tree on console\n");
	// printf("\033[0m");
	// printParseTreePreorder(prefix, ast, true);
	// printf("AST Print hua");
	// printf("\033[0;36m");
	// printf("\nStoring In-Order traversal of abstract syntax tree in %s\n",outputFile);
	// printf("\033[0m");
	// char col1[] = "Lexeme CurrentNode";
	// char col2[] = "Line no";
	// char col3[] = "Token name";
	// char col4[] = "ValueIfNumber";
	// char col5[] = "ParentNodeSymbol";
	// char col6[] = "IsLeafNode";
	// char col7[] = "NodeSymbol";
	// fprintf(output, "%-30s %-30s %-30s %-30s %-30s %-30s %-30s\n", col1, col2, col3, col4, col5, col6, col7);
	// // printParseTreePreorder(prefix1, ast, true);
	// printParseTree(ast, output);//printParsetree me dikka
	// printf("AST File me dala");
	freeGrammar();
	fclose(fp);
	fclose(source);
	fclose(output);
}

void generateTokenMap() {

	FILE *fp = fopen("./tokens.txt", "r");

	fseek(fp, 0, SEEK_END);
	int fileLength = ftell(fp);

	fseek(fp, 0, SEEK_SET);


	char *tokens = malloc(sizeof(char) * (fileLength + 1));

  	if (tokens == NULL) {
		perror("terminal string filling failed\n");
		exit(1);
	}

	fread(tokens, sizeof(char), fileLength, fp);
	tokens[fileLength] = '\0';

	fclose(fp);

	char *tok = NULL;

  	int key = 0;
	tok = strtok(tokens,", \n");
	strncpy(keyToToken[key++], tok, MAX_SYMBOL_LENGTH);
	while((tok = strtok(NULL, ", \n"))){
		strncpy(keyToToken[key++], tok, MAX_SYMBOL_LENGTH);
	}

	free(tokens);
}

void generateNTMap() {

	FILE *fp = fopen("./nonTerminals.txt", "r");

	fseek(fp, 0, SEEK_END);
	int fileLength = ftell(fp);
	fseek(fp, 0, SEEK_SET);


	char *nonTerminals = malloc(sizeof(char) * (fileLength + 1));
  	if (nonTerminals == NULL) {
		perror("terminal string filling failed\n");
		exit(1);
	}

	fread(nonTerminals, sizeof(char), fileLength, fp);
	nonTerminals[fileLength] = '\0';
		fclose(fp);

	char *nt = NULL;

  int key = 0;
	nt = strtok(nonTerminals,", \n");
	strncpy(keyToNT[key++], nt, MAX_SYMBOL_LENGTH);
	while((nt = strtok(NULL, ", \n"))){
		strncpy(keyToNT[key++], nt, MAX_SYMBOL_LENGTH);
	}

	free(nonTerminals);
}


int main(int argc, char* argv[]){
	if(argc!=3){
		printf("Invalid Input: Run as ./compiler <source.txt> <code.asm>\n");
		exit(0);
	}
	printf("Group 10\n");
	printf("Status:\n");
	printf("(a)FIRST and FOLLOW sets automated\n");
	printf("(b)Both lexical and syntax analysis modules implemented\n");
	printf("(c)Modules work with test cases: testcase2.txt, testcase3.txt, testcase4.txt, testcase5.txt\n");
	printf("(d)Lexical analysis works with testcase1.txt and testcase6.txt as well but syntax analysis gives different errors\n\n\n");
	printf("LEVEL 4: Symbol Table / AST / Semantic Rules Work\n\n");
	char sourceFile[20];
    char outputFile[20];

	strcpy(sourceFile, argv[1]);
	strcpy(outputFile, argv[2]);



	generateTokenMap();
	generateNTMap();
	int choice;
	bool semanticAnalysisDone = false;
	while(true){
		printMenu();
		scanf("%d", &choice);
		printf("\n");

		switch(choice){
			case 0:;
				exit(0);
			case 1:;
				// runRemoveComments(sourceFile, outputFile);
				runLexer(sourceFile, outputFile);
				break;
			case 2:;
				runParser(sourceFile, outputFile);
				break;
			case 3:;
				runAST(sourceFile, outputFile);
				break;
			case 4:;
				calculateMemory(sourceFile, outputFile);
                break;
			case 5:
				// runSemanticAnalyzer(sourceFile, outputFile);
				if(semanticAnalysisDone)
				printSymbolTable(rootSymbolTablePtr);
				else
					printf("press 9 and run semantic analyzer\n");
				// printf("Lite\n");
				break;
			case 6:
				// runGlobal(sourceFile, outputFile);
				// runSemanticAnalyzer(sourceFile, outputFile);
				if(semanticAnalysisDone)
					printGLobalVariables(rootSymbolTablePtr);
				else
					printf("press 9 and run semantic analyzer\n");
				// printf("Lite\n");
				break;
			case 7:
				// runActivationRecords(sourceFile, outputFile);
				// runSemanticAnalyzer(sourceFile, outputFile);
				if(semanticAnalysisDone)
					printActivationRecord(rootSymbolTablePtr);
				else
					printf("press 9 and run semantic analyzer\n");
				// printf("Lite\n");
				break;
			case 8:
				// runRecord(sourceFile, outputFile);
				// runSemanticAnalyzer(sourceFile, outputFile);
				if(semanticAnalysisDone)
				printRecordTypesAndSizes(rootSymbolTablePtr);
				else
					printf("press 9 and run semantic analyzer\n");
				// printf("Lite\n");
				break;
			case 9:
			errorsDetected = false;
				runSemanticAnalyzer(sourceFile, outputFile);
				semanticAnalysisDone = true;
				break;
			case 10:
				// runCodegen(sourceFile, outputFile);
				printf("Lite\n");
				break;
			default:;
			// runTimer(sourceFile, outputFile);
				printf("Lite\n");
				break;
		}

	}
}
