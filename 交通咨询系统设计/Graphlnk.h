#include<sstream>
#include<iostream>
#pragma once
const int MAXVertices = 999;
const int DefaultVertices = MAXVertices; //最大顶点数(=n) 


template <class T, class E>
struct Edge {			   	//边结点的定义
	int dest;				//边的另一顶点位置
	E cost;				//边上的权值
	Edge<T, E> *link;		   	//下一条边链指针
	int weight;			//边上的权值
	Edge() {}				//构造函数
	Edge(int num, E cost) 	   	//构造函数
		: dest(num), weight(cost), link(NULL) { }
	bool operator != (Edge<T, E>& R) const
	{
		return dest != R.dest;
	}	   //判边等否
};


template <class T, class E>
struct Vertex {			   //顶点的定义
	T data;				   //顶点的名字
	Edge<T, E> *adj;		   //边链表的头指针
};


template <class T, class E>
class Graphlnk{   //图的类定义
//	friend class PathDist<T,E>;
protected:
	int maxVertices;		    	//图中最大顶点数
	int numEdges;			//当前边数
	int numVertices;

private:
	Vertex<T, E> *NodeTable;        	//顶点表 (各边链表的头结点)
public:
	Graphlnk(int sz = DefaultVertices);  	//构造函数
	~Graphlnk();				//析构函数
	T getValue(int i);
	int getVertexPos(const T vertx);      //给出顶点vertex在图中的位置
	E getWeight(int v1, int v2);	     	//取边(v1,v2)权值
	int NumberOfVertices();
	int NumberOfEdges();
	bool insertVertex(const T& vertex);
	bool removeVertex(int v);
	bool setEdgeweight(int v1, int v2, E cost);
	bool insertEdge(int v1, int v2);
	bool insertEdge(int v1, int v2, E cost);
	bool removeEdge(int v1, int v2);
	int getFirstNeighbor(int v);
	int getNextNeighbor(int v, int w);
	void DFS(const T& v0,const T& v1,int M);//M用来限制节点数
	void DFS(int v0, int v1, bool visited[],int M);//M用来限制节点数
	bool HaveEdge(int v1, int v2);
	void CreateChineseTrafficgraph();
	void interval(int v0);//找间隔2内路径
	void DjShortestPath(Graphlnk<T, E>& G, T vT, T v1T, E dist[], int path[]);//最短路径
	void AdaptedDjShortestPath(Graphlnk<T, E>& G, T vT, T v1T,T SkipovervT,E dist[], int path[]);//去节点最短路径
	void Floyd(Graphlnk<T, E>& G, E **a, int **path);
	void AdaptedDFS(const T& v0, const T& v1, int M);//M用来限制节点数
	void AdaptedDFS(int v0, int v1, bool visited[], int M);//M用来限制节点数
};

template<class T, class E>
class PathDist {//记录路径和距离存放进数组方便排序
private:
	T path[MAXVertices];
	int numberpath;
	E dist;
public:
	PathDist() {
		for (int i = 0; i < MAXVertices; i++)
			path[i] = -1;
		numberpath = 0;
		dist = 0;
	}
	void push(Graphlnk<T, E> *G, T v) {
		if (numberpath > 0) dist += G->getWeight(G->getVertexPos(path[numberpath-1]), G->getVertexPos(v));
		path[numberpath] = v;
		numberpath++;
	}
	friend std::ofstream& operator<<(std::ofstream& _os, const PathDist<T, E>&pathdist) {
		for (int i = 0; i < pathdist.numberpath - 1; i++)
			_os << pathdist.path[i].c_str() << "-->";
		_os << pathdist.path[pathdist.numberpath - 1].c_str() << endl;
		_os << "路径长度为" << pathdist.dist;
		return _os;
	}
	bool operator >(PathDist<T, E> &ptahdist) {
		return this->dist > ptahdist.dist;
	}
	bool operator <(PathDist<T, E> &ptahdist) {
		return this->dist < ptahdist.dist;
	}
	bool operator ==(PathDist<T, E> &ptahdist) {
		return this->dist == ptahdist.dist;
	}
};