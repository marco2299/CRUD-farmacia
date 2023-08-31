#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
using namespace std;

struct farmacia
{
  long int id_produto;
  char valor[9];
  char nome_produto[100];
  char tipo[9]; // houve uma troca nesta variável, antes era "estado" como
                // definido no campus e agora se tornou "tipo"
  char situacao[9];
};
void converterCSVparaBinario(const string &arquivoCSV, const string &arquivoBinario, bool &converteu)
{
  ofstream baseBin(arquivoBinario, ios::binary); // abre o arquivo em modo binário
  ifstream base(arquivoCSV);
  farmacia produto;
  string linha;

  if (!base) // se o arq não abrir
  {
    cout << "Erro ao abrir o arquivo CSV." << endl;
    converteu = false;
  }
  else
  {
    while (getline(base, linha)) //imprime os campos convertidos
    {
      istringstream iss(linha);
      string item;

      // Leitura dos campos separados por ponto e vírgula
      if (getline(iss, item, ';'))
      {
        produto.id_produto = stol(item); // Converte o campo código para long int
      }
      else
      {
        cout << "Erro na leitura do campo codigo." << endl;
      }

      if (getline(iss, item, ';'))
      {
        strncpy(produto.valor, item.c_str(),sizeof(produto.valor) - 1); // Copia o campo preço para a estrutura farmacia
        produto.valor[sizeof(produto.valor) - 1] = '\0'; // Adiciona um fim de linha para a string
      }
      else
      {
        cout << "Erro na leitura do campo preco." << endl;
      }

      if (getline(iss, item, ';'))
      {
        strncpy(produto.nome_produto, item.c_str(),sizeof(produto.nome_produto) - 1); // Copia o campo nome para a estrutura farmacia
        produto.nome_produto[sizeof(produto.nome_produto) - 1] = '\0'; // Adiciona um fim de linha para a string
      }
      else
      {
        cout << "Erro na leitura do campo nome." << endl;
      }

      if (getline(iss, item, ';'))
      {
        strncpy(produto.tipo, item.c_str(),sizeof(produto.tipo) - 1); // Copia o campo tipo para a estrutura farmacia
        produto.tipo[sizeof(produto.tipo) - 1] = '\0'; // Adiciona um fim de linha para a string
      }
      else
      {
        cout << "Erro na leitura do campo tipo." << endl;
      }

      if (getline(iss, item, ';'))
      {
        strncpy(produto.situacao, item.c_str(),sizeof(produto.situacao) - 1); // Copia o campo situação para a estrutura farmacia
        produto.situacao[sizeof(produto.situacao) - 1] = '\0'; // Adiciona um fim de linha para a string
      }
      else
      {
        cout << "Erro na leitura do campo situacao." << endl;
      }

      baseBin.write( (const char *)&produto, sizeof(farmacia)); // Escreve a estrutura farmacia no arquivo binário
      
      // Imprime os valores lidos
      cout << produto.id_produto << " " << produto.valor << " "
           << produto.nome_produto << " " << produto.tipo << " "
           << produto.situacao << "\n";
    }
    converteu = true;
  }
  if (converteu == true)
  {
    cout << endl << "Arquivo CSV convertido com sucesso!" << endl;
  }
}

void guarda(farmacia produto[], int &tamanho) // guarda as informações da base no vetor de estrutura
{
  ifstream lendo("base.dat", ios::binary);
  tamanho = 0;
  while (lendo.read( (char *)&produto[tamanho], sizeof(farmacia))) // lê os dados do arquivo e armazena no vetor produto
  {
    tamanho++;
  }
  lendo.close();
}

// insere uma informação no vetor de struct
void inserir(farmacia vet[], int &tamanho, long int id_produto)
{
  string aux;
  for (int i = 0; i < tamanho; i++) // verifica se o codigo ja existe
  {
    if (id_produto == vet[i].id_produto)
    {
      cout << "Este codigo já existe na base de dados!" << endl
           << "Por favor, digite outro codigo: ";
      cin >> id_produto;
      i = 0; // reinicia a variavel i para fazer uma nova verificação
    }
  }
  for (int i = 0; i < tamanho; i++) // encontra a posicao vazia e insere os novos dados
  {
    if (vet[i].id_produto == -1)
    {
      vet[i].id_produto = id_produto;
      cout << "Digite o preco: ";
      cin >> vet[i].valor;
      cout << "Digite o nome: ";
      cin.ignore();
      getline(cin, aux);
      strncpy(vet[i].nome_produto, aux.c_str(), sizeof(vet[i].nome_produto) - 1); // copia o conteúdo de aux para vet[i].nome
      vet[i].nome_produto[sizeof(vet[i].nome_produto) - 1] ='\0'; // insere o termino de linha para a string
      cout << "Digite o tipo de remedio: ";
      cin >> vet[i].tipo;
      cout << "Digite a situacao: ";
      cin >> vet[i].situacao;
      cout << "Produto inserido com sucesso!" << endl;
      i = tamanho + 1; // ordem de parada
    }
  }
}

// exlui logicamente um valor do vetor de estrutura
void excluir(farmacia vet[], int &tamanho, long int id_produto)
{
  bool codigoEncontrado = false;

  for (int i = 0; i < tamanho; i++)
  {
    if (id_produto == vet[i].id_produto) // ao encontrar o codigo, a chave -1 é atribuida para indicar sua exclusão
    {
      vet[i].id_produto = -1;
      int t = 9;
      for (int j = 0; j < t; j++) // adiciona caracter vazio aos elementos do produto excluido
      {
        vet[i].valor[j] = ' ';
        vet[i].situacao[j] = ' ';
        vet[i].tipo[j] = ' ';
      }
      t = strlen(vet[i].nome_produto);
      for (int k = 0; k < t; k++)
      {
        vet[i].nome_produto[k] = ' ';
      }
      codigoEncontrado = true;
      cout << "Excluido!!!" << endl;
      i = tamanho + 1; // ordem de parada
    }
  }
  if (!codigoEncontrado)
  {
    cout << "Este codigo nao existe" << endl;
  }
}
void exportarCSV(farmacia produto[], int tamanho)
{
  ofstream arquivo("dados.csv"); // Abre o arquivo para escrita

  // Escreve os dados de cada produto no arquivo
  for (int i = 0; i < tamanho; i++)
  {
    arquivo << produto[i].id_produto << ";" << produto[i].valor << ";"
            << produto[i].nome_produto << ";" << produto[i].tipo << ";"
            << produto[i].situacao << endl;
  }
  arquivo.close(); // Fecha o arquivo
}

// mostra o vetor inteiro
void mostraTudo(farmacia produto[], int &tamanho)
{
  for (int j = 0; j < tamanho; j++)
  {
    cout << produto[j].id_produto << " " << produto[j].valor << " "
         << produto[j].nome_produto << " " << produto[j].tipo << " "
         << produto[j].situacao << "\n";
  }
}

// mostra um intervalo inserido pelo usuário
void mostraIntervalo(farmacia produto[], int inicio, int fim)
{
  for (int i = inicio; i < fim; i++)
  {
    cout << produto[i].id_produto << " " << produto[i].valor << " "
         << produto[i].nome_produto << " " << produto[i].tipo << " "
         << produto[i].situacao << "\n";
  }
}
// ordena o vetor por código usando o método quickSort
void quickSort(farmacia vet[], int esquerda, int direita)
{
  int i = esquerda, j = direita; // inicio e fim
  long int pivo = vet[(esquerda + direita) / 2].id_produto; // Pivo = meio do vetor

  while (i <= j) // Enquanto os índices não se cruzarem
  {
    while ( vet[i].id_produto < pivo) // Encontra o elemento à esquerda do pivô que é maior que o pivô
    {
      i++;
    }
    while (vet[j].id_produto > pivo) // Encontra o elemento à direita do pivô que é maior que o pivô
    {
      j--;
    }
    if (i <= j) // Se os índices não se cruzaram
    {
      swap(vet[i], vet[j]);
      i++;
      j--;
    }
  }

  if (esquerda < j) // Se existem elementos à esquerda do pivô
  {
    quickSort( vet, esquerda,j); // Chama a função recursivamente para ordenar a sublista à esquerda
  }
  if (i < direita) // Se existem elementos à direita do pivô
  {                            
    quickSort(vet, i, direita); // Chama a função recursivamente para ordenar a sublista à direita
  }
}

// busca um código no vetor
int buscaCodigo(farmacia vet[], int &tamanho, long int id_produto)
{
  for (int i = 0; i < tamanho; i++)
  {
    if (id_produto == vet[i].id_produto)
    {
      return i;
    }
  }
  return -1;
}

// função de editar
void editar(farmacia vet[], int &tamanho, long int id_produto)
{
  int posicao = buscaCodigo(vet, tamanho, id_produto);
  if (posicao != -1)
  {
    cout << "Produto encontrado. Insira os novos valores:" << endl;
    cin.ignore(); // Ignora o caractere de nova linha deixado pelo cin anterior

    cout << "Digite o novo codigo: ";
    cin >> vet[posicao].id_produto;

    cout << "Digite o novo preco: ";
    cin >> vet[posicao].valor;

    cout << "Digite o novo nome: ";
    cin.ignore();
    cin.getline(vet[posicao].nome_produto, sizeof(vet[posicao].nome_produto));

    cout << "Digite o novo tipo de remedio: ";
    cin >> vet[posicao].tipo;

    cout << "Digite a nova situacao: ";
    cin >> vet[posicao].situacao;

    cout << "Produto editado com sucesso!" << endl;
  }
  else
  {
    cout << "Codigo nao encontrado." << endl;
  }
}

int main()
{
  farmacia *produto = new farmacia[100];
  int tamanho = 0, opcao;
  bool converteu = false;
  string arquivoCSV = "base.csv";
  string arquivoBinario = "base.dat"; 
  
  cout 
    << "     ___ ___  ___  ___  _   _ _____ ___  ___          " << endl
    << "    | _ | _ \\/ _ \\|   \\| | | |_   _/ _ \\/ __|         " << endl
    << "    |  _|   | (_) | |) | |_| | | || (_) \\__ \\         " << endl
    << "    |_| |_|_\\\\___/|___/ \\___/  |_| \\___/|___/         " << endl
    << "    _     ___  ___   _   _ __  __   _       _         " << endl
    << "  _| |_  |   \\| __| | | | |  \\/  | /_\\    _| |_       " << endl
    << " |_   _| | |) | _|  | |_| | |\\/| |/ _ \\  |_   _|      " << endl
    << "   |_|   |___/|___|  \\___/|_|  |_/_/ \\_\\   |_|        " << endl
    << "      ___ _   ___ __  __   _   ___ ___   _            " << endl
    << "     | __/_\\ | _ |  \\/  | /_\\ / __|_ _| /_\\           " << endl
    << "     | _/ _ \\|   | |\\/| |/ _ \\ (__ | | / _ \\          " << endl
    << "     |_/_/ \\_|_|_|_|  |_/_/ \\_\\___|___/_/ \\_\\         " << endl << endl;

  cout << "Digite 1 para converter a base CSV para binario." << endl
       << "Digite 2 caso ela ja esteja convertida." << endl;
  cin >> opcao;

  if (opcao == 1)
  {
    converterCSVparaBinario(arquivoCSV, arquivoBinario, converteu);
  }
  else if (opcao == 2)
  {
    converteu = true;
  }

  if (converteu)
  {
    cout << "continuando..." << endl;
    guarda(produto, tamanho);
    int menu;
    long int cod;
    
    do {
  
      cout << "\n\nMenu\n\n";
      cout << "Digite 1 para inserir um produto\n";
      cout << "Digite 2 para excluir um produto\n";
      cout << "Digite 3 para mostrar todos os produtos\n";
      cout << "Digite 4 para mostrar produtos por intervalo\n";
      cout << "Digite 5 para mostrar produtos ordenados pelo codigo\n";
      cout << "Digite 6 para editar um produto\n";
      cout << "Digite 7 para exportar os dados em um arquivo CSV\n";
      cout << "Digite 0 para sair do programa\n";
      cout << "Opcao: ";
      cin >> menu;

      switch (menu)
      {
        case 1:
          cout << "\n Digite o codigo do produto: ";
          cin >> cod;
          inserir(produto, tamanho, cod);
          break;
        case 2:
          cout << "\nDigite o codigo do produto a ser excluido: ";
          cin >> cod;
          excluir(produto, tamanho, cod);
          break;
        case 3:
          mostraTudo(produto, tamanho);
          break;
        case 4:
          int inicio, fim;
          cout << "\nDigite o intervalo de produtos a serem mostrados:\n";
          cout << "Início: ";
          cin >> inicio;
          cout << "Fim: ";
          cin >> fim;
          mostraIntervalo(produto, inicio, fim);
          break;
        case 5:
          quickSort(produto, 0, tamanho - 1);
          mostraTudo(produto, tamanho);
          break;
        case 6:
          cout << "\nDigite o codigo do produto a ser editado: ";
          cin >> cod;
          editar(produto, tamanho, cod);
          break;
        case 7:
          cout << "\nO arquivo dados.csv foi gerado!\n";
          exportarCSV(produto, tamanho);
          break;
        case 0:
          cout << "\nPrograma encerrado.\n";
          break;
        default:
          cout << "\nOpcao invalida!!!\n";
          break;
      }
    } while (menu != 0);

    // Salva as alterações no arquivo
    ofstream escrevendo("base.dat", ios::binary);
    for (int i = 0; i < tamanho; i++)
    {
      escrevendo.write((char *)&produto[i], sizeof(farmacia));
    }
    escrevendo.close();

    delete[] produto;
  }
  else
  {
    cout << "O programa foi encerrado.";
  }
  return 0;
}