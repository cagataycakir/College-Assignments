/**
The code has been developed by Cagatay Cakir for the Gebze Technical University Computer Science department, course 222 CSE.
This used to be a private build and only intended to be viewed by the course lecturer and assistants. But now uploaded to github as a business reference.
*/

import java.util.ArrayList;
/**
This is the main class.
*/
public class BinaryImplementation{
	static BinarySearchTree testTree1 = constructTree1();
	static BinarySearchTree testTree2 = constructTree2();
	public static void main(String[] args){
		//test1();
		//test2();
		test3();
	}
	static BinarySearchTree constructTree1(){
		BinarySearchTree myTree = new BinarySearchTree();
		myTree.insertElement(5);//               ┏━━━━━━5━━━━━━┓
		myTree.insertElement(40);//              2━┓       ┏━━40
		myTree.insertElement(2);//                 3━━┓ ┏━━7━━┓
		myTree.insertElement(3);//                    4 6    10
		myTree.insertElement(4);//
		myTree.insertElement(7);
		myTree.insertElement(6);
		myTree.insertElement(10);

		return myTree;
	}
	static BinarySearchTree constructTree2(){
		BinarySearchTree myTree = new BinarySearchTree();
		myTree.insertElement(20);//               ┏━━━━━━20━━━━━━┓
		myTree.insertElement(35);//            ┏━━7━━┓          35━━┓
		myTree.insertElement(7);//            6   ┏━━10━━┓          40
		myTree.insertElement(10);//              14      17
		myTree.insertElement(40);//
		myTree.insertElement(17);
		myTree.insertElement(6);
		myTree.insertElement(14);

		return myTree;
	}

	static void test1(){
		System.out.println("Searching for element 40 in tree1...");
		IntegerHeapNode s = testTree1.searchElement(40);
		if(s.parent!=null) System.out.println("My parent: " + String.valueOf(s.parent.data));
		if(s.left!=null) System.out.println("Left node: " + String.valueOf(s.left.data));
		if(s.right!=null) System.out.println("Right node: " + String.valueOf(s.right.data));
	}
	static void test2(){
		BinarySearchTree merged = new BinarySearchTree(testTree1,testTree2);
		merged.printTree();
	}
	static void test3(){
		testTree1.removeNth(3);
		testTree1.printTree();
		//becomes
		//┏━━━━━━5━━━━━━┓
		//2━┓       ┏━━40
		//  3━━┓ ┏━━10
		//     4 6
	}
}

class IntegerHeapNode{
	int data=0;
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

	int DataL(){return left.data;}
	int DataR(){return right.data;}
	//returns left node pointer
	IntegerHeapNode NodeL(){return left;}
	//returns right node pointer
	IntegerHeapNode NodeR(){return right;}
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
}

class BinarySearchTree{
	IntegerHeapNode root;

	BinarySearchTree(){}
	BinarySearchTree(BinarySearchTree a, BinarySearchTree b){
		ArrayList<Integer>allList = a.root.returnArrList();
		allList.addAll(b.root.returnArrList());
		for(int i=0; i<allList.size(); i++)
			insertElement(allList.get(i));
	}
	void addFromRoot(int item){
		if(root == null) root = new IntegerHeapNode(item);
		else if(item >= root.data) {
			if(root.right == null) 
				root.right = new IntegerHeapNode(item);
			else{
				IntegerHeapNode temp = root.right;
				root.right = new IntegerHeapNode(item,null,temp);
			}
		}
		else{
			if(root.left == null)
				root.left = new IntegerHeapNode(item);
			else{
				IntegerHeapNode temp = root.left;
				root.right = new IntegerHeapNode(item,temp,null);
			}
		}
	}
	void insertElement(int item){
		if(root == null) root = new IntegerHeapNode(item);
		else root.insertElement(item);
	}
	void printTree(){
		if(root==null) return;
		System.out.println(root.data);
		printBranch(0,root.right);
		printBranch(0,root.left);
	}
	private void printBranch(int depth, IntegerHeapNode branch){
		if(branch == null) return;
		for(int i=0; i<depth; i++) System.out.print("    ");
		System.out.print("`---");
		System.out.println(branch.data);
		printBranch(depth+1,branch.right);
		printBranch(depth+1,branch.left);
	}
	IntegerHeapNode searchElement(int item){
		IntegerHeapNode pointer = root;
		for(;;){
			if (pointer == null) return null;
			else if(item == pointer.data) return pointer;
			else if(item >= pointer.data) pointer = pointer.right;
			else if(item < pointer.data) pointer = pointer.left;
			else System.exit(0);
		}
	}
	class Counter{int c=0;}
	IntegerHeapNode nthElement(int c){
		Counter steps = new Counter();
		return nthEl(c,root,steps);
	}
	IntegerHeapNode nthEl(int c, IntegerHeapNode curRoot, Counter steps){
		if (curRoot == null || steps.c >= c) return null;
		
		{
		IntegerHeapNode temp = nthEl(c, curRoot.right, steps);
		if (temp!=null) return temp;
		}
		++steps.c;
		
		if(steps.c==c) return curRoot;
		
		return nthEl(c, curRoot.left, steps);
	}
	IntegerHeapNode getDeepSuccessor(IntegerHeapNode current, Counter depth){
		while(current.right != null){
			current = current.right;
			++depth.c;
		}
		return current;
	}
	IntegerHeapNode getDeepPredecessor(IntegerHeapNode current, Counter depth){
		while(current.left != null){
			current = current.left;
			if(depth!=null) ++depth.c;
		}
		return current;
	}
	void removeRoot(IntegerHeapNode removal){
		if(removal == null) return;
		if(removal == root) root = null;
		else{
			if (removal.parent.left == removal) removal.parent.deleteLeft();
			else if (removal.parent.right == removal) removal.parent.deleteRight();
		}
	}
	void removeNode(IntegerHeapNode removal){
		if(removal == null) return;

		if(removal.left == null && removal.right == null){removeRoot(removal); return;}

		IntegerHeapNode leftN = removal.left, rightN = removal.right;
		Counter depthL=new Counter(), depthR=new Counter();
		if(leftN != null) leftN=getDeepSuccessor(leftN, depthL);
		if(rightN != null) rightN=getDeepPredecessor(rightN, depthR);

		if(depthR.c >= depthL.c && rightN != null){
			removal.data = rightN.data;
			removeRoot(rightN);
		}
		else{
			removal.data = leftN.data;
			removeRoot(leftN);
		}
	}
	void removeNth(int num){
		removeNode(nthElement(num));
	}
}