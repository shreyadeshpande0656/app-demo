#include<iostream>
#include<fstream>
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

void fetch_user(unordered_map<int, User> &users, graph &g) { 
    string line;
    ifstream fin("userdetails.txt");
    if (!fin) {
        cout << "Error opening file\n";
        return;
    }

    while (getline(fin, line)) {
        if (line.empty()) continue; 
        stringstream ss(line);
        string id_str, uname, pswd, f1, p1, intr;

        getline(ss, id_str, '|');
        getline(ss, uname, '|');
        getline(ss, pswd, '|');
        getline(ss, f1, '|');
        getline(ss, p1, '|');
        getline(ss, intr, '|');

        
        if (id_str.empty()) {
            cerr << "Warning: empty user ID in line: " << line << endl;
            continue;
        }

        int id1;
        try {
            id1 = stoi(id_str);
        } catch (const std::invalid_argument &e) {
            cerr << "Warning: invalid user ID '" << id_str << "' in line: " << line << endl;
            continue;
        }
      
        unordered_set<int> f2;
        if (!f1.empty()) f2 = stringToSet(f1); 
       
        g.create_adjlist(id1, f2);

        
        User u;
        u.userid = id1;
        u.username = uname;
        u.password_hash = pswd;
        u.posts =stringToVector(p1);
        users[u.userid] = u;
    }

    fin.close();
}

void fetch_post(unordered_map<int, Post> &posts) {
    string line;
    ifstream fin("posts.txt");
    if (!fin) {
        cout << "Error opening posts.txt\n";
        return;
    }

    while (getline(fin, line)) {
        if (line.empty()) continue; // skip empty lines

        stringstream ss(line);
        string pid_str, uid_str, url, tag, lb, ts_str;

        getline(ss, pid_str, '|');
        getline(ss, uid_str, '|');
        getline(ss, url, '|');
        getline(ss, tag, '|');
        getline(ss, lb, '|');
        getline(ss, ts_str, '|');

        // Skip invalid lines
        if (pid_str.empty() || uid_str.empty() || ts_str.empty()) {
            cerr << "Warning: invalid post line: " << line << endl;
            continue;
        }

        int pid, uid, ts;
        try {
            pid = stoi(pid_str);
            uid = stoi(uid_str);
            ts = stoi(ts_str);
        } catch (const std::invalid_argument &e) {
            cerr << "Warning: stoi failed for line: " << line << endl;
            continue;
        }

        Post p;
        p.post_id = pid;
        p.user_id = uid;
        p.image_path = url;
        p.tags = tag;
        p.liked_by = stringToVector(lb);
        p.timestamp = ts;

        posts[p.post_id] = p;
    }

    fin.close();
}
void update_intr(unordered_map<int, Post>& posts, User &u1) {

    for (auto &p : posts) {
        Post &p1 = p.second;

        // checking if user liked this post
        for (int uid : p1.liked_by) {
            if (uid == u1.userid) {
                u1.interests[p1.tags] += 1;
               break;
            }
        }
    }
    
    for(auto &a:u1.interests){
        cout<<a.first<<" "<<a.second<<endl;
    }
        
}

 class vecnode{
    public:
        int score;
        int post_id;
        vecnode(int s, int id) : score(s), post_id(id) {}
        friend class feed;
    };
class feed{
        
                   int partition(vector<vecnode> &v, int low, int high) {
                    int pivot = v[high].score;
                    int i = low - 1;

                    for (int j = low; j < high; j++) {
                        if (v[j].score >= pivot) {
                            i++;
                            swap(v[i], v[j]);
                        }
                    }

                    swap(v[i + 1], v[high]);
                    return i + 1;
                }

                void quicksort(vector<vecnode> &v, int low, int high) {
                    if (low < high) {
                        int pi = partition(v, low, high);
                        quicksort(v, low, pi - 1);
                        quicksort(v, pi + 1, high);
                    }
            }
            public:
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

                
                if (post.liked_by.size() > 3) {   // threshold (you can change)
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



int main(){
    graph g;
    feed f;
    unordered_map<int, User> users;   // userId → User
    unordered_map<int, Post> posts; 
    vector<vecnode> v;  // postId → Post
    fetch_user(users,g);
    fetch_post(posts);
    
    //g.display();
    
    
    int z;
cout << "Enter the user to find friend suggestions for: ";
cin >> z;


if (users.find(z) == users.end()) {
    cout << "User not found\n";
    return 0;   
}


User &u = users[z];

cout << "Friend Suggestions: ";
g.frdsugg(z);
cout << endl;
cout << "Updated interest values" << endl;
update_intr(posts, u);
f.ranking(posts, u);
return 0;
    
}
 


