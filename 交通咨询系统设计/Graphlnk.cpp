#include "Graphlnk.h"
#include<fstream>
#include<iostream>
#include<string>
#include <sstream>
using namespace std;
const int MAX = 9999;
template <class T, class E>
Graphlnk<T, E>::Graphlnk(int sz) {
	//构造函数：建立一个空的邻接表
	maxVertices = sz;
	numVertices = 0;  numEdges = 0;

	//创建顶点表数组
	NodeTable = new Vertex<T, E>[maxVertices];
	if (NodeTable == NULL)
	{
		cerr << "存储分配错！" << endl;  exit(1);
	}
	for (int i = 0; i < maxVertices; i++)
		NodeTable[i].adj = NULL;
}; //算法时间复杂度

template <class T, class E>
Graphlnk<T, E>::~Graphlnk() {
	//析构函数：删除一个邻接表
	for (int i = 0; i < numVertices; i++) {
		Edge<T, E> *p = NodeTable[i].adj;
		while (p != NULL) {
			NodeTable[i].adj = p->link;
			delete p;  p = NodeTable[i].adj;
		}
	}
	delete[]NodeTable;  		     //删除顶点表数组
}; //算法时间复杂度

template <class T, class E>
T Graphlnk<T, E>::getValue(int i) {      //取顶点 i 的值
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
; //算法时间复杂度

template <class T, class E>
E Graphlnk<T, E>::getWeight(int v1, int v2)
{
	if (v1 != -1 && v2 != -1) {
		Edge<T, E> *p = NodeTable[v1].adj;	//V1的第一条关联的边	
		while (p != NULL && p->dest != v2) 	p = p->link; //寻找邻接顶点v2
		if (p != NULL) return p->cost;		//找到此边, 返回权值
	}
	return MAX;			//边(v1,v2)不存在
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
; //算法时间复杂度

template <class T, class E>
bool Graphlnk<T, E>::insertVertex(const T& vertex) {
	//在图的顶点表中插入一个新顶点vertex。若插入成功，函数返回true, 否则返回false。
	if (numVertices == maxVertices) {
		return false; 	//顶点表满, 不能插入
	}
	NodeTable[numVertices].data = vertex;			//插入在表的最后
	numVertices++;
	return true;
}; //算法时间复杂度

template <class T, class E>
bool Graphlnk<T, E>::insertEdge(int v1, int v2, E weight)
{
	if (v1 >= 0 && v1 < numVertices && v2 >= 0 && v2 < numVertices)
	{
		Edge<T, E> *q, *p = NodeTable[v1].adj;   	//v1对应的边链表头指针
		while (p != NULL && p->dest != v2)  p = p->link;   //寻找邻接顶点v2	
		if (p != NULL) 	return false;     	//找到此边, 不插入		
		p = new Edge<T, E>;		//否则, 创建新结点
		q = new Edge<T, E>;
		p->dest = v2;  p->cost = weight;
		p->link = NodeTable[v1].adj; 		//链入v1边链表
		NodeTable[v1].adj = p;
		q->dest = v1;  q->cost = weight;
		q->link = NodeTable[v2].adj; 		//链入v2边链表
		NodeTable[v2].adj = q;
		numEdges++;
		return true;
	}
	return false;
}; //算法时间复杂度

template <class T, class E>
bool Graphlnk<T, E>::removeEdge(int v1, int v2)
{
	if (v1 != -1 && v2 != -1)
	{
		Edge<T, E> *p = NodeTable[v1].adj, *q = NULL, *s = p;
		while (p != NULL && p->dest != v2) { q = p; p = p->link; }  //v1对应边链表中找被删边	
		if (p != NULL) {		//找到被删边结点		
			if (p == s)    NodeTable[v1].adj = p->link;//该结点是边链表首结点
			else     q->link = p->link;     delete p;
		}
		else    return false;   //没有找到被删边结点		
		p = NodeTable[v2].adj;   q = NULL, s = p；    //v2对应边链表中删除
			while (p->dest != v1) { q = p;    p = p->link; }     //寻找被删边结点
		if (p == s)    NodeTable[v2].adj = p->link;   //该结点是边链表首结点
		else     q->link = p->link;
		delete p;   numEdges--;
		return true;
	}
	return false;	   //没有找到结点
}; //算法时间复杂度



template <class T, class E>
bool Graphlnk<T, E>::removeVertex(int v) {      //在图中删除一个指定顶点v, v是顶点号。若删除成功, 函数返回true, 否则返回false。
	if (numVertices == 1 || v < 0 || v >= numVertices) {
		return false;
	}   		 //表空或顶点号超出范围
	Edge<T, E> *p, *s, *t;  int k;
	while (NodeTable[v].adj != NULL) { 	//删除第v个边链表中所有结点
		p = NodeTable[v].adj;  k = p->dest;    //取邻接顶点k
		s = NodeTable[k].adj;  t = NULL; 	//找对称存放的边结点
		while (s != NULL && s->dest != v) {
			t = s;  s = s->link;
		}
		if (s != NULL) {		//删除对称存放的边结点		
			if (t == NULL)    NodeTable[k].adj = s->link;
			else    t->link = s->link;
			delete s;
		}
		NodeTable[v].adj = p->link;	//清除顶点v的边链表结点
		delete p; numEdges--;	    //与顶点v相关联的边数减一
	}
	numVertices--;	 //图的顶点个数减1
	NodeTable[v].data = NodeTable[numVertices].data;
	//填补#用最后一个顶点来代替
	p = NodeTable[v].adj = NodeTable[numVertices].adj;
	while (p != NULL) {//用来顶替的顶点下标变了，对应边要改变
		s = NodeTable[p->dest].adj;
		while (s != NULL) {
			if (s->dest == numVertices)
			{
				s->dest = v;//修改对应边顶点的下标   
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
		Edge<T, E> *p = NodeTable[v1].adj;	//V1的第一条关联的边	
		while (p != NULL && p->dest != v2) 	p = p->link; //寻找邻接顶点v2
		if (p != NULL)
		{
			p->cost = cost;
		}
		Edge<T, E> *q = NodeTable[v2].adj;	//V1的第二条关联的边	
		while (q != NULL && q->dest != v1) 	q = q->link; //寻找邻接顶点v1
		if (q != NULL)
		{
			q->cost = cost;
			return true;
		}
	}
	return false;			//边(v1,v2)不存在
}

template<class T, class E>
bool Graphlnk<T, E>::insertEdge(int v1, int v2)
{
	if (v1 >= 0 && v1 < numVertices && v2 >= 0 && v2 < numVertices)
	{
		Edge<T, E> *q, *p = NodeTable[v1].adj;   	//v1对应的边链表头指针
		while (p != NULL && p->dest != v2)  p = p->link;   //寻找邻接顶点v2	
		if (p != NULL) 	return false;     	//找到此边, 不插入		
		p = new Edge<T, E>;		//否则, 创建新结点
		q = new Edge<T, E>;
		p->dest = v2;
		p->link = NodeTable[v1].adj; 		//链入v1边链表
		NodeTable[v1].adj = p;
		q->dest = v1;
		q->link = NodeTable[v2].adj; 		//链入v2边链表
		NodeTable[v2].adj = q;
		numEdges++;
		return true;
	}
	return false;
}
; //算法时间复杂度

template <class T, class E>
int Graphlnk<T, E>::getFirstNeighbor(int v) {
	//给出顶点位置为 v 的第一个邻接顶点的位置, 
	//如果找不到, 则函数返回-1
	if (v != -1) {			//顶点v存在
		Edge<T, E> *p = NodeTable[v].adj;	//对应边链表第一个边结点
		if (p != NULL) return p->dest;		//存在, 返回第一个邻接顶点
	}
	return -1;		//第一个邻接顶点不存在
}; //算法时间复杂度

template <class T, class E>
int Graphlnk<T, E>::getNextNeighbor(int v, int w) {
	//给出顶点v的邻接顶点w的下一个邻接顶点的位置,
	//若没有下一个邻接顶点, 则函数返回-1
	if (v != -1) {				//顶点v存在
		Edge<T, E> *p = NodeTable[v].adj;
		while (p != NULL && p->dest != w)
			p = p->link;
		if (p != NULL && p->link != NULL)
			return p->link->dest; 	//返回下一个邻接顶点
	}
	return -1; 			//下一邻接顶点不存在
}; //算法时间复杂度





template <class T, class E>
void Graphlnk<T, E>::DFS(const T& v0,const T& v1,int M) {//从顶点v出发对图G进行深度优先遍历的主过程
	int i, loc0, loc1;
	int n = numVertices;    	//顶点个数
	m = 0;
	bool *visited = new bool[n];          	//创建辅助数组

	for (i = 0; i < n; i++)
	{
		visited[i] = false;	//辅助数组visited初始化
	}

	loc0 = getVertexPos(v0);
	loc1 = getVertexPos(v1);
	DFS(loc0, loc1, visited,M); 			//从顶点0开始深度优先搜索
	delete[] visited;			//释放visited
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

//			visited[v] = false;	 	         	//取消相邻访问

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
	visited[v0] = true;	 	         	//作访问标记
	s.push(v0);
	int w = getFirstNeighbor(v0);     	//第一个邻接顶点
											//	if (w == -1) q.pop();

	while (w != -1) {			//若邻接顶点w存在
		if (!visited[w])
		{
			DFS(w, v1, visited,M);  //若w未访问过, 递归访问顶点w
			visited[w] = false;
		}
		w = getNextNeighbor(v0, w); 	//下一个邻接顶点
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
	visited[v1] = true; 	                     	//做已访问标记   
	w = getFirstNeighbor(v1);  	//第一个邻接顶点
	while (w != -1) {			//若邻接顶点w存在
		if (!visited[w]) {			//若未访问过
			visited[w] = true;
			if (w == v2)
			{
				return true;
			}
		}
		w = getNextNeighbor(v1, w); 	//找顶点loc的下一个邻接顶点
	}
	return false;
}

template<class T, class E>
void Graphlnk<T, E>::CreateChineseTrafficgraph()
{
	string str;//
	string strcity1;//左列城市
	int city1;//城市一的标号
//	string strcity2;//上行城市
	int tag;//1 0是否有边
	int cost;
	ifstream inputadjacency("adjacency.txt");
	ifstream inputdistance("distance.txt");
	if (!inputadjacency)
	{
		cout << "“adjacency”的 txt 文件不在 d 盘的“txt”的文件夹中！" << endl;
		return;
	}
	if (!inputdistance)
	{
		cout << "“distance”的 txt 文件不在 d 盘的“txt”的文件夹中！" << endl;
		return;
	}
	getline(inputdistance, str);
	getline(inputadjacency, str);
	istringstream ss(str);//将一行字符串转换成流
	while (ss >> str) {
		insertVertex(str);
	}

	for (int i = 0; i < numVertices; i++) {//有无边
		inputadjacency >> strcity1;
		city1 = getVertexPos(strcity1);
		for (int j = 0; j < numVertices; j++) {
			inputadjacency >> tag;
			if (tag == 1&& city1!=j) {
				insertEdge(city1, j);
			}
		}
	}

	for (int i = 0; i < numVertices; i++) {//边值
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
		cout << "从";
		printf("%s", getValue(v0).c_str());
		cout << "到";
		printf("%s", getValue(i).c_str());
		cout << "的中间不超过两个省的路径有：" << endl;
		DFS(getValue(v0), getValue(i),4);
	}
	cout << endl;
}
template<class T, class E>
void Graphlnk<T, E>::DjShortestPath(Graphlnk<T, E>& G, T vT, T v1T, E dist[], int path[])
{
	//Graph是一个带权有向图。dist[j], 0≤j<n, 是当前求到的从顶点v到顶点j的
	//最短路径长度, path[j], 0≤j<n, 存放求到的最短路径。
	int v = getVertexPos(vT);
	int v1 = getVertexPos(v1T);
	int n = G.numVertices;
	bool *S = new bool[n]; 	   		//最短路径顶点集
	int i, j, k;  E w, min;
	for (i = 0; i < n; i++) {
		dist[i] = G.getWeight(v, i);
		S[i] = false;
		if (i != v && dist[i] < MAX) path[i] = v;
		else path[i] = -1;
	}
	S[v] = true;  dist[v] = 0;     		//顶点v加入顶点集合
	for (i = 0; i < n - 1; i++) {       		//求解各顶点最短路径
		min = MAX;  int u = v;
		for (j = 0; j < n; j++)
			if (!S[j] && dist[j] < min) 		//选不在S中具有最短路径的顶点u
			{
				u = j; min = dist[j];
			}
		S[u] = true;	               		//将顶点u加入集合S
//		if (u == v1)  return;               //找到直接退出
		for (k = 0; k < n; k++) {       		//修改
			w = G.getWeight(u, k);
			if (!S[k] && w < MAX && dist[u] + w < dist[k]) {    //顶点k未加入S                
				dist[k] = dist[u] + w;
				path[k] = u;     			//修改到k的最短路径
			}
		}
	}
}
template<class T, class E>
void Graphlnk<T, E>::AdaptedDjShortestPath(Graphlnk<T, E>& G, T vT, T v1T,T SkipovervT, E dist[], int path[])
{
	//Graph是一个带权有向图。dist[j], 0≤j<n, 是当前求到的从顶点v到顶点j的
	//最短路径长度, path[j], 0≤j<n, 存放求到的最短路径。
	int v = getVertexPos(vT);
	int v1 = getVertexPos(v1T);
	int Skipoverv = getVertexPos(SkipovervT);
	int n = G.numVertices;
	bool *S = new bool[n]; 	   		//最短路径顶点集
	int i, j, k;  E w, min;
	for (i = 0; i < n; i++) {
		if (i == Skipoverv) continue;
		dist[i] = G.getWeight(v, i);
		S[i] = false;
		if (i != v && dist[i] < MAX) path[i] = v;
		else path[i] = -1;
	}
	S[v] = true;  dist[v] = 0;     		//顶点v加入顶点集合
	for (i = 0; i < n - 1-1; i++) {       		//求解各顶点最短路径
		min = MAX;  int u = v;
		for (j = 0; j < n; j++)
			if (!S[j] && dist[j] < min&&j!=Skipoverv) 		//选不在S中具有最短路径的顶点u
			{
				u = j; min = dist[j];
			}
		S[u] = true;	               		//将顶点u加入集合S
		if (u == v1)  return;               //找到直接退出
		for (k = 0; k < n; k++) {       		//修改
			if (k == Skipoverv) continue;
			w = G.getWeight(u, k);
			if (!S[k] && w < MAX && dist[u] + w < dist[k]) {    //顶点k未加入S                
				dist[k] = dist[u] + w;
				path[k] = u;     			//修改到k的最短路径
			}
		}
	}
}
template<class T, class E>
void Graphlnk<T, E>::Floyd(Graphlnk<T, E>& G, E **a, int **path)
{
	//a[i][j]是顶点i和j之间的最短路径长度。path[i][j]是相应路径上顶点j的前一顶点的顶点号
	int i, j, k, n = G.numVertices;
	for (i = 0; i < n; i++)    		//矩阵a与path初始化
		for (j = 0; j < n; j++) {
			a[i][j] = G.getWeight(i, j);
			if (i != j && a[i][j] < MAX) path[i][j] = i;
			else path[i][j] = -1;
		}
	for (k = 0; k < n; k++)	//针对每一个k, 产生a(k)及path(k)        
		for (i = 0; i < n; i++)
			for (j = 0; j < n; j++)
				if (a[i][k] + a[k][j] < a[i][j]) {
					a[i][j] = a[i][k] + a[k][j];
					path[i][j] = path[k][j];
				} 	//缩短路径长度, 绕过 k 到 j
}
template<class T, class E>
void Graphlnk<T, E>::AdaptedDFS(const T & v0, const T & v1, int M)
{
	int i, loc0, loc1;
	int n = numVertices;    	//顶点个数
	m = 0;
	bool *visited = new bool[n];          	//创建辅助数组

	for (i = 0; i < n; i++)
	{
		visited[i] = false;	//辅助数组visited初始化
	}

	loc0 = getVertexPos(v0);
	loc1 = getVertexPos(v1);
	AdaptedDFS(loc0, loc1, visited, M); 			//从顶点0开始深度优先搜索
	delete[] visited;			//释放visited
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
	visited[v0] = true;	 	         	//作访问标记
	s.push(v0);
	int w = getFirstNeighbor(v0);     	//第一个邻接顶点
										//	if (w == -1) q.pop();

	while (w != -1) {			//若邻接顶点w存在
		if (!visited[w])
		{
			AdaptedDFS(w, v1, visited, M);  //若w未访问过, 递归访问顶点w
			visited[w] = false;
		}
		w = getNextNeighbor(v0, w); 	//下一个邻接顶点
	}
	s.pop();
	m--;
}
;

