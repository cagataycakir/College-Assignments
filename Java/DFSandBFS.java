/**
The code has been developed by Cagatay Cakir for the Gebze Technical University Computer Science department, course 222 CSE.
This used to be a private build and only intended to be viewed by the course lecturer and assistants. But now uploaded to github as a business reference.
*/

import java.util.*;

class DFSandBFS{
	public static void main(String[] args){
		DFSGraph<String> test = new DFSGraph<String>("root");
		test.fillRandStrings();
		System.out.println(test.root.edges.size());
		//test.printAllVert();
	}
}

class vertNode<T>{
	T vert;
	HashSet<vertNode<T>>edges;
	vertNode(){}
	vertNode(T in){vert = in; edges = new HashSet<vertNode<T>>();}
	vertNode(T in,HashSet<vertNode<T>>e){vert=in; edges = e;}
	void addEdge(T val){edges.add(new vertNode(val));}
	void addEdge(vertNode<T>ptr){edges.add(ptr);}
	boolean hasEdge(T search){
		if(vert == search) return true;
		for(vertNode<T>item :edges)
			if(item.vert == search) return true;
		return false;
	}
}

//bidirectional graph class
class Graph<T> {
	vertNode<T>root;
	Graph(){}
	Graph(T in){root = new vertNode(in);}
	vertNode<T> getVert(T search){return null;}
	void addEdge(T a, T b){
		vertNode<T>aP=null,bP=null;
		if(	(aP=getVert(a))==null &&
			(bP=getVert(a))==null) return; //cannot add unlinked vert
		if(aP==null && bP!=null)		bP.addEdge(a);
		else if(aP!=null && bP==null)	aP.addEdge(b);
		else{
			bP.addEdge(a);
			aP.addEdge(b);
		}
	}
	void printAllVert(){
		vertNode<T> cur = root;
		while(true){
			System.out.println(cur.vert);
			if(cur.edges.size()==0)return;
			for(vertNode<T>a:cur.edges){
				cur=a;
				break;
			}
		}
	}
	void fillRandStrings(){
		if(root==null)return;	//root must be initialized before run!
		for(int i=0; i<1000; i++)
			putRandString();
	}
	void putRandString(){
		vertNode<T>current = root;
		while(current.edges.size()>0){
			Random rand = new Random();
			if(rand.nextInt(2)==1)break;
			int breakAt = rand.nextInt(current.edges.size());
			for(vertNode<T>a:current.edges){
				if(breakAt==0){
					current = a;
					break;
				}
				--breakAt;
			}
		}
		current.edges.add(new vertNode(randString(5)));
	}
	String randString(int n){
    	String AlphaNumericString = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                    + "0123456789"
                                    + "abcdefghijklmnopqrstuvxyz";
        StringBuilder sb = new StringBuilder(n);
  
        for (int i = 0; i < n; i++) {
			int index = (int)(AlphaNumericString.length() * Math.random());
			sb.append(AlphaNumericString.charAt(index));
        }
        return sb.toString();
    }
}

class BFSGraph<T> extends Graph<T>{
	BFSGraph(){}
	BFSGraph(T in){root = new vertNode(in);}

	@Override
	vertNode<T> getVert(T search){
		if(root.vert==search) return root;
		for(vertNode<T> subs: root.edges)
			if(subs.vert == search) return subs;
		for(vertNode<T> subs: root.edges){
			HashSet<T>rec = new HashSet<T>();
			rec.add(root.vert);
			vertNode<T> temp = getVertAntiRec(search,subs,rec);
			if(temp != null) return temp;
		}
		
		return null;
	}
	private vertNode<T> getVertAntiRec(T search, vertNode<T>cur, HashSet<T>recur){
		for(vertNode<T> subs: cur.edges)
			if(subs.vert == search) return subs;
		for(vertNode<T> subs: cur.edges){
			if(recur.contains(subs))continue;
			recur.add(cur.vert);
			vertNode<T> temp = getVertAntiRec(search,subs,recur);
			if(temp != null) return temp;
		}
		return null;
	}
}

class DFSGraph<T> extends Graph<T>{
	DFSGraph(T in){root = new vertNode(in);}
	@Override
	vertNode<T> getVert(T search){
		if(root.vert==search) return root;
		HashSet<T>recur = new HashSet<T>();
		recur.add(root.vert);
		for(vertNode<T> subs: root.edges){
			if(subs.vert == search) return subs;
			recur.add(subs.vert);
			vertNode<T> temp = getVertAntiRec(search,subs,recur);
			if(temp!=null) return temp;
		}
		return null;
	}
	private vertNode<T> getVertAntiRec(T search, vertNode<T>cur, HashSet<T>recur){
		for(vertNode<T> subs: cur.edges.entrySet()){
			if(recur.contains(subs))continue;
			recur.add(cur.vert);
			vertNode<T> temp = getVertAntiRec(search,subs,recur);
			if(temp != null) return temp;
		}
		return null;
	}
}