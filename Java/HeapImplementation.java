/**
The code has been developed by Cagatay Cakir for the Gebze Technical University Computer Science department, course 222 CSE.
This used to be a private build and only intended to be viewed by the course lecturer and assistants. But now uploaded to github as a business reference.
*/

import java.util.ArrayList;
/**
This is the main class.
*/
public class HeapImplementation{
	static MaxHeapNode myHeap = new MaxHeapNode();
	public static void main(String[] args){
		test();
	}
	static void test(){
		myHeap.insert(20,1);
		myHeap.insert(12,1);
		myHeap.insert(30,1);
		myHeap.insert(10,1);
		myHeap.insert(31,1);
		myHeap.insert(21,2);
		myHeap.insert(19,1);
		myHeap.printTree();
	}
}

class IntegerHeapNode{
	int data=0;
	int occur=0;
	IntegerHeapNode left=null, right=null, parent=null;

	IntegerHeapNode(){}
	IntegerHeapNode(IntegerHeapNode other){
		data = other.data;
		if(other.left!=null) left = new IntegerHeapNode(other.left);
		if(other.right!=null) right = new IntegerHeapNode(other.right);
	}
	IntegerHeapNode(int ndata, IntegerHeapNode nleft, IntegerHeapNode nright){
		data = ndata;
		if(nleft!=null) left = nleft;
		if(nright!=null) right = nright;
	}
	IntegerHeapNode(int newdata){
		data = newdata;
		left=null; right=null; parent=null;
	}
	IntegerHeapNode(int newdata, IntegerHeapNode par){
		data = newdata;
		left=null; right=null; parent=null;
		parent = par;
	}
	IntegerHeapNode(int newdata, int nleft){
		data = newdata;
		left=new IntegerHeapNode(nleft,this); right=null;
	}
	IntegerHeapNode(int newdata, int nleft, IntegerHeapNode par){
		data = newdata;
		left=new IntegerHeapNode(nleft,this); right=null;
		parent = par;
	}
	IntegerHeapNode(int newdata, Integer nleft, int nright){
		data = newdata;
		if(nleft!=null) left=new IntegerHeapNode(nleft,this);
		right=new IntegerHeapNode(nright,this);
	}
	IntegerHeapNode(int newdata, Integer nleft, int nright, IntegerHeapNode par){
		data = newdata;
		if(nleft!=null) left=new IntegerHeapNode(nleft,this);
		right=new IntegerHeapNode(nright,this);
		parent = par;
	}

	int dataL(){return left.data;}
	int dataR(){return right.data;}
	//returns left node pointer
	IntegerHeapNode NodeL(){return left;}
	//returns right node pointer
	IntegerHeapNode NodeR(){return right;}
	//returns parent node pointer
	IntegerHeapNode NodeP(){return parent;}
	//returns copy of left node structure
	IntegerHeapNode newNodeL(){return new IntegerHeapNode(left);}
	//returns copy of right node structure
	IntegerHeapNode newNodeR(){return new IntegerHeapNode(right);}
	//returns tree as arraylist
	ArrayList<Integer> returnArrList(){
		ArrayList<Integer>ret = new ArrayList<Integer>();
		ret.add(data);
		if(left!=null) ret.addAll(left.returnArrList());
		if(right!=null) ret.addAll(right.returnArrList());
		return ret;
	}
	void insertElement(int item){
		if(item>=data){
			if(right==null) right = new IntegerHeapNode(item,this);
			else right.insertElement(item);
		}
		else{
			if(left==null) left = new IntegerHeapNode(item,this);
			else left.insertElement(item);
		}
	}
	void deleteLeft(){ left=null;}
	void deleteRight(){ right=null;}
	void decrLeft(){
		if(left==null) return;
		if(left.occur <= 1) left = null;
		else --left.occur;
	}
	void decrRight(){
		if(left==null) return;
		if(left.occur <= 1) left = null;
		else --left.occur;
	}
	IntegerHeapNode searchVal(int src){
		if(data == src) return this;
		IntegerHeapNode temp=null;
		if(left!=null) temp = left.searchVal(src);
		if(temp != null) return temp;
		if(right!=null) temp = root.right.searchVal(src);
		if(temp != null) return temp;
		return null;
	}
}

class MaxHeapNode{
	IntegerHeapNode root=null; int size;
	MaxHeapNode left=null, right=null, parent=null;

	MaxHeapNode(){size =0;}
	MaxHeapNode(int rootVal, int rootOc){
		root = new IntegerHeapNode(rootVal);
		root.occur=rootVal;
		size = 1;
	}
	boolean insert(int val){
		IntegerHeapNode temp;
		if((temp = root.searchVal(val)) == null) return insertNew(val,1);
		++temp.occur;
		return true;
	}
	boolean insertNew(int val, int occur){
		if(size>=7) return false;
		IntegerHeapNode ptr=null;

		if(root == null)					ptr = (root = new IntegerHeapNode(val));
		else if(root.left == null)			ptr = (root.left = new IntegerHeapNode(val,root));
		else if(root.right == null)			ptr = (root.right = new IntegerHeapNode(val,root));
		else if(root.left.left == null)		ptr = (root.left.left = new IntegerHeapNode(val,root.left));
		else if(root.left.right == null)	ptr = (root.left.right = new IntegerHeapNode(val,root.left));
		else if(root.right.left == null)	ptr = (root.right.left = new IntegerHeapNode(val,root.right));
		else if(root.right.right == null)	ptr = (root.right.right = new IntegerHeapNode(val,root.right));

		ptr.occur=occur;
		++size;
		
		heapify(ptr);
		return true;
	}
	private void heapify(IntegerHeapNode ptr){
		if(ptr.parent == null) return;
		
		if(ptr.parent.data > ptr.data) return;
		{
			int temp = ptr.parent.data;
			ptr.parent.data = ptr.data;
			ptr.data = temp;
			temp = ptr.parent.occur;
			ptr.parent.occur=ptr.occur;
			ptr.occur=temp;
		}
		
		heapify(ptr.parent);
	}
	void printTree(){
		if(root==null) return;
		System.out.println(root.data + "," +root.occur);
		printBranch(0,root.right);
		printBranch(0,root.left);
	}
	private void printBranch(int depth, IntegerHeapNode branch){
		if(branch == null) return;
		for(int i=0; i<depth; i++) System.out.print("    ");
		System.out.print("`---");
		System.out.println(branch.data+ "," +branch.occur);
		printBranch(depth+1,branch.right);
		printBranch(depth+1,branch.left);
	}
	void deleteLeft(){ left=null;}
	void deleteRight(){ right=null;}
	void newLeft(){left=new MaxHeapNode(); left.parent = this;}
	void newRight(){right=new MaxHeapNode(); right.parent = this;}
}

class BTSHeapTree{
	MaxHeapNode sRoot=null;
	
	BTSHeapTree(){}
	void insert(int val, int occur)
}