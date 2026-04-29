#include<iostream>
#include<fstream>
#include<string>
#include<cmath>
#include<algorithm>
using namespace std;

class student{
    int rno,marks;
    string name;
    friend class records;
};
class hashing{
    int rno,pos;
    public:
        hashing(){
            rno=-1;
            pos=-1;
        }
        friend class records;
};
class records{
    student stud;
    hashing h[10];
    int s,relt;
    public:
     records();
    void create_wor();
     void create_wr();
     void revival();
     void modify();
     void display();

};
records::records(){
    s=sizeof(stud);
    relt=0;
    for(int i=0;i<10;i++){
        h[i].rno=-1;
        h[i].pos=-1;
    }
}
void records::create_wor(){
    char c;
    fstream fin("students.txt",ios::binary|ios::app|ios::out);
    int loc;
    do{
        cout<<"Enter roll number";
        cin>>stud.rno;
        cout<<"Enter name";
        cin>>stud.name;
        cout<<"Enter marks";
        cin>>stud.marks;
         loc=stud.rno%10;
        if(h[loc].rno==-1){
            h[loc].rno=stud.rno;
            h[loc].pos=relt;
        }
        else{
            for(int i=0;i<10;i++){
                int newloc=(loc+i)%10;
                if(h[newloc].rno==-1){
                    h[newloc].rno=stud.rno;
                    h[newloc].pos=relt;
                    break;
                }
            }
            }
              fin.write((char*)&stud,s);
            relt++;

      
         cout<<"Enter y to add more data";
        cin>>c;
    }while(c=='y');
    fin.close();
    

}
void records::create_wr(){
    char c;
    fstream fin("students.txt",ios::app|ios::binary|ios::out);
    if(!fin){
    cout<<"File opening failed!";
    return;
}
    int loc;
    do{
        cout<<"Enter student rno";
        cin>>stud.rno;
        cout<<"Enter student name";
        cin>>stud.name;
        cout<<"Enter student marks";
        cin>>stud.marks;
        loc=stud.rno%10;
        if(h[loc].rno==-1){
            h[loc].rno=stud.rno;
            h[loc].pos=relt;
        }
        else{
            int oo=h[loc].rno%10;
            if(oo==loc){
                for(int i=1;i<10;i++){
                    int newloc=(loc+i)%10;
                    if(h[newloc].rno==-1){
                        h[newloc].rno=stud.rno;
                        h[newloc].pos=relt;
                        break;
                    }
                }
            }
            else{
                int trno=h[loc].rno;
                int tpos=h[loc].pos;
                h[loc].rno=stud.rno;
                h[loc].pos=relt;
                for(int i=1;i<10;i++){
                    int newloc=(loc+i)%10;
                    if(h[newloc].rno==-1){
                        h[newloc].rno=trno;
                        h[newloc].pos=tpos;
                        break;
                    }
                }

            }
        }
        fin.write((char*)&stud,s);
        relt++;
        cout<<"Enter y to continue";
        cin>>c;
    }while(c=='y');
    fin.close();
}
void records::revival(){
    int roll;
    fstream fin("students.txt",ios::in| ios::binary);
    if(!fin){
        cout<<"file not found";
        return;
    }
    cout<<"Enter roll number to be found";
    cin>>roll;
    bool found=false;
    student temp;
    while(fin.read((char*)&temp,s)){
    if(temp.rno==roll){
        cout<<temp.rno<<" "<<temp.name<<" "<<temp.marks;
        found=true;
        break;

    }
}

    if(found==false)cout<<"Record not found";
    fin.close();
}
void records:: modify(){
    fstream fin("students.txt",ios::binary|ios::in);
    if(!fin)return;
    student temp;
    int r;
    cout<<"Enter roll number";
    cin>>r;
    while(fin.read((char*)&temp,s)){
        if(temp.rno==r){
            cout<<"Enter rno";
            cin>>temp.rno;
            cout<<"Enter name";
            cin>>temp.name;
            cout<<"Enter marks";
            cin>>temp.marks;
            fin.seekp(-s,ios::cur);
            fin.write((char*)&temp,s);
            cout<<"Successful";
            break;

        }
    }
    fin.close();
}
void records::display(){
    fstream fin("students.txt",ios::in|ios::binary);
    student temp;
    while(fin.read((char*)&temp,s)){
        cout<<temp.rno<<" "<<temp.name<<" "<<temp.marks<<endl;
    }
    fin.close();

    
}
class node{
    string name,meaning;
    node* left;
    node* right;
    public:friend class avltree;
};
class avltree{
    node *root;
    public:
    avltree(){
        root=NULL;
    }
    public:
    int height(node *temp){
        int h=0;
        if(temp!=NULL){
            int lh=height(temp->left);
            int rh=height(temp->right);
            int mh=std::max(lh,rh);
            h=mh+1;

        }
        return h;
    }
    int balancefac(node *temp){
        if(temp==NULL)return 0;
        int lh=height(temp->left);
        int rh=height(temp->right);
        int bf=lh-rh;
        return bf;
    }
    node* ll(node *a){
        node* b=a->left;
        a->left=b->right;
        b->right=a;
        return b;
    }
    node* rr(node *a){
        node* b=a->right;
        a->right=b->left;
        b->left=a;
        return b;
    }
    node* rl(node *a){
        node* b=a->right;
        node* c=b->left;
        a->right=c->left;
        b->left=c->right;
        c->left=a;
        c->right=b;
        return c;

    }
    node* lr(node* a){
        node* b=a->left;
        node* c=b->right;
        b->right=c->left;
        a->left=c->right;
        c->left=b;
        c->right=a;
        return c;
    }
    node* balancetree(node* temp){
        int bf=balancefac(temp);
        if(bf>1){
            if(balancefac(temp->left)>0)
            temp=ll(temp);
            else 
            temp=lr(temp);
        }
        if(bf<-1){
            if(balancefac(temp->right)<0)
            temp=rr(temp);
            else
            temp= rl(temp);
        }
        return temp;

    }
    node* insert(node* root,node* temp){
        if(root==NULL){
            root=new node;
            root->name=temp->name;
            root->meaning=temp->meaning;
            root->left=NULL;
            root->right=NULL;
            return root;
        }
        if(temp->name<root->name){
            root->left=insert(root->left,temp);
           root=balancetree(root);
        }
        else if(temp->name>=root->name){
            root->right=insert(root->right,temp);
            root=balancetree(root);
        }
        return root;


    }
    void insert(){
        char c;
        do{
            node* temp=new node;
            cout<<"Enter name and meaning";
            cout<<"Enter name: ";
            cin >> temp->name;

            cout<<"Enter meaning: ";
            cin.ignore();
            getline(cin, temp->meaning);
            temp->left=NULL;
            temp->right=NULL;
            root=insert(root,temp);
            cout<<"Do you want to enter more";
            cin>>c;

        }while(c=='y');
    }
    void display(node* ptr,int level){
        int mlev=height(root);
        if(ptr!=NULL){

        
        display(ptr->right,level+1);
        for(int i=0;i<mlev && ptr!=root;i++){
            cout<<"  ";
        }
        cout<<root->name;
        display(ptr->right,level+1);
    }
    }


};
class queue{
    int q[50],front,rear;
    public:
    queue(){front=-1,rear=-1;}
    void push(int x){
        if(front==rear==-1)front=rear=0;
        else rear++;
        q[rear]=x;
    }
    int pop(){
        int temp=q[front];
        if(front==rear)front=rear=-1;
        else front++;
        return temp;

    }
    bool isempty(){return front==-1;}
};
class stack{
   int stk[50];
    int top;
    public:
    stack(){top=-1;}
    void push(int x){stk[++top]=x;}
    int pop(){return stk[top--];}
    bool isempty(){return top==-1;}


};
class gnode{
    int uid;
    string name;
    gnode *next;
    friend class graph;
};
class graph{
   gnode* head[20];
   int visited[20];
   int n;
   public:
   graph(){
    int no;
    string nh;
    cout<<"Enter number of nodes";
    cin>>no;
    n=no;
    for(int i=0;i<no;i++){
    cout<<"Enter name";
    cin>>nh;
    head[i]=new gnode();
    head[i]->uid=i;
    head[i]->name=nh;
    head[i]->next=NULL;
    }

   }
   void create(){
    for(int i=0;i<n;i++){
        cout<<"Friend list of "<<i;
        gnode* temp=head[i];
        char c;
        int t;
        do{
           
            if(t<0 || t>n || t==i){
                cout<<"Invalid node";
                continue;
            }
             cin>>t;
            gnode *curr=new gnode();
            curr->uid=t;
            curr->name=head[t]->name;
            curr->next=NULL;
            temp->next=curr;
            temp=curr;
            cout<<"Do you want to enter more nodes";
            cin>>c;
            

        }while(c=='y');
    }
   }
   void dfsr(int v){
    cout<<v<<ends;
    visited[v]=1;
    gnode* temp=head[v]->next;
    while(temp!=NULL){
        int w=temp->uid;
        if(visited[w]==0){
            dfsr(w);


        }
        temp=temp->next;
    }
   }
   void dfs(){
    int v;
    cout<<"Enter v";
    cin>>v;
   
    dfsr(v);
   }

};
int main(){
    records r;
    //r.create_wr();
    //r.revival();
    //r.modify();
    //r.display();
    avltree t;
   // node* root=new node;
    //t.insert();
    //t.display(root,0);
    graph g;
    g.create();
    g.dfs();

}