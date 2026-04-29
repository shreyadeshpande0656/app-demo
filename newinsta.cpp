#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<queue>
#include<unordered_map>
#include<vector>
#include<algorithm>
using namespace std;
#define max1 20
class User{
    public:
    int userid;
    string username;
    string password_hash;
    vector<int> following;
    vector<int> posts;
    unordered_map<string, double> interests;
};
class Post{
    public:
    int post_id;
    int user_id; 
    string image_path;
    string tags;
    vector<int> liked_by;
    int timestamp; };
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

unordered_map<string, double> stringToMap(const string &s) {
    unordered_map<string, double> mp;

    stringstream ss(s);
    string pair;

    while (getline(ss, pair, ',')) {
        int pos = pair.find(':');

        if (pos == string::npos) continue; // safety check

        string key = pair.substr(0, pos);
        double value = stod(pair.substr(pos + 1));

        mp[key] = value;
    }

    return mp;
}

void fetch_user(string x, User &u) {
    ifstream fin("userdetails2.txt");

    if (!fin) {
        cout << "Error opening users.txt\n";
        return;
    }
    string id,uname, pass, f, p, intr;
    while (fin >> id >> uname >> pass >> f >> p >> intr) {
        if (id==x) {
            u.userid = stoi(id);
            u.username = uname;
            u.password_hash = pass;
            u.following = stringToVector(f);
            u.posts = stringToVector(p);
             u.interests = stringToMap(intr);
             return;
           
        }
    }

    cout << "User not found\n";
}

void fetch_post(string x,Post &p) {
    ifstream fin("postdetails2.txt");

    if (!fin) {
        cout << "Error opening posts.txt\n";
        return;
    }
   
    string pid,uid,url, tag, likedby,ts;
    while (fin>>pid >> uid >> url >> tag >> likedby >> ts) {

        if (pid==x) {
            p.post_id=stoi(pid);
            p.user_id=stoi(uid);
            p.image_path=url;
            p.tags=tag;
            p.liked_by=stringToVector(likedby);
            p.timestamp=stoi(ts);
            //cout<<pid<<" "<<uid<<" "<<url<<" "<<tag<<endl;

            return;
        }
    }
    

    cout << "Post not found\n";
}

class gnode {
    int vid;
    gnode *next;
    friend class graph;
};
class graph{
     public:
    gnode *head[max1];
       graph(){
    for (int i = 0; i < max1; i++) {
            head[i] = NULL;
        }
    }
    void create_adjlist(int x,vector<int> y){
    gnode* temp = NULL;
    for(int i : y){
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
        for (int i = 0; i < max1; i++) {
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
    int visited[max1];
    for(int i=0;i<max1;i++){
        visited[i]=0;
    }
    vector<int> frds;
    queue<int> q;
    visited[u] = 1;
    gnode* temp = head[u];
    while (temp != NULL) {
        int f = temp->vid;
        q.push(f);
        visited[f]=1;
         temp = temp->next;
    }
    int s=q.size();
    while (!q.empty()) {
        int curr = q.front();
        q.pop();
        gnode* t = head[curr];
        while (t != NULL) {
            int fof = t->vid;

            if (!visited[fof]) {
                frds.push_back(fof); 
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

void makegraph(graph &g){
   ifstream fin("userdetails2.txt");

    if (!fin) {
        cout << "Error opening users.txt\n";
        return;
    }

    
    string id,uname, pass, f, p, inst;

    while (fin >> id >> uname >> pass >> f >> p >> inst) {
         int id1;
        try {
            id1 = stoi(id);
        } catch (const std::invalid_argument &e) {
            cerr << "Warning: invalid user ID '" << id << "'"<< endl;
            continue;
        }
        vector<int> f1=stringToVector(f);
        g.create_adjlist(id1,f1);
         }

g.display();
}
int ranking(User &u,string y,priority_queue<pair<int,int>> &pq){
    int score=0;
    Post post;
    fetch_post(y,post);
    if(post.user_id==u.userid)return 0;
    if (u.interests.find(post.tags) != u.interests.end()) {
                    score += u.interests[post.tags] * 10;
      }
 if (find(u.following.begin(), u.following.end(), post.user_id) 
    != u.following.end()) {
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
         pq.push({score, post.post_id});
                }
        //cout<<y<<" "<<score<<endl;
                return score;
}
void updatepq(string y,User &u,priority_queue<pair<int,int>>& pq){
    fetch_user(y,u);
    for(int i = 100; i < 115; i++) {
    string h = to_string(i);
    ranking(u, h, pq);
}
}

void greedy(priority_queue<pair<int,int>> &pq) {
    char g = 'y';
    while (g == 'y' && !pq.empty()) {
        int score = pq.top().first;
        int pid = pq.top().second;
         pq.pop();
        Post p;
        fetch_post(to_string(pid), p);
        cout << pid << "  " << p.user_id << "  "<< p.image_path << "  " << p.tags<< "  " << p.timestamp << endl;
         vector<pair<int,int>> temp;
        while (!pq.empty()) {
             int nscore = pq.top().first;
            int npid = pq.top().second;
             pq.pop();
            Post p2;
            fetch_post(to_string(npid), p2);
         
            if (p.tags == p2.tags) {
                nscore -= 10;
            }
            if (p.user_id == p2.user_id) {
                nscore -= 10;
            }
            temp.push_back({nscore, npid});
        }
        for (auto &x : temp) { pq.push(x); }  
        cin >> g;
    }
}




bool isValid(vector<int> &curr, Post &p) {
    int c1 = 0,c2=0;

    for (auto id : curr) {
        Post temp;
        fetch_post(to_string(id), temp);

        if (temp.tags == p.tags) c1++;
        if(temp.user_id==p.user_id)c1++;
    }

    return (c1<2 && c2<1);  
}

void backtrack(vector<pair<int,int>> &posts, vector<int> &curr, int index, int curr_score,vector<int> &best_feed,int &best_score) {
        if (curr.size() == 5) {
        if (curr_score > best_score) {
            best_score = curr_score;
            best_feed = curr;
        }
        return;
    }

    for (int i = index; i < posts.size(); i++) {
       int post_id = posts[i].second;
        int score = posts[i].first;
    
        Post p;
            fetch_post(to_string(post_id), p);

            if (!isValid(curr, p)) continue;
             curr.push_back(post_id);
            backtrack(posts, curr, i + 1, curr_score + score,best_feed,best_score);
            curr.pop_back();
    }
}
void backtracking(priority_queue<pair<int,int>> &pq){
     vector<pair<int,int>> posts;
            while (!pq.empty()) {
                posts.push_back(pq.top());
                pq.pop();
            }
            vector<int> best_feed;
            int best_score = 0;
            vector<int> curr;
            int curr_score;
                backtrack(posts, curr, 0, 0,best_feed,best_score);
                char g='y';
                Post p;

                while(g=='y' && !best_feed.empty()){
                    int h = best_feed.back();
                    best_feed.pop_back();

                    fetch_post(to_string(h),p);
                    cout<<p.post_id<<"  "<<p.user_id<<"  "<<p.image_path<<"  "<<p.tags<<"  "<<p.timestamp<<endl;
                    cin>>g;
                }
                

}
void dynamicprog( vector<pair<int,int>> &posts){
    int n = posts.size();
    vector<vector<int>> dp(n+1, vector<int>(6, 0));
    for(int i=0;i<=n;i++){
        dp[i][0]=0;

    }
    for(int k=0;k<=5;k++){
        dp[0][k]=0;
    }
    for (int i = 1; i <= n; i++) {
    int score = posts[i-1].first;

    for ( int k = 1; k <= 5; k++) {

        
        dp[i][k] = dp[i-1][k];

        dp[i][k] =std:: max(dp[i][k],
                       dp[i-1][k-1] + score);
    }
}
        vector<int> selected;
        int i = n, k = 5;
        while (i > 0 && k > 0) {
            if (dp[i][k] != dp[i-1][k]) {
                selected.push_back(posts[i-1].second);
                k--;
            }
            i--;
        }
        char g='y';
        while(g=='y'&& !selected.empty()){
            Post p;
            int h = selected.back();  
            selected.pop_back();       

            fetch_post(to_string(h),p);
            cout<<p.post_id<<"  "<<p.user_id<<"  "<<p.image_path<<"  "<<p.tags<<"  "<<p.timestamp<<endl;
                    cin>>g;
        }
    

}
void dynp(priority_queue<pair<int,int>> &pq){
    vector<pair<int,int>> posts; 
    while(!pq.empty()){
        int s=pq.top().first;
        int ide=pq.top().second;
        posts.push_back({s, ide});
        pq.pop();
    }
    dynamicprog(posts);
    
}


int main(){
    priority_queue<pair<int,int>> pq;
    User u;
    Post p;
    graph g;
    string d;
    cout<<"Enter User id";
    cin>>d;
    updatepq(d,u,pq);
    int w=0;
    while(w!=4){
        cout<<"Enter 1 for greedy optimization, 2 for backtracking to select top 5 posts, 3 for DP and 4 to exit "<<endl;
        cin>>w;
        if(w==1)greedy(pq);
        else if(w==2)backtracking(pq);
        else if(w==3)dynp(pq);
        else if(w==4)cout<<"Exiting";
        else cout<<"wrong input";

    }
    //fetch_user(h,u);
    //fetch_post(z,p);
    //makegraph(g);
   // g.frdsugg(5);
   
    return 0;

}
