#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <windows.h>
#include <stdbool.h>
#include <conio.h>
#include <string.h>

// Alteração de Usuários
// Exclusão de Usuários
// Listagem de Usuários
// Obrigatoriamente os dados para esta versão beta deverão ser armazenados em um arquivo de texto simples (TXT) com dados criptografados.

int main();
void linhaColuna(int lin, int col);
void TextColor(int letra, int fundo);
int menu(int lin1, int col1, int qtd, char lista[3][40]);

#define MAX_USUARIOS 100

// COR DA LETRA
enum
{
   BLACK,
   WHITE
};
// COR DO FUNDO
enum
{
   _BLACK = 0,
   _WHITE = 240
};

int qtdUsuarios;

struct Usuarios
{
   int id;
   char nome[30];
   char senha[10];
};

struct Usuarios usuario[MAX_USUARIOS];
int indiceUsuario = 0;

void clear()
{
#ifdef _WIN32
   system("cls");
#else
   system("clear");
#endif
}

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

// Função para guardar um usuário na memória
void guardarNaMemoria(int indice, int id, char nome[30], char senha[10])
{
   usuario[indice].id = id;
   strcpy(usuario[indice].nome, nome);
   strcpy(usuario[indice].senha, senha);
}

// TESTE PARA CRIAÇÃO DE NOVO USUARIO
void criarUsuario()
{
   char nome[30];
   char senha[10];
   int id;

   linhaColuna(10, 1);
   printf("ID: ");
   scanf("%i", &id);

   printf("\nNOME: ");
   fflush(stdin);
   gets(nome);

   printf("\nSENHA: ");
   fflush(stdin);
   gets(senha);

   guardarNaMemoria(qtdUsuarios, id, nome, senha);
   qtdUsuarios++;

   main();
}

void listarUsuarios()
{
   int i, tecla;

   if (qtdUsuarios > 0)
      {
         for (i = 0; i < qtdUsuarios; i++)
         {
            printf("-----------------------------\n");
            printf("USUARIO %i\n", i + 1);
            printf("ID: %i\n", usuario[i].id);
            printf("NOME: %s\n", usuario[i].nome);
            printf("SENHA: %s\n", usuario[i].senha);
         }
      } else {
         printf("SEM USUARIOS CADASTRADOS");
      }

   tecla = getch();

   if ((tecla == 27) || (tecla == 13))
   {
      clear();
      exit;
   }
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

      if (opc == 1)
      {
         clear();
         criarUsuario();
         break;
      }

      if (opc == 4)
      {
         clear();
         listarUsuarios();
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