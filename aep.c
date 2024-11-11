#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <windows.h>
#include <stdbool.h>
#include <conio.h>
#include <string.h>
#include <openssl/sha.h>

#define MAX_USUARIOS 100

struct Usuarios
{
   int id;
   char nome[30];
   char senha[65];
};

struct Senhas
{
   char senha[65];
};

struct Usuarios usuario[MAX_USUARIOS];
struct Senhas senhas[MAX_USUARIOS];
int qtdUsuarios = 0;
int indiceUsuario = 0;

int main();
int menu(int lin1, int col1, int qtd, char lista[3][40]);
void criarUsuario();
void guardarNaMemoria(int indice, int id, char nome[30], char senha[65]);
bool verificarUsuario(int id, char nome[30]);
void linhaColuna(int lin, int col);
void textcolor(int letra, int fundo);
void gerarTXT(struct Usuarios usuario[MAX_USUARIOS], int quantidade);
void criptografarSenha(char *senha, char *senhaCriptografada);
void carregarUsuariosDoTXT();
void listarUsuarios();
bool loginAdmin();
void listarUsuariosComDescriptografia();
void guardarSenhas(char senha[65], int indice);

enum // COR DA LETRA
{
   BLACK,
   BLUE,
   GREEN,
   CYAN,
   RED,
   MAGENTA,
   BROWN,
   LIGHTGRAY,
   DARKGRAY,
   LIGHTBLUE,
   LIGHTGREEN,
   LIGHTCYAN,
   LIGHTRED,
   LIGHTMAGENTA,
   YELLOW,
   WHITE
};

enum // COR DO FUNDO
{
   _BLACK = 0,
   _BLUE = 16,
   _GREEN = 32,
   _CYAN = 48,
   _RED = 64,
   _MAGENTA = 80,
   _BROWN = 96,
   _LIGHTGRAY = 112,
   _DARKGRAY = 128,
   _LIGHTBLUE = 144,
   _LIGHTGREEN = 160,
   _LIGHTCYAN = 176,
   _LIGHTRED = 192,
   _LIGHTMAGENTA = 208,
   _YELLOW = 224,
   _WHITE = 240
};

void clear() // limpar cmd
{
#ifdef _WIN32
   system("cls");
#else
   system("clear");
#endif
}

void linhaColuna(int lin, int col) // procedimento linha e coluna
{
   SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), (COORD){col - 1, lin - 1});

   HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
   CONSOLE_CURSOR_INFO info;
   info.dwSize = 100;
   info.bVisible = FALSE;
   SetConsoleCursorInfo(consoleHandle, &info);
}

void textcolor(int letra, int fundo) // procedmento cores texto e fundo
{
   SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), letra + fundo);
}

void criarUsuario() // novo usuario
{
   char nome[30];
   char senha[65];
   char senhaCriptografada[65];
   int id, tecla;

   while (true)
   {
      linhaColuna(10, 1);
      printf("ID: ");
      scanf("%i", &id);
      printf("\nNOME: ");
      fflush(stdin);
      gets(nome);
      nome[strcspn(nome, "\n")] = 0;

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
            printf("             "); // Limpa o ID
            linhaColuna(12, 6);
            printf("             "); // Limpa o nome
            linhaColuna(15, 1);
            continue;
         }
      }

      printf("\nSENHA: ");
      fflush(stdin);
      gets(senha);
      senha[strcspn(senha, "\n")] = 0;

      guardarSenhas(senha, qtdUsuarios);
      criptografarSenha(senha, senhaCriptografada);

      guardarNaMemoria(qtdUsuarios, id, nome, senhaCriptografada);
      qtdUsuarios++;

      gerarTXT(usuario, qtdUsuarios);
      break;
   }

   main();
}

void guardarSenhas(char senha[65], int indice)
{
   strcpy(senhas[indice].senha, senha);
}

int procurarUsuario()
{
   int id, encontrou = -1;

   linhaColuna(10, 1);
   printf("DIGITE O ID DO USUARIO QUE DESEJA ATUALIZAR: ");
   scanf("%d", &id);

   for (int i = 0; i < qtdUsuarios; i++)
   {
      if (usuario[i].id == id)
      {
         encontrou = i;
         break;
      }
   }

   if (encontrou == -1)
   {
      linhaColuna(1, 45);
      printf("\033[31mUSUARIO COM ID %d NAO ENCONTRADO\033[0m", id);
      Sleep(800);

      linhaColuna(1, 30);
      printf("                                                              ");
   }
   return encontrou;
}

void atualizarUsuario()
{
   char novoNome[30], novaSenha[10], novaSenhaCriptografada[65];
   char confirmarAlteracao;
   int indice;

   // procura
   indice = procurarUsuario();

   if (indice == -1)
   {
      return;
   }

   // atualizar
   linhaColuna(10, 1);
   printf("\nNOME: ");
   fflush(stdin);
   gets(novoNome);
   novoNome[strcspn(novoNome, "\n")] = 0;

   printf("DIGITE A NOVA SENHA: ");
   fflush(stdin);
   fgets(novaSenha, sizeof(novaSenha), stdin);
   novaSenha[strcspn(novaSenha, "\n")] = 0;

   guardarSenhas(novaSenha, indice);
   criptografarSenha(novaSenha, novaSenhaCriptografada);

   linhaColuna(12, 1);
   printf("                          ");
   linhaColuna(12, 1);
   printf("CONFIRMAR ALTERAÇOES?");
   linhaColuna(13, 1);
   printf("NOME: %s", novoNome);
   linhaColuna(14, 1);
   printf("SENHA: %s", novaSenha);

   linhaColuna(15, 1);
   printf("PRESSIONE 'S' PARA CONFIRMAR OU 'N' PARA CANCELAR: ");
   fflush(stdin);
   confirmarAlteracao = getch();

   if (confirmarAlteracao == 'S' || confirmarAlteracao == 's')
   {
      strcpy(usuario[indice].nome, novoNome);
      strcpy(usuario[indice].senha, novaSenhaCriptografada);
      gerarTXT(usuario, qtdUsuarios);

      linhaColuna(1, 30);
      printf("USUÁRIO ATUALIZADO COM SUCESSO!\n");
   }
   else
   {
      linhaColuna(1, 30);
      printf("ATUALIZAÇÃO CANCELADA!\n");
   }
}

void excluirUsuario()
{
   int indice;
   // procura
   indice = procurarUsuario();

   // exluir
   for (int i = indice; i < qtdUsuarios; i++)
   {
      usuario[i] = usuario[i + 1];
   }

   qtdUsuarios--;

   gerarTXT(usuario, qtdUsuarios);

   linhaColuna(1, 30);
   printf("USUÁRIO EXCLUÍDO COM SUCESSO!\n");
}

void guardarNaMemoria(int indice, int id, char nome[30], char senha[65]) // Função para guardar um usuário na memória
{
   usuario[indice].id = id;
   strcpy(usuario[indice].nome, nome);
   strcpy(usuario[indice].senha, senha);
}

bool verificarUsuario(int id, char nome[30]) // Verificar se usuario ja existe
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

void criptografarSenha(char *senha, char *senhaCriptografada)
{
   unsigned char hash[SHA256_DIGEST_LENGTH];
   SHA256_CTX sha256_ctx;

   SHA256_Init(&sha256_ctx);
   SHA256_Update(&sha256_ctx, senha, strlen(senha));
   SHA256_Final(hash, &sha256_ctx);

   for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
   {
      sprintf(&senhaCriptografada[i * 2], "%02x", hash[i]);
   }
}

void gerarTXT(struct Usuarios usuario[MAX_USUARIOS], int quantidade) // gerar txt
{
   FILE *arquivo = fopen("dados.txt", "w");
   if (arquivo == NULL)
   {
      linhaColuna(1, 45);
      printf("\033[31mERRO AO ABRIR O ARQUIVO\033[0m\n");
      return;
   }
   fprintf(arquivo, "ID,NOME,SENHA\n");
   for (int i = 0; i < quantidade; i++)
   {
      fprintf(arquivo, "%d,%s,%s\n", usuario[i].id, usuario[i].nome, usuario[i].senha);
   }
   fclose(arquivo);
   linhaColuna(1, 40);
   printf("\033[32mArquivo TXT GERADO COM SUCESSO!\033[0m\n");
}

void carregarUsuariosDoTXT() // puxar dados do txt
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
   char nome[30], senha[65];
   fgets(linha, sizeof(linha), arquivo);
   qtdUsuarios = 0;
   while (fgets(linha, sizeof(linha), arquivo))
   {
      sscanf(linha, "%d,%29[^,],%9[^\n]", &id, nome, senha);
      guardarNaMemoria(qtdUsuarios, id, nome, senha);
      qtdUsuarios++;
   }
   fclose(arquivo);
}

void listarUsuarios() // listar
{
   int i, tecla;
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

bool loginAdmin()
{
   char nome[30], senha[65];

   printf("\nDigite o nome de usuario: ");
   fflush(stdin);
   gets(nome);

   printf("Digite a senha: ");
   fflush(stdin);
   gets(senha);

   if (strcmp(nome, "Adm123") == 0 && strcmp(senha, "123") == 0)
   {
      return true;
   }
   else
   {
      return false;
   }
}

void listarUsuariosComDescriptografia()
{
   int i, tecla;
   if (qtdUsuarios > 0)
   {
      for (i = 0; i < qtdUsuarios; i++)
      {
         printf("-----------------------------\n");
         printf("USUARIO %i\n", i + 1);
         printf("ID...: %i\n", usuario[i].id);
         printf("NOME.: %s\n", usuario[i].nome);
         printf("SENHA: %s\n", senhas[i].senha);
      }
      printf("-----------------------------\n");
   }
   else
   {
      printf("SEM USUARIOS CADASTRADOS\n");
   }

   tecla = getch();
   if ((tecla == 27) || (tecla == 13))
   {
      clear();
      exit;
   }
}

int menu(int lin1, int col1, int qtd, char lista[3][40]) // funcao menu
{

   clear();
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

   textcolor(WHITE, _BLACK);
   setlocale(LC_ALL, "C");
   setlocale(LC_ALL, "");

   while (1)
   {
      linha = lin1 + 2;

      for (i = 0; i < qtd; i++)
      {
         if (i + 1 == opc)
         {
            linhaColuna(linha, col1 + 2);
            printf("                            ");
            textcolor(BLACK, _WHITE);
            linhaColuna(linha, col1 + 3);
            printf("%s", lista[i]);
            linhaColuna(linha, col1 + 2);
         }
         else
         {
            textcolor(WHITE, _BLACK);
            linhaColuna(linha, col1 + 2);
            printf("                              ");
            linhaColuna(linha, col1 + 2);
            printf("%s", lista[i]);
         }
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

int main() // funcao principal
{

   carregarUsuariosDoTXT();

   int opc;
   char lista[6][40] = {"CRIAR NOVO USUARIO", "ATUALIZAR USUARIO", "EXCLUIR USUARIO", "LISTAR USUARIOS", "ADM LOGIN", "SAIR"};

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

      if (opc == 2)
      {
         clear();
         atualizarUsuario();
      }

      if (opc == 3)
      {
         clear();
         excluirUsuario();
      }

      if (opc == 4)
      {
         clear();
         listarUsuarios();
      }

      if (opc == 5)
      {
         clear();
         if (loginAdmin())
         {
            printf("\nLogin realizado com sucesso!\n");
            listarUsuariosComDescriptografia();
         }
         else
         {
            printf("\nNome de usuario ou senha incorretos.\n");
         }
      }

      if (opc == 6)
      {
         textcolor(WHITE, _BLACK);
         clear();
         break;
      }
   }

   return 0;
}