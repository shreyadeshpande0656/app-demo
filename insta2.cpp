#include<iostream>
#include<fstream>
#include<string>
#include<bits/stdc++.h>
using namespace std;
#define max 20

class User{
    public:
    int userid;
    string username;
    string password_hash;
    unordered_set<int> following;
    vector<int> posts;
    unordered_map<string, double> interests;
    friend class graph;
 `  
  
};
class Post{
    public:
    int post_id;
    int user_id; 
    string image_path;
    string tags;
    vector<int> liked_by;
    int timestamp; 
   
};
class gnode {
    int vid;
    gnode *next;
    friend class graph;
};
class graph{
     public:
    gnode *head[max];
    
   
    graph(){
    for (int i = 0; i < max; i++) {
            head[i] = NULL;
        }
    }
    void create_adjlist(int x,unordered_set<int> y){
    gnode* temp = NULL;
    for(auto i : y){
        gnode *curr=new gnode;
        curr->vid=i;
        curr->next=NULL;
        if (head[x] == NULL) {
                head[x] = curr;
                temp = curr;
            } else {
                temp->next = curr;
                temp = curr;
            }
        
    }
    

}

void display() {
        for (int i = 0; i < max; i++) {
            if (head[i] != NULL) {
                cout << i << " -> ";
                gnode* temp = head[i];
                while (temp != NULL) {
                    cout << temp->vid << " ";
                    temp = temp->next;
                }
                cout << endl;
            }
        }
    
}

void frdsugg(int u) {
    int visited[max];
    for(int i=0;i<max;i++){
        visited[i]=0;
    }
    unordered_set<int> frds;
    queue<int> q;
    visited[u] = 1;

    
    gnode* temp = head[u];
    while (temp != NULL) {
        int f = temp->vid;
        q.push(f);
        visited[f]=1;
         temp = temp->next;
    }

    // BFS (friends of friends)
    int s=q.size();
    while (!q.empty()) {
        int curr = q.front();
        q.pop();
        gnode* t = head[curr];

        while (t != NULL) {
            int fof = t->vid;

            if (!visited[fof]) {
                frds.insert(fof); 
                visited[fof]=1;    // mutual friend count
            }

            t = t->next;
        }
    }
   cout << "Suggestions for " << u << ":\n";
    for (auto i : frds) {
        cout << i <<" ";
    }
}
};


vector<int> stringToVector(string s) {
    vector<int> v;
    stringstream ss(s);
    string temp;

    while(getline(ss, temp, ',')) {
        if(!temp.empty())
            v.push_back(stoi(temp));
    }
    return v;
}


unordered_set<int> stringToSet(string s) {
    unordered_set<int> st;
    stringstream ss(s);
    string temp;

    while(getline(ss, temp, ',')) {
        if(!temp.empty())
            st.insert(stoi(temp));
    }
    return st;
}
/*
void makegraph(graph &g){
   ifstream fin("userdetails2.txt");

    if (!fin) {
        cout << "Error opening users.txt\n";
        return;
    }

    
    string id,uname, pass, following, posts, interests;

    while (fin >> id >> uname >> pass >> following >> posts >> interests) {
         int id1;
        try {
            id1 = stoi(id);
        } catch (const std::invalid_argument &e) {
            cerr << "Warning: invalid user ID '" << id_str << "' in line: " << line << endl;
            continue;
        }
        vector f1-stringToVector(following);
        g.create_adjlist(id1,f1);


    }


}*/
void fetch_user(string x, User &u) {
    ifstream fin("userdetails2.txt");

    if (!fin) {
        cout << "Error opening users.txt\n";
        return;
    }

    int id;
    string uname, pass, following, posts, interests;

    while (fin >> id >> uname >> pass >> following >> posts >> interests) {

        if (id.compare(x)) {
            u.userid = id;
            u.username = uname;
            u.password_hash = pass;
            u.following = stringToSet(following);
            u.posts = stringToVectorInt(posts);
             u.interests = stringToMap(interests);
           cout << "UserID: " << u.userid << "\n";
            cout << "Username: " << u.username << "\n";
            cout << "Password: " << u.password_hash << "\n";

            cout << "Following: ";
            
            for (auto f : u.following) cout << f << " ";
            cout << "\n";

            cout << "Posts: ";
            for (auto p : u.posts) cout << p << " ";
            cout << "\n";

            return;
            
        }
    }

    cout << "User not found\n";
}
/*
void fetch_post(string x,Post &p) {
    ifstream fin("postdetails2.txt");

    if (!fin) {
        cout << "Error opening posts.txt\n";
        return;
    }

    int pid, uid, ts;
    string image_path, tags, liked_by;

    while (pid >> uid >> image_path >> tag >> likedby >> ts) {

        if (pid.equals(x)) {
            p.post_id=pid;
            p.user_id=uid;
            p.url=image_path;
            p.tags=tag;
            p.liked_by=stringToVector(likedby);
            p.timestamp=stoi(ts);

            return;
        }
    }
    

    cout << "Post not found\n";
}

double calculateScore(User &u, Post &p) {
    double score = 0;

    // 1. Interest match
    if (u.interests.find(p.tags) != u.interests.end()) {
        score += u.interests[p.tags] * 10;  // weight
    }

    // 2. Following boost
    if (u.following.count(p.user_id)) {
        score += 20;
    }

    // 3. Popularity (likes count)
    score += p.liked_by.size() * 2;

    // 4. Recency (higher timestamp = newer)
    score += p.timestamp * 0.01;

    return score;
}

class feed{
            public:
            void game(){
                char ch='y';
                char d;

                while(ch=='y'){
                    cout<<"Do you like the post";
                    cin>>d;
                    if(d=='y'){
                        post.liked_by.push_back(user_id);
                        user.interests+=10;
                    }




                    cout<<"Do you want to see more posts";
                    cin>>ch;


                }
                
            }
            void ranking(unordered_map<int, Post>& posts, User &u) {
            vector<vecnode> v;

            for (auto &p : posts) {
                Post &post = p.second;
                int score = 0;

                
                if (u.interests.find(post.tags) != u.interests.end()) {
                    score += u.interests[post.tags] * 10;
                }

                
                if (u.following.find(post.user_id) != u.following.end()) {
                    score += 15;
                }

                
                if (post.liked_by.size() > 4) {   // threshold (you can change)
                    score += 10;
                }

                
                int current_time = 1100;  // assume current time
                if (current_time - post.timestamp < 10) {
                    score += 5;
                }

                if (score > 0) {
                    v.push_back({score, post.post_id});
                }
            }

            int s=v.size();
            if (s == 0) return;
            quicksort(v, 0, s - 1);
           
            int k = min(5, s);

        for (int i = 0; i < k; i++) {
            cout << v[i].post_id << " ";
        }

        cout << endl;
        }
    
    };


*/
int main(){
    graph g;
   // feed f;
      
     User u;
     Post p;
     string z;
    cout << "Enter the user to find friend suggestions for: ";
    cin >> z;
     
    fetch_user(u,z);
    //fetch_post(posts);
    
    //g.display();
    
    
    


return 0;
    
}
 


