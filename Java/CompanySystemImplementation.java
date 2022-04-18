/**
The code has been developed by Cagatay Cakir for the Gebze Technical University Computer Science department, course 222 CSE.
This used to be a private build and only intended to be viewed by the course lecturer and assistants. But now uploaded to github as a business reference.
*/

import java.io.File;
import java.io.FileWriter;
import java.util.Scanner;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.IOException;
import java.io.Serializable;
import java.util.Random;
import java.util.ArrayList;
import java.util.LinkedList;

/**
This is the main class. Calls initialization function for the created {@link myComp} Company class.
After initialization is done, login menu loop activated. After menu loop terminates everything is saved.
Company data is saved and loaded in a serializable file format where it is specified at {@link dataPath}.
*/
public class CompanySystemImplementation{
	static Company myComp;
	static Menu myMenu;
	/**
	Datapath of serializabled company data to load from & save to
	*/
	static String dataPath = ".\\companyData.ser";
	/**
	Calls initializer, creates a new menu class and runs the main menu
	After the menu loop is terminated database is overwritten
	@param args Unused
	*/
	public static void main(String[] args){
		//load from file if exists
		initDatabase();

		myMenu = new Menu(myComp);
		myMenu.mainMenu();

		storeDatabase();
		System.out.println("Bye!");
	}
	/**
	If companyData doesn't exist, a new instance of file is generated along with a root admin and password.
	*/
	static void initDatabase(){
		if (! (new File(dataPath)).exists() ){
			System.out.println("Warning! Database does not exist. Default admin name is \"RootAdmin\".");
			System.out.println("Generating pass for RootAdmin....");
			myComp = new Company();
			return;
		}
		try{
			FileInputStream fis = new FileInputStream(dataPath);
			ObjectInputStream ois = new ObjectInputStream(fis);
			myComp = (Company) ois.readObject();
			ois.close();
		} catch (IOException e){
			System.out.println("DATABASE LOCATION ERROR");
			e.printStackTrace();
		} catch (ClassNotFoundException e){
			System.out.println("CLASS SERIALIZATION FAILED");
			e.printStackTrace();
		}
	}
	static void storeDatabase(){
		try{
			FileOutputStream fout = new FileOutputStream(dataPath);
			ObjectOutputStream oos = new ObjectOutputStream(fout);
			oos.writeObject(myComp);
			oos.close();
		} catch (IOException e) {
			System.out.println("SAVING TO DATABASE FAILED");
			e.printStackTrace();
		} 
	}
}

/**
This is the menu class. User login handling and Company data manupilation is done in here.
When "exit" is typed, {@link exitFlag} variable is set to true and causes to end every single menu loop.
Typing "back" returns back from the current method loop.
*/
class Menu{
	private Company compPtr;
	private User logCred;
	private Scanner reader = new Scanner(System.in);
	private boolean exitFlag=false;
	Menu(Company c) {compPtr = c;}
	/**
	Main menu method where user creditentials taken with {@link logIn} to set {@link logCred} or new user signup is registered with {@link newUser}
	*/
	void mainMenu(){
		String opt;

		System.out.println("Welcome to the Brotherhood Furniture (tm) automation system.");
		do{
			System.out.println("Type 1 to login to the system.");
			System.out.println("Type 2 to register as a customer.");
			System.out.println("Type \"exit\" anytime to quit the system.");
			System.out.println("Type \"back\" anytime to return to the previous menu.");
			opt = reader.nextLine();
			if(opt.equalsIgnoreCase("exit") || opt.equalsIgnoreCase("back")) return;
			
			if(opt.equals("1")) logIn();
			else if(opt.equals("2")) newUser();
			else System.out.println("Unknown command. Please try again.");
		}while(!exitFlag);
	}
	
	private void logIn(){
		//ask for credentials.
		String inUser, inPass;
		do{
			//check username and password
			System.out.println("Please enter your username:");
			inUser = reader.nextLine();
			if(inUser.equalsIgnoreCase("exit")) {exitFlag=true; return;}
			if(inUser.equalsIgnoreCase("back")) return;
			
			System.out.println("Please enter your password:");
			inPass = reader.nextLine();
			if(inPass.equalsIgnoreCase("exit")) {exitFlag=true; return;}
			if(inPass.equalsIgnoreCase("back")) return;
			
			logCred = compPtr.getUserByNameAndPass(inUser,inPass);
			if(logCred != null) {userMenu(); return;}
			
			System.out.println("Wrong credentials. Please try again.");
		}while (!exitFlag);
	}
	private void newColleague(boolean admin){
		if(admin){
			String inUser, inMail;
			System.out.println("Please enter admin username:");
			inUser = reader.nextLine();
			if(inUser.equalsIgnoreCase("exit")) {exitFlag=true; return;}
			if(inUser.equalsIgnoreCase("back")) return;

			System.out.println("Please enter admin email:");
			inMail = reader.nextLine();
			if(inMail.equalsIgnoreCase("exit")) {exitFlag=true; return;}
			if(inMail.equalsIgnoreCase("back")) return;
			
			if(compPtr.addUser(inUser, inMail, 2)) return;
			System.out.println("Credentials are invalid. Please try again.");
		}
		else{
			String inUser, inMail, inBranch;
			System.out.println("Please enter employee username:");
			inUser = reader.nextLine();
			if(inUser.equalsIgnoreCase("exit")) {exitFlag=true; return;}
			if(inUser.equalsIgnoreCase("back")) return;

			System.out.println("Please enter employee email:");
			inMail = reader.nextLine();
			if(inMail.equalsIgnoreCase("exit")) {exitFlag=true; return;}
			if(inMail.equalsIgnoreCase("back")) return;
			
			System.out.println("Please enter employee branch:");
			inBranch = reader.nextLine();
			if(inBranch.equalsIgnoreCase("exit")) {exitFlag=true; return;}
			if(inBranch.equalsIgnoreCase("back")) return;
			
			if(compPtr.addUser(inUser, inMail, inBranch, 1)) return;
			System.out.println("Credentials are invalid. Please try again.");
		}
	}
	private void newUser(){
		//create new user account
		while(true){
			String inUser, inMail, inPass;
			System.out.println("Please enter your username:");
			inUser = reader.nextLine();
			if(inUser.equalsIgnoreCase("exit")) {exitFlag=true; return;}
			if(inUser.equalsIgnoreCase("back")) return;

			System.out.println("Please enter your email:");
			inMail = reader.nextLine();
			if(inMail.equalsIgnoreCase("exit")) {exitFlag=true; return;}
			if(inMail.equalsIgnoreCase("back")) return;

			System.out.println("Please enter your password:");
			inPass = reader.nextLine();
			if(inPass.equalsIgnoreCase("exit")) {exitFlag=true; return;}
			if(inPass.equalsIgnoreCase("back")) return;
			
			if(compPtr.addUser(inUser, inMail, inPass, 0)) return;

		}
	}
	private void newBranch(){
		//create new branch
		String inUser;
		while(true){
			System.out.println("Please enter branch:");
			inUser = reader.nextLine();
			if(inUser.equalsIgnoreCase("exit")) {exitFlag=true; return;}
			if(inUser.equalsIgnoreCase("back")) return;
			
			if(compPtr.addBranch(inUser)) return;
			System.out.println("Please try again.");
		}
	}
	private void userMenu(){
		String opt;
		System.out.print("Logged in! Welcome back ");
		System.out.print(logCred.getName());
		System.out.println("!");
		if(logCred instanceof Administrator){
			//do admin stuff
			do{
				System.out.println("Type 1 to add a new admin.");
				System.out.println("Type 2 to add a new employee.");
				System.out.println("Type 3 to add a new branch.");
				System.out.println("Type 4 to query list of out of stock products.");
				opt = reader.nextLine();
				if(opt.equalsIgnoreCase("exit")) {exitFlag=true; return;}
				else if(opt.equalsIgnoreCase("back")) return;
				else if(opt.equals("1")) newColleague(true);
				else if(opt.equals("2")) newColleague(false);
				else if(opt.equals("3")) newBranch();
				else if(opt.equals("4")) printOOS();
				else System.out.println("Unrecognized command. Please try again.");
			}while(!exitFlag);
		}
		else if(logCred instanceof Employee){
			do{
				System.out.println("Type 1 to list stock of every product.");
				System.out.println("Type 2 to add a product.");
				System.out.println("Type 3 to query list of out of stock products.");
				System.out.println("Type 4 to get orders of user by no.");
				opt = reader.nextLine();
				if(opt.equalsIgnoreCase("exit")) {exitFlag=true; return;}
				else if(opt.equalsIgnoreCase("back")) return;
				else if(opt.equals("1")) compPtr.listProducts();
				else if(opt.equals("2")) compPtr.addProduct(reader.nextLine());
				else if(opt.equals("3")) printOOS();
				else if(opt.equals("4")) printUsrPur();
				else System.out.println("Unrecognized command. Please try again.");
			}while(!exitFlag);
		}
		else if(logCred instanceof Client){
			do{
				System.out.println("Type 1 to list products.");
				System.out.println("Type 2 to order a product.");
				System.out.println("Type 3 to list your purchases.");
				opt = reader.nextLine();
				if(opt.equalsIgnoreCase("exit")) {exitFlag=true; return;}
				if(opt.equalsIgnoreCase("back")) return;
				else if(opt.equals("1")) compPtr.listProducts();
				else if(opt.equals("2")) orderMenu();
				else if(opt.equals("3")) listPurc();
				else System.out.println("Unrecognized command. Please try again.");
			}while(!exitFlag);
		}
	}
	private void removeUser(String delUser){
		if(compPtr.getUserByName(delUser) == null) return;
		if(logCred instanceof Administrator){
			System.out.print("You are about to terminate user \"");
			System.out.print(delUser);
			System.out.println("\" do you want to proceed?(y\\n)");
			if(reader.nextLine() == "y") compPtr.removeUserByName(delUser);
			else if (reader.nextLine() == "n") return;
			else{ System.out.println("Invalid input."); return; }
		}
		else if(logCred==compPtr.getUserByName(logCred.getName())){
			System.out.println("You are about to terminate your account. Do you want to proceed?(y\\n)");
			if(reader.nextLine() == "y") {exitFlag=true; compPtr.removeUserByName(delUser);}
			else if (reader.nextLine() == "n") return;
			else{ System.out.println("Invalid input."); return; }
		}
	}
	private void printOOS(){
		ArrayList <Product> list = compPtr.listOOS();
		System.out.println("Out of stock items are:");
		for(byte i=0; i<list.size(); i++) compPtr.printProductInfo(list.get(i));
	}
	private void orderMenu(){
		String item;
		System.out.println("Type your item to order.");
		item = reader.nextLine();
		if(item.equalsIgnoreCase("exit")) {exitFlag=true; return;}
		if(item.equalsIgnoreCase("back")) return;

		String adress;
		System.out.println("Type your adress.");
		adress = reader.nextLine();
		if(adress.equalsIgnoreCase("exit")) {exitFlag=true; return;}
		if(adress.equalsIgnoreCase("back")) return;
		compPtr.requestOrder(logCred,item,adress);
	}
	private void listPurc(){
		listPurc(logCred);
	}
	private void listPurc(User userPtr){
		if(! (userPtr instanceof Client) || userPtr.history.size()==0){
			System.out.println("You have no purchases.");
			return;
		}
		for(byte i=0; i<userPtr.history.size(); i++)
			System.out.println(userPtr.history.get(i));
	}
	private void printUsrPur(){
		String usrNo;
		do{
			System.out.println("Please enter user no.");
			usrNo = reader.nextLine();
			if(usrNo.equalsIgnoreCase("exit")) {exitFlag=true; return;}
			if(usrNo.equalsIgnoreCase("back")) return;
			User userPtr = compPtr.getClientByNo(Integer.parseInt(usrNo));
			listPurc(userPtr);
		}while(!exitFlag);
	}
}

/**
All company data and related functions are encapsuled in this class.
*/
class Company implements Serializable{
	private ArrayList<User>users = new ArrayList<User>();
	private LinkedList<String>branches =new LinkedList<String>();
	private byte MAX_NUMBER = 8;
	private LinkedList<ArrayList<Product>> products = new LinkedList<ArrayList<Product>>();
	/**
	Should be increased with every new sign up. But never be decreased.
	*/
	private int customerCount=0;
	
	User getUserByNameAndPass(String name, String pass){
		for (byte i=0; i<users.size(); i++)
			if (users.get(i).getName().equalsIgnoreCase(name) && users.get(i).passCheck(pass) ) return users.get(i);
		return null;
	}
	User getUserByName(String name){
		for (byte i=0; i<users.size(); i++)
			if (users.get(i).getName().equals(name)) return users.get(i);
		return null;
	}
	
	Company(){
		users.add(new Administrator());
		users.get(0).changeName("RootAdmin");
	}
	/**
	Checks if credentials are valid, returns false if not.
	Then add user depending on the type.
	If type is 1, new employee is added and {@link passorbranch} becomes branch.
	Password cannot be predetermined with branch users.
	If type is 2, new administrator is added and {@link passorbranch} becomes pass.
	Anything else considered a client and {@link passorbranch} becomes pass.
	{@link customerCount} is also increased.
	Returns true when no problem is encountered.
	*/
	boolean addUser(String name, String email, String passorbranch, int type){
		if(!nameCheck(name)) {System.out.println("Invalid name. Please try again."); return false;}
		if(!mailCheck(email)) {System.out.println("Invalid mail. Please try again."); return false;}
		if(type!=1 && !passCheck(passorbranch)){
			System.out.println("Invalid pass. Please try again.");
			System.out.println("Pass length should be 8-30 letters and have at least a one lowercase, uppercase, one special and one numerical character.");
			return false;}
		if(type==1 && !branchCheck(passorbranch)){System.out.println("Branch doesn't exist. Please try again."); return false;}
		
		switch(type){
			case 1: users.add(new Employee(name, email, passorbranch)); break;
			case 2: users.add(new Administrator(name, email, passorbranch)); break;
			default: users.add(new Client(name, email, passorbranch,++customerCount)); break;
		}
		Hw3.storeDatabase();
		return true;
	}
	/**
	Create users with random pasword.
	Checks if credentials are valid, returns false if not.
	Then add user depending on the type.
	If type is 1, new employee is added.
	If type is 2, new administrator is added and {@link passorbranch} becomes pass.
	Anything else considered a client, {@link customerCount} is also increased.
	Returns true when no problem is encountered.
	*/
	boolean addUser(String name, String email, int type){
		//check if credentials are valid return false if not
		if(!nameCheck(name)) {System.out.println("Invalid name. Please try again."); return false;}
		if(!mailCheck(email)) {System.out.println("Invalid mail. Please try again."); return false;}
		
		//if valid, add to the database and return true.
		switch(type){
			case 1: users.add(new Employee(name, email)); break;
			case 2: users.add(new Administrator(name, email)); break;
			default: users.add(new Client(name, email, ++customerCount)); break;
		}
		Hw3.storeDatabase();
		return true;
	}
	void listProducts(){
		for(byte j=0; j<products.size(); j++)
			for(byte i=0; i<products.get(j).size(); i++)
				printProductInfo(products.get(j).get(i));
	}
	/**
	Returns product pointer by name. If not found, returns null.
	*/
	Product searchProduct(String name){
		for(byte j=0; j<products.size(); j++)
			for(byte i=0; i<products.get(j).size(); i++)
				if (products.get(j).get(i).name.equalsIgnoreCase(name)) return products.get(j).get(i);
		return null;
	}
	/**
	Adds product to system. If item already exists, print message and return.
	*/
	void addProduct(String prodn){
		if (searchProduct(prodn) != null){
			System.out.println("This product already exists on the system.");
			return;
		}
		for(byte i=0; i<products.size()-1; i++)
			if(products.get(i).size() < MAX_NUMBER){
				products.get(i).add(new Product(prodn));
				return;
			}
		if (products.size()==0 ||
			products.get(products.size()-1).size() > MAX_NUMBER)
				products.add(new ArrayList<Product>());
		
		products.get(products.size()-1).add(new Product(prodn));
	}
	/**
	Removes product from system. If item doesn't exist, print message and return.
	*/
	void removeProduct(String prodn){
		int i=0; boolean flag=true;
		for(; i<products.size() && flag; i++)
			for(byte j=0; j<products.get(i).size() && flag; j++)
				if(products.get(i).get(j).name.equalsIgnoreCase(prodn)){
					products.get(i).remove(j);
					flag=false;
				}
		if(flag){
			System.out.println("Product is not found.");
			return;
		}
		if(products.size()>0 && products.get(i).size()==0)
			products.remove(i);
	}
	/**
	Adds branch to system. If branch already exists, print message and return.
	*/
	boolean addBranch(String bran){
		if (branchCheck(bran)){
			System.out.println("This branch already exists on the system.");
			return false;
		}
		branches.add(bran);
		return true;
	}
	/**
	Removes branch from system. If branch doesn't exist, print message and return.
	*/
	void removeBranch(String bran){
		for(byte i=0;i<branches.size();i++)
			if(branches.get(i).equalsIgnoreCase(bran)){
				branches.remove(i);
				return;
			}
		System.out.println("This branch doesn't exist on the system.");
	}
	/**
	Returns an arraylist of out of stock products
	*/
	ArrayList<Product> listOOS(){
		ArrayList<Product> retOOS = new ArrayList<Product>();
		for(byte j=0; j<products.size(); j++)
			for(byte i=0; i<products.get(j).size(); i++)
				if(products.get(j).get(i).stock == 0)
					retOOS.add(products.get(j).get(i));
		
		return retOOS;
	}
	/**
	Initiates an order by a user. Checks if item has stock. If not out of stock, adds item to the user purchase history.
	*/
	void requestOrder(User myUser, String item, String info){
		Product myItem = searchProduct(item);
		if(myItem==null){
			System.out.println("This item doesn't exist. Try again.");
			return;
		}
		if(myItem.stock==0){
			System.out.println("This item is out of stock.");
			return;
		}
		myUser.history.add(myItem.name + " to the adress:" + info
		+ " cost:" +myItem.price);
		--(myItem.stock);
	}
	/**
	Returns user class by userno. If not found returns null.
	*/
	User getClientByNo(int no){
		if(no>customerCount) return null;
		for(byte i=0; i<users.size(); ++i)
			if(users.get(i) instanceof Client && ((Client) users.get(i)).getUserNo() == no)
				return users.get(i);
		return null;
	}
	/**
	Removes user by index from database
	*/
	private void removeUser(byte index){
		if(index == -1 || index >= users.size()) return;
		users.remove(index);
	}
	private byte getIndexByName(String name){
		for (byte i=0; i<users.size(); i++)
			if (users.get(i).getName().equalsIgnoreCase(name)) return i;
		return (byte) -1;
	}
	void removeUserByName(String name){
		removeUser(getIndexByName(name));
	}
	/**
	Checks if name is valid
	Name must be between 2-20 characters
	Name cannot be "exit" or "back"
	Name must only have letters
	Name must not be already taken
	*/
	boolean nameCheck(String name){
		if(name.length()<2 || name.length()>20) return false;
		if(name.equalsIgnoreCase("exit") || name.equalsIgnoreCase("back")) return false;
		for(byte i=0; i<name.length(); i++) if(!Character.isLetter(name.charAt(i))) return false;	
		for (byte i=0; i<users.size(); i++) if(users.get(i).getName().equalsIgnoreCase(name)) return false;
		return true;
	}
	/**
	Checks if mail address is valid
	Mail must be between 5-30 characters
	Mail cannot be "exit" or "back"
	Mail must not be already taken
	*/
	boolean mailCheck(String mail){
		if(mail.length()<5 || mail.length()>30) return false;
		if(mail.equalsIgnoreCase("exit") || mail.equalsIgnoreCase("back")) return false;
		for (byte i=0; i<users.size(); i++) if(users.get(i).getMail() != null && users.get(i).getMail().equalsIgnoreCase(mail)) return false;
		return true;
	}
	/**
	Checks if password is valid
	Pass should be between 8-20 characters
	Pass cannot be "exit" or "back"
	Pass should have at least one lowercase, uppercase, one special and one numerical character
	*/
	boolean passCheck(String pass){
		if(pass.length()<8 || pass.length()>20) return false;
		if(pass.equalsIgnoreCase("exit") || pass.equalsIgnoreCase("back")) return false;

		boolean s=false, n=false, l=false, u=false;
		for (byte i=0; i<pass.length(); i++){	
			if(pass.charAt(i) >= '0' && pass.charAt(i) <= '9') n=true;
			else if(pass.charAt(i) == '*' || pass.charAt(i) == '+' || pass.charAt(i) == '-' || pass.charAt(i) == '.' || pass.charAt(i) == '_') s=true;
			else if(pass.charAt(i) >= 'a' && pass.charAt(i) <= 'z') l=true;
			else if(pass.charAt(i) >= 'A' && pass.charAt(i) <= 'Z') u=true;
			else return false;				//invalid character
		}
			
		if (!s || !n || !l || !u) return false;
		return true;
	}
	/**
	Checks if branch exists on the database
	*/
	boolean branchCheck(String branch){
		for (byte i=0; i<branches.size(); i++)
			if(branches.get(i).equalsIgnoreCase(branch)) return true;
		return false;
	}
	void printProductInfo(Product prod){
		System.out.print("(");
		System.out.print(prod.name);
		System.out.print(")= Stock: ");
		System.out.print(prod.stock);
		System.out.print(" Current price: ");
		System.out.println(prod.price);
	}
}

class Product implements Serializable{
	double price=0;
	int stock=0;
	String name;
	Product(double pr, int st, String nm){price = pr; stock=st; name=nm;}
	Product(String nm){name=nm;}
}

class User implements Serializable{
	protected String name="";
	protected String mail="";
	protected String password="";
	protected ArrayList<String> history=new ArrayList<String>();
	
	User(){
		//random pass generation only
		passGen();
	}
	User(String n, String m, String p){name = n; mail = m; password = p;}
	User(String n, String m){name = n; mail = m; passGen();}
	void changeName(String newName){name = newName;}
	void changeMail(String newMail){mail = newMail;}
	void changePass(String newPass){password = newPass;}

	void passGen(){
		//generate a random password and print it on console for user
		password = "";
		for (byte i=0; i<8; i++) password += randLett();
		System.out.println("Your generated password is: ");
		System.out.println(password);
	}
	
	private char randLett(){
		Random rand = new Random();
		int perc = rand.nextInt(100);
		if(perc<30) return (char) (rand.nextInt(10)+'0');	//numerical
		if(perc<60) return (char) (rand.nextInt(24)+'A');	//uppercase letters
		if(perc<95) return (char) (rand.nextInt(24)+'a');	//lowercase letters

		switch (rand.nextInt(6)){							//special characters
			case 0: return '*';
			case 1: return '+';
			case 2: return '-';
			case 3: return '.';
			case 4: return '_';
			default: break;
		}
		return '|';
	}

	String getName(){return name;}
	String getMail(){return mail;}
	
	boolean passCheck(String inp){return inp.equals(password);}
}

class Administrator extends User{
	Administrator(){
		super();	//output root pass in a file
		try {
			FileWriter rootPassOut = new FileWriter("rootadminpassword.txt");
			rootPassOut.write(password);
			rootPassOut.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
	} 
	Administrator(String n, String m, String p){super(n, m, p);}
	Administrator(String n, String m){super(n, m);}
}

class Employee extends User{
	Employee(String n, String m, String p, String b){super(n, m, p); branch=b;}
	Employee(String n, String m, String b){super(n, m); branch=b;}
	Employee(String n, String m){super(n, m);}
	String branch;
}

class Client extends User{
	Client(String n, String m, int uN){super(n, m); userNo = uN;}
	Client(String n, String m, String p, int uN){super(n, m, p); userNo = uN;}
	ArrayList<String>purchases = new ArrayList<String>();
	protected int userNo;
	int getUserNo(){return userNo;}
	void setUserNo(int i){userNo=i;}
}
