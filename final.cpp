#include <stdlib.h>
#include <iostream>
#include <cmath>
#include <cstdio>
#include <stack>
#include <queue>
#include <bitset>
#include <functional>
#include <vector>

using namespace std;

#define MAX  3

int limit = -1;
bitset<876543211> visit;
int posFinal[10][2];
int flag;
bool found;
int mat_in[MAX][MAX], mat_fi[MAX][MAX];

/*int mat_in[MAX][MAX] = {
  {3,4,2},
  {5,1,7},
  {6,0,8}
  };

  int mat_fi[MAX][MAX] = {
  {1,2,3},
  {8,0,4},
  {7,6,5}
  };
*/

struct savedata{ 
  int mat[MAX][MAX]; // Matriz
  int i; // Linha do espaço/0
  int j; // Coluna do espaço/0
  const char* dir; // Direção
  int level; // Alura da arvore
  int cost; // Custo
  savedata* last; // Apontador para "Pai"
};
typedef savedata* Psave;

Psave now; // Estrutura atual

auto cmp = [](Psave left, Psave right) // Ordem da priority_queue
{
  return (left->cost) > (right->cost);
};

queue <Psave> bag0;
stack <Psave> bag1;
priority_queue<Psave, std::vector<Psave>, decltype(cmp)> bag2(cmp);

int calCost (int mat[][MAX]) // Calcular o custo  para a priority_queue
{
  int cost=0;
  if(flag == 3 || flag == 4)
    {
      for(int i=0; i< MAX; i++)
	for(int j=0; j<MAX; j++)
	  if(posFinal[mat[i][j]][0] != i || posFinal[mat[i][j]][1] != j)
	    cost += abs(i - posFinal[mat[i][j]][0]) + abs(j - posFinal[mat[i][j]][1]);
    }
  return cost;
}

void copy(int mat1[][MAX], int mat2[][MAX])  // Faz uma copia da mat2 para a mat1
{
  int i, j;
  for(i=0; i<MAX; i++)
    for(j=0; j<MAX; j++)
      mat1[i][j]=mat2[i][j];
}

int toInt(int mat[][MAX])  // Transforma uma matriz num Int
{
  int num = 0;
  int mul = 1;
  for(int i=MAX-1; i>=0; i--)
    for(int j=MAX-1; j>=0; j--)
      {
	num += mat[i][j]*mul;
	mul *= 10;
      }
  return num;
}

Psave goAdd(int mat[][MAX], int si, int sj,  const char* dir, int level) // Adiciona à queue os dados recebidos
{
  Psave one = (Psave) malloc(sizeof(savedata));

  int num = toInt(mat);
  visit[num] = true;
  
  int cost = calCost(mat);
  
  if(flag == 4)
    cost += level;
  
  copy(one->mat, mat);
  
  one->i = si;
  one->j = sj;
  
  one->dir = dir;
  one->level = level;
  one->cost = cost;
  one->last = now; // Guarda o apontador para o pai, que é a estrutura atual
  
  switch(flag) // Dependendo da busca selecionada, guarda os dados na respetiva estrutura
    {
    case 0: { bag0.push(one); break; }
    case 1: case 2: { bag1.push(one); break; }
    case 3: case 4: { bag2.push(one); break; }
    }
  return one;
}

void space(int v[]) // Procura a posição do espaço, neste caso o 0
{ 
  int i, j;
  for(i=0; i<MAX; i++)
    for(j=0; j<MAX; j++)
      if(mat_in[i][j] == 0)
	{
	  v[0] = i;
	  v[1] = j;
	}
}

void print(int mat[][MAX]) // Imprime a matriz recebida
{
  int i, j;
  cout << endl;
  for(i=0; i<MAX; i++)
    {
      for(j=0; j<MAX; j++)
	printf("%d ",mat[i][j]);
      printf("\n");
    }
  cout << endl;
}

bool final (int mat[][MAX])  // Vê se já encontou a matriz final
{
  for(int i=0; i<MAX; i++)
    for(int j=0; j<MAX; j++)
      if(mat_fi[i][j] != mat[i][j])
	return false;
  return true;  
}

string endSt; // Ultima direção
int endMat[MAX][MAX]; // Ultima matriz

int search(int x, int y, const char* st) // Trata da respetiva descendencia do now
{
  int mat[MAX][MAX];
  Psave two;
  int num;
  copy(mat, now->mat);
  mat[now->i][now->j] = mat[now->i + x][now->j + y];
  mat[now->i + x][now->j + y] = 0;
  
  num = toInt(mat);
  if(limit != -1 || !visit[num])
    two=goAdd(mat, now->i + x, now->j + y, st, now->level + 1);
  
  if(final(mat))
    {     
      found = true;
      now = two;
      endSt = st;
      copy(endMat, mat);
      return -1;
    }
  return 0;
}

void goSearch() // Pesquisa
{
  bool empty = false;
  while(!empty)
    {
      switch(flag)
	{
	case 0: { now = bag0.front(); bag0.pop(); break; }
	case 1: case 2: { now = bag1.top(); bag1.pop(); break; }
	case 3: case 4: { now = bag2.top(); bag2.pop(); break; }
	}
      if(limit == -1 || now->level <= limit)
	{
	  if(now->i + 1 < MAX  && now->dir[0] != 'C')
	    if(search(1, 0, "Baixo") == -1)
	      break;
	  
	  if(now->j + 1 < MAX  && now->dir[0] != 'E')
	    if(search(0, 1, "Direita") == -1)
	      break;
	  
	  if(now->i - 1 >= 0  && now->dir[0] != 'B')
	    if(search(-1, 0, "Cima") == -1)
	      break;
	  
	  if(now->j - 1 >= 0  && now->dir[0] != 'D')
	    if(search(0, -1, "Esquerda") == -1)
	      break;
	}
      switch(flag)
	{
	case 0: { empty = bag0.empty(); break; }
	case 1: case 2: { empty = bag1.empty(); break; }
	case 3: case 4: { empty = bag2.empty(); break; }
	}
    }
}

stack<Psave> trace;

void backTrace()  // Descodificar o caminho de volta
{
  while(now->dir[0] != 'N')
    {
      trace.push(now->last);

      copy(now->mat, now->last->mat);
      now->i = now->last->i;
      now->j = now->last->j;
      now->dir = now->last->dir;
      now->last = now->last->last;
    }
}

int solvable(int m1[], int m2[])
{
  int count1 = 0;
  int count2 = 0;
  for (int i = 0; i < MAX*MAX - 1; i++)
    for (int j = i + 1; j < MAX*MAX; j++)
      if (m1[j] && m1[i] &&  m1[i] > m1[j])
	count1++;
  for (int i = 0; i < MAX*MAX - 1; i++)
    for (int j = i + 1; j < MAX*MAX; j++)
      if (m2[j] && m2[i] &&  m2[i] > m2[j])
	count2++; 
  if(count1 % 2 == count2 % 2)
    return true;
  return false;
}

int main()
{
  int temp;

  cout << "Puzzle inicial" << endl;
  
  for(int i=0; i<MAX; i++)
    for(int j=0; j<MAX; j++)
      {
	cin >> temp;
	mat_in[i][j] = temp;
	posFinal[temp][0] = i;
	posFinal[temp][1] = j;
      }
  cout << "Puzzle final" << endl;
  for(int i=0; i<MAX; i++)
    for(int j=0; j<MAX; j++)
      cin >> mat_fi[i][j];

  if(!solvable((int*) mat_in, (int*) mat_fi))
    {
      cout << "Não existe solução" << endl;
      return 0;
    }
  cout << "Tipo de busca:" << endl;
  cout << "0 para BFS, 1 para DFS, 2 para DFSi, 3 para Greedy, 4 para A*" << endl;
  
  cin >> flag;
  cout << endl << endl;
  int v[2];
  space(v);
  
  switch(flag)
    {
    case 2:
      {
	limit = 1;
	while(!found)
	  {
	    goAdd(mat_in, v[0], v[1], "Normal", 0);
	    goSearch();
	    visit.reset();
	    limit++;
	  }
	break;
      }
    default: { limit = -1; goAdd(mat_in, v[0], v[1], "Normal", 0);  goSearch(); }
    }

  cout << "Inicio do Percurso" << endl;
  backTrace();
  int tamanho = trace.size();
  print(mat_in);
  trace.pop();
  
  while(trace.size() > 0)
    {
      now = trace.top();
      trace.pop();
      cout << now->dir << endl;
      print(now->mat);
    }

  cout << endSt << endl;
  print(endMat);
  cout << "Percurso terminado!" << endl << tamanho << " passos." << endl;
  return 0;
}
