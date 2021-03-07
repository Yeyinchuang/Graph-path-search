#include<sstream>
#include<iostream>
#pragma once
const int MAXVertices = 999;
const int DefaultVertices = MAXVertices; //��󶥵���(=n) 


template <class T, class E>
struct Edge {			   	//�߽��Ķ���
	int dest;				//�ߵ���һ����λ��
	E cost;				//���ϵ�Ȩֵ
	Edge<T, E> *link;		   	//��һ������ָ��
	int weight;			//���ϵ�Ȩֵ
	Edge() {}				//���캯��
	Edge(int num, E cost) 	   	//���캯��
		: dest(num), weight(cost), link(NULL) { }
	bool operator != (Edge<T, E>& R) const
	{
		return dest != R.dest;
	}	   //�бߵȷ�
};


template <class T, class E>
struct Vertex {			   //����Ķ���
	T data;				   //���������
	Edge<T, E> *adj;		   //�������ͷָ��
};


template <class T, class E>
class Graphlnk{   //ͼ���ඨ��
//	friend class PathDist<T,E>;
protected:
	int maxVertices;		    	//ͼ����󶥵���
	int numEdges;			//��ǰ����
	int numVertices;

private:
	Vertex<T, E> *NodeTable;        	//����� (���������ͷ���)
public:
	Graphlnk(int sz = DefaultVertices);  	//���캯��
	~Graphlnk();				//��������
	T getValue(int i);
	int getVertexPos(const T vertx);      //��������vertex��ͼ�е�λ��
	E getWeight(int v1, int v2);	     	//ȡ��(v1,v2)Ȩֵ
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
	void DFS(const T& v0,const T& v1,int M);//M�������ƽڵ���
	void DFS(int v0, int v1, bool visited[],int M);//M�������ƽڵ���
	bool HaveEdge(int v1, int v2);
	void CreateChineseTrafficgraph();
	void interval(int v0);//�Ҽ��2��·��
	void DjShortestPath(Graphlnk<T, E>& G, T vT, T v1T, E dist[], int path[]);//���·��
	void AdaptedDjShortestPath(Graphlnk<T, E>& G, T vT, T v1T,T SkipovervT,E dist[], int path[]);//ȥ�ڵ����·��
	void Floyd(Graphlnk<T, E>& G, E **a, int **path);
	void AdaptedDFS(const T& v0, const T& v1, int M);//M�������ƽڵ���
	void AdaptedDFS(int v0, int v1, bool visited[], int M);//M�������ƽڵ���
};

template<class T, class E>
class PathDist {//��¼·���;����Ž����鷽������
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
		_os << "·������Ϊ" << pathdist.dist;
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