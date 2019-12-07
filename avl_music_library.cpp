#include<iostream>
#include<string>
#include<fstream>            //this library is included for file handling, both input and output
using namespace std;

class node                   // nodes for the avl tree which will store the records of the artist
{
	public:
		string artist_name;
		string song_name;
		string format_of_song;
		string year_of_release;
		node*right;
		node*left;
		node*next;
		node() //constructor
		{
			right=NULL;
			left=NULL;
			next=NULL;
		}
}*root;

class avltree
{
	public:
		int searchtitle(ostream&,node*,string);
		int hashfunction(string);
		int height(node *);
        int balancefactor(node *);
		void display(ostream&,node*,int);
		void insert(string);
		void deletenode(ostream&, string);
		bool check(string);
		node* searchname(string);
		node* balancing(node*);
        node *right_right_rotation(node *);
        node *left_left_rotation(node *);
        node *left_right_rotation(node *);
        node *right_left_rotation(node *);
		avltree()
		{
			root=NULL;
		}
};

///////////////////////////////////////////////////
///////////////// DELETION OF A NODE //////////////
///////////////////////////////////////////////////

void avltree::deletenode(ostream& os,string name)//ostream variable is used in order to write to a file when a function call is involved
{      /*
		* when the node to be deleted is at the root:
		*/
		if (root->artist_name == name)
		{
			os<<"Data on "<<name<<" is present in the tree.\n";

			while(root->next!=NULL)             // deleting the linkedlist attached to the root, if any
			{
					node*c=root;
					node*u;
					while (c->next!=NULL)
					{
						u=c;
						c=c->next;
					}
					u->next=NULL;
					delete c;
			}

			if (root->right==NULL && root->left==NULL)   //when the root is the only node in the tree
			{
				root=NULL;
			}
			else                                        //when there are other nodes present in the tree
			{
				node*a=root,*b;         ///   b is the node which will replace the root.

				///If root has no node on the right, that means that there will be only one node on the left
				// as the tree is balanced, the only node on the left will replace the root.
				if (root->right==NULL)                  
				{
					b=root->left;
				}

				// And if there are nodes on the right side of root, we will find the smallest or left most item on 
				//  the right side of root, and replace the root with it.
				else                                    
				{
					b = root->right;
				}

				//here we find the left most item
				//a is made equal to the parent node of b
				while (b->left != NULL)                
				{	a = b;
					b = b->left;
				}

				//replacing the data of root with that of b
				root->artist_name = b->artist_name; 
				root->song_name=b->song_name;
				root->format_of_song=b->format_of_song;
				root->year_of_release=b->year_of_release;			
				root->next=b->next;           //we directly link the linked list of b to a
			
				if(a==root && root->right!=NULL) //when there is a single node on the right side of tree, the left side may or may not have nodes
				{
					root->right=NULL;
				}
				else if (a==root && root->right==NULL)      // when there is a single node on the left side of tree
				{
					root->left=NULL;
				}
				else                          //when there are more than one node on the right of the tree
				{
					a->left = b->right; //as b->left is already NULL, so a points to the right node of b, if any
				
				}
				b->next=NULL;
				b->right=NULL;
				delete b;
			}
			os<<"The node containing records of "<<name<<" has been deleted.\n\n";
		
		}

		/*        TNTBD=the node to be deleted
		 *       when TNTBD is not the root
		 */      

		else
		{
			node*ptr=root;  //ptr will be TNTBD
			node*prev;  //it will store the address of the parent node of TNTBD
			while(0!=2)
			{
				if(name>ptr->artist_name)
				{
					if (ptr->right==NULL)   //if we reach NULL, then the given node give not exist in the tree
					{
						os<<"Data on "<<name<<" is not present in the tree."<<endl;
						return;
					}
					prev=ptr;
					ptr=ptr->right;
				}
				else if(name<ptr->artist_name)
				{
					if (ptr->left==NULL)      //if we reach NULL, then the given node give not exist in the tree
					{
						os<<"Data on "<<name<<" is not present in the tree."<<endl;
						return;
					}
					prev=ptr;
					ptr=ptr->left;
				}
				else if (name==ptr->artist_name)
				{
					os<<"Data on "<<name<<" is present in the tree."<<endl;
					break;
				}
			}

			while(ptr->next!=NULL) //deleting the linked list attached to the TBTBD, if any
			{
				node*c=ptr;
				node*u;
				while (c->next!=NULL)
				{
					u=c;
					c=c->next;
				}
				u->next=NULL;
				delete c;
			}

			if (ptr->left == NULL && ptr->right == NULL) //// when the node to be deleted is a leaf node
			{				
				if (prev->left == ptr) //when the node to be deleted is on the left of its parent
				{
					ptr=NULL;
					delete ptr;
					prev->left = NULL;
					
				}
				else //when the node to be deleted is on the right of its parent
				{
					ptr=NULL;
					delete ptr;
					prev->right = NULL;
					
				}
			}
			
			// when the node to be deleted has a single child
			else if (ptr->left == NULL && ptr->right != NULL) // when TNTBD has a child on the right
			{
				if (prev->left == ptr) 
				{
					prev->left = ptr->right;
				}
				else
				{
					prev->right = ptr->right;
				}
				ptr->right=NULL;
				delete ptr;
			}


			else if (ptr->left != NULL && ptr->right == NULL) // when TNTBD has a child on the left
			{
				if (prev->left == ptr)    //checking whether ptr is on the right or left of its parent
				{
					prev->left = ptr->left;
				}
				else
				{
					prev->right = ptr->left;
				}
				ptr->left=NULL;
				delete ptr;
			}

			else //when TNTBD has child nodes on both sides
			{
				node*prev = ptr;
				node*b = ptr->left;
				while (b->right != NULL)   //we find the maximum item on the left side of ptr to replace it
				{
					prev = b;
					b = b->right;
				}

				//replacing data of ptr with b
				ptr->artist_name = b->artist_name;
				ptr->song_name=b->song_name;
				ptr->format_of_song=b->format_of_song;
				ptr->year_of_release=b->year_of_release;
				ptr->next=b->next; //linking the linked list of b with ptr		
				
				if (prev == ptr)   //when ptr->left is the largest item on the left of ptr
				{
					prev->left = b->left; //as b->right=NULL so we connect the prev to b->left
				}
				else 
				{			
					prev->right = b->left; //as b->right=NULL so we connect the prev to b->left
				}		
				delete b;
			}
			os<<"The node containing records of "<<name<<" has been deleted.\n\n";
		}
		
}



int avltree::height(node *c) 
{
    int h = 0;
    if (c != NULL) // if c is NULL that means height is zero
    {
        int left_height = height (c->left);
        int right_height = height (c->right);
        int max_height = max (left_height, right_height);// max is an in built function which returns the greater one of its 2 arguments
        h = max_height + 1;  //1 is added to account for the level of c
    }
    return h;
}
 
////////////////////////////////////////////////////////////////////
///////////Height difference between left and right of tree/////////
////////////////////////////////////////////////////////////////////

int avltree::balancefactor(node *c)
{
    int left_height = height (c->left);
    int right_height = height (c->right);
    int balance_factor= left_height - right_height;
    return balance_factor;  // if bfactor is greater than 1 or less than -1, that means that the tree is not balanced
}
 
///////////////////////////////////////////////////
//// RIGHT RIGHT ROTATION /////////////////////////
///////////////////////////////////////////////////

node *avltree::right_right_rotation(node *t)
{
    node *c;
    c = t->right;
    t->right = c->left;
    c->left = t;
    return c;
}

/////////////////////////////////////

node *avltree::left_left_rotation(node *t)
{
    node *c;
    c = t->left;
    t->left = c->right;
    c->right = t;
    return c;
}

//////////////////////////

node *avltree::left_right_rotation(node *t)
{
    node *c;
    c = t->left;
    t->left = right_right_rotation (c);
    return left_left_rotation (t);
}
 
/////////////////////////////////////////////////
///////////////////////////////////////////////

node *avltree::right_left_rotation(node *t)
{
    node *c;
    c = t->right;
    t->right = left_left_rotation (c);
    return right_right_rotation (t);
}
 
//////////////////////
//////BALANCING///////
//////////////////////

node* avltree::balancing(node *c) //c receives the root
{
    int bal_factor = balancefactor (c); 

    if (bal_factor > 1) //tree is unbalanced and left side of tree has more nodes than the right
    {

        if (balancefactor (c->left) > 0) //left side of c->left has more nodes than right
            c = left_left_rotation (c);   //combination of left side of root and left side of root->left

        else                              //right side of c->left has more nodes than left  
            c = left_right_rotation (c);	//combination of left side of root and right side of root->left	
    }

    else if (bal_factor < -1) //tree is unbalanced and the right side of tree has more nodes than the left
    {
        if (balancefactor (c->right) > 0) //left side of c->right has more nodes than right
            c = right_left_rotation (c);  //combination of right side of root and left side of root->right	

        else                                //left side of c->left has more nodes than right
            c = right_right_rotation (c);	//combination of right side of root and right side of root->right		
    }

    return c;
}

////////////////////////////////////////////////////////////////
///////////insert a node////////////////////////////////////////
////////////////////////////////////////////////////////////////

void avltree::insert(string line)
{
	//first we separate data
	size_t pos = line.find_first_of("\t");  //find_first_of searches the string for the character specified in its arguments and returns its location in the string
											//size_t is the variable used to store the position of tab

	int arr[3];  //it stores the location of tabs in the string
	int count=0; //used to specify location in array

	while (pos != string::npos) //npos indicates the failure in finding any value for pos
	{                           //so pos==string::npos, when no value can further be found for pos
		arr[count]=pos;
		count++;		
		pos = line.find_first_of("\t", pos + 1); //adding the argument pos+1 so that the tab is searched for in the string from the position pos+1 onwards
													//previous positions are ignored
	}
	
	string artistname=line.substr(0,arr[0]);//to get the artist name
	string remaining3=line.substr(arr[0]+1);
	
	string songname=remaining3.substr(0,arr[1]-arr[0]-1); // to get song name
	string remaining2=remaining3.substr(arr[1]-arr[0]);

	string format=remaining2.substr(0,arr[2]-arr[1]-1);// to get format
	string year=remaining2.substr(arr[2]-arr[1]);// to get year
	
	///////////////////////////////
	//now we start the real insertion

		node*n=new node;
		n->artist_name=artistname;
		n->right=NULL;
		n->left=NULL;

		n->song_name=songname;
		n->format_of_song=format;
		n->year_of_release=year;
		n->next=NULL;

		if (root==NULL) //when tree is empty
		{
			root=n;
		}
		else
		{
			node*c=root;
			while(0!=3)
			{
				if (artistname>c->artist_name)
				{	
					if (c->right==NULL)
					{
						c->right=n;
						root=balancing(root);
						break;
					}
					c=c->right;
				}

				else if (artistname<c->artist_name)
				{
					if (c->left==NULL)
					{
						c->left=n;
						root=balancing(root);
						break;
					}
					c=c->left;
				}

				else if (artistname==c->artist_name)
				{
					if (songname==c->song_name)
					{
						break; //as a duplicate entry is not allowed
					}
					else
					{
						while (c->next!=NULL) //inserting at end of the linked list of the respective node
						{
							c=c->next;
						}
						c->next=n;
						break;
					}
				}
			}
		}
}
//////////////////////////////////////////////////////////
//checking command line///////////////////////////////////
//////////////////////////////////////////////////////////

bool avltree:: check(string a) //to detect the command
{
	if (a=="ARTIST" ||a=="artist")
	{
		return true;
	}
	else if (a=="LIST" || a=="list")
	{
		return true;
	}
	else if (a=="COUNT" || a=="count")
	{
		return true;
	}
	else if (a=="REMOVE" ||a=="remove")
	{
		return true;
	}
	else if (a=="TITLE"||a=="title")
	{
		return true;
	}
	else if (a=="DISPLAY" ||a=="display")
	{
		return true;
	}
	else if (a=="EXIT"||a=="exit")
	{
		return true;
	}
	else
	{return false;}
	
}

////////////////////////////////////////////////////
///////////////DISPLAY THE TREE/////////////////////
////////////////////////////////////////////////////

void avltree::display(ostream &os,node *ptr, int level)
{
    int i;
    if (ptr!=NULL)
    {
        display(os,ptr->right, level + 1); //displays the right side of tree first
        os<<endl;
        if (ptr == root)
        {
			os<<"Root -> ";
		}
        for (i = 0; i < level && ptr != root; i++) //no space for root
        {
			os<<"       ";
		}
		os<<ptr->artist_name<<" ("<<ptr->song_name;
		node*c=ptr;
		while (c->next!=NULL) //to display songs in the linked list if any
		{
				os<<",";
				os<<c->next->song_name;
				c=c->next;
		}
		os<<')';
        display(os,ptr->left, level + 1); //to display the left side of tree
    }
}

////////////////////////////////////////////////////
node* avltree:: searchname(string a) ///for searching artist
	{
		node*c=root;
		while(c!=NULL)
		{
			if (a>c->artist_name)
				c=c->right;
			else if (a<c->artist_name)
				c=c->left;
			else if (a==c->artist_name)
			{
				return c;
			}
		}
		return NULL;
		
	}

///////////////////////////////////////////////////
/////////////SEARCH A SONG/////////////////////////
///////////////////////////////////////////////////

int avltree:: searchtitle(ostream& os,node* c,string title) ///for searching title
	{
		int count=0; //to count the no of titles present
		if (c!=NULL)
		{

			count+=searchtitle(os,c->left,title); 
			node*ptr=c;
			while(ptr!=NULL)
			{
				if(title==ptr->song_name)
				{
					count++;
					os<<ptr->artist_name<<"\t";
					os<<ptr->song_name<<"\t";
					os<<ptr->year_of_release<<endl;
				}
				ptr=ptr->next;
			}
			count+=searchtitle(os,c->right,title);
			
		}
		return count;
		
	}

//////////////////////////////
////////MAIN//////////////////
//////////////////////////////

void main()
{

	avltree tree;
	cout<<"\n\t\t\t\tAVL MUSIC LIBRARY"<<endl<<endl;
	
	/*
	 *     First we deal with the recording input file
	 */
	system("color 0B");
	string line; //to store the lines of input file
	string filename;
	cout<<"DEAR USER!! WELCOME!!\n\nPlease follow the following instructions before entering the input file below.";
	cout<<"\n\nDO NOT LEAVES ANY BLANK LINES in the text file you will give as input, furthermore do not enter useless spaces.\n\n";
	cout<<"Enter the data in the following format:\nARTIST NAME(TAB)SONG NAME(TAB)FORMAT(TAB)YEAR\n\n";
	cout<<"Give a single tab between the data.\n\n";
	cout<<"Give all inputs in UPPERCASE.\n\nFailure to comply with the above instructions might result in an error, in such a case, correct your input and run the program again :P\n\n";
	cout<<"Add \".txt\" after your original file name.\n\n";
e: cout<<"Enter the name of the data file containing the song records: ";

	getline(cin,filename);
	cout<<"Searching for input file...\n\n";

	ifstream datafile (filename); //opening file with variable datafile so as to write in it
	if (datafile.is_open())//to check if the file is open or not
	{
		cout<<"Data file found.\n\n";
		cout<<"Inserting the data file into tree...\n\n";
		while(getline (datafile,line,'\n')&&(!datafile.eof()))//inserting line by line
		{
			 if (line[0]=='\n' || line[0]==' ')
			{
				continue;
			}
			else
				tree.insert(line);
		}
		datafile.close(); //closing the file as it is no longer needed because input is completed
		cout<<"Insertion completed...\n\n";
	}
	else 
	{ 
		cout << "Data file not found. Unable to open file.\n\n"; 
		goto e;
	}
		 
	/*
	 * Now we open the log file
	 */

	q: cout<<"Enter the name of log file: ";
	getline(cin,filename);      //we use the same variable as used in the above input because it is no longer in use in input
	cout<<"\nSearching for log file...\n\n";
	ifstream logfile(filename);
	if (logfile.is_open())
	{
		cout<<"Log file found.\n\n";
		
	}
	else
	{
		cout<<"Log file not found. Unable to open file.\n\n";
		goto q;
	}
	ofstream log;
	log.open(filename); //we open the file with an ofstream variables which allows to write to the file
	log<<"\n\t\t\tAVL MUSIC LIBRARY"<<endl<<endl;	
	log<<"Recordings list successfully inserted into tree.\n\n\n";
		
	/*
	 * Now we deal with the command file
	 */

	
	cout<<"You can give the following commands in your command file:\n\n";
	cout<<"1. ARTIST<NAME> (to search for the name of the artist and to find the level of its node)\n";
	cout<<"2. LIST<NAME> (to display the songs of the artist)\n";
	cout<<"3. COUNT<NAME> (to count the number of songs of the artist)\n";
	cout<<"4. TITLE<TITLE> (to display the list of singers who have sung this song)\n";
	cout<<"5. REMOVE<NAME> (to delete the records of the particular artist)\n";
	cout<<"6. DISPLAY (to display the AVL tree)\n";
	cout<<"7. EXIT (To exit the program)\n";
	cout<<"\nwhere NAME=Name of the artist\n      TITLE=Name of the song\n\nThe greater than and less than signs are necessary to add.\n\n";
	
w: cout<<"Enter the name of command file: ";
	getline(cin,filename);
	cout<<"\nSearching for command file..."<<endl<<endl;
	ifstream commandfile (filename);
	if (commandfile.is_open())
	{
		cout<<"Command file found.\n\n";
		while(getline (commandfile,line,'\n')&&(!commandfile.eof()))//inserting line by line
		{
			 if (line[0]==';' || line[0]=='\n' || line[0]==' ')
			{
				continue;
			}
			else 
			{
				string check4=line.substr(0,4); //to check for list and exit
				string check5=line.substr(0,5); //to check for count and title
				string check6=line.substr(0,6); //to check for artist and remove
				string check7=line.substr(0,7); //to check for display
				string comm;
				string s[4]={check4,check5,check6,check7};
				for (int i=0;i<4;i++)
				{
					if (tree.check(s[i])==true) //returns the command
					{
						comm=s[i];
						break;
					}
				}

				/*
				 * when command is ARTIST
				 */

				if (comm=="ARTIST") 
				{
					string a=line.substr(7); //a=="ARTIST<"
					string name=a.substr(0,a.size()-1);
					log<<"SEARCHING for an ARTIST:\n";
					log<<"Searching for "<<name<<endl;
					if (root==NULL)
					{
						log<<"The tree is empty.\n\n";
					}
					else
					{
						node *rnode=tree.searchname(name);
						if (rnode==NULL)
						{
							log<<"No data on this artist is located in the AVL tree.\n\n";
						}
						else
						{
							log<<"Data on "<<name<<" is present in the tree.\n";
							log<<"The level of the node containing its data is "<<tree.height(root)+1-tree.height(rnode)<<endl<<endl;
						}						
					}
				}

				/*
				 * when command is LIST
				 */

				else if (comm=="LIST") 
				{
					string a=line.substr(5);
					string name=a.substr(0,a.size()-1);
					log<<"DISPLAYING SONGS of an ARTIST:\n";
					log<<"\n\nSearching for "<<name<<endl;
					if (root==NULL)
					{
						log<<"The tree is empty.\n\n";
					}
					else
					{
						node *rnode=tree.searchname(name);
						if (rnode==NULL)
						{
							log<<"No data on this artist is located in the AVL tree.\n\n";
						}
						else
						{
							log<<"Data on "<<name<<" is present in the tree.\n";
							log<<"The recordings of this artist are:\n\t"<<rnode->song_name;
							while (rnode->next!=NULL) //to display the songs in the linked list if any
							{
								log<<"\n\t";
								log<<rnode->next->song_name;
								rnode=rnode->next;
							}
							log<<endl<<endl;
						}
					}					
				}

				/*
				 * when command is COUNT
				 */

				else if (comm=="COUNT")
				{
					string a=line.substr(6);
					string name=a.substr(0,a.size()-1);
					log<<"COUNTING SONGS of an ARTIST:\n";
					log<<"Searching for "<<name<<endl;
					if (root==NULL)
					{
						log<<"The tree is empty.\n\n";
					}
					else
					{
						node *rnode=tree.searchname(name);
						if (rnode==NULL)
						{
							log<<"No data on this artist is located in the AVL tree.\n\n";
						}
						else
						{
							log<<"Data on "<<name<<" is present in the tree.\n";
							log<<"The number of recordings of this artist are ";
							int count=1; 
							node*r=rnode;
							while (rnode->next!=NULL)
							{
								count++;
								rnode=rnode->next;
							}
							log<<count<<endl;

							log<<"The recordings of this artist are:\n\t"<<r->song_name;
							while (r->next!=NULL) //to display the songs in the linked list if any
							{
								log<<"\n\t";
								log<<r->next->song_name;
								r=r->next;
							}
							log<<endl<<endl;
						}
					}
				}

				/*
				 * when command is REMOVE
				 */

				else if (comm=="REMOVE")
				{
					string a=line.substr(7);
					string name=a.substr(0,a.size()-1);
					log<<"DELETE an ARTIST'S data:\n";
					log<<"Searching for "<<name<<endl;
					if (root==NULL)
					{
						log<<"The tree is empty.\n\n";
					}
					else
					{
						tree.deletenode(log,name);
						if (root!=NULL) //if root is NULL then balancing is not needed
						{
							root=tree.balancing(root);
						}
					}
				}

				/*
				 * when command is TITLE
				 */

				else if (comm=="TITLE")
				{
					string a=line.substr(6);
					string title=a.substr(0,a.size()-1);
					log<<"SEARCHING for a SONG:\n";
					log<<"Searching for "<<title<<endl;
					if (root==NULL)
					{
						log<<"The tree is empty.\n\n";
					}
					else
					{
						int count=tree.searchtitle(log,root,title); 
						cout<<endl<<endl;
						if (count==0)
							{log<<"No data on "<<title<<" is present in the tree.\n\n";}
						
					}
				}

				/*
				 * when command is DISPLAY
				 */

				else if (comm=="DISPLAY")
				{
					log<<"DISPLAY:\n";
					if (root==NULL)
					{
						log<<"The tree is empty.\n";
					}
					else
					{
						tree.display(log,root,1);		//log is sent as an argument to write from functions to the log file		
						log<<"\nBalancing factor="<<tree.balancefactor(root)<<endl<<endl<<endl;
					}
				}

				/*
				 * when command is EXIT
				 */

				else if (comm=="EXIT")
				{
					log<<"EXIT:\n";
					log<<"You have successfully exited the program.\n\n";
					goto r;
				}				
			}			
		}	
		commandfile.close();
		cout<<"Command file successfully executed.\n\nCheck the log file for output.\n\n";
	}
	else
	{
		cout<<"Command file not found. Unable to open file.\n\n";
		goto w;
	}
	log.close();//to close the log file
	logfile.close();
	
	system ("pause");
	r:;
}
