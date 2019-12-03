#include<iostream.h>
#include<conio.h>
#include<string.h>
#include<math.h>
#include<stdlib.h>
#include<stdio.h>
#include<Ctype.h>
#include<fstream.h>

#define SIZE 100

char stack1[SIZE];									// Define stack for coverting infix to postfix
int top = -1;

char stack[20];  									// define temporary stack size

ofstream outfile; 									// for setting file in write/output mode
struct var											// Structure defination for using temporary variables or registers which will be used as destination reg during airthmetic expressions
{
	char temp;
	int used;
};

struct r											// Definig the structure of register banks
{
	int value;
	int valid;
	char variable;
};

void findfreeregister(struct r *bank,int *a)		// Function to find the free register
{
	int j=0;
	while(bank[j].valid!=0) j++;
	*a = j;
}

void findmap(char op,struct r *bank,int *a)			// Function to find the mapping of register to variable
{

	int j=0;
	for(j=0;bank[j].variable!=op && j<16;j++);
	if(bank[j].valid==1)
	{
	     *a=j;
	}
}

void findfreevar(struct var *bank, char *a)			// Finding the free temporary variable unused
{
	int j;
	for(j=0;j<3 && bank[j].used !=0 ;j++);
	*a = bank[j].temp;
	bank[j].used=1;
}

int ismapped(char s,struct r *bank)					// To check is a particular register is currently used (mapped)
{
	int j;
	for(j=0;j<16;j++)
	{
		if(bank[j].variable==s&&bank[j].valid==1)
		       {
				return 1;
				exit(0);
		       }
	}
	return 0;
}

char push(char m, int n)   						    // function for stack push
{
stack[n]=m;
return (m);
}

char pop(char m, int n)  							// function for stack pop
{
if(n<1)   // stack empty
cout<<"no operand to evaluate"<<endl;
else
{
m=stack[n];
}
return (m);
}

void calc(char ch, char m, char n)  				//operate when requested on + - * /
{
	
switch (ch)
{
case '+':  cout<<"ADD "; outfile<<"     ADD "; break;
case '-':  cout<<"SUB "; outfile<<"     SUB ";break;
case '*':  cout<<"MUL "; outfile<<"     MUL ";break;
case '/':  cout<<"SDIV "; outfile<<"     SDIV ";break;
default : cout<<"invalid operation request;"<<endl;
}
}

int is_operator(char symbol)					// Function to find given symbol is operator or not
{
	if(symbol == '^' || symbol == '*' || symbol == '/' || symbol == '+' || symbol =='-')
	{
		return 1;
	}
	else
	{
	return 0;
	}
}

/* define fucntion that is used to assign precendence to operator.
* Here ^ denotes exponent operator.
* In this fucntion we assume that higher integer value
* means higher precendence */

int precedence(char symbol)
{
	if(symbol == '^')/* exponent operator, highest precedence*/
	{
		return(3);
	}
	else if(symbol == '*' || symbol == '/')
	{
		return(2);
	}
	else if(symbol == '+' || symbol == '-')          /* lowest precedence */
	{
		return(1);
	}
	else
	{
		return(0);
	}
}

/* define push operation */

void push1(char item)
{
	if(top >= SIZE-1)
	{
		printf("\nStack Overflow.");
	}
	else
	{
		top = top+1;
		stack1[top] = item;
	}
}

/* define pop operation */
char pop1()
{
	char item ;

	if(top <0)
	{
		printf("stack under flow: invalid infix expression");
		getchar();
		/* underflow may occur for invalid expression */
		/* where ( and ) are not matched */
		exit(1);
	}
	else
	{
		item = stack1[top];
		top = top-1;
		return(item);
	}
}

void InfixToPostfix(char infix_exp[], char postfix_exp[])
{
	int i, j;
	char item;
	char x;
	push1('(');                              	 /* push '(' onto stack */
	strcat(infix_exp,")");                  	/* add ')' to infix expression */

	i=0;
	j=0;
	item=infix_exp[i];         					/* initialize before loop*/

	while(item != '\0')        					/* run loop till end of infix expression */
	{
		if(item == '(')
		{
			push1(item);
		}
		else if( isdigit(item) || isalpha(item))
		{
			postfix_exp[j] = item;              /* add operand symbol to postfix expr */
			j++;
		}
		else if(is_operator(item) == 1)        /* means symbol is operator */
		{
			x=pop1();
			while(is_operator(x) == 1 && precedence(x)>= precedence(item))
			{
				postfix_exp[j] = x;                  /* so pop all higher precendence operator and */
				j++;
				x = pop1();                       /* add them to postfix expresion */
			}
			push1(x);
			/* because just above while loop will terminate we have
			oppped one extra item
			for which condition fails and loop terminates, so that one*/

			push1(item);                 		/* push current oprerator symbol onto stack */
		}
		else if(item == ')')         			/* if current symbol is ')' then */
		{
			x = pop1();                   /* pop and keep popping until */
			while(x != '(')                /* '(' encounterd */
			{
				postfix_exp[j] = x;
				j++;
				x = pop1();
			}
		}
		else
		{ /* if current symbol is neither operand not '(' nor ')' and nor
			operator */
			printf("\nInvalid infix Expression.\n");        /* the it is illegeal  symbol */
			getchar();
			exit(1);
		}
		i++;


		item = infix_exp[i]; /* go to next symbol of infix expression */
	} /* while loop ends here */
	if(top>0)
	{
		printf("\nInvalid infix Expression.\n");        /* the it is illegeal  symbol */
		getchar();
		exit(1);
	}
	if(top>0)
	{
		printf("\nInvalid infix Expression.\n");        /* the it is illegeal  symbol */
		getchar();
		exit(1);
	}


	postfix_exp[j] = '\0'; /* add sentinel else puts() fucntion */
	/* will print entire postfix[] array upto SIZE */
}

void main()  // main code
{

clrscr();
char infix[SIZE], postfix[SIZE];
int i=0;       						// counter
int s=0;        					// stores digits in the input expression in form of intergers
int l=0;        					// length of input expression
int t=0,t1=0,t2=0,t3=0;   			// stack pointer
char oper1='0';    					// operands for operations
char oper2='0';	

struct r rbank[16];					// Defining the 16 register banks of ARM
struct var vbank[3];				// Defing the remporary reg banks
int flag,index,map,k;				// Some flags delaration
char str2[20];						// String declaration
char str[20]; 
FILE *infile;						// Input file pointer declaration
char cp;

for(i=0;i<16;i++)					// Initializing the valid bits of all register to 0
	rbank[i].valid=0;

for(i=0;i<3;i++)					// Initializing the used bits of temporary registers to 0
	vbank[i].used=0;

vbank[0].temp='x';					// Initializing the temporary registers
vbank[1].temp='y';
vbank[2].temp='z';



// OPEN file named MYassemb.s and write the header area in it targetting Kiel - Cortex-M4

outfile.open("MYassemb.s");
outfile<<";Done by Karthick.D - MT2018509"<<endl;
outfile<<";	     Karthik.S - MT2018510"<<endl;
outfile<<";HELLO ALL!!"<<endl<<";Cortex-M4 Assembly code generated by c code"<<endl;
cout<<"     area     appcode, CODE, READONLY"<<endl;
outfile<<"     export __main"<<endl<<" ENTRY"<<endl<<"__main  function"<<endl;

infile=fopen("input.txt","r");								// opening the input file
while(fgets(str,20,infile))									// read each line of input.txt file
{
    if(str[1]!='=')											// If else operation is enountered (ternary operator)
    {
		findmap(str[0],rbank, &t1);							// Find the two registers of the comparition operation
		findmap(str[2],rbank,&t2);
		cout <<"CMP R"<<t1<<", R"<<t2<<endl;
		outfile <<"     CMP R"<<t1<<", R"<<t2<<endl;		// writing CMP operation into file
		flag=ismapped(str[4],rbank);
		if(flag==0)
		{
			findfreeregister(rbank,&index);		// If not mapped map it to free register available
			rbank[index].variable=str[4];
			rbank[index].valid=1;
		}
		findmap(str[4],rbank,&t1);
		cout <<"MOVGT R"<<t1<<", #"<<str[6]<<endl;
		outfile <<"     MOVGT R"<<t1<<", #"<<str[6]<<endl;
		flag=ismapped(str[8],rbank);
		if(flag==0)
		{
			findfreeregister(rbank,&index);		// If not mapped map it to free register available
			rbank[index].variable=str[8];
			rbank[index].valid=1;
		}
		findmap(str[8],rbank,&t1);
		cout <<"MOVLT R"<<t1<<", #"<<str[10]<<endl;
		outfile <<"     MOVLT R"<<t1<<", #"<<str[10]<<endl;
    }
	
	else
    {

    l=strlen(str);						// Finding the length of string
    str[l-1]='\0';						// replacing the new line charecter with null
    l=strlen(str);						
	flag=ismapped(str[0],rbank);		// check if the variable is mapped to register bank
	if(flag==0)
	{
	findfreeregister(rbank,&index);		// If not mapped map it to free register available
	rbank[index].variable=str[0];
	rbank[index].valid=1;
	}
	if(str[1]=='=')						// If the assignment operator is found, either it's MOV or airthmetic exprression
	{
		for(k=0;k<20;k++)
		str2[k]='\0';
		strncpy(str2,str+2,l-2);
		InfixToPostfix(str2,postfix);	// Convert the expression to postfix for 

	}

l=strlen(postfix);   					// find input expression length

// ************************
for(i=0;i<l;i++)
{
if (isdigit(postfix[i]))    			// if detect a digit the PUSH else POP and CALCULATE
{
int s= (postfix[i])-'0';    			// for digit of string to interger conversion

findmap(str[0],rbank,&map);             // increment stack pointer (initially  from 0 to 1 and so on)
cout<<"MOV R"<<map<<","<<s<<endl;
outfile <<"     MOV R"<<map<<", #"<<s<<endl;
}
else if(isalpha(postfix[i]))		    // Check if read charecter is variable and push to stack
{
	t++;
	s=postfix[i];
	push(s,t);
}
else
{
if(postfix[i]=='+'||postfix[i]=='-'||postfix[i]=='*'||postfix[i]=='/')  // if operation detected
oper2=pop(s,t);                                             			// POP up the RHS operand
t--;                                                        			// decrement stack
oper1=pop(s,t);		  													// POP up the LHS operand
findmap(oper1,rbank,&t1);
findmap(oper2,rbank,&t2);
if(i<l-1)
{
 calc(postfix[i],oper1,oper2);											// To find the operation which need to be performed
 findfreeregister(rbank, &t3);
 findfreevar(vbank,&cp);
 rbank[t3].variable=cp;
 rbank[t3].valid=1;
 cout<<" R"<<t3<<", R"<<t1<<", R"<<t2<<endl;
 outfile<<"	R"<<t3<<", R"<<t1<<", R"<<t2<<endl;							// Writing register contnets to .s file
 push(rbank[t3].variable,t);
}
else
{
findmap(str[0],rbank,&t3);
calc(postfix[i],oper1,oper2); 											// calculate the operation demanded
cout<<"R"<<t2<<", R"<<t1<<", R"<<t2<<endl; 								// related to output the assembly code expression on screen
outfile <<"	R"<<t3<<", R"<<t1<<", R"<<t2<<endl; 						// related to writing the assembly code expression into file

push(rbank[t2].variable,t);						 						// push result into stack
}
}

}
}
}
cout<<"stop B stop"<<endl<<" ENDFUNC"<<endl<<" end"<<endl;
outfile<<"stop B stop"<<endl<<" ENDFUNC"<<endl<<" end"<<endl;   // including the footers in the Cortex-M4 assembly file

cout<<"\n\nMapping of registers to variable is as below : \n";		
for(i=0;rbank[i].valid!=0 && i<16;i++)
{	
	cout<<"register R"<<i<< " : "<<rbank[i].variable<<"\n";
}

outfile.close();	 											// CLOSE the opened file.
fclose(infile);
getch();                        								// for output to stay on screen
}