// Beatriz Vaz, Felipe Loschi, Felipe Reis, João Gabriel Chereze, Pedro Henrique Dias
#include <iostream>
#include <iomanip>
#include <list>
#include <climits>
#include <locale.h>
#include <windows.h> // Para conseguir alterar a cor do terminal.

using namespace std;

string tipo1[18] =
{
	"Água", "Fogo", "Grama", "Inseto", "Normal", "Pedra", "Psíquico", "Terrestre", "Venenoso",
	"Fantasma", "Lutador", "Noturno", "Voador", "Eletrico", "Fada", "Gelo", "Dragão", "Aço",
};
string tipo2[19] =
{
	"Água", "Fogo", "Grama", "Inseto", "Normal", "Pedra", "Psíquico", "Terrestre", "Venenoso",
	"Fantasma", "Lutador", "Noturno", "Voador", "Eletrico", "Fada", "Gelo", "Dragão", "Aço", "Null"
};

struct ponto
{
	int x, y; // Coordenadas.
};

struct pokemon
{
	string nome;
	int id, tipo1, tipo2;
	ponto posicao; // Coordenadas
};

struct treenode
{
	pokemon dados; // Dados dos pokémon para inserí-los.
	treenode *left;
	treenode *right;
};

struct Estrada
{
	int origem, destino, peso;
};

struct cidades
{
	list<Estrada> vizinhos;
	int codigo;  // codigo da cidade
	bool centro; // possui um centro poke ou nao
	string nome; // nome da cidade
};


typedef treenode *treenodeptr;

void dados_pokemon(pokemon &poke) { // Passando por referência para modificar o objeto real
    cout << "Escreva o nome, o tipo1, o tipo2, o id, e as coordenadas x e y do seu Pokémon: " << endl;
    getline(cin >> ws, poke.nome);
    cin >> poke.tipo1 >> poke.tipo2 >> poke.id >> poke.posicao.x >> poke.posicao.y;
}

/*int compara_pokemon(pokemon &poke1, pokemon &poke2)
{
	if (poke1.id == poke2.id) // Caso muito específico que o usuário pode digitar dois pokémons com mesmo ID.
		// Interessante para inserir mega-evoluções.
	{
		if (poke1.nome == poke2.nome)
		{
			return 0;
		}
		else if (poke1.nome < poke2.nome)
		{
			return -1;
		}
		else
		{
			return 1;
		}
	}
	else if (poke1.id < poke2.id)
	{
		return -1;
	}
	else
	{
		return 1;
	}
}*/

int compara_nome_pokemon(string &nome, pokemon &poke)
{
    if (nome == poke.nome)
    {
        return 0;
    }
    else if (nome < poke.nome)
    {
        return -1;
    }
    else
    {
        return 1;
    }
}

void pokeInsert(treenodeptr &p, pokemon poke)
{
    if (p == NULL)
    {
        p = new treenode;
        p->dados = poke;
        p->left = NULL;
        p->right = NULL;
        cout << "Inserido: " << poke.nome << endl;
    }
    else
    {
        int compare = compara_nome_pokemon(poke.nome, p->dados);
        if (compare == -1)
        {
            pokeInsert(p->left, poke);
        }
        else
        {
            pokeInsert(p->right, poke);
        }
    }
}

void entrada_grafo(cidades cidade[], int num_vertices, int arestas, bool orientado)
{
	int origem, destino, peso;

	for (int i = 0; i < num_vertices; i++)
	{
		cout << "Agora preciso que informe o nome da cidade: " << endl;
		getline(cin >> ws, cidade[i].nome);
		cout << "Agora preciso que informe o código da cidade: " << endl;
		cin >> cidade[i].codigo;
		cout << "Para verificação de centros pokémon na cidade, digite '0' para 'não' e '1' para 'sim' : " << endl;
		cin >> cidade[i].centro;
		cout << endl;
	}

	cout << endl;

	// inserindo os dados do grafo

	// Instrução para entrada dos caminhos do grafo
	cout << "Agora vamos inserir os caminhos entre as cidades." << endl;
	cout << "Para cada caminho, escreva o ponto de origem, o ponto de chegada e a distância entre as cidades." << endl;
	cout << "Exemplo de entrada: '0 1 10' indica um caminho da cidade 0 para a cidade 1 com distância 10." << endl;
	cout << endl;

	for (int i = 0; i < arestas; i++)
	{
		cin >> origem >> destino >> peso;

		cidade[origem].vizinhos.push_back( {origem, destino, peso});

		if (orientado == 0)
		{
			cidade[destino].vizinhos.push_back( {destino, origem, peso});
		}
	}
}

// Função para mostrar as cidades e seus vizinhos.
void print_cidades(cidades cidade[], int numVertices)
{
	list<Estrada>::iterator p;
	cout << "Lista das cidades cadastradas: " << endl;
	cout << endl;
	cout << "| " << setw(2) << "Nome(s) " << setw(15) << " | " << setw(6) << right << setw(10) << "Código" << setw(7) << " | " << setw(15) << " Centro Pokémon" << right << setw(43) << " | Vizinhos - Origem, destino e distância" << endl;
	for (int i = 0; i < numVertices; i++)
	{
		cout << "| ";
		cout << setw(19) << left << setw(20) << left << cidade[i].nome << left << " | ";
		cout << left << setw(8) << right << cidade[i].codigo << right << setw(9) << " | ";
		if (!cidade[i].centro)
			cout << "Não possui centro |";
		else
			cout << right << setw(15) << "Possui centro" << right << setw(4) << " |";

		// Verifica se há vizinhos para esta cidade
		if (!cidade[i].vizinhos.empty())
		{
			cout << " ";
			for (p = cidade[i].vizinhos.begin(); p != cidade[i].vizinhos.end(); p++)
			{
				cout << p->origem << " -> " << p->destino << "(" << p->peso << ")" << " ";
			}
		}
		cout << endl;
	}
}

int shortest_path_dijkstra(cidades cidades[], int vertices, int cod_atual)
{
	bool intree[vertices];
	int distance[vertices], parent[vertices];
	list<Estrada>::iterator p;

	for (int u = 0; u < vertices; u++)
	{
		intree[u] = false;
		distance[u] = INT_MAX;
		parent[u] = -1;
	}
	distance[cod_atual] = 0;
	int v = cod_atual;
	while (intree[v] == false)
	{
		intree[v] = true;
		for (p = cidades[v].vizinhos.begin(); p != cidades[v].vizinhos.end(); p++)
		{
			int dest = p->destino;
			int weight = p->peso;
			if (distance[dest] > distance[v] + weight)
			{
				distance[dest] = distance[v] + weight;
				parent[dest] = v;
			}
		}
		v = 0;
		int dist = INT_MAX;
		for (int u = 0; u < vertices; u++)
		{
			if (intree[u] == false && dist > distance[u])
			{
				dist = distance[u];
				v = u;
			}
		}
	}
	v = 0;
	int dist = INT_MAX;
	for (int u = 0; u < vertices; u++)
	{
		if (intree[u] == true && dist > distance[u] && cidades[u].centro)
		{
			dist = distance[u];
			v = u;
		}
	}
	return v;
}


void busca_centro(cidades cidade[], int num_vertices)
{
	int pos = 0, cod_atual, end;
	cout << "Escreva o código da cidade em que você está: ";
	cin >> cod_atual;
	for (int i = 0; i < num_vertices; i++)
	{

		if (cod_atual == cidade[i].codigo)
		{
			pos = i;
			if (cidade[pos].centro)
			{
				cout << "Sua cidade já tem um centro pokémon!" << endl;
				pos = -1;
				break;
			}
		}
	}
	if (pos != -1)
	{

		end = shortest_path_dijkstra(cidade, num_vertices, pos);
		cout << "A cidade mais próxima com um centro Pokémon é: " << cidade[end].nome << endl;
	}
}

treenodeptr pokeSearch(treenodeptr p, string poke)
{
    if (p == NULL)
    {
        return NULL;
    }
    else
    {
        cout << "Comparando: " << poke << " com " << p->dados.nome << endl;
        if (poke == p->dados.nome) // Elemento encontrado.
        {
            return p;
        }
        else if (poke < p->dados.nome) // Procura na subárvore esquerda.
        {
            return pokeSearch(p->left, poke);
        }
        else // Procura na subárvore direita.
        {
            return pokeSearch(p->right, poke);
        }
    }
}

void pesquisarPokemon(treenodeptr arvore) {
    string op;
    cout << "Digite o nome do pokémon que deseja pesquisar: ";
    getline(cin >> ws, op);
    treenodeptr encontrado = pokeSearch(arvore, op);
    if (encontrado != NULL) {
        cout << "Encontrado" << endl;
    } else {
        cout << "Não encontrado" << endl;
    }
}

treenodeptr tMenor(treenodeptr &p) {
    treenodeptr t;
    t = p;
    if (t->left == NULL) { // Encontrou o menor valor
        p = p->right;
        return t;
    } else // Continua procurando na esquerda
        return tMenor(p->left);
}

bool pokeRemove(treenodeptr &p, string nome){
    treenodeptr pk;
    
    if(p == NULL)
        return false;
        
    if(compara_nome_pokemon(nome, p->dados) == 0){
        pk = p;
        
        if(p->left == NULL)
            p = p->right;
        
        else{
            if(p->right == NULL)
                p = p->left;
            
            else{
                pk = tMenor(p->right);
                p->dados = pk->dados;
            }
        }
        
        delete pk;
        return true;
    }
    
    else{
        if(compara_nome_pokemon(nome, p->dados) == -1)
            return pokeRemove(p->left, nome);
        else
            return pokeRemove(p->right, nome);
    }
}

void removerPokemon(treenodeptr &arvore) {
    string a_remover;
    bool removido;
    cout << "Digite o nome do pokémon que deseja remover: ";
    getline(cin >> ws, a_remover);
    removido = pokeRemove(arvore, a_remover);
    if (removido) {
        cout << "Removido" << endl;
    } else {
        cout << "Não encontrado para remoção" << endl;
    }
}

int main()
{
	// Permitindo acentuação no código
	setlocale(LC_ALL, "");
	system("color 74"); // Altera cor do terminal.

	// Declarando variaveis
	int num_vertices, arestas, menu = 1;
	bool orientado;
	treenodeptr arvore = NULL;
	pokemon novo_pokemon;

	// Introdução
	cout << "Olá, treinador. Bem vindo ao sistema de localização de pokémons!" << endl;
	// cout << "\x1b[3mEste texto está em itálico!\x1b[0m" << endl; // Texto em itálico.
	cout << endl;

	// Entrada dos dados do grafo a ser contruido:
	cout << "Escreva o número de cidades e o número de caminhos:" << endl;
	cin >> num_vertices >> arestas;
	cidades cidade[num_vertices];

	// Definindo se o grafo sera orientado ou nao
	cout << "Digite '0' para dizer se o caminho será não é orientado ou '1' para orientado." << endl;

	do
	{
		cin >> orientado;
		if (orientado == 0 || orientado == 1)
		{
			break;
		}

		cout << "Digite '0' para dizer se o caminho será não é orientado ou '1' para orientado." << endl;
	}
	while (orientado != 0 || orientado != 1);

	// Codigo de menu simples, usando a variavel menu no loop
	while (menu != 0)
	{
		// Introducao do menu e entrada da variavel
		cout << endl;
		cout << "MENU" << endl
			 << "1-Entrada de dados das cidades"
			 << endl
			 << "2-Imprimir as cidades cadastradas"
			 << endl
			 << "3-Buscar cidade com centro pokémon mais próximo"
			 << endl
			 << "4-Fazer a Inserção dos Pokémons"
			 << endl
			 << "5-Pesquisar os pokémons"
			 << endl
			 << "6-Remover algum pokémon"
			 << endl
			 << "Pressione qualquer outro número pra sair"
			 << endl;
		cin >> menu;
		cout << endl;

		// Usando switch case para o menu
		switch (menu)
		{
			// Caso 1: Entrada do grafo
		case 1:
			// fazendo a inicialização dos dados das cidades (codigo, centro poke e nome)
			entrada_grafo(cidade, num_vertices, arestas, orientado);
			break;
			// Caso 2: Imprime as cidades salvas
		case 2:
			print_cidades(cidade, num_vertices);
			break;
			// Caso 3: Busca a cidade com centro pokémon mais próxima.
		case 3:
			busca_centro(cidade, num_vertices);
			break;
		case 4:
			// Caso 4: Insere os pokémons na árvore.
			dados_pokemon(novo_pokemon);
			pokeInsert(arvore, novo_pokemon);
			break;
		case 5 :
			// Caso 5: busca os pokémons na árvore.
			pesquisarPokemon(arvore);
			break;
		case 6:	
			// Caso 6: Remove os pokémons na árvore.
			
			removerPokemon(arvore);
			break;
				
		// Caso receba outro valor: sai do loop
		default:
			menu = 0;
		}
	}
	return 0;
}
