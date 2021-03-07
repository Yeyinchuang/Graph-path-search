#include "Graphlnk.h"
#include<fstream>
#include<iostream>
#include<string>
#include <sstream>
using namespace std;
const int MAX = 9999;
template <class T, class E>
Graphlnk<T, E>::Graphlnk(int sz) {
	//���캯��������һ���յ��ڽӱ�
	maxVertices = sz;
	numVertices = 0;  numEdges = 0;

	//�������������
	NodeTable = new Vertex<T, E>[maxVertices];
	if (NodeTable == NULL)
	{
		cerr << "�洢�����" << endl;  exit(1);
	}
	for (int i = 0; i < maxVertices; i++)
		NodeTable[i].adj = NULL;
}; //�㷨ʱ�临�Ӷ�

template <class T, class E>
Graphlnk<T, E>::~Graphlnk() {
	//����������ɾ��һ���ڽӱ�
	for (int i = 0; i < numVertices; i++) {
		Edge<T, E> *p = NodeTable[i].adj;
		while (p != NULL) {
			NodeTable[i].adj = p->link;
			delete p;  p = NodeTable[i].adj;
		}
	}
	delete[]NodeTable;  		     //ɾ�����������
}; //�㷨ʱ�临�Ӷ�

template <class T, class E>
T Graphlnk<T, E>::getValue(int i) {      //ȡ���� i ��ֵ
	return (i >= 0 && i < numVertices) ?
		NodeTable[i].data : 0;
}
template<class T, class E>
int Graphlnk<T, E>::getVertexPos(const T vertx)
{
	for (int i = 0; i < numVertices; i++)
		if (NodeTable[i].data == vertx) return i;
	return -1;
}
; //�㷨ʱ�临�Ӷ�

template <class T, class E>
E Graphlnk<T, E>::getWeight(int v1, int v2)
{
	if (v1 != -1 && v2 != -1) {
		Edge<T, E> *p = NodeTable[v1].adj;	//V1�ĵ�һ�������ı�	
		while (p != NULL && p->dest != v2) 	p = p->link; //Ѱ���ڽӶ���v2
		if (p != NULL) return p->cost;		//�ҵ��˱�, ����Ȩֵ
	}
	return MAX;			//��(v1,v2)������
}
template<class T, class E>
int Graphlnk<T, E>::NumberOfVertices()
{
	return numVertices;
}
template<class T, class E>
int Graphlnk<T, E>::NumberOfEdges()
{
	return numEdges;
}
; //�㷨ʱ�临�Ӷ�

template <class T, class E>
bool Graphlnk<T, E>::insertVertex(const T& vertex) {
	//��ͼ�Ķ�����в���һ���¶���vertex��������ɹ�����������true, ���򷵻�false��
	if (numVertices == maxVertices) {
		return false; 	//�������, ���ܲ���
	}
	NodeTable[numVertices].data = vertex;			//�����ڱ�����
	numVertices++;
	return true;
}; //�㷨ʱ�临�Ӷ�

template <class T, class E>
bool Graphlnk<T, E>::insertEdge(int v1, int v2, E weight)
{
	if (v1 >= 0 && v1 < numVertices && v2 >= 0 && v2 < numVertices)
	{
		Edge<T, E> *q, *p = NodeTable[v1].adj;   	//v1��Ӧ�ı�����ͷָ��
		while (p != NULL && p->dest != v2)  p = p->link;   //Ѱ���ڽӶ���v2	
		if (p != NULL) 	return false;     	//�ҵ��˱�, ������		
		p = new Edge<T, E>;		//����, �����½��
		q = new Edge<T, E>;
		p->dest = v2;  p->cost = weight;
		p->link = NodeTable[v1].adj; 		//����v1������
		NodeTable[v1].adj = p;
		q->dest = v1;  q->cost = weight;
		q->link = NodeTable[v2].adj; 		//����v2������
		NodeTable[v2].adj = q;
		numEdges++;
		return true;
	}
	return false;
}; //�㷨ʱ�临�Ӷ�

template <class T, class E>
bool Graphlnk<T, E>::removeEdge(int v1, int v2)
{
	if (v1 != -1 && v2 != -1)
	{
		Edge<T, E> *p = NodeTable[v1].adj, *q = NULL, *s = p;
		while (p != NULL && p->dest != v2) { q = p; p = p->link; }  //v1��Ӧ���������ұ�ɾ��	
		if (p != NULL) {		//�ҵ���ɾ�߽��		
			if (p == s)    NodeTable[v1].adj = p->link;//�ý���Ǳ������׽��
			else     q->link = p->link;     delete p;
		}
		else    return false;   //û���ҵ���ɾ�߽��		
		p = NodeTable[v2].adj;   q = NULL, s = p��    //v2��Ӧ��������ɾ��
			while (p->dest != v1) { q = p;    p = p->link; }     //Ѱ�ұ�ɾ�߽��
		if (p == s)    NodeTable[v2].adj = p->link;   //�ý���Ǳ������׽��
		else     q->link = p->link;
		delete p;   numEdges--;
		return true;
	}
	return false;	   //û���ҵ����
}; //�㷨ʱ�临�Ӷ�



template <class T, class E>
bool Graphlnk<T, E>::removeVertex(int v) {      //��ͼ��ɾ��һ��ָ������v, v�Ƕ���š���ɾ���ɹ�, ��������true, ���򷵻�false��
	if (numVertices == 1 || v < 0 || v >= numVertices) {
		return false;
	}   		 //��ջ򶥵�ų�����Χ
	Edge<T, E> *p, *s, *t;  int k;
	while (NodeTable[v].adj != NULL) { 	//ɾ����v�������������н��
		p = NodeTable[v].adj;  k = p->dest;    //ȡ�ڽӶ���k
		s = NodeTable[k].adj;  t = NULL; 	//�ҶԳƴ�ŵı߽��
		while (s != NULL && s->dest != v) {
			t = s;  s = s->link;
		}
		if (s != NULL) {		//ɾ���Գƴ�ŵı߽��		
			if (t == NULL)    NodeTable[k].adj = s->link;
			else    t->link = s->link;
			delete s;
		}
		NodeTable[v].adj = p->link;	//�������v�ı�������
		delete p; numEdges--;	    //�붥��v������ı�����һ
	}
	numVertices--;	 //ͼ�Ķ��������1
	NodeTable[v].data = NodeTable[numVertices].data;
	//�#�����һ������������
	p = NodeTable[v].adj = NodeTable[numVertices].adj;
	while (p != NULL) {//��������Ķ����±���ˣ���Ӧ��Ҫ�ı�
		s = NodeTable[p->dest].adj;
		while (s != NULL) {
			if (s->dest == numVertices)
			{
				s->dest = v;//�޸Ķ�Ӧ�߶�����±�   
				break;
			}
			else    s = s->link;
		}
	}
	return true;
}

template<class T, class E>
bool Graphlnk<T, E>::setEdgeweight(int v1, int v2, E cost)
{
	if (v1 != -1 && v2 != -1) {
		Edge<T, E> *p = NodeTable[v1].adj;	//V1�ĵ�һ�������ı�	
		while (p != NULL && p->dest != v2) 	p = p->link; //Ѱ���ڽӶ���v2
		if (p != NULL)
		{
			p->cost = cost;
		}
		Edge<T, E> *q = NodeTable[v2].adj;	//V1�ĵڶ��������ı�	
		while (q != NULL && q->dest != v1) 	q = q->link; //Ѱ���ڽӶ���v1
		if (q != NULL)
		{
			q->cost = cost;
			return true;
		}
	}
	return false;			//��(v1,v2)������
}

template<class T, class E>
bool Graphlnk<T, E>::insertEdge(int v1, int v2)
{
	if (v1 >= 0 && v1 < numVertices && v2 >= 0 && v2 < numVertices)
	{
		Edge<T, E> *q, *p = NodeTable[v1].adj;   	//v1��Ӧ�ı�����ͷָ��
		while (p != NULL && p->dest != v2)  p = p->link;   //Ѱ���ڽӶ���v2	
		if (p != NULL) 	return false;     	//�ҵ��˱�, ������		
		p = new Edge<T, E>;		//����, �����½��
		q = new Edge<T, E>;
		p->dest = v2;
		p->link = NodeTable[v1].adj; 		//����v1������
		NodeTable[v1].adj = p;
		q->dest = v1;
		q->link = NodeTable[v2].adj; 		//����v2������
		NodeTable[v2].adj = q;
		numEdges++;
		return true;
	}
	return false;
}
; //�㷨ʱ�临�Ӷ�

template <class T, class E>
int Graphlnk<T, E>::getFirstNeighbor(int v) {
	//��������λ��Ϊ v �ĵ�һ���ڽӶ����λ��, 
	//����Ҳ���, ��������-1
	if (v != -1) {			//����v����
		Edge<T, E> *p = NodeTable[v].adj;	//��Ӧ�������һ���߽��
		if (p != NULL) return p->dest;		//����, ���ص�һ���ڽӶ���
	}
	return -1;		//��һ���ڽӶ��㲻����
}; //�㷨ʱ�临�Ӷ�

template <class T, class E>
int Graphlnk<T, E>::getNextNeighbor(int v, int w) {
	//��������v���ڽӶ���w����һ���ڽӶ����λ��,
	//��û����һ���ڽӶ���, ��������-1
	if (v != -1) {				//����v����
		Edge<T, E> *p = NodeTable[v].adj;
		while (p != NULL && p->dest != w)
			p = p->link;
		if (p != NULL && p->link != NULL)
			return p->link->dest; 	//������һ���ڽӶ���
	}
	return -1; 			//��һ�ڽӶ��㲻����
}; //�㷨ʱ�临�Ӷ�





template <class T, class E>
void Graphlnk<T, E>::DFS(const T& v0,const T& v1,int M) {//�Ӷ���v������ͼG����������ȱ�����������
	int i, loc0, loc1;
	int n = numVertices;    	//�������
	m = 0;
	bool *visited = new bool[n];          	//������������

	for (i = 0; i < n; i++)
	{
		visited[i] = false;	//��������visited��ʼ��
	}

	loc0 = getVertexPos(v0);
	loc1 = getVertexPos(v1);
	DFS(loc0, loc1, visited,M); 			//�Ӷ���0��ʼ�����������
	delete[] visited;			//�ͷ�visited
}

template <class T, class E>
void Graphlnk<T, E>::DFS(int v0, int v1, bool visited[],int M) {
	if (v0 == v1)
	    {
			int v;
			while (!s.empty())
			{
				v = s.top();
				s1.push(v);
				s.pop();
			}
			while (!s1.empty())
			{
				v = s1.top();
//				string a = G.getValue(v);
				printf("%s", getValue(v).c_str());
				printf("--->");
				s.push(v);
				s1.pop();
			}

//			visited[v] = false;	 	         	//ȡ�����ڷ���

//			string a = G.getValue(v1);
			printf("%s", getValue(v1).c_str());
			cout << endl;
			return;
		}
	m++;
	if (m == M)
	{
		m--;
		return;
	}
	visited[v0] = true;	 	         	//�����ʱ��
	s.push(v0);
	int w = getFirstNeighbor(v0);     	//��һ���ڽӶ���
											//	if (w == -1) q.pop();

	while (w != -1) {			//���ڽӶ���w����
		if (!visited[w])
		{
			DFS(w, v1, visited,M);  //��wδ���ʹ�, �ݹ���ʶ���w
			visited[w] = false;
		}
		w = getNextNeighbor(v0, w); 	//��һ���ڽӶ���
	}
	s.pop();
	m--;
}


template<class T, class E>
bool Graphlnk<T, E>::HaveEdge( int v1, int v2)
{
	int w,n = numVertices;
	bool *visited = new bool[n];
	for (int i = 0; i < n; i++) visited[i] = false;
	visited[v1] = true; 	                     	//���ѷ��ʱ��   
	w = getFirstNeighbor(v1);  	//��һ���ڽӶ���
	while (w != -1) {			//���ڽӶ���w����
		if (!visited[w]) {			//��δ���ʹ�
			visited[w] = true;
			if (w == v2)
			{
				return true;
			}
		}
		w = getNextNeighbor(v1, w); 	//�Ҷ���loc����һ���ڽӶ���
	}
	return false;
}

template<class T, class E>
void Graphlnk<T, E>::CreateChineseTrafficgraph()
{
	string str;//
	string strcity1;//���г���
	int city1;//����һ�ı��
//	string strcity2;//���г���
	int tag;//1 0�Ƿ��б�
	int cost;
	ifstream inputadjacency("adjacency.txt");
	ifstream inputdistance("distance.txt");
	if (!inputadjacency)
	{
		cout << "��adjacency���� txt �ļ����� d �̵ġ�txt�����ļ����У�" << endl;
		return;
	}
	if (!inputdistance)
	{
		cout << "��distance���� txt �ļ����� d �̵ġ�txt�����ļ����У�" << endl;
		return;
	}
	getline(inputdistance, str);
	getline(inputadjacency, str);
	istringstream ss(str);//��һ���ַ���ת������
	while (ss >> str) {
		insertVertex(str);
	}

	for (int i = 0; i < numVertices; i++) {//���ޱ�
		inputadjacency >> strcity1;
		city1 = getVertexPos(strcity1);
		for (int j = 0; j < numVertices; j++) {
			inputadjacency >> tag;
			if (tag == 1&& city1!=j) {
				insertEdge(city1, j);
			}
		}
	}

	for (int i = 0; i < numVertices; i++) {//��ֵ
		inputdistance >> strcity1;
		city1=getVertexPos(strcity1);
		for (int j = 0; j < i; j++) {
			inputdistance >> cost;
			if (HaveEdge(city1, j)) {
				setEdgeweight(city1,j,cost);
			}
		}
	}
}
template<class T, class E>
void Graphlnk<T, E>::interval(int v0)
{
	for (int i = 0; i < numVertices; i++)
	{
		if (i == v0) continue;
		cout << "��";
		printf("%s", getValue(v0).c_str());
		cout << "��";
		printf("%s", getValue(i).c_str());
		cout << "���м䲻��������ʡ��·���У�" << endl;
		DFS(getValue(v0), getValue(i),4);
	}
	cout << endl;
}
template<class T, class E>
void Graphlnk<T, E>::DjShortestPath(Graphlnk<T, E>& G, T vT, T v1T, E dist[], int path[])
{
	//Graph��һ����Ȩ����ͼ��dist[j], 0��j<n, �ǵ�ǰ�󵽵ĴӶ���v������j��
	//���·������, path[j], 0��j<n, ����󵽵����·����
	int v = getVertexPos(vT);
	int v1 = getVertexPos(v1T);
	int n = G.numVertices;
	bool *S = new bool[n]; 	   		//���·�����㼯
	int i, j, k;  E w, min;
	for (i = 0; i < n; i++) {
		dist[i] = G.getWeight(v, i);
		S[i] = false;
		if (i != v && dist[i] < MAX) path[i] = v;
		else path[i] = -1;
	}
	S[v] = true;  dist[v] = 0;     		//����v���붥�㼯��
	for (i = 0; i < n - 1; i++) {       		//�����������·��
		min = MAX;  int u = v;
		for (j = 0; j < n; j++)
			if (!S[j] && dist[j] < min) 		//ѡ����S�о������·���Ķ���u
			{
				u = j; min = dist[j];
			}
		S[u] = true;	               		//������u���뼯��S
//		if (u == v1)  return;               //�ҵ�ֱ���˳�
		for (k = 0; k < n; k++) {       		//�޸�
			w = G.getWeight(u, k);
			if (!S[k] && w < MAX && dist[u] + w < dist[k]) {    //����kδ����S                
				dist[k] = dist[u] + w;
				path[k] = u;     			//�޸ĵ�k�����·��
			}
		}
	}
}
template<class T, class E>
void Graphlnk<T, E>::AdaptedDjShortestPath(Graphlnk<T, E>& G, T vT, T v1T,T SkipovervT, E dist[], int path[])
{
	//Graph��һ����Ȩ����ͼ��dist[j], 0��j<n, �ǵ�ǰ�󵽵ĴӶ���v������j��
	//���·������, path[j], 0��j<n, ����󵽵����·����
	int v = getVertexPos(vT);
	int v1 = getVertexPos(v1T);
	int Skipoverv = getVertexPos(SkipovervT);
	int n = G.numVertices;
	bool *S = new bool[n]; 	   		//���·�����㼯
	int i, j, k;  E w, min;
	for (i = 0; i < n; i++) {
		if (i == Skipoverv) continue;
		dist[i] = G.getWeight(v, i);
		S[i] = false;
		if (i != v && dist[i] < MAX) path[i] = v;
		else path[i] = -1;
	}
	S[v] = true;  dist[v] = 0;     		//����v���붥�㼯��
	for (i = 0; i < n - 1-1; i++) {       		//�����������·��
		min = MAX;  int u = v;
		for (j = 0; j < n; j++)
			if (!S[j] && dist[j] < min&&j!=Skipoverv) 		//ѡ����S�о������·���Ķ���u
			{
				u = j; min = dist[j];
			}
		S[u] = true;	               		//������u���뼯��S
		if (u == v1)  return;               //�ҵ�ֱ���˳�
		for (k = 0; k < n; k++) {       		//�޸�
			if (k == Skipoverv) continue;
			w = G.getWeight(u, k);
			if (!S[k] && w < MAX && dist[u] + w < dist[k]) {    //����kδ����S                
				dist[k] = dist[u] + w;
				path[k] = u;     			//�޸ĵ�k�����·��
			}
		}
	}
}
template<class T, class E>
void Graphlnk<T, E>::Floyd(Graphlnk<T, E>& G, E **a, int **path)
{
	//a[i][j]�Ƕ���i��j֮������·�����ȡ�path[i][j]����Ӧ·���϶���j��ǰһ����Ķ����
	int i, j, k, n = G.numVertices;
	for (i = 0; i < n; i++)    		//����a��path��ʼ��
		for (j = 0; j < n; j++) {
			a[i][j] = G.getWeight(i, j);
			if (i != j && a[i][j] < MAX) path[i][j] = i;
			else path[i][j] = -1;
		}
	for (k = 0; k < n; k++)	//���ÿһ��k, ����a(k)��path(k)        
		for (i = 0; i < n; i++)
			for (j = 0; j < n; j++)
				if (a[i][k] + a[k][j] < a[i][j]) {
					a[i][j] = a[i][k] + a[k][j];
					path[i][j] = path[k][j];
				} 	//����·������, �ƹ� k �� j
}
template<class T, class E>
void Graphlnk<T, E>::AdaptedDFS(const T & v0, const T & v1, int M)
{
	int i, loc0, loc1;
	int n = numVertices;    	//�������
	m = 0;
	bool *visited = new bool[n];          	//������������

	for (i = 0; i < n; i++)
	{
		visited[i] = false;	//��������visited��ʼ��
	}

	loc0 = getVertexPos(v0);
	loc1 = getVertexPos(v1);
	AdaptedDFS(loc0, loc1, visited, M); 			//�Ӷ���0��ʼ�����������
	delete[] visited;			//�ͷ�visited
}

template<class T, class E>
void Graphlnk<T, E>::AdaptedDFS(int v0, int v1, bool visited[], int M)
{
	if (v0 == v1)
	{
		PathDist<T,E> pathdist;
		int v;
		int dist=0;
		int tag = 0;
		while (!s.empty())
		{
			v = s.top();
			s1.push(v);
			s.pop();
		}
		while (!s1.empty())
		{
			v = s1.top();
			pathdist.push(this,getValue(v));
			printf("%s", getValue(v).c_str());
			printf("--->");
			s.push(v);
			s1.pop();
		}
		pathdist.push(this,getValue(v1));
		Pathsave.push_back(pathdist);
		printf("%s", getValue(v1).c_str());
		cout << endl;
		return;
	}
	m++;
	if (m == M)
	{
		m--;
		return;
	}
	visited[v0] = true;	 	         	//�����ʱ��
	s.push(v0);
	int w = getFirstNeighbor(v0);     	//��һ���ڽӶ���
										//	if (w == -1) q.pop();

	while (w != -1) {			//���ڽӶ���w����
		if (!visited[w])
		{
			AdaptedDFS(w, v1, visited, M);  //��wδ���ʹ�, �ݹ���ʶ���w
			visited[w] = false;
		}
		w = getNextNeighbor(v0, w); 	//��һ���ڽӶ���
	}
	s.pop();
	m--;
}
;

