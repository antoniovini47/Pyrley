/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "../Compiler/grammar.y"

	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <stdarg.h>

	#define MAXRECST 200
	#define MAXST 100
	#define MAXCHILDREN 100
	#define MAXLEVELS 20
	#define MAXQUADS 1000

	
	

	extern int yylineno;
	extern int depth;
	extern int top();
	extern int pop();
	int currentScope = 1, previousScope = 1;
	
	int *arrayScope = NULL;

	void yyerror(const char *msg){
		//printf("\nSyntax Error at Line %d, Column : %d\n",  yylineno, yylloc.last_column);
		printf("\nSyntax Error at Line %d, Column : ERROR IDENTIFYING COLLUMN\n",  yylineno);
		exit(0);
	}

	int yylex(void);
	
	typedef struct record
	{
		char *type;
		char *name;
		int decLineNo;
		int lastUseLine;
	} record;

	typedef struct STable
	{
		int no;
		int noOfElems;
		int scope;
		record *Elements;
		int Parent;
		
	} STable;
	
	typedef struct ASTNode
	{
		int nodeNo;
    /*Operator*/
    char *NType;
    int noOps;
    struct ASTNode** NextLevel;
    
    /*Identifier or Const*/
    record *id;
	
	} node;
  
	typedef struct Quad
	{
		char *R;
		char *A1;
		char *A2;
		char *Op;
		int I;
	} Quad;
	
	STable *symbolTables = NULL;
	int sIndex = -1, aIndex = -1, tabCount = 0, tIndex = 0 , lIndex = 0, qIndex = 0, nodeCount = 0;
	node *rootNode;
	char *argsList = NULL;
	char *tString = NULL, *lString = NULL;
	Quad *allQ = NULL;
	node ***Tree = NULL;
	int *levelIndices = NULL;
	
	/*-----------------------------Declarations----------------------------------*/
	
	record* findRecord(const char *name, const char *type, int scope);
  node *createID_Const(char *value, char *type, int scope);
  int power(int base, int exp);
  void updateCScope(int scope);
  void resetDepth();
	int scopeBasedTableSearch(int scope);
	void initNewTable(int scope);
	void init();
	int searchRecordInScope(const char* type, const char *name, int index);
	void insertRecord(const char* type, const char *name, int lineNo, int scope);
	int searchRecordInScope(const char* type, const char *name, int index);
	void checkList(const char *name, int lineNo, int scope);
	void printSTable();
	void freeAll();
	void addToList(char *newVal, int flag);
	void clearArgsList();
	int checkIfBinOperator(char *Op);
	/*------------------------------------------------------------------------------*/
	
	void Xitoa(int num, char *str)
	{
		if(str == NULL)
		{
			 printf("Allocate Memory\n");
		   return;
		}
		sprintf(str, "%d", num);
	}

	
	char *makeStr(int no, int flag)
	{
		char A[10];
		Xitoa(no, A);
		
		if(flag==1)
		{
				strcpy(tString, "T");
				strcat(tString, A);
				insertRecord("ICGTempVar", tString, -1, 0);
				return tString;
		}
		else
		{
				strcpy(lString, "L");
				strcat(lString, A);
				insertRecord("ICGTempLabel", lString, -1, 0);
				return lString;
		}

	}
	
	void makeQ(char *R, char *A1, char *A2, char *Op)
	{
		
		allQ[qIndex].R = (char*)malloc(strlen(R)+1);
		allQ[qIndex].Op = (char*)malloc(strlen(Op)+1);
		allQ[qIndex].A1 = (char*)malloc(strlen(A1)+1);
		allQ[qIndex].A2 = (char*)malloc(strlen(A2)+1);
		
		strcpy(allQ[qIndex].R, R);
		strcpy(allQ[qIndex].A1, A1);
		strcpy(allQ[qIndex].A2, A2);
		strcpy(allQ[qIndex].Op, Op);
		allQ[qIndex].I = qIndex;
 
		qIndex++;
		
		return;
	}
	
	int checkIfBinOperator(char *Op)
	{
		if((!strcmp(Op, "+")) || (!strcmp(Op, "*")) || (!strcmp(Op, "/")) || (!strcmp(Op, ">=")) || (!strcmp(Op, "<=")) || (!strcmp(Op, "<")) || (!strcmp(Op, ">")) || 
			 (!strcmp(Op, "in")) || (!strcmp(Op, "==")) || (!strcmp(Op, "and")) || (!strcmp(Op, "or")))
			{
				return 1;
			}
			
			else 
			{
				return 0;
			}
	}
	
	void codeGenOp(node *opNode)
	{
		if(opNode == NULL)
		{
			return;
		}
		
		if(opNode->NType == NULL)
		{
			if((!strcmp(opNode->id->type, "Identifier")) || (!strcmp(opNode->id->type, "Constant")))
			{
				printf("T%d = %s\n", opNode->nodeNo, opNode->id->name);
				makeQ(makeStr(opNode->nodeNo, 1), opNode->id->name, "-", "=");
			}
			return;
		}
		
		if((!strcmp(opNode->NType, "If")) || (!strcmp(opNode->NType, "Elif")))
		{			
			switch(opNode->noOps)
			{
				case 2 : 
				{
					int temp = lIndex;
					codeGenOp(opNode->NextLevel[0]);
					printf("If False T%d goto L%d\n", opNode->NextLevel[0]->nodeNo, lIndex);
					makeQ(makeStr(temp, 0), makeStr(opNode->NextLevel[0]->nodeNo, 1), "-", "If False");
					lIndex++;
					codeGenOp(opNode->NextLevel[1]);
					lIndex--;
					printf("L%d: ", temp);
					makeQ(makeStr(temp, 0), "-", "-", "Label");
					break;
				}
				case 3 : 
				{
					int temp = lIndex;
					codeGenOp(opNode->NextLevel[0]);
					printf("If False T%d goto L%d\n", opNode->NextLevel[0]->nodeNo, lIndex);
					makeQ(makeStr(temp, 0), makeStr(opNode->NextLevel[0]->nodeNo, 1), "-", "If False");					
					codeGenOp(opNode->NextLevel[1]);
					printf("goto L%d\n", temp+1);
					makeQ(makeStr(temp+1, 0), "-", "-", "goto");
					printf("L%d: ", temp);
					makeQ(makeStr(temp, 0), "-", "-", "Label");
					codeGenOp(opNode->NextLevel[2]);
					printf("L%d: ", temp+1);
					makeQ(makeStr(temp+1, 0), "-", "-", "Label");
					lIndex+=2;
					break;
				}
			}
			return;
		}
		
		if(!strcmp(opNode->NType, "Else"))
		{
			codeGenOp(opNode->NextLevel[0]);
			return;
		}
		
		if(!strcmp(opNode->NType, "While"))
		{
			int temp = lIndex;
			codeGenOp(opNode->NextLevel[0]);
			printf("L%d: If False T%d goto L%d\n", lIndex, opNode->NextLevel[0]->nodeNo, lIndex+1);
			makeQ(makeStr(temp, 0), "-", "-", "Label");		
			makeQ(makeStr(temp+1, 0), makeStr(opNode->NextLevel[0]->nodeNo, 1), "-", "If False");								
			lIndex+=2;			
			codeGenOp(opNode->NextLevel[1]);
			printf("goto L%d\n", temp);
			makeQ(makeStr(temp, 0), "-", "-", "goto");
			printf("L%d: ", temp+1);
			makeQ(makeStr(temp+1, 0), "-", "-", "Label"); 
			lIndex = lIndex+2;
			return;
		}
		
		if(!strcmp(opNode->NType, "Next"))
		{
			codeGenOp(opNode->NextLevel[0]);
			codeGenOp(opNode->NextLevel[1]);
			return;
		}
		
		if(!strcmp(opNode->NType, "BeginBlock"))
		{
			codeGenOp(opNode->NextLevel[0]);
			codeGenOp(opNode->NextLevel[1]);		
			return;	
		}
		
		if(!strcmp(opNode->NType, "EndBlock"))
		{
			switch(opNode->noOps)
			{
				case 0 : 
				{
					break;
				}
				case 1 : 
				{
					codeGenOp(opNode->NextLevel[0]);
					break;
				}
			}
			return;
		}
		
		if(!strcmp(opNode->NType, "ListIndex"))
		{
			printf("T%d = %s[%s]\n", opNode->nodeNo, opNode->NextLevel[0]->id->name, opNode->NextLevel[1]->id->name);
			makeQ(makeStr(opNode->nodeNo, 1), opNode->NextLevel[0]->id->name, opNode->NextLevel[1]->id->name, "=[]");
			return;
		}
		
		if(checkIfBinOperator(opNode->NType)==1)
		{
			codeGenOp(opNode->NextLevel[0]);
			codeGenOp(opNode->NextLevel[1]);
			char *X1 = (char*)malloc(10);
			char *X2 = (char*)malloc(10);
			char *X3 = (char*)malloc(10);
			
			strcpy(X1, makeStr(opNode->nodeNo, 1));
			strcpy(X2, makeStr(opNode->NextLevel[0]->nodeNo, 1));
			strcpy(X3, makeStr(opNode->NextLevel[1]->nodeNo, 1));

			printf("T%d = T%d %s T%d\n", opNode->nodeNo, opNode->NextLevel[0]->nodeNo, opNode->NType, opNode->NextLevel[1]->nodeNo);
			makeQ(X1, X2, X3, opNode->NType);
			free(X1);
			free(X2);
			free(X3);
			return;
		}
		
		if(!strcmp(opNode->NType, "-"))
		{
			if(opNode->noOps == 1)
			{
				codeGenOp(opNode->NextLevel[0]);
				char *X1 = (char*)malloc(10);
				char *X2 = (char*)malloc(10);
				strcpy(X1, makeStr(opNode->nodeNo, 1));
				strcpy(X2, makeStr(opNode->NextLevel[0]->nodeNo, 1));
				printf("T%d = %s T%d\n", opNode->nodeNo, opNode->NType, opNode->NextLevel[0]->nodeNo);
				makeQ(X1, X2, "-", opNode->NType);	
			}
			
			else
			{
				codeGenOp(opNode->NextLevel[0]);
				codeGenOp(opNode->NextLevel[1]);
				char *X1 = (char*)malloc(10);
				char *X2 = (char*)malloc(10);
				char *X3 = (char*)malloc(10);
			
				strcpy(X1, makeStr(opNode->nodeNo, 1));
				strcpy(X2, makeStr(opNode->NextLevel[0]->nodeNo, 1));
				strcpy(X3, makeStr(opNode->NextLevel[1]->nodeNo, 1));

				printf("T%d = T%d %s T%d\n", opNode->nodeNo, opNode->NextLevel[0]->nodeNo, opNode->NType, opNode->NextLevel[1]->nodeNo);
				makeQ(X1, X2, X3, opNode->NType);
				free(X1);
				free(X2);
				free(X3);
				return;
			
			}
		}
		
		if(!strcmp(opNode->NType, "import"))
		{
			printf("import %s\n", opNode->NextLevel[0]->id->name);
			makeQ("-", opNode->NextLevel[0]->id->name, "-", "import");
			return;
		}
		
		if(!strcmp(opNode->NType, "NewLine"))
		{
			codeGenOp(opNode->NextLevel[0]);
			codeGenOp(opNode->NextLevel[1]);
			return;
		}
		
		if(!strcmp(opNode->NType, "="))
		{
			codeGenOp(opNode->NextLevel[1]);
			printf("%s = T%d\n", opNode->NextLevel[0]->id->name, opNode->NextLevel[1]->nodeNo);
			makeQ(opNode->NextLevel[0]->id->name, makeStr(opNode->NextLevel[1]->nodeNo, 1), "-", opNode->NType);
			return;
		}
		
		if(!strcmp(opNode->NType, "Func_Name"))
		{
			printf("Begin Function %s\n", opNode->NextLevel[0]->id->name);
			makeQ("-", opNode->NextLevel[0]->id->name, "-", "BeginF");
			codeGenOp(opNode->NextLevel[2]);
			printf("End Function %s\n", opNode->NextLevel[0]->id->name);
			makeQ("-", opNode->NextLevel[0]->id->name, "-", "EndF");
			return;
		}
		
		if(!strcmp(opNode->NType, "Func_Call"))
		{
			if(!strcmp(opNode->NextLevel[1]->NType, "Void"))
			{
				printf("(T%d)Call Function %s\n", opNode->nodeNo, opNode->NextLevel[0]->id->name);
				makeQ(makeStr(opNode->nodeNo, 1), opNode->NextLevel[0]->id->name, "-", "Call");
			}
			else
			{
				char A[10];
				char* token = strtok(opNode->NextLevel[1]->NType, ","); 
  			int i = 0;
				while (token != NULL) 
				{
						i++; 
				    printf("Push Param %s\n", token);
				    makeQ("-", token, "-", "Param"); 
				    token = strtok(NULL, ","); 
				}
				
				printf("(T%d)Call Function %s, %d\n", opNode->nodeNo, opNode->NextLevel[0]->id->name, i);
				sprintf(A, "%d", i);
				makeQ(makeStr(opNode->nodeNo, 1), opNode->NextLevel[0]->id->name, A, "Call");
				printf("Pop Params for Function %s, %d\n", opNode->NextLevel[0]->id->name, i);
								
				return;
			}
		}		
		
		if(!(strcmp(opNode->NType, "Print")))
		{
			codeGenOp(opNode->NextLevel[0]);
			printf("Print T%d\n", opNode->NextLevel[0]->nodeNo);
			makeQ("-", makeStr(opNode->nodeNo, 1), "-", "Print");
		}
		if(opNode->noOps == 0)
		{
			if(!strcmp(opNode->NType, "break"))
			{
				printf("goto L%d\n", lIndex);
				makeQ(makeStr(lIndex, 0), "-", "-", "goto");
			}

			if(!strcmp(opNode->NType, "pass"))
			{
				makeQ("-", "-", "-", "pass");
			}

			if(!strcmp(opNode->NType, "return"))
			{
				printf("return\n");
				makeQ("-", "-", "-", "return");
			}
		}
		
		
	}
	
  node *createID_Const(char *type, char *value, int scope)
  {
    node *newNode;
    newNode = (node*)calloc(1, sizeof(node));
    newNode->NType = NULL;
    newNode->noOps = -1;
    newNode->id = findRecord(value, type, scope);
    newNode->nodeNo = nodeCount++;
    return newNode;
  }

  node *createOp(char *oper, int noOps, ...)
  {
  
    va_list params;
    node *newNode;
    int i;
    newNode = (node*)calloc(1, sizeof(node));
    
    newNode->NextLevel = (node**)calloc(noOps, sizeof(node*));
    
    newNode->NType = (char*)malloc(strlen(oper)+1);
    strcpy(newNode->NType, oper);
    newNode->noOps = noOps;
    va_start(params, noOps);
    
    for (i = 0; i < noOps; i++)
      newNode->NextLevel[i] = va_arg(params, node*);
    
    va_end(params);
    newNode->nodeNo = nodeCount++;
    return newNode;
  }
  
  void addToList(char *newVal, int flag)
  {
  	if(flag==0)
  	{
		  strcat(argsList, ", ");
		  strcat(argsList, newVal);
		}
		else
		{
			strcat(argsList, newVal);
		}
    //printf("\n\t%s\n", newVal);
  }
  
  void clearArgsList()
  {
    strcpy(argsList, "");
  }
  
	int power(int base, int exp)
	{
		int i, res = 1;
		for(i=0; i<exp; i++)
		{
			res *= base;
		}
		return res;
	}
	
	void updateCScope(int scope)
	{
		currentScope = scope;
	}
	
	void resetDepth()
	{
		while(top()) pop();
		depth = 10;
	}
	
	int scopeBasedTableSearch(int scope)
	{
		int i;
		for(i = sIndex; i > -1; i--)
		{
			if(symbolTables[i].scope == scope)
			{
				return i;
			}
		}
		return -1;
	}
	
	void initNewTable(int scope)
	{
		arrayScope[scope]++;
		sIndex++;
		symbolTables[sIndex].no = sIndex;
		symbolTables[sIndex].scope = power(scope, arrayScope[scope]);
		symbolTables[sIndex].noOfElems = 0;		
		symbolTables[sIndex].Elements = (record*)calloc(MAXRECST, sizeof(record));
		
		symbolTables[sIndex].Parent = scopeBasedTableSearch(currentScope); 
	}
	
	void init()
	{
		int i = 0;
		symbolTables = (STable*)calloc(MAXST, sizeof(STable));
		arrayScope = (int*)calloc(10, sizeof(int));
		initNewTable(1);
		argsList = (char *)malloc(100);
		strcpy(argsList, "");
		tString = (char*)calloc(10, sizeof(char));
		lString = (char*)calloc(10, sizeof(char));
		allQ = (Quad*)calloc(MAXQUADS, sizeof(Quad));
		
		levelIndices = (int*)calloc(MAXLEVELS, sizeof(int));
		Tree = (node***)calloc(MAXLEVELS, sizeof(node**));
		for(i = 0; i<MAXLEVELS; i++)
		{
			Tree[i] = (node**)calloc(MAXCHILDREN, sizeof(node*));
		}
	}

	int searchRecordInScope(const char* type, const char *name, int index)
	{
		int i =0;
		for(i=0; i<symbolTables[index].noOfElems; i++)
		{
			if((strcmp(symbolTables[index].Elements[i].type, type)==0) && (strcmp(symbolTables[index].Elements[i].name, name)==0))
			{
				return i;
			}	
		}
		return -1;
	}
		
	void modifyRecordID(const char *type, const char *name, int lineNo, int scope)
	{
		int i =0;
		int index = scopeBasedTableSearch(scope);
		if(index==0)
		{
			for(i=0; i<symbolTables[index].noOfElems; i++)
			{
				
				if(strcmp(symbolTables[index].Elements[i].type, type)==0 && (strcmp(symbolTables[index].Elements[i].name, name)==0))
				{
					symbolTables[index].Elements[i].lastUseLine = lineNo;
					return;
				}	
			}
			printf("%s '%s' at line %d Not Declared\n", type, name, yylineno);
			exit(1);
		}
		
		for(i=0; i<symbolTables[index].noOfElems; i++)
		{
			if(strcmp(symbolTables[index].Elements[i].type, type)==0 && (strcmp(symbolTables[index].Elements[i].name, name)==0))
			{
				symbolTables[index].Elements[i].lastUseLine = lineNo;
				return;
			}	
		}
		return modifyRecordID(type, name, lineNo, symbolTables[symbolTables[index].Parent].scope);
	}
	
	void insertRecord(const char* type, const char *name, int lineNo, int scope)
	{ 
		int FScope = power(scope, arrayScope[scope]);
		int index = scopeBasedTableSearch(FScope);
		int recordIndex = searchRecordInScope(type, name, index);
		if(recordIndex==-1)
		{
			
			symbolTables[index].Elements[symbolTables[index].noOfElems].type = (char*)calloc(30, sizeof(char));
			symbolTables[index].Elements[symbolTables[index].noOfElems].name = (char*)calloc(20, sizeof(char));
		
			strcpy(symbolTables[index].Elements[symbolTables[index].noOfElems].type, type);	
			strcpy(symbolTables[index].Elements[symbolTables[index].noOfElems].name, name);
			symbolTables[index].Elements[symbolTables[index].noOfElems].decLineNo = lineNo;
			symbolTables[index].Elements[symbolTables[index].noOfElems].lastUseLine = lineNo;
			symbolTables[index].noOfElems++;

		}
		else
		{
			symbolTables[index].Elements[recordIndex].lastUseLine = lineNo;
		}
	}
	
	void checkList(const char *name, int lineNo, int scope)
	{
		int index = scopeBasedTableSearch(scope);
		int i;
		if(index==0)
		{
			
			for(i=0; i<symbolTables[index].noOfElems; i++)
			{
				
				if(strcmp(symbolTables[index].Elements[i].type, "ListTypeID")==0 && (strcmp(symbolTables[index].Elements[i].name, name)==0))
				{
					symbolTables[index].Elements[i].lastUseLine = lineNo;
					return;
				}	

				else if(strcmp(symbolTables[index].Elements[i].name, name)==0)
				{
					printf("Identifier '%s' at line %d Not Indexable\n", name, yylineno);
					exit(1);

				}

			}
			printf("Identifier '%s' at line %d Not Indexable\n", name, yylineno);
			exit(1);
		}
		
		for(i=0; i<symbolTables[index].noOfElems; i++)
		{
			if(strcmp(symbolTables[index].Elements[i].type, "ListTypeID")==0 && (strcmp(symbolTables[index].Elements[i].name, name)==0))
			{
				symbolTables[index].Elements[i].lastUseLine = lineNo;
				return;
			}
			
			else if(strcmp(symbolTables[index].Elements[i].name, name)==0)
			{
				printf("Identifier '%s' at line %d Not Indexable\n", name, yylineno);
				exit(1);

			}
		}
		
		return checkList(name, lineNo, symbolTables[symbolTables[index].Parent].scope);

	}
	
	record* findRecord(const char *name, const char *type, int scope)
	{
		int i =0;
		int index = scopeBasedTableSearch(scope);
		//printf("FR: %d, %s\n", scope, name);
		if(index==0)
		{
			for(i=0; i<symbolTables[index].noOfElems; i++)
			{
				
				if(strcmp(symbolTables[index].Elements[i].type, type)==0 && (strcmp(symbolTables[index].Elements[i].name, name)==0))
				{
					return &(symbolTables[index].Elements[i]);
				}	
			}
			printf("\n%s '%s' at line %d Not Found in Symbol Table\n", type, name, yylineno);
			exit(1);
		}
		
		for(i=0; i<symbolTables[index].noOfElems; i++)
		{
			if(strcmp(symbolTables[index].Elements[i].type, type)==0 && (strcmp(symbolTables[index].Elements[i].name, name)==0))
			{
				return &(symbolTables[index].Elements[i]);
			}	
		}
		return findRecord(name, type, symbolTables[symbolTables[index].Parent].scope);
	}

	void printSTable()
	{
		int i = 0, j = 0;
		
		printf("\n----------------------------All Symbol Tables----------------------------");
		printf("\nScope\tName\tType\t\tDeclaration\tLast Used Line\n");
		for(i=0; i<=sIndex; i++)
		{
			for(j=0; j<symbolTables[i].noOfElems; j++)
			{
				printf("(%d, %d)\t%s\t%s\t%d\t\t%d\n", symbolTables[i].Parent, symbolTables[i].scope, symbolTables[i].Elements[j].name, symbolTables[i].Elements[j].type, symbolTables[i].Elements[j].decLineNo,  symbolTables[i].Elements[j].lastUseLine);
			}
		}
		
		printf("-------------------------------------------------------------------------\n");
		
	}
	
	void ASTToArray(node *root, int level)
	{
	  if(root == NULL )
	  {
	    return;
	  }
	  
	  if(root->noOps <= 0)
	  {
	  	Tree[level][levelIndices[level]] = root;
	  	levelIndices[level]++;
	  }
	  
	  if(root->noOps > 0)
	  {
	 		int j;
	 		Tree[level][levelIndices[level]] = root;
	 		levelIndices[level]++; 
	    for(j=0; j<root->noOps; j++)
	    {
	    	ASTToArray(root->NextLevel[j], level+1);
	    }
	  }
	}
	
	void printAST(node *root)
	{
		printf("\n-------------------------Abstract Syntax Tree--------------------------\n");
		ASTToArray(root, 0);
		int j = 0, p, q, maxLevel = 0, lCount = 0;
		
		while(levelIndices[maxLevel] > 0) maxLevel++;
		
		while(levelIndices[j] > 0)
		{
			for(q=0; q<lCount; q++)
			{
				printf(" ");
			
			}
			for(p=0; p<levelIndices[j] ; p++)
			{
				if(Tree[j][p]->noOps == -1)
				{
					printf("%s  ", Tree[j][p]->id->name);
					lCount+=strlen(Tree[j][p]->id->name);
				}
				else if(Tree[j][p]->noOps == 0)
				{
					printf("%s  ", Tree[j][p]->NType);
					lCount+=strlen(Tree[j][p]->NType);
				}
				else
				{
					printf("%s(%d) ", Tree[j][p]->NType, Tree[j][p]->noOps);
				}
			}
			j++;
			printf("\n");
		}
	}
	
	int IsValidNumber(char * string)
	{
   for(int i = 0; i < strlen( string ); i ++)
   {
      //ASCII value of 0 = 48, 9 = 57. So if value is outside of numeric range then fail
      //Checking for negative sign "-" could be added: ASCII value 45.
      if (string[i] < 48 || string[i] > 57)
         return 0;
   }
 
   return 1;
	}
	
	int deadCodeElimination()
	{
		int i = 0, j = 0, flag = 1, XF=0;
		while(flag==1)
		{
			
			flag=0;
			for(i=0; i<qIndex; i++)
			{
				XF=0;
				if(!((strcmp(allQ[i].R, "-")==0) | (strcmp(allQ[i].Op, "Call")==0) | (strcmp(allQ[i].Op, "Label")==0) | (strcmp(allQ[i].Op, "goto")==0) | (strcmp(allQ[i].Op, "If False")==0)))
				{
					for(j=0; j<qIndex; j++)
					{
							if(((strcmp(allQ[i].R, allQ[j].A1)==0) && (allQ[j].I!=-1)) | ((strcmp(allQ[i].R, allQ[j].A2)==0) && (allQ[j].I!=-1)))
							{
								XF=1;
							}
					}
				
					if((XF==0) & (allQ[i].I != -1))
					{
						allQ[i].I = -1;
						flag=1;
					}
				}
			}
		}
		return flag;
	}
	
	void printQuads()
	{
		printf("\n--------------------------------All Quads---------------------------------\n");
		int i = 0;
		for(i=0; i<qIndex; i++)
		{
			if(allQ[i].I > -1)
				printf("%d\t%s\t%s\t%s\t%s\n", allQ[i].I, allQ[i].Op, allQ[i].A1, allQ[i].A2, allQ[i].R);
		}
		printf("--------------------------------------------------------------------------\n");
	}
	
	void freeAll()
	{
		deadCodeElimination();
		printQuads();
		printf("\n");
		int i = 0, j = 0;
		for(i=0; i<=sIndex; i++)
		{
			for(j=0; j<symbolTables[i].noOfElems; j++)
			{
				free(symbolTables[i].Elements[j].name);
				free(symbolTables[i].Elements[j].type);	
			}
			free(symbolTables[i].Elements);
		}
		free(symbolTables);
		free(allQ);
	}

#line 918 "grammar.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "grammar.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_T_EndOfFile = 3,                /* T_EndOfFile  */
  YYSYMBOL_T_Return = 4,                   /* T_Return  */
  YYSYMBOL_T_Number = 5,                   /* T_Number  */
  YYSYMBOL_T_True = 6,                     /* T_True  */
  YYSYMBOL_T_False = 7,                    /* T_False  */
  YYSYMBOL_T_ID = 8,                       /* T_ID  */
  YYSYMBOL_T_Print = 9,                    /* T_Print  */
  YYSYMBOL_T_Cln = 10,                     /* T_Cln  */
  YYSYMBOL_T_NL = 11,                      /* T_NL  */
  YYSYMBOL_T_EQL = 12,                     /* T_EQL  */
  YYSYMBOL_T_NEQ = 13,                     /* T_NEQ  */
  YYSYMBOL_T_EQ = 14,                      /* T_EQ  */
  YYSYMBOL_T_GT = 15,                      /* T_GT  */
  YYSYMBOL_T_LT = 16,                      /* T_LT  */
  YYSYMBOL_T_EGT = 17,                     /* T_EGT  */
  YYSYMBOL_T_ELT = 18,                     /* T_ELT  */
  YYSYMBOL_T_Or = 19,                      /* T_Or  */
  YYSYMBOL_T_And = 20,                     /* T_And  */
  YYSYMBOL_T_Not = 21,                     /* T_Not  */
  YYSYMBOL_T_In = 22,                      /* T_In  */
  YYSYMBOL_ID = 23,                        /* ID  */
  YYSYMBOL_ND = 24,                        /* ND  */
  YYSYMBOL_DD = 25,                        /* DD  */
  YYSYMBOL_T_String = 26,                  /* T_String  */
  YYSYMBOL_T_If = 27,                      /* T_If  */
  YYSYMBOL_T_Elif = 28,                    /* T_Elif  */
  YYSYMBOL_T_While = 29,                   /* T_While  */
  YYSYMBOL_T_Else = 30,                    /* T_Else  */
  YYSYMBOL_T_Import = 31,                  /* T_Import  */
  YYSYMBOL_T_Break = 32,                   /* T_Break  */
  YYSYMBOL_T_Pass = 33,                    /* T_Pass  */
  YYSYMBOL_T_MN = 34,                      /* T_MN  */
  YYSYMBOL_T_PL = 35,                      /* T_PL  */
  YYSYMBOL_T_DV = 36,                      /* T_DV  */
  YYSYMBOL_T_ML = 37,                      /* T_ML  */
  YYSYMBOL_T_OP = 38,                      /* T_OP  */
  YYSYMBOL_T_CP = 39,                      /* T_CP  */
  YYSYMBOL_T_OB = 40,                      /* T_OB  */
  YYSYMBOL_T_CB = 41,                      /* T_CB  */
  YYSYMBOL_T_Def = 42,                     /* T_Def  */
  YYSYMBOL_T_Comma = 43,                   /* T_Comma  */
  YYSYMBOL_T_List = 44,                    /* T_List  */
  YYSYMBOL_YYACCEPT = 45,                  /* $accept  */
  YYSYMBOL_StartDebugger = 46,             /* StartDebugger  */
  YYSYMBOL_47_1 = 47,                      /* $@1  */
  YYSYMBOL_constant = 48,                  /* constant  */
  YYSYMBOL_term = 49,                      /* term  */
  YYSYMBOL_list_index = 50,                /* list_index  */
  YYSYMBOL_StartParse = 51,                /* StartParse  */
  YYSYMBOL_52_2 = 52,                      /* $@2  */
  YYSYMBOL_basic_stmt = 53,                /* basic_stmt  */
  YYSYMBOL_arith_exp = 54,                 /* arith_exp  */
  YYSYMBOL_bool_exp = 55,                  /* bool_exp  */
  YYSYMBOL_bool_term = 56,                 /* bool_term  */
  YYSYMBOL_bool_factor = 57,               /* bool_factor  */
  YYSYMBOL_import_stmt = 58,               /* import_stmt  */
  YYSYMBOL_pass_stmt = 59,                 /* pass_stmt  */
  YYSYMBOL_break_stmt = 60,                /* break_stmt  */
  YYSYMBOL_return_stmt = 61,               /* return_stmt  */
  YYSYMBOL_assign_stmt = 62,               /* assign_stmt  */
  YYSYMBOL_print_stmt = 63,                /* print_stmt  */
  YYSYMBOL_finalStatements = 64,           /* finalStatements  */
  YYSYMBOL_cmpd_stmt = 65,                 /* cmpd_stmt  */
  YYSYMBOL_if_stmt = 66,                   /* if_stmt  */
  YYSYMBOL_elif_stmts = 67,                /* elif_stmts  */
  YYSYMBOL_else_stmt = 68,                 /* else_stmt  */
  YYSYMBOL_while_stmt = 69,                /* while_stmt  */
  YYSYMBOL_start_suite = 70,               /* start_suite  */
  YYSYMBOL_71_3 = 71,                      /* $@3  */
  YYSYMBOL_suite = 72,                     /* suite  */
  YYSYMBOL_end_suite = 73,                 /* end_suite  */
  YYSYMBOL_74_4 = 74,                      /* $@4  */
  YYSYMBOL_75_5 = 75,                      /* $@5  */
  YYSYMBOL_args = 76,                      /* args  */
  YYSYMBOL_77_6 = 77,                      /* $@6  */
  YYSYMBOL_args_list = 78,                 /* args_list  */
  YYSYMBOL_79_7 = 79,                      /* $@7  */
  YYSYMBOL_call_list = 80,                 /* call_list  */
  YYSYMBOL_81_8 = 81,                      /* $@8  */
  YYSYMBOL_call_args = 82,                 /* call_args  */
  YYSYMBOL_83_9 = 83,                      /* $@9  */
  YYSYMBOL_84_10 = 84,                     /* $@10  */
  YYSYMBOL_85_11 = 85,                     /* $@11  */
  YYSYMBOL_func_def = 86,                  /* func_def  */
  YYSYMBOL_87_12 = 87,                     /* $@12  */
  YYSYMBOL_func_call = 88                  /* func_call  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE) \
             + YYSIZEOF (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   239

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  45
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  44
/* YYNRULES -- Number of rules.  */
#define YYNRULES  93
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  161

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   299


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   864,   864,   864,   866,   867,   869,   870,   871,   874,
     876,   876,   876,   876,   878,   879,   880,   881,   882,   883,
     884,   885,   887,   888,   889,   890,   891,   892,   893,   896,
     897,   898,   899,   900,   901,   902,   903,   905,   906,   907,
     908,   910,   911,   913,   914,   915,   916,   918,   919,   920,
     921,   923,   925,   926,   927,   928,   929,   931,   932,   935,
     936,   938,   939,   941,   943,   945,   946,   946,   948,   949,
     951,   951,   952,   952,   953,   955,   955,   956,   958,   958,
     958,   961,   961,   961,   963,   963,   964,   964,   965,   965,
     966,   968,   968,   970
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "T_EndOfFile",
  "T_Return", "T_Number", "T_True", "T_False", "T_ID", "T_Print", "T_Cln",
  "T_NL", "T_EQL", "T_NEQ", "T_EQ", "T_GT", "T_LT", "T_EGT", "T_ELT",
  "T_Or", "T_And", "T_Not", "T_In", "ID", "ND", "DD", "T_String", "T_If",
  "T_Elif", "T_While", "T_Else", "T_Import", "T_Break", "T_Pass", "T_MN",
  "T_PL", "T_DV", "T_ML", "T_OP", "T_CP", "T_OB", "T_CB", "T_Def",
  "T_Comma", "T_List", "$accept", "StartDebugger", "$@1", "constant",
  "term", "list_index", "StartParse", "$@2", "basic_stmt", "arith_exp",
  "bool_exp", "bool_term", "bool_factor", "import_stmt", "pass_stmt",
  "break_stmt", "return_stmt", "assign_stmt", "print_stmt",
  "finalStatements", "cmpd_stmt", "if_stmt", "elif_stmts", "else_stmt",
  "while_stmt", "start_suite", "$@3", "suite", "end_suite", "$@4", "$@5",
  "args", "$@6", "args_list", "$@7", "call_list", "$@8", "call_args",
  "$@9", "$@10", "$@11", "func_def", "$@12", "func_call", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-117)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-73)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -117,    17,    70,  -117,    22,  -117,  -117,  -117,  -117,     3,
      15,    70,    -2,  -117,   168,   168,    55,  -117,  -117,   145,
     168,    60,  -117,  -117,  -117,    83,  -117,   202,  -117,     2,
    -117,  -117,  -117,  -117,  -117,  -117,  -117,    61,  -117,  -117,
    -117,  -117,  -117,  -117,   122,    59,    57,    79,  -117,   168,
    -117,    42,   202,    85,    90,  -117,   145,    52,   193,    71,
    -117,  -117,   145,   145,   145,   145,   145,   101,   145,   145,
     145,   145,   168,   168,   112,   -20,    75,   202,  -117,  -117,
    -117,  -117,  -117,    92,    91,    94,   159,   159,    -8,  -117,
    -117,    97,    89,    89,    89,    89,    89,  -117,    52,    52,
    -117,  -117,    10,  -117,  -117,    70,  -117,    93,    93,    93,
    -117,  -117,  -117,    -1,   114,  -117,    -5,  -117,   130,  -117,
      79,  -117,  -117,  -117,  -117,   168,   131,  -117,  -117,  -117,
     110,  -117,   113,   142,   159,   111,   147,    93,   148,   159,
    -117,   150,  -117,   159,  -117,    69,  -117,    -5,  -117,  -117,
     113,    62,  -117,  -117,   111,   148,   113,  -117,  -117,  -117,
    -117
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       2,     0,     0,     1,     0,    46,     4,    39,    40,     6,
       0,     0,     0,     5,     0,     0,     0,    45,    44,     0,
       0,     0,     7,    22,     8,     0,    52,    18,    19,    36,
      37,    16,    14,    15,    21,    17,    20,     0,    53,    57,
      58,    54,    55,    56,     0,    90,     0,     0,    10,     0,
      41,     6,     0,     0,     0,    43,     0,    27,     0,     0,
      91,     3,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    11,     6,     0,    47,    48,    49,
      86,    84,    88,     0,     0,     0,     0,     0,     0,    28,
      42,     0,    38,    32,    30,    34,    33,    35,    24,    23,
      26,    25,     0,    29,    31,     0,    50,    83,    83,    83,
      93,     9,    51,     6,     0,    65,    59,    64,    77,    12,
       0,    87,    85,    89,    66,     0,     0,    60,    61,    75,
       0,    81,     0,     0,     0,    80,     0,    83,     0,     0,
      63,     0,    76,     0,    82,    74,    67,     0,    78,    92,
       0,    70,    69,    62,    80,     0,     0,    73,    79,    68,
      71
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -117,  -117,  -117,   115,   -40,  -117,    -7,  -117,   -74,   -14,
     -12,    34,   157,  -117,  -117,  -117,  -117,  -117,  -117,  -116,
    -117,  -117,    25,  -117,  -117,   -73,  -117,    23,  -117,  -117,
    -117,  -117,  -117,    27,  -117,   -99,  -117,  -117,  -117,  -117,
    -117,  -117,  -117,   133
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     1,     2,    22,    23,    24,    25,   105,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,   127,   128,    40,   116,   132,   146,   152,   156,
     157,   130,   135,   142,   154,   121,   137,    83,   108,   107,
     109,    41,    91,    42
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      52,    52,    53,    54,    48,    57,    58,    85,    59,   122,
     123,    44,   115,   115,   117,    44,   138,     3,    45,    12,
      46,    72,    73,   125,    62,   126,    68,    69,    70,    71,
      77,    89,    78,    43,   155,    52,    49,    59,   144,    46,
     160,    45,    88,    46,    68,    69,    70,    71,    92,    93,
      94,    95,    96,    47,    98,    99,   100,   101,   102,   102,
     115,   140,     6,    55,    80,   115,   147,    81,    60,   115,
     149,     4,    74,   -72,     5,     6,     7,     8,     9,    10,
     131,    11,    46,    13,     6,    82,    61,    51,    70,    71,
     -72,    12,   -72,   150,   151,    86,    13,    14,   119,    15,
      87,    16,    17,    18,    19,    13,   103,   104,    20,    97,
      90,    52,    21,   133,     4,   -13,   106,     5,     6,     7,
       8,     9,    10,    68,    69,    70,    71,     6,     7,     8,
      75,   110,   111,   112,    12,   118,   120,   124,   129,    13,
      14,   134,    15,    12,    16,    17,    18,    19,    13,   136,
       6,    20,   139,    51,   141,    21,    19,   143,   148,   145,
      20,    84,    76,     5,     6,     7,     8,   113,    10,    50,
     114,    13,   153,     6,     7,     8,    51,    79,   159,    19,
      12,   158,     0,    56,     0,    13,     0,     0,     0,    12,
      16,    17,    18,    19,    13,     0,     0,    20,     0,     0,
       0,     0,    19,     0,     0,     0,    20,    62,    63,    64,
      65,    66,     0,     0,     0,    67,    62,    63,    64,    65,
      66,     0,     0,     0,    67,     0,     0,    68,    69,    70,
      71,     0,    89,     0,     0,     0,    68,    69,    70,    71
};

static const yytype_int16 yycheck[] =
{
      14,    15,    14,    15,    11,    19,    20,    47,    20,   108,
     109,    12,    86,    87,    87,    12,   132,     0,    38,    21,
      40,    19,    20,    28,    14,    30,    34,    35,    36,    37,
      44,    39,    44,    11,   150,    49,    38,    49,   137,    40,
     156,    38,    56,    40,    34,    35,    36,    37,    62,    63,
      64,    65,    66,    38,    68,    69,    70,    71,    72,    73,
     134,   134,     5,     8,     5,   139,   139,     8,     8,   143,
     143,     1,    11,    11,     4,     5,     6,     7,     8,     9,
     120,    11,    40,    26,     5,    26,     3,     8,    36,    37,
      28,    21,    30,    24,    25,    10,    26,    27,   105,    29,
      10,    31,    32,    33,    34,    26,    72,    73,    38,     8,
      39,   125,    42,   125,     1,     3,    41,     4,     5,     6,
       7,     8,     9,    34,    35,    36,    37,     5,     6,     7,
       8,    39,    41,    39,    21,    38,    43,    23,     8,    26,
      27,    10,    29,    21,    31,    32,    33,    34,    26,    39,
       5,    38,    10,     8,    43,    42,    34,    10,     8,    11,
      38,    46,    40,     4,     5,     6,     7,     8,     9,    12,
      11,    26,   147,     5,     6,     7,     8,    44,   155,    34,
      21,   154,    -1,    38,    -1,    26,    -1,    -1,    -1,    21,
      31,    32,    33,    34,    26,    -1,    -1,    38,    -1,    -1,
      -1,    -1,    34,    -1,    -1,    -1,    38,    14,    15,    16,
      17,    18,    -1,    -1,    -1,    22,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    22,    -1,    -1,    34,    35,    36,
      37,    -1,    39,    -1,    -1,    -1,    34,    35,    36,    37
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    46,    47,     0,     1,     4,     5,     6,     7,     8,
       9,    11,    21,    26,    27,    29,    31,    32,    33,    34,
      38,    42,    48,    49,    50,    51,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      69,    86,    88,    11,    12,    38,    40,    38,    51,    38,
      57,     8,    54,    55,    55,     8,    38,    54,    54,    55,
       8,     3,    14,    15,    16,    17,    18,    22,    34,    35,
      36,    37,    19,    20,    11,     8,    40,    54,    55,    88,
       5,     8,    26,    82,    48,    49,    10,    10,    54,    39,
      39,    87,    54,    54,    54,    54,    54,     8,    54,    54,
      54,    54,    54,    56,    56,    52,    41,    84,    83,    85,
      39,    41,    39,     8,    11,    53,    70,    70,    38,    51,
      43,    80,    80,    80,    23,    28,    30,    67,    68,     8,
      76,    49,    71,    55,    10,    77,    39,    81,    64,    10,
      70,    43,    78,    10,    80,    11,    72,    70,     8,    70,
      24,    25,    73,    67,    79,    64,    74,    75,    78,    72,
      64
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    45,    47,    46,    48,    48,    49,    49,    49,    50,
      51,    52,    51,    51,    53,    53,    53,    53,    53,    53,
      53,    53,    54,    54,    54,    54,    54,    54,    54,    55,
      55,    55,    55,    55,    55,    55,    55,    56,    56,    56,
      56,    57,    57,    58,    59,    60,    61,    62,    62,    62,
      62,    63,    64,    64,    64,    64,    64,    65,    65,    66,
      66,    67,    67,    68,    69,    70,    71,    70,    72,    72,
      74,    73,    75,    73,    73,    77,    76,    76,    79,    78,
      78,    81,    80,    80,    83,    82,    84,    82,    85,    82,
      82,    87,    86,    88
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     3,     1,     1,     1,     1,     1,     4,
       2,     0,     4,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     3,     3,     3,     2,     3,     3,
       3,     3,     3,     3,     3,     3,     1,     1,     3,     1,
       1,     2,     3,     2,     1,     1,     1,     3,     3,     3,
       4,     4,     1,     1,     1,     1,     2,     1,     1,     4,
       5,     1,     5,     3,     4,     1,     0,     5,     4,     2,
       0,     3,     0,     2,     0,     0,     3,     0,     0,     4,
       0,     0,     4,     0,     0,     3,     0,     3,     0,     3,
       0,     0,     8,     4
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF

/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)


/* YYLOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

# ifndef YYLOCATION_PRINT

#  if defined YY_LOCATION_PRINT

   /* Temporary convenience wrapper in case some people defined the
      undocumented and private YY_LOCATION_PRINT macros.  */
#   define YYLOCATION_PRINT(File, Loc)  YY_LOCATION_PRINT(File, *(Loc))

#  elif defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static int
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  int res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
}

#   define YYLOCATION_PRINT  yy_location_print_

    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT(File, Loc)  YYLOCATION_PRINT(File, &(Loc))

#  else

#   define YYLOCATION_PRINT(File, Loc) ((void) 0)
    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT  YYLOCATION_PRINT

#  endif
# endif /* !defined YYLOCATION_PRINT */


# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value, Location); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  YY_USE (yylocationp);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  YYLOCATION_PRINT (yyo, yylocationp);
  YYFPRINTF (yyo, ": ");
  yy_symbol_value_print (yyo, yykind, yyvaluep, yylocationp);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)],
                       &(yylsp[(yyi + 1) - (yynrhs)]));
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
{
  YY_USE (yyvaluep);
  YY_USE (yylocationp);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Location data for the lookahead symbol.  */
YYLTYPE yylloc
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

    /* The location stack: array, bottom, top.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls = yylsa;
    YYLTYPE *yylsp = yyls;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

  /* The locations where the error started and ended.  */
  YYLTYPE yyerror_range[3];



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  yylsp[0] = yylloc;
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yyls1, yysize * YYSIZEOF (*yylsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
        yyls = yyls1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      yyerror_range[1] = yylloc;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location. */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  yyerror_range[1] = yyloc;
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* $@1: %empty  */
#line 864 "../Compiler/grammar.y"
                {init();}
#line 2240 "grammar.tab.c"
    break;

  case 3: /* StartDebugger: $@1 StartParse T_EndOfFile  */
#line 864 "../Compiler/grammar.y"
                                                 {printf("\nValid Python Syntax\n");  printAST((yyvsp[-1].node)); codeGenOp((yyvsp[-1].node)); printQuads(); printSTable(); freeAll(); exit(0);}
#line 2246 "grammar.tab.c"
    break;

  case 4: /* constant: T_Number  */
#line 866 "../Compiler/grammar.y"
                    {insertRecord("Constant", (yyvsp[0].text), (yylsp[0]).first_line, currentScope); (yyval.node) = createID_Const("Constant", (yyvsp[0].text), currentScope);}
#line 2252 "grammar.tab.c"
    break;

  case 5: /* constant: T_String  */
#line 867 "../Compiler/grammar.y"
                    {insertRecord("Constant", (yyvsp[0].text), (yylsp[0]).first_line, currentScope); (yyval.node) = createID_Const("Constant", (yyvsp[0].text), currentScope);}
#line 2258 "grammar.tab.c"
    break;

  case 6: /* term: T_ID  */
#line 869 "../Compiler/grammar.y"
            {modifyRecordID("Identifier", (yyvsp[0].text), (yylsp[0]).first_line, currentScope); (yyval.node) = createID_Const("Identifier", (yyvsp[0].text), currentScope);}
#line 2264 "grammar.tab.c"
    break;

  case 7: /* term: constant  */
#line 870 "../Compiler/grammar.y"
                {(yyval.node) = (yyvsp[0].node);}
#line 2270 "grammar.tab.c"
    break;

  case 8: /* term: list_index  */
#line 871 "../Compiler/grammar.y"
                  {(yyval.node) = (yyvsp[0].node);}
#line 2276 "grammar.tab.c"
    break;

  case 9: /* list_index: T_ID T_OB constant T_CB  */
#line 874 "../Compiler/grammar.y"
                                     {checkList((yyvsp[-3].text), (yylsp[-3]).first_line, currentScope); (yyval.node) = createOp("ListIndex", 2, createID_Const("ListTypeID", (yyvsp[-3].text), currentScope), (yyvsp[-1].node));}
#line 2282 "grammar.tab.c"
    break;

  case 10: /* StartParse: T_NL StartParse  */
#line 876 "../Compiler/grammar.y"
                             {(yyval.node)=(yyvsp[0].node);}
#line 2288 "grammar.tab.c"
    break;

  case 11: /* $@2: %empty  */
#line 876 "../Compiler/grammar.y"
                                                            {resetDepth();}
#line 2294 "grammar.tab.c"
    break;

  case 12: /* StartParse: finalStatements T_NL $@2 StartParse  */
#line 876 "../Compiler/grammar.y"
                                                                                       {(yyval.node) = createOp("NewLine", 2, (yyvsp[-3].node), (yyvsp[0].node));}
#line 2300 "grammar.tab.c"
    break;

  case 13: /* StartParse: finalStatements T_NL  */
#line 876 "../Compiler/grammar.y"
                                                                                                                                                    {(yyval.node)=(yyvsp[-1].node);}
#line 2306 "grammar.tab.c"
    break;

  case 14: /* basic_stmt: pass_stmt  */
#line 878 "../Compiler/grammar.y"
                       {(yyval.node)=(yyvsp[0].node);}
#line 2312 "grammar.tab.c"
    break;

  case 15: /* basic_stmt: break_stmt  */
#line 879 "../Compiler/grammar.y"
                        {(yyval.node)=(yyvsp[0].node);}
#line 2318 "grammar.tab.c"
    break;

  case 16: /* basic_stmt: import_stmt  */
#line 880 "../Compiler/grammar.y"
                         {(yyval.node)=(yyvsp[0].node);}
#line 2324 "grammar.tab.c"
    break;

  case 17: /* basic_stmt: assign_stmt  */
#line 881 "../Compiler/grammar.y"
                         {(yyval.node)=(yyvsp[0].node);}
#line 2330 "grammar.tab.c"
    break;

  case 18: /* basic_stmt: arith_exp  */
#line 882 "../Compiler/grammar.y"
                       {(yyval.node)=(yyvsp[0].node);}
#line 2336 "grammar.tab.c"
    break;

  case 19: /* basic_stmt: bool_exp  */
#line 883 "../Compiler/grammar.y"
                      {(yyval.node)=(yyvsp[0].node);}
#line 2342 "grammar.tab.c"
    break;

  case 20: /* basic_stmt: print_stmt  */
#line 884 "../Compiler/grammar.y"
                        {(yyval.node)=(yyvsp[0].node);}
#line 2348 "grammar.tab.c"
    break;

  case 21: /* basic_stmt: return_stmt  */
#line 885 "../Compiler/grammar.y"
                         {(yyval.node)=(yyvsp[0].node);}
#line 2354 "grammar.tab.c"
    break;

  case 22: /* arith_exp: term  */
#line 887 "../Compiler/grammar.y"
                 {(yyval.node)=(yyvsp[0].node);}
#line 2360 "grammar.tab.c"
    break;

  case 23: /* arith_exp: arith_exp T_PL arith_exp  */
#line 888 "../Compiler/grammar.y"
                                       {(yyval.node) = createOp("+", 2, (yyvsp[-2].node), (yyvsp[0].node));}
#line 2366 "grammar.tab.c"
    break;

  case 24: /* arith_exp: arith_exp T_MN arith_exp  */
#line 889 "../Compiler/grammar.y"
                                       {(yyval.node) = createOp("-", 2, (yyvsp[-2].node), (yyvsp[0].node));}
#line 2372 "grammar.tab.c"
    break;

  case 25: /* arith_exp: arith_exp T_ML arith_exp  */
#line 890 "../Compiler/grammar.y"
                                       {(yyval.node) = createOp("*", 2, (yyvsp[-2].node), (yyvsp[0].node));}
#line 2378 "grammar.tab.c"
    break;

  case 26: /* arith_exp: arith_exp T_DV arith_exp  */
#line 891 "../Compiler/grammar.y"
                                       {(yyval.node) = createOp("/", 2, (yyvsp[-2].node), (yyvsp[0].node));}
#line 2384 "grammar.tab.c"
    break;

  case 27: /* arith_exp: T_MN arith_exp  */
#line 892 "../Compiler/grammar.y"
                           {(yyval.node) = createOp("-", 1, (yyvsp[0].node));}
#line 2390 "grammar.tab.c"
    break;

  case 28: /* arith_exp: T_OP arith_exp T_CP  */
#line 893 "../Compiler/grammar.y"
                                {(yyval.node) = (yyvsp[-1].node);}
#line 2396 "grammar.tab.c"
    break;

  case 29: /* bool_exp: bool_term T_Or bool_term  */
#line 896 "../Compiler/grammar.y"
                                    {(yyval.node) = createOp("or", 2, (yyvsp[-2].node), (yyvsp[0].node));}
#line 2402 "grammar.tab.c"
    break;

  case 30: /* bool_exp: arith_exp T_LT arith_exp  */
#line 897 "../Compiler/grammar.y"
                                    {(yyval.node) = createOp("<", 2, (yyvsp[-2].node), (yyvsp[0].node));}
#line 2408 "grammar.tab.c"
    break;

  case 31: /* bool_exp: bool_term T_And bool_term  */
#line 898 "../Compiler/grammar.y"
                                     {(yyval.node) = createOp("and", 2, (yyvsp[-2].node), (yyvsp[0].node));}
#line 2414 "grammar.tab.c"
    break;

  case 32: /* bool_exp: arith_exp T_GT arith_exp  */
#line 899 "../Compiler/grammar.y"
                                    {(yyval.node) = createOp(">", 2, (yyvsp[-2].node), (yyvsp[0].node));}
#line 2420 "grammar.tab.c"
    break;

  case 33: /* bool_exp: arith_exp T_ELT arith_exp  */
#line 900 "../Compiler/grammar.y"
                                     {(yyval.node) = createOp("<=", 2, (yyvsp[-2].node), (yyvsp[0].node));}
#line 2426 "grammar.tab.c"
    break;

  case 34: /* bool_exp: arith_exp T_EGT arith_exp  */
#line 901 "../Compiler/grammar.y"
                                     {(yyval.node) = createOp(">=", 2, (yyvsp[-2].node), (yyvsp[0].node));}
#line 2432 "grammar.tab.c"
    break;

  case 35: /* bool_exp: arith_exp T_In T_ID  */
#line 902 "../Compiler/grammar.y"
                               {checkList((yyvsp[0].text), (yylsp[0]).first_line, currentScope); (yyval.node) = createOp("in", 2, (yyvsp[-2].node), createID_Const("Constant", (yyvsp[0].text), currentScope));}
#line 2438 "grammar.tab.c"
    break;

  case 36: /* bool_exp: bool_term  */
#line 903 "../Compiler/grammar.y"
                     {(yyval.node)=(yyvsp[0].node);}
#line 2444 "grammar.tab.c"
    break;

  case 37: /* bool_term: bool_factor  */
#line 905 "../Compiler/grammar.y"
                        {(yyval.node) = (yyvsp[0].node);}
#line 2450 "grammar.tab.c"
    break;

  case 38: /* bool_term: arith_exp T_EQ arith_exp  */
#line 906 "../Compiler/grammar.y"
                                     {(yyval.node) = createOp("==", 2, (yyvsp[-2].node), (yyvsp[0].node));}
#line 2456 "grammar.tab.c"
    break;

  case 39: /* bool_term: T_True  */
#line 907 "../Compiler/grammar.y"
                   {insertRecord("Constant", "True", (yylsp[0]).first_line, currentScope); (yyval.node) = createID_Const("Constant", "True", currentScope);}
#line 2462 "grammar.tab.c"
    break;

  case 40: /* bool_term: T_False  */
#line 908 "../Compiler/grammar.y"
                    {insertRecord("Constant", "False", (yylsp[0]).first_line, currentScope); (yyval.node) = createID_Const("Constant", "False", currentScope);}
#line 2468 "grammar.tab.c"
    break;

  case 41: /* bool_factor: T_Not bool_factor  */
#line 910 "../Compiler/grammar.y"
                                {(yyval.node) = createOp("!", 1, (yyvsp[0].node));}
#line 2474 "grammar.tab.c"
    break;

  case 42: /* bool_factor: T_OP bool_exp T_CP  */
#line 911 "../Compiler/grammar.y"
                                 {(yyval.node) = (yyvsp[-1].node);}
#line 2480 "grammar.tab.c"
    break;

  case 43: /* import_stmt: T_Import T_ID  */
#line 913 "../Compiler/grammar.y"
                            {insertRecord("PackageName", (yyvsp[0].text), (yylsp[0]).first_line, currentScope); (yyval.node) = createOp("import", 1, createID_Const("PackageName", (yyvsp[0].text), currentScope));}
#line 2486 "grammar.tab.c"
    break;

  case 44: /* pass_stmt: T_Pass  */
#line 914 "../Compiler/grammar.y"
                   {(yyval.node) = createOp("pass", 0);}
#line 2492 "grammar.tab.c"
    break;

  case 45: /* break_stmt: T_Break  */
#line 915 "../Compiler/grammar.y"
                     {(yyval.node) = createOp("break", 0);}
#line 2498 "grammar.tab.c"
    break;

  case 46: /* return_stmt: T_Return  */
#line 916 "../Compiler/grammar.y"
                       {(yyval.node) = createOp("return", 0);}
#line 2504 "grammar.tab.c"
    break;

  case 47: /* assign_stmt: T_ID T_EQL arith_exp  */
#line 918 "../Compiler/grammar.y"
                                   {insertRecord("Identifier", (yyvsp[-2].text), (yylsp[-2]).first_line, currentScope); (yyval.node) = createOp("=", 2, createID_Const("Identifier", (yyvsp[-2].text), currentScope), (yyvsp[0].node));}
#line 2510 "grammar.tab.c"
    break;

  case 48: /* assign_stmt: T_ID T_EQL bool_exp  */
#line 919 "../Compiler/grammar.y"
                                  {insertRecord("Identifier", (yyvsp[-2].text), (yylsp[-2]).first_line, currentScope);(yyval.node) = createOp("=", 2, createID_Const("Identifier", (yyvsp[-2].text), currentScope), (yyvsp[0].node));}
#line 2516 "grammar.tab.c"
    break;

  case 49: /* assign_stmt: T_ID T_EQL func_call  */
#line 920 "../Compiler/grammar.y"
                                    {insertRecord("Identifier", (yyvsp[-2].text), (yylsp[-2]).first_line, currentScope); (yyval.node) = createOp("=", 2, createID_Const("Identifier", (yyvsp[-2].text), currentScope), (yyvsp[0].node));}
#line 2522 "grammar.tab.c"
    break;

  case 50: /* assign_stmt: T_ID T_EQL T_OB T_CB  */
#line 921 "../Compiler/grammar.y"
                                   {insertRecord("ListTypeID", (yyvsp[-3].text), (yylsp[-3]).first_line, currentScope); (yyval.node) = createID_Const("ListTypeID", (yyvsp[-3].text), currentScope);}
#line 2528 "grammar.tab.c"
    break;

  case 51: /* print_stmt: T_Print T_OP term T_CP  */
#line 923 "../Compiler/grammar.y"
                                    {(yyval.node) = createOp("Print", 1, (yyvsp[-1].node));}
#line 2534 "grammar.tab.c"
    break;

  case 52: /* finalStatements: basic_stmt  */
#line 925 "../Compiler/grammar.y"
                             {(yyval.node) = (yyvsp[0].node);}
#line 2540 "grammar.tab.c"
    break;

  case 53: /* finalStatements: cmpd_stmt  */
#line 926 "../Compiler/grammar.y"
                            {(yyval.node) = (yyvsp[0].node);}
#line 2546 "grammar.tab.c"
    break;

  case 54: /* finalStatements: func_def  */
#line 927 "../Compiler/grammar.y"
                           {(yyval.node) = (yyvsp[0].node);}
#line 2552 "grammar.tab.c"
    break;

  case 55: /* finalStatements: func_call  */
#line 928 "../Compiler/grammar.y"
                            {(yyval.node) = (yyvsp[0].node);}
#line 2558 "grammar.tab.c"
    break;

  case 56: /* finalStatements: error T_NL  */
#line 929 "../Compiler/grammar.y"
                             {yyerrok; yyclearin; (yyval.node)=createOp("SyntaxError", 0);}
#line 2564 "grammar.tab.c"
    break;

  case 57: /* cmpd_stmt: if_stmt  */
#line 931 "../Compiler/grammar.y"
                    {(yyval.node) = (yyvsp[0].node);}
#line 2570 "grammar.tab.c"
    break;

  case 58: /* cmpd_stmt: while_stmt  */
#line 932 "../Compiler/grammar.y"
                       {(yyval.node) = (yyvsp[0].node);}
#line 2576 "grammar.tab.c"
    break;

  case 59: /* if_stmt: T_If bool_exp T_Cln start_suite  */
#line 935 "../Compiler/grammar.y"
                                          {(yyval.node) = createOp("If", 2, (yyvsp[-2].node), (yyvsp[0].node));}
#line 2582 "grammar.tab.c"
    break;

  case 60: /* if_stmt: T_If bool_exp T_Cln start_suite elif_stmts  */
#line 936 "../Compiler/grammar.y"
                                                     {(yyval.node) = createOp("If", 3, (yyvsp[-3].node), (yyvsp[-1].node), (yyvsp[0].node));}
#line 2588 "grammar.tab.c"
    break;

  case 61: /* elif_stmts: else_stmt  */
#line 938 "../Compiler/grammar.y"
                       {(yyval.node)= (yyvsp[0].node);}
#line 2594 "grammar.tab.c"
    break;

  case 62: /* elif_stmts: T_Elif bool_exp T_Cln start_suite elif_stmts  */
#line 939 "../Compiler/grammar.y"
                                                          {(yyval.node)= createOp("Elif", 3, (yyvsp[-3].node), (yyvsp[-1].node), (yyvsp[0].node));}
#line 2600 "grammar.tab.c"
    break;

  case 63: /* else_stmt: T_Else T_Cln start_suite  */
#line 941 "../Compiler/grammar.y"
                                     {(yyval.node) = createOp("Else", 1, (yyvsp[0].node));}
#line 2606 "grammar.tab.c"
    break;

  case 64: /* while_stmt: T_While bool_exp T_Cln start_suite  */
#line 943 "../Compiler/grammar.y"
                                                {(yyval.node) = createOp("While", 2, (yyvsp[-2].node), (yyvsp[0].node));}
#line 2612 "grammar.tab.c"
    break;

  case 65: /* start_suite: basic_stmt  */
#line 945 "../Compiler/grammar.y"
                         {(yyval.node) = (yyvsp[0].node);}
#line 2618 "grammar.tab.c"
    break;

  case 66: /* $@3: %empty  */
#line 946 "../Compiler/grammar.y"
                      {initNewTable((yyvsp[0].depth)); updateCScope((yyvsp[0].depth));}
#line 2624 "grammar.tab.c"
    break;

  case 67: /* start_suite: T_NL ID $@3 finalStatements suite  */
#line 946 "../Compiler/grammar.y"
                                                                                                {(yyval.node) = createOp("BeginBlock", 2, (yyvsp[-1].node), (yyvsp[0].node));}
#line 2630 "grammar.tab.c"
    break;

  case 68: /* suite: T_NL ND finalStatements suite  */
#line 948 "../Compiler/grammar.y"
                                      {(yyval.node) = createOp("Next", 2, (yyvsp[-1].node), (yyvsp[0].node));}
#line 2636 "grammar.tab.c"
    break;

  case 69: /* suite: T_NL end_suite  */
#line 949 "../Compiler/grammar.y"
                       {(yyval.node) = (yyvsp[0].node);}
#line 2642 "grammar.tab.c"
    break;

  case 70: /* $@4: %empty  */
#line 951 "../Compiler/grammar.y"
               {updateCScope((yyvsp[0].depth));}
#line 2648 "grammar.tab.c"
    break;

  case 71: /* end_suite: DD $@4 finalStatements  */
#line 951 "../Compiler/grammar.y"
                                                          {(yyval.node) = createOp("EndBlock", 1, (yyvsp[0].node));}
#line 2654 "grammar.tab.c"
    break;

  case 72: /* $@5: %empty  */
#line 952 "../Compiler/grammar.y"
               {updateCScope((yyvsp[0].depth));}
#line 2660 "grammar.tab.c"
    break;

  case 73: /* end_suite: DD $@5  */
#line 952 "../Compiler/grammar.y"
                                          {(yyval.node) = createOp("EndBlock", 0);}
#line 2666 "grammar.tab.c"
    break;

  case 74: /* end_suite: %empty  */
#line 953 "../Compiler/grammar.y"
            {(yyval.node) = createOp("EndBlock", 0); resetDepth();}
#line 2672 "grammar.tab.c"
    break;

  case 75: /* $@6: %empty  */
#line 955 "../Compiler/grammar.y"
            {addToList((yyvsp[0].text), 1);}
#line 2678 "grammar.tab.c"
    break;

  case 76: /* args: T_ID $@6 args_list  */
#line 955 "../Compiler/grammar.y"
                                                {(yyval.node) = createOp(argsList, 0); clearArgsList();}
#line 2684 "grammar.tab.c"
    break;

  case 77: /* args: %empty  */
#line 956 "../Compiler/grammar.y"
       {(yyval.node) = createOp("Void", 0);}
#line 2690 "grammar.tab.c"
    break;

  case 78: /* $@7: %empty  */
#line 958 "../Compiler/grammar.y"
                         {addToList((yyvsp[0].text), 0);}
#line 2696 "grammar.tab.c"
    break;

  case 81: /* $@8: %empty  */
#line 961 "../Compiler/grammar.y"
                         {addToList((yyvsp[-1].text), 0);}
#line 2702 "grammar.tab.c"
    break;

  case 84: /* $@9: %empty  */
#line 963 "../Compiler/grammar.y"
                 {addToList((yyvsp[0].text), 1);}
#line 2708 "grammar.tab.c"
    break;

  case 85: /* call_args: T_ID $@9 call_list  */
#line 963 "../Compiler/grammar.y"
                                                     {(yyval.node) = createOp(argsList, 0); clearArgsList();}
#line 2714 "grammar.tab.c"
    break;

  case 86: /* $@10: %empty  */
#line 964 "../Compiler/grammar.y"
                                                   {addToList((yyvsp[0].text), 1);}
#line 2720 "grammar.tab.c"
    break;

  case 87: /* call_args: T_Number $@10 call_list  */
#line 964 "../Compiler/grammar.y"
                                                                                       {(yyval.node) = createOp(argsList, 0); clearArgsList();}
#line 2726 "grammar.tab.c"
    break;

  case 88: /* $@11: %empty  */
#line 965 "../Compiler/grammar.y"
                                                   {addToList((yyvsp[0].text), 1);}
#line 2732 "grammar.tab.c"
    break;

  case 89: /* call_args: T_String $@11 call_list  */
#line 965 "../Compiler/grammar.y"
                                                                                       {(yyval.node) = createOp(argsList, 0); clearArgsList();}
#line 2738 "grammar.tab.c"
    break;

  case 90: /* call_args: %empty  */
#line 966 "../Compiler/grammar.y"
                                          {(yyval.node) = createOp("Void", 0);}
#line 2744 "grammar.tab.c"
    break;

  case 91: /* $@12: %empty  */
#line 968 "../Compiler/grammar.y"
                      {insertRecord("Func_Name", (yyvsp[0].text), (yylsp[0]).first_line, currentScope);}
#line 2750 "grammar.tab.c"
    break;

  case 92: /* func_def: T_Def T_ID $@12 T_OP args T_CP T_Cln start_suite  */
#line 968 "../Compiler/grammar.y"
                                                                                                                           {(yyval.node) = createOp("Func_Name", 3, createID_Const("Func_Name", (yyvsp[-6].text), currentScope), (yyvsp[-3].node), (yyvsp[0].node));}
#line 2756 "grammar.tab.c"
    break;

  case 93: /* func_call: T_ID T_OP call_args T_CP  */
#line 970 "../Compiler/grammar.y"
                                     {(yyval.node) = createOp("Func_Call", 2, createID_Const("Func_Name", (yyvsp[-3].text), currentScope), (yyvsp[-1].node));}
#line 2762 "grammar.tab.c"
    break;


#line 2766 "grammar.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  yyerror_range[1] = yylloc;
  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, &yylloc);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp, yylsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  ++yylsp;
  YYLLOC_DEFAULT (*yylsp, yyerror_range, 2);

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp, yylsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 973 "../Compiler/grammar.y"




int main()
{
	//printf("Enter the Expression\n");
	yyparse();
	return 0;
}

