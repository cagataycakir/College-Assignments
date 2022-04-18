/**
The code has been developed by Cagatay Cakir for the Gebze Technical University Computer Science department, course 222 CSE.
This used to be a private build and only intended to be viewed by the course lecturer and assistants. But now uploaded to github as a business reference.
*/

import java.util.*;

class DjikstraImplementation{
	public static void main(String[] args){
		Graph<String> test = new Graph<String>();
		test.addEdge("A", "B", 3);
		test.addEdge("A", "F", 3);
		test.addEdge("B", "C", 3);
		test.addEdge("C", "D", 3);
		test.addEdge("F", "D", 3);
		Dijkstra<String> djTest = new Dijkstra<String>(test);
		for(String item : djTest.findShortest("A", "D"))
			System.out.print(item+"->");
		System.out.println("done!");
		
	}
}

class edgeNode<T>{
	T n1;
	T n2;
	Integer distance = -1;
	edgeNode(){}
	edgeNode(T n1,T n2){this.n1 = n1; this.n2 = n2;}
	edgeNode(T n1,T n2,Integer dis){this.n1 = n1; this.n2 = n2; distance=dis;}
	boolean isEqual(T n1, T n2){
		if(	(this.n1==n1 && this.n2==n2) ||
			(this.n1==n2 && this.n2==n1)
		) return true;
		return false;
	}
}

//bidirectional graph class
class Graph<T> {
	LinkedList<T>vertices = new LinkedList<T>();
	LinkedList<edgeNode<T>>edges = new LinkedList<edgeNode<T>>();

	Graph(){}

	void addVertex(T s){
		vertices.add(s);
	}

	LinkedList<edgeNode<T>> getVertexEdges(T vert){
		if(!hasVertex(vert)) return null;
		LinkedList<edgeNode<T>>ret = new LinkedList<edgeNode<T>>();
		for(edgeNode<T>e : edges)
			if(e.n1 == vert || e.n2 == vert)
				ret.add(e);
		if(ret.size()==0)return null;
		return ret;
	}

	void addEdge(T source, T destination, Integer dis){
		if (!hasVertex(source)) addVertex(source);
		if (!hasVertex(destination)) addVertex(destination);
		if (hasEdge(source,destination) == null)
			edges.add(new edgeNode<T>(source,destination,dis));
	}

	Integer getVertexCount(){
		return vertices.size();
	}

	Integer getEdgesCount(){
		return edges.size();
	}

	boolean hasVertex(T s){
		if (linkedSearch(vertices,s)) return true;
		else return false;
	}

	edgeNode<T> hasEdge(T s, T d){
		for(edgeNode<T> comp : edges)
			if (comp.isEqual(s,d)) return comp;
		return null;
	}

	Graph<T>copy(){
		Graph<T>ret=new Graph<T>();
		ret.vertices = (LinkedList) vertices.clone();
		ret.edges = (LinkedList) edges.clone();
		return ret;
	}

	void removeVertex(T r){
		vertices.remove(r);
		for(int i=0; i<edges.size(); i++){
			if(edges.get(i).n1 == r || edges.get(i).n2 == r){
				edges.remove(i);
				i--;
			}
		}
	}

	void printEdges(){
		for(T item:vertices)
			System.out.print(item + " ");
		System.out.println();
		for(edgeNode<T> item:edges)
			System.out.println(item.n1 + "<->" + item.n2);
		System.out.println("---");
	}

	private boolean linkedSearch(LinkedList<T>mlist, T search){
		for(T item : mlist)
			if(item == search)
				return true;
		return false;
	}
}

class MatrixGraph<T> extends Graph<T>{
	Integer matrixGraph[][];
	
	MatrixGraph(){}
	MatrixGraph(Graph<T>in){
		//apparently these two lines cause unsafety message. not sure why
		vertices = (LinkedList)in.vertices.clone();
		edges = (LinkedList)in.edges.clone();
		updateMatrixGraph();
	}

	void updateMatrixGraph(){
		matrixGraph = new Integer[vertices.size()][vertices.size()];
		edgeNode<T> ptr=null;
		for(int j=0; j<vertices.size(); j++){
			for(int i=j; i<vertices.size(); i++){
				if(
					(ptr = hasEdge( vertices.get(i), vertices.get(j)))!=null
				) matrixGraph[i][j] = matrixGraph[j][i] = ptr.distance;
				else matrixGraph[i][j] = matrixGraph[j][i] = -1;
			}
		}
	}
}

class Dijkstra<T>{
	Graph<T> graph;
	
	Dijkstra(){}
	Dijkstra(Graph<T> g){graph = g.copy();}
	
	LinkedList<T> findShortest(T start, T finish){
		if(!graph.hasVertex(start) || !graph.hasVertex(finish))
			return null;
		if(graph instanceof MatrixGraph)
			return null;
		else
			return findShortestL(start,finish);
	}
	
	private LinkedList<T> findShortestL(T start, T finish){
		//int numV = graph.getVertexCount();
		//to keep traversal list
		priorityLink<T>vs = new priorityLink<T>();
		//to keep optimised traversal table
		LinkedList<verDis<T>> opt = new LinkedList<verDis<T>>();

		// Initialize V–S.
		for (T i : graph.vertices)
			if (i != start)
				vs.add(new verDis<T>(i));
		vs.addFirst(new verDis<T>(start,0));

		// Main loop
		while (vs.size() != 0){
			verDis<T> curNode = vs.poll();
			//System.out.println("Current node ->" + curNode.vert);
			if(curNode == finish) {opt.add(curNode); break;}
			LinkedList<edgeNode<T>> loop = graph.getVertexEdges(curNode.vert);
			if(loop != null)
				for(edgeNode<T>e : loop)
					if(curNode.dis + e.distance < vs.get(e.n2==curNode.vert ? e.n1 : e.n2).dis)
						vs.reset(new verDis<T>(curNode,(e.n2==curNode.vert ? e.n1 : e.n2),e.distance+curNode.dis));
						
			opt.add(curNode);
			graph.removeVertex(curNode.vert);
			//graph.printEdges();
			//vs.print();
		}

		//output
		verDis<T> tail = opt.getLast();
		if(tail.vert != finish){return null;}
		LinkedList<T> out = new LinkedList<T>();
		while(tail.pre != null){
			out.addFirst(tail.vert);
			tail = tail.pre;
		}
		out.addFirst(tail.vert);

		return out;
	}
	
	private class verDis<T>{
		T vert;
		verDis<T> pre=null;
		double dis = Double.POSITIVE_INFINITY;
		verDis(){}
		verDis(T v){vert = v;}
		verDis(T v, double d){vert = v; dis=d;}
		verDis(verDis<T> p, T v, double d){vert = v; dis=d; pre=p;}
	}
	private class priorityLink<T> extends LinkedList<verDis<T>>{

		void addOrder(verDis<T> item){
			if (size()==0) {add(item); return;}
			int ind = 0;
			while(get(ind).dis<item.dis) ind++;
			add(ind,item);
		}
		verDis<T> get(T item){
			for(int i=0; i<size(); i++)
				if(get(i).vert == item)
					return get(i);
			return null;
		}
		void reset(verDis<T>itemnew){
			for(int i=0; i<size(); i++)
				if(get(i).vert == itemnew.vert)
					remove(i);
			addOrder(itemnew);
		}
		void print(){
			System.out.println("----");
			for(int i=0; i<size(); i++)
				System.out.println(get(i).vert + " " +get(i).dis);
			System.out.println("xxxx");
		}
	}
}