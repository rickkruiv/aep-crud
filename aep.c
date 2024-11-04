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
void criarUsuario();
bool verificarUsuario(int id, char nome[30]);
void carregarUsuariosDoTXT();

#define MAX_USUARIOS 100
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

// Verificar se usuario ja existe
bool verificarUsuario(int id, char nome[30])
{
   for (int i = 0; i < qtdUsuarios; i++)
   {
      if (usuario[i].id == id || strcmp(usuario[i].nome, nome) == 0)
      {
         return true;
      }
   }
   return false;
}

// TESTE PARA CRIAÇÃO DE NOVO USUARIO
void criarUsuario()
{
   char nome[30];
   char senha[10];
   int id, tecla;
   while (true)
   {
      linhaColuna(10, 1);
      printf("ID: ");
      scanf("%i", &id);

      printf("\nNOME: ");
      fflush(stdin);
      gets(nome);

      if (verificarUsuario(id, nome))
      {
         linhaColuna(1, 45);
         printf("\033[31mERRO: USUARIO COM ID OU NOME JA CADASTRADO!\033[0m\n");
         linhaColuna(2, 50);
         printf("\033[91m[ENTER] PARA INSERIR NOVOS DADOS\n");
         linhaColuna(3, 53);
         printf("\033[91m[ESC] PARA VOLTAR PARA MENU\033[0m\n");

         tecla = getch();

         if (tecla == 27)
         {
            break;
         }
         else
         {
            linhaColuna(10, 5);
            printf("       "); // Limpa o ID
            linhaColuna(12, 6);
            printf("                             "); // Limpa o nome
            linhaColuna(15, 1);

            continue;
         }
      }

      printf("\nSENHA: ");
      fflush(stdin);
      gets(senha);

      guardarNaMemoria(qtdUsuarios, id, nome, senha);
      qtdUsuarios++;

      break;
   }
   main();
}

void listarUsuarios()
{
   int i, tecla;

   carregarUsuariosDoTXT();

   if (qtdUsuarios > 0)
   {
      for (i = 0; i < qtdUsuarios; i++)
      {
         printf("-----------------------------\n");
         printf("USUARIO %i\n", i + 1);
         printf("ID...: %i\n", usuario[i].id);
         printf("NOME.: %s\n", usuario[i].nome);
         printf("SENHA: %s\n", usuario[i].senha);
      }
      printf("-----------------------------\n");
   }
   else
   {
      linhaColuna(2, 46);
      printf("SEM USUARIOS CADASTRADOS");
   }

   tecla = getch();

   if ((tecla == 27) || (tecla == 13))
   {
      clear();
      exit;
   }
}

// Gerar TXT
void gerarTXT(struct Usuarios usuario[MAX_USUARIOS], int quantidade)
{
   FILE *arquivo = fopen("dados.txt", "w");

   if (arquivo == NULL)
   {
      linhaColuna(1, 45);
      printf("\033[31mERRO AO ABRIR O ARQUIVO\033[0m\n");
      return;
   }

   // Cabeçalho do TXT
   fprintf(arquivo, "ID,NOME,SENHA\n");

   for (int i = 0; i < quantidade; i++)
   {
      fprintf(arquivo, "%d,%s,%s\n", usuario[i].id, usuario[i].nome, usuario[i].senha);
   }

   fclose(arquivo);
   linhaColuna(1,40);
   printf("\033[32mArquivo TXT GERADO COM SUCESSO!\033[0m\n");
}

void carregarUsuariosDoTXT()
{
   FILE *arquivo = fopen("dados.txt", "r");

   if (arquivo == NULL)
   {
      linhaColuna(1, 44);
      printf("\033[31mERRO AO ABRIR O ARQUIVO TXT\033[0m\n");
      return;
   }

   char linha[100];
   int id;
   char nome[30], senha[10];

   // Ignora o cabeçalho
   fgets(linha, sizeof(linha), arquivo);

   qtdUsuarios = 0; // Reinicia a contagem de usuários

   while (fgets(linha, sizeof(linha), arquivo))
   {
      // Extrai os dados da linha usando sscanf
      sscanf(linha, "%d,%29[^,],%9[^\n]", &id, nome, senha);

      // Armazena o usuário na estrutura
      guardarNaMemoria(qtdUsuarios, id, nome, senha);
      qtdUsuarios++;
   }

   fclose(arquivo);
}

int main()
{

   int opc;
   char lista[6][40] = {"CRIAR NOVO USUARIO", "ATUALIZAR USUARIO", "EXCLUIR USUARIO", "LISTAR USUARIOS", "GERAR TXT", "SAIR"};

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

      if (opc == 5) {
         clear();
         gerarTXT(usuario, qtdUsuarios);
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
