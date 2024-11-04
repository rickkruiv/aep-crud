#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <windows.h>
#include <stdbool.h>
#include <conio.h>

// Alteração de Usuários
// Exclusão de Usuários
// Listagem de Usuários
// Obrigatoriamente os dados para esta versão beta deverão ser armazenados em um arquivo de texto simples (TXT) com dados criptografados.

int main();
void linhaColuna(int lin, int col);
void TextColor(int letra, int fundo);
int menu(int lin1, int col1, int qtd, char lista[3][40]);

void clear()
{
#ifdef _WIN32
   system("cls");
#else
   system("clear");
#endif
}

// COR DA LETRA
enum
{
   BLACK,        // 0
   BLUE,         // 1
   GREEN,        // 2
   CYAN,         // 3
   RED,          // 4
   MAGENTA,      // 5
   BROWN,        // 6
   LIGHTGRAY,    // 7
   DARKGRAY,     // 8
   LIGHTBLUE,    // 9
   LIGHTGREEN,   // 10
   LIGHTCYAN,    // 11
   LIGHTRED,     // 12
   LIGHTMAGENTA, // 13
   YELLOW,       // 14
   WHITE         // 15
};
// COR DO FUNDO
enum
{
   _BLACK = 0,          // 0
   _BLUE = 16,          // 1
   _GREEN = 32,         // 2
   _CYAN = 48,          // 3
   _RED = 64,           // 4
   _MAGENTA = 80,       // 5
   _BROWN = 96,         // 6
   _LIGHTGRAY = 112,    // 7
   _DARKGRAY = 128,     // 8
   _LIGHTBLUE = 144,    // 9
   _LIGHTGREEN = 160,   // 10
   _LIGHTCYAN = 176,    // 11
   _LIGHTRED = 192,     // 12
   _LIGHTMAGENTA = 208, // 13
   _YELLOW = 224,       // 14
   _WHITE = 240         // 15
};

void linhaColuna(int lin, int col)
{
   SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), (COORD){col - 1, lin - 1});

   HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
   CONSOLE_CURSOR_INFO info;
   info.dwSize = 100;
   info.bVisible = FALSE;
   SetConsoleCursorInfo(consoleHandle, &info);
}

void TextColor(int letra, int fundo)
{
   SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), letra + fundo);
}

int menu(int lin1, int col1, int qtd, char lista[3][40])
{
   int opc = 1, lin2, col2, linha, i, tamMaxItem, tecla;

   tamMaxItem = strlen(lista[0]);

   for (i = 1; i < qtd; i++)
   {
      if (strlen(lista[i]) > tamMaxItem)
      {
         tamMaxItem = strlen(lista[i]);
      }
   }
   lin2 = lin1 + (qtd * 2 + 2);
   col2 = col1 + tamMaxItem + 4;

   TextColor(WHITE, _BLACK);
   setlocale(LC_ALL, "C");
   setlocale(LC_ALL, "");

   while (1)
   {
      linha = lin1 + 2;

      for (i = 0; i < qtd; i++)
      {
         if (i + 1 == opc)
            TextColor(BLACK, _WHITE);
         else
            TextColor(WHITE, _BLACK);
         linhaColuna(linha, col1 + 2);
         printf("%s", lista[i]);
         linha += 2;
      }

      linhaColuna(1, 1);
      tecla = getch();
      linhaColuna(22, 1);

      if (tecla == 27)
      {
         opc = 6;
         break;
      }
      else if (tecla == 13)
      {
         break;
      }
      else if (tecla == 72)
      {
         if (opc > 1)
         {
            opc--;
         }
         else
         {
            opc = qtd;
         }
      }
      else if (tecla == 80)
      {
         if (opc < qtd)
         {
            opc++;
         }
         else
         {
            opc = 1;
         }
      }
   }
   return opc;
}

// TESTE PARA CRIAÇÃO DE NOVO USUARIO
void criarUsuario() {
   char nome[30];
   char senha[10];
   int id;

   linhaColuna(1,1);

   printf("ID: ");
   scanf("%i", &id);

   printf("\nNOME: ");
   fflush(stdin);
   gets(nome);

   printf("\nSENHA: ");
   fflush(stdin);
   gets(senha);

   main();
}

int main()
{
   int opc;
   char lista[6][40] = {"CRIAR NOVO USUARIO", "ATUALIZAR USUARIO", "EXCLUIR USUARIO", "LISTAR USUARIOS", "GERAR CSV", "SAIR"};

   setlocale(LC_ALL, "portuguese");
   clear();

   while (true)
   {
      opc = menu(10, 10, 6, lista);

      if (opc == 1) {
         clear();
         criarUsuario();
         break;
      }

      if (opc == 6)
      {
         TextColor(WHITE, _BLACK);
         clear();
         break;
      }
   }

   TextColor(WHITE, _BLACK);
   return 0;
}