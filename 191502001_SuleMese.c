#include <stdio.h> //G�R�� �IKI� ��LEMLER� ���N
#include <string.h> //MET�N ��LEMLER� ���N
#include <ctype.h> //KARAKTER ��LEMLER� ���N 

#define MAX_TOKEN_LENGTH 100 //B�Z�M PROGRAMIMIZDA LEXER MAKS�MUM 100 KARAKTER UZUNLU�UNDA B�R TOKEN� ��LEYEB�LS�N. B�R SINIR KONULMAK ZORUNDADIR.


//TOKENLER�M�Z�N T�RLER� BU ENUMDA TUTULUR.
//LEXER KAYNAK KODU ANAL�Z EDERKEN BULDU�U HER TOKEN� BU T�RLERDEN B�R�NE TANIMLAR.
enum TokenType
{
    TOKEN_KEYWORD, //ANAHTAR KEL�MELER
    TOKEN_IDENTIFIER, //DE���KEN VB. TANIMLAYICILAR 
    TOKEN_PUNCTUATOR, //PARANTEZ VB. ��ARETLER 
    TOKEN_OPERATOR, //OPERAT�RLER
    TOKEN_CONSTANT, // SAB�TLER 
    TOKEN_STRING_LITERAL, //STR�NGLER 
    TOKEN_UNKNOWN //BUNLAR DI�INDA KALANLAR 
};

// Enum de�erlerini metinle e�le�tiren bir dizi ( en son token t�r�n� bast�r�rken ihtiya� olacak)
const char *tokenTypeStrings[] = {
    "KEYWORD",
    "IDENTIFIER",
    "PUNCTUATOR",
    "OPERATOR",
    "CONSTANT",
    "STRING_LITERAL",
    "UNKNOWN"
};



// B�R TOKEN� TEMS�L EDEN STRUCT(YAPI) OLU�TURULUR.  
//B�R TOKEN�N 2 �ZELL��� VARDIR. 1.S� TOKEN T�R�, 2.S�  TOKEN�N MET�N OLARAK ��ER���D�R.
struct Token 
{
    enum TokenType type; //TOKEN T�R� 
    char lexeme[MAX_TOKEN_LENGTH]; // TOKEN�N MET�NSEL ��ER��� OLUR. 
};



//KEYWORDSLER� TEMS�L EDEN B�R D�Z� OLU�TURULDU.
//BU D�Z�N�N ���NDE C D�L�NDE TANIMLI KEYWORDSLAR YER ALMAKTADIR.
//LEXER ANAL�Z YAPARKEN BULDU�U TOKEN� BURADAK� ELEMANLARLA KAR�ILA�TIRARAK KEYWORDS OLUP OLMADI�INI ANAL�Z EDECEKT�R.
//bu dizi iki boyutludur ��nk� c dilinde string dizisi diye bir kavram yoktur o nedenle pointer dizisi �eklinde tan�mlanm��t�r.
char *keywords[] = {
    "auto", "break", "case", "char", "const", "continue", "default",
    "do", "double", "else", "enum", "extern", "float", "for",
    "goto", "if", "int", "long", "register", "return", "short",
    "signed", "sizeof", "static", "struct", "switch", "typedef",
    "union", "unsigned", "void", "volatile", "while", "main"
};

// BU FONKS�YON "lexeme" in keyword olup olmad���n� tespit eder.
// "lexeme" i keyword dizisinin t�m elemanlar� ile kar��la�t�r�r. E�er herhangi bir elemanla e�le�me varsa 1 yoksa 0 d�nd�r�r. 
int isKeyword(char *lexeme) {
	int i;
	//burada keywords[][] dizisinin eleman say�s� t�m dizinin boyutu / ilk eleman�n boyutundan bulunur. ilk eleman da char* tipli oldu�undan boyutu sabittir.
    for (i = 0; i < sizeof(keywords) / sizeof(keywords[0]); i++) {
        if (strcmp(lexeme, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

int main() 
{
	
	//LEXER ���N G�R�LECEK METN� TUTACAK  "input" ADINDA B�R D�Z� TANIMLANDI . BU D�Z�YE 1000 KARAKTERL�K BOYUT ATANDI.
    char input[1000]; 
    //Token yap�s� tan�mland�.
    struct Token token; 
    //i=0 ba�lang�� i�in gerekli bir de�i�kendir. ve bu tan�mland�. 
    int i = 0;



	//LEXER �LE ANAL�Z ED�LECEK MET�N KONSOLDAN ALINIR 	"input" D�Z�S�NE ATANIR.
    printf("Lutfen bir C kodu girin (max 1000 karakter): ");
    fgets(input, sizeof(input), stdin);
    
    
    //ANAL�Z KISMI BURADA BA�LAR. "input" D�Z�S� KARAKTER KARAKTER TARANIR. D�Z�N�N T�M KARAKTERLER� TARANINCAYA KADAR D�NG� DEVAM EDER. 
    while (input[i] != '\0') 
	{
	 	
	 		//BO�LUK OKUNDUYSA ATLANIR VE B�R SONRAK� KARAKTER ��LEN�R.
		    if(isspace(input[i])) 
			{
				i++;
				continue;
			}
			
			
			//E�ER OKUNAN KARAKTER B�R HARF VEYA ALT ��ZG� �SE LEXER TOKEN� KEYWORD YADA IDENTIFIER OLARAK TESP�T EDER.
			else if (isalpha(input[i]) || input[i] == '_') 
			{
	            int j = 0;
	            
	            //ARTIK LEXEME OLU�TURULUR . WH�LE D�NG�S� ���NDE KARAKTERLER HARF, RAKAM, VEYA ALT ��ZG� �SE LEXEME D�Z�S�NE EKLENEREK DEVAM ED�L�R. 
	            while (isalpha(input[i]) || isdigit(input[i]) || input[i] == '_') 
				{
	                token.lexeme[j++] = input[i++];
	            }
	            
	            //SON KARAKTER HER ZAMAN '\0'DIR.
	            token.lexeme[j] = '\0';
	            //TOKEN�N T�R� isKeyword() fonksiyonundan d�nen de�ere ba�l� olarak KEYWORD YADA IDENTIFIER OLARAK SE��L�R.
	            token.type = isKeyword(token.lexeme) ? TOKEN_KEYWORD : TOKEN_IDENTIFIER;
        	}



			//��LENEN KARAKTER , ; ( ) { } [ ] DAN B�R�S� �SE LEXER TOKEN� PUNCTUATOR OLARAK TESP�T EDER. 
			else if (strchr(",;(){}[]", input[i]) != NULL) 
			{
	            token.lexeme[0] = input[i];
	            token.lexeme[1] = '\0';
	            token.type = TOKEN_PUNCTUATOR;
	            i++;
            }
            
            //��LENEN KARAKTER + - * / = < > ! & | % �SE LEXER TOKEN� OPERATOR OLARAK TESP�T EDER.
            else if (strchr("+-*/=<>!&|%", input[i]) != NULL) 
			{
	            token.lexeme[0] = input[i];
	            token.lexeme[1] = '\0';
	            token.type = TOKEN_OPERATOR;
	            i++;
        	} 
        	
        	
        	//E�ER ��LENEN KARAKTER B�R RAKAM �SE LEXER TOKEN� CONSTANT OLARAK TESP�T EDER . 
        	else if (isdigit(input[i])) 
			{
	            int j = 0;
	            
	            //SAYI B�RDEN FAZLA RAKAMDAN OLU�AB�L�R VE SAYI FLOAT �SE NOKTA ��EREB�L�R. SAYININ T�M KARAKTERLER� LEXEME D�Z�S�NE ATANIR. 
	            while(isdigit(input[i]) || input[i] == '.') 
				{
	                token.lexeme[j++] = input[i++];
	            }
	            
	            token.lexeme[j] = '\0';
				
				//SAYI NOKTA ��ER�YORSA FLOAT OLDU�UNDAN CONSTANT DIR. 
	            if(strchr(token.lexeme, '.') != NULL) 
				{
	                token.type = TOKEN_CONSTANT;
	            } 
				
				//SAYI NOKTA ��ERM�YORSA �NT OLDU�UNDAN Y�NE DE CONSTANT DIR. 
				else 
				{
	                token.type = TOKEN_CONSTANT;
	            }
        	}
        	
        	
        	//E�ER ��LENEN KARAKTER " (��FT TIRNAK) �SE LEXER TOKEN� STR�NG METN� OLARAK TESP�T EDECEKT�R. ��NK� STR�NGLER ��FT TIRNAK �LE BA�LARLAR. 
        	else if (input[i] == '"') 
			{
	            int j = 0;
	            token.lexeme[j++] = input[i++];
	            
	            //LEXEME D�Z�S�NE STR�NG ��ER��� ALINIR. 
	            while (input[i] != '"' && input[i] != '\0') 
				{
	                token.lexeme[j++] = input[i++];
	            }
	            //SON KARAKTER ��FT TIRNAK �SE TOKEN T�P� STR�NG L�TERALD�R. 
	            if (input[i] == '"') 
				{
	                token.lexeme[j++] = input[i++];
	                token.lexeme[j] = '\0';
	                token.type = TOKEN_STRING_LITERAL;
	                i++;
	            } 
				//SON KARAKTER ��FT TIRNAK DE��L �SE BEL�RS�Z B�R TOKEN T�P�D�R. ��NK� STR�NGLER ��FT TIRNAK ���NDE TANIMLANIRLAR. 
				else 
				{
	                token.type = TOKEN_UNKNOWN;
	            }
            }
            
            
            //E�ER �F BLOKLARININ H��B�R�NE G�RMED�YSE B�L�NMEYEN B�R TOKEN T�R�D�R. 
            else 
			{
	            int j = 0;
	            token.lexeme[j++] = input[i++];
	            token.lexeme[j] = '\0';
	            token.type = TOKEN_UNKNOWN;
        	}
        	
        	
        	//EN SON TOKEN�N LEXEME KISMI YAN� MET�N ��ER��� KONSOL EKRANINA BASTIRILIR. 		
			printf(" '%s'  ",token.lexeme);
			
			//TOKEN�N LEXEME VE TOKEN T�RLER� B�RL�KTE DE BASTIRILAB�L�R.
			//printf("Lexeme = %s  | Token Turu = %s \n",token.lexeme, tokenTypeStrings[token.type]);
    }

    return 0;
}








