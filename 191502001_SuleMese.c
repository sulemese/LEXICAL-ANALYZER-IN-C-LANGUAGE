#include <stdio.h> //GÝRÝÞ ÇIKIÞ ÝÞLEMLERÝ ÝÇÝN
#include <string.h> //METÝN ÝÞLEMLERÝ ÝÇÝN
#include <ctype.h> //KARAKTER ÝÞLEMLERÝ ÝÇÝN 

#define MAX_TOKEN_LENGTH 100 //BÝZÝM PROGRAMIMIZDA LEXER MAKSÝMUM 100 KARAKTER UZUNLUÐUNDA BÝR TOKENÝ ÝÞLEYEBÝLSÝN. BÝR SINIR KONULMAK ZORUNDADIR.


//TOKENLERÝMÝZÝN TÜRLERÝ BU ENUMDA TUTULUR.
//LEXER KAYNAK KODU ANALÝZ EDERKEN BULDUÐU HER TOKENÝ BU TÜRLERDEN BÝRÝNE TANIMLAR.
enum TokenType
{
    TOKEN_KEYWORD, //ANAHTAR KELÝMELER
    TOKEN_IDENTIFIER, //DEÐÝÞKEN VB. TANIMLAYICILAR 
    TOKEN_PUNCTUATOR, //PARANTEZ VB. ÝÞARETLER 
    TOKEN_OPERATOR, //OPERATÖRLER
    TOKEN_CONSTANT, // SABÝTLER 
    TOKEN_STRING_LITERAL, //STRÝNGLER 
    TOKEN_UNKNOWN //BUNLAR DIÞINDA KALANLAR 
};

// Enum deðerlerini metinle eþleþtiren bir dizi ( en son token türünü bastýrýrken ihtiyaç olacak)
const char *tokenTypeStrings[] = {
    "KEYWORD",
    "IDENTIFIER",
    "PUNCTUATOR",
    "OPERATOR",
    "CONSTANT",
    "STRING_LITERAL",
    "UNKNOWN"
};



// BÝR TOKENÝ TEMSÝL EDEN STRUCT(YAPI) OLUÞTURULUR.  
//BÝR TOKENÝN 2 ÖZELLÝÐÝ VARDIR. 1.SÝ TOKEN TÜRÜ, 2.SÝ  TOKENÝN METÝN OLARAK ÝÇERÝÐÝDÝR.
struct Token 
{
    enum TokenType type; //TOKEN TÜRÜ 
    char lexeme[MAX_TOKEN_LENGTH]; // TOKENÝN METÝNSEL ÝÇERÝÐÝ OLUR. 
};



//KEYWORDSLERÝ TEMSÝL EDEN BÝR DÝZÝ OLUÞTURULDU.
//BU DÝZÝNÝN ÝÇÝNDE C DÝLÝNDE TANIMLI KEYWORDSLAR YER ALMAKTADIR.
//LEXER ANALÝZ YAPARKEN BULDUÐU TOKENÝ BURADAKÝ ELEMANLARLA KARÞILAÞTIRARAK KEYWORDS OLUP OLMADIÐINI ANALÝZ EDECEKTÝR.
//bu dizi iki boyutludur çünkü c dilinde string dizisi diye bir kavram yoktur o nedenle pointer dizisi þeklinde tanýmlanmýþtýr.
char *keywords[] = {
    "auto", "break", "case", "char", "const", "continue", "default",
    "do", "double", "else", "enum", "extern", "float", "for",
    "goto", "if", "int", "long", "register", "return", "short",
    "signed", "sizeof", "static", "struct", "switch", "typedef",
    "union", "unsigned", "void", "volatile", "while", "main"
};

// BU FONKSÝYON "lexeme" in keyword olup olmadýðýný tespit eder.
// "lexeme" i keyword dizisinin tüm elemanlarý ile karþýlaþtýrýr. Eðer herhangi bir elemanla eþleþme varsa 1 yoksa 0 döndürür. 
int isKeyword(char *lexeme) {
	int i;
	//burada keywords[][] dizisinin eleman sayýsý tüm dizinin boyutu / ilk elemanýn boyutundan bulunur. ilk eleman da char* tipli olduðundan boyutu sabittir.
    for (i = 0; i < sizeof(keywords) / sizeof(keywords[0]); i++) {
        if (strcmp(lexeme, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

int main() 
{
	
	//LEXER ÝÇÝN GÝRÝLECEK METNÝ TUTACAK  "input" ADINDA BÝR DÝZÝ TANIMLANDI . BU DÝZÝYE 1000 KARAKTERLÝK BOYUT ATANDI.
    char input[1000]; 
    //Token yapýsý tanýmlandý.
    struct Token token; 
    //i=0 baþlangýç için gerekli bir deðiþkendir. ve bu tanýmlandý. 
    int i = 0;



	//LEXER ÝLE ANALÝZ EDÝLECEK METÝN KONSOLDAN ALINIR 	"input" DÝZÝSÝNE ATANIR.
    printf("Lutfen bir C kodu girin (max 1000 karakter): ");
    fgets(input, sizeof(input), stdin);
    
    
    //ANALÝZ KISMI BURADA BAÞLAR. "input" DÝZÝSÝ KARAKTER KARAKTER TARANIR. DÝZÝNÝN TÜM KARAKTERLERÝ TARANINCAYA KADAR DÖNGÜ DEVAM EDER. 
    while (input[i] != '\0') 
	{
	 	
	 		//BOÞLUK OKUNDUYSA ATLANIR VE BÝR SONRAKÝ KARAKTER ÝÞLENÝR.
		    if(isspace(input[i])) 
			{
				i++;
				continue;
			}
			
			
			//EÐER OKUNAN KARAKTER BÝR HARF VEYA ALT ÇÝZGÝ ÝSE LEXER TOKENÝ KEYWORD YADA IDENTIFIER OLARAK TESPÝT EDER.
			else if (isalpha(input[i]) || input[i] == '_') 
			{
	            int j = 0;
	            
	            //ARTIK LEXEME OLUÞTURULUR . WHÝLE DÖNGÜSÜ ÝÇÝNDE KARAKTERLER HARF, RAKAM, VEYA ALT ÇÝZGÝ ÝSE LEXEME DÝZÝSÝNE EKLENEREK DEVAM EDÝLÝR. 
	            while (isalpha(input[i]) || isdigit(input[i]) || input[i] == '_') 
				{
	                token.lexeme[j++] = input[i++];
	            }
	            
	            //SON KARAKTER HER ZAMAN '\0'DIR.
	            token.lexeme[j] = '\0';
	            //TOKENÝN TÜRÜ isKeyword() fonksiyonundan dönen deðere baðlý olarak KEYWORD YADA IDENTIFIER OLARAK SEÇÝLÝR.
	            token.type = isKeyword(token.lexeme) ? TOKEN_KEYWORD : TOKEN_IDENTIFIER;
        	}



			//ÝÞLENEN KARAKTER , ; ( ) { } [ ] DAN BÝRÝSÝ ÝSE LEXER TOKENÝ PUNCTUATOR OLARAK TESPÝT EDER. 
			else if (strchr(",;(){}[]", input[i]) != NULL) 
			{
	            token.lexeme[0] = input[i];
	            token.lexeme[1] = '\0';
	            token.type = TOKEN_PUNCTUATOR;
	            i++;
            }
            
            //ÝÞLENEN KARAKTER + - * / = < > ! & | % ÝSE LEXER TOKENÝ OPERATOR OLARAK TESPÝT EDER.
            else if (strchr("+-*/=<>!&|%", input[i]) != NULL) 
			{
	            token.lexeme[0] = input[i];
	            token.lexeme[1] = '\0';
	            token.type = TOKEN_OPERATOR;
	            i++;
        	} 
        	
        	
        	//EÐER ÝÞLENEN KARAKTER BÝR RAKAM ÝSE LEXER TOKENÝ CONSTANT OLARAK TESPÝT EDER . 
        	else if (isdigit(input[i])) 
			{
	            int j = 0;
	            
	            //SAYI BÝRDEN FAZLA RAKAMDAN OLUÞABÝLÝR VE SAYI FLOAT ÝSE NOKTA ÝÇEREBÝLÝR. SAYININ TÜM KARAKTERLERÝ LEXEME DÝZÝSÝNE ATANIR. 
	            while(isdigit(input[i]) || input[i] == '.') 
				{
	                token.lexeme[j++] = input[i++];
	            }
	            
	            token.lexeme[j] = '\0';
				
				//SAYI NOKTA ÝÇERÝYORSA FLOAT OLDUÐUNDAN CONSTANT DIR. 
	            if(strchr(token.lexeme, '.') != NULL) 
				{
	                token.type = TOKEN_CONSTANT;
	            } 
				
				//SAYI NOKTA ÝÇERMÝYORSA ÝNT OLDUÐUNDAN YÝNE DE CONSTANT DIR. 
				else 
				{
	                token.type = TOKEN_CONSTANT;
	            }
        	}
        	
        	
        	//EÐER ÝÞLENEN KARAKTER " (ÇÝFT TIRNAK) ÝSE LEXER TOKENÝ STRÝNG METNÝ OLARAK TESPÝT EDECEKTÝR. ÇÜNKÜ STRÝNGLER ÇÝFT TIRNAK ÝLE BAÞLARLAR. 
        	else if (input[i] == '"') 
			{
	            int j = 0;
	            token.lexeme[j++] = input[i++];
	            
	            //LEXEME DÝZÝSÝNE STRÝNG ÝÇERÝÐÝ ALINIR. 
	            while (input[i] != '"' && input[i] != '\0') 
				{
	                token.lexeme[j++] = input[i++];
	            }
	            //SON KARAKTER ÇÝFT TIRNAK ÝSE TOKEN TÝPÝ STRÝNG LÝTERALDÝR. 
	            if (input[i] == '"') 
				{
	                token.lexeme[j++] = input[i++];
	                token.lexeme[j] = '\0';
	                token.type = TOKEN_STRING_LITERAL;
	                i++;
	            } 
				//SON KARAKTER ÇÝFT TIRNAK DEÐÝL ÝSE BELÝRSÝZ BÝR TOKEN TÝPÝDÝR. ÇÜNKÜ STRÝNGLER ÇÝFT TIRNAK ÝÇÝNDE TANIMLANIRLAR. 
				else 
				{
	                token.type = TOKEN_UNKNOWN;
	            }
            }
            
            
            //EÐER ÝF BLOKLARININ HÝÇBÝRÝNE GÝRMEDÝYSE BÝLÝNMEYEN BÝR TOKEN TÜRÜDÜR. 
            else 
			{
	            int j = 0;
	            token.lexeme[j++] = input[i++];
	            token.lexeme[j] = '\0';
	            token.type = TOKEN_UNKNOWN;
        	}
        	
        	
        	//EN SON TOKENÝN LEXEME KISMI YANÝ METÝN ÝÇERÝÐÝ KONSOL EKRANINA BASTIRILIR. 		
			printf(" '%s'  ",token.lexeme);
			
			//TOKENÝN LEXEME VE TOKEN TÜRLERÝ BÝRLÝKTE DE BASTIRILABÝLÝR.
			//printf("Lexeme = %s  | Token Turu = %s \n",token.lexeme, tokenTypeStrings[token.type]);
    }

    return 0;
}








