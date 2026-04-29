#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <stdexcept>
using namespace std;
/*
class stufee{
    int rno;
    string name;
    bool f1;
    bool f2;
    double fine1;
    double fine2;
    public:
    stufee(){
        rno=0;
        name="";
        f1=true;
        f2=true;
        fine1=0;
        fine2=0;
    }
    void getdetails(int r){
    ifstream fin("fees.txt");
    if(!fin){
        cout << "File not found";
        return;
    }

    int rn;
    string n;
    int f11, f12;   // read as int for validation
    double f21, f22;
    int flag=0;
    while(fin >> rn >> n >> f11 >> f12 >> f21 >> f22){
        try{
            // manual validation
            if(!(f11 == 0 || f11 == 1) || !(f12 == 0 || f12 == 1)){
                throw 1;   // simple throw (no object)
            }
            

            if(rn == r){
                cout << "Student: " << n << endl;

                if(f11 == 1)
                    cout << "Fees paid for semester 1\n";
                else
                    cout << "Fees not paid for semester 1\n";

                if(f12 == 1)
                    cout << "Fees paid for semester 2\n";
                else
                    cout << "Fees not paid for semester 2\n";
                flag=1;
                break;
            }

        }
        catch(...){   // catches anything
            cout << "Invalid data in file\n";
        }
    }
    if(flag==0)cout<<"Roll number not found";

    fin.close();
}
    void records(){
        vector<int> s1;
        vector<int>s2;
        vector<int>s3;
        int rn;
        string n;
        bool f11,f12;
        double f21,f22;
          ifstream fin("fees.txt");
          if(!fin)return;
        while(fin>>rn>>n>>f11>>f12>>f21>>f22){
             try{
            // manual validation
            if(!(f11 == 0 || f11 == 1) || !(f12 == 0 || f12 == 1)){
                throw 1;   
            }
                if(f11==false)s1.push_back(rn);
                if(f12==false)s2.push_back(rn);
                if(f11==false && f12==false)s3.push_back(rn);
        }
        catch(...){   
            cout << "Invalid data in file\n";
        }
    }
    
       cout << "Semester 1 unpaid: ";
        for (int x : s1) cout << x << " ";
        cout << endl;

        cout << "Semester 2 unpaid: ";
        for (int x : s2) cout << x << " ";
        cout << endl;

        cout << "Both unpaid: ";
        for (int x : s3) cout << x << " ";
        cout << endl;

        fin.close();

    }
    void addFine() {
    ifstream fin("fees.txt");
    ofstream fout("temp.txt");   // write updated data here

    if(!fin || !fout){
        cout << "File error\n";
        return;
    }

    int rn;
    string n;
    int f1, f2;
    double fine1, fine2;

    while(fin >> rn >> n >> f1 >> f2 >> fine1 >> fine2){
        try{
        // validate boolean values
        if(!(f1 == 0 || f1 == 1) || !(f2 == 0 || f2 == 1)){
           throw 1;
        }

        // apply fine
        if(f1 == 0)   // not paid semester 1
            fine1 += 1000;

        if(f2 == 0)   // not paid semester 2
            fine2 += 1000;

        // write updated record
        fout << rn << " " << n << " " << f1 << " " << f2 << " "
             << fine1 << " " << fine2 << endl;
    }
             catch(...){
                cout<<"Invalid data in file";
             }
    }

    fin.close();
    fout.close();
    cout << "Fines updated successfully\n";
}
};
int main(){
    //stufee s;
   // s.getdetails(3);
   // s.records();
   // s.addFine();

    return 0;
}
class room{
    protected:
   
   int days,nights;
    public:
    virtual double calcrates(int d,int n)=0;
    virtual double roomser(int x)=0;
    friend class reservations;

};
class standardroom : public room{
    bool ac;
    public: 
    double calcrates(int d,int n){
        double total=0;
        cout<<"Ac/ NON ac";
        cin>>ac;
        if(ac==0)total+=d*500+n*1000;
        else total+=d*1000+n*2000;
       // cout<<"Your total fare will be "<<total;
         return total;

    }
    double roomser(int x){
        return x*500;

    }

};
class luxuryroom : public room{
     bool beachview;
    public: 
    double calcrates(int d,int n){
        double total=0;
        cout<<"BeachView?";
        cin>>beachview;
        if(beachview==0)total+=d*1000+n*3000;
        else total+=d*2000+n*3000;
      //  cout<<"Your total fare will be "<<total;
      return total;


}
 double roomser(int x){
        return x*1000;

    }
};
class hotelmanager{
    vector<vector<int>> data;   
    public:
    void addService(int roomId){
        // check if room already exists
        for(int i = 0; i < data.size(); i++){
            if(data[i][0] == roomId){
                data[i][1]++;   // increment count
                cout << "Service added. Total: " << data[i][1] << endl;
                return;
            }
        }

        // if room not found → add new entry
        data.push_back({roomId, 1});
        cout << "First service added for room " << roomId << endl;
    }
    int getServiceCount(int roomId){
    for(auto &row : data){
        if(row[0] == roomId){
            return row[1];
        }
    }
    return 0;  // if no services
}
    void checkout(int roomId){
    ifstream fin("hotem.txt");

    if(!fin){
        cout << "File error\n";
        return;
    }

    int i, d, n, rserv;
    int ci, p, t;   // use int instead of bool
    double c;
    string name;

    bool found = false;

    standardroom sdr;
    luxuryroom lur;

    while(fin >> i >> name >> ci >> d >> n >> rserv >> c >> p >> t){
        
        if(i == roomId){
            found = true;

          int serviceCount = getServiceCount(roomId);

            double roomCharge;

            if(t == 0)
                roomCharge = sdr.roomser(serviceCount);
            else
                roomCharge = lur.roomser(serviceCount);

            c+=roomCharge;

            cout << "\n--- BILL ---\n";
            cout << "Room ID: " << roomId << endl;
            cout << "Name: " << name << endl;
            cout << "Room Charges: " << roomCharge << endl;
            
            cout << "Total Bill: " << c << endl;

            break;
        }
    }

    if(!found){
        cout << "Room ID not found\n";
    }

    fin.close();
}


};
class reservations{
    int id;
    string name;
    bool checkin;
    int days,nights;
    int roomserv;
    double charges;
    bool paid;
    bool type;
    public:
    void makereserv(){
        ifstream fin("hotem.txt");
        if(!fin )return;
       
        
        int i,d,n,rs;
        bool ci,p,t;
        double c;
        string nam;
        int id1=0;
        while(fin>>i>>nam>>ci>>d>>n>>rs>>c>>p>>t){
            id1=i;
        }
        id1+=1;
       cout<<id1;
        fin.close();
       
    // 🔹 open in append mode ONLY
    ofstream fout("hotem.txt", ios::app);

    if(!fout){
        cout << "File error\n";
        return;
    }

    int nd=0, nn=0, nrs = 0;
    bool nci, np = 0, type;
    double nc = 0;
    string nnam;

    cout << "Enter name: ";
    cin >> nnam;

    cout << "Enter checkin (0/1): ";
    cin >> nci;

    cout << "Enter days and nights: ";
    cin >> nd >> nn;
    standardroom sdr;
    luxuryroom lur;
    cout << "Enter type: ";
    cin >> type;
     if(type==0)nc=sdr.calcrates(nd,nn);
    else nc= lur.calcrates(nd,nn);

  

    // 🔹 append at end
    fout << id1 << " " << nnam << " " << nci << " "
         << nd << " " << nn << " " << nrs << " "
         << nc << " " << np << " " << type << endl;

    fout.close();

    cout << "Reservation appended successfully\n";
}



 };*/

int main(){
     
   
   // reservations rev;
   // rev.makereserv();
   //// hotelmanager hm;
   // hm.checkout(3);
    ifstream fin("hotem.txt");

    int i, d, n, rs;
    int ci, p, t;
    double c;
    string name;

    while(true){
        try{
            // check each extraction
            if(!(fin >> i)) throw 1;       // int check
            if(!(fin >> name)) throw 1;    // string check
            if(!(fin >> ci)) throw 1;
            if(!(fin >> d)) throw 1;
            if(!(fin >> n)) throw 1;
            if(!(fin >> rs)) throw 1;
            if(!(fin >> c)) throw 1;       // double check
            if(!(fin >> p)) throw 1;
            if(!(fin >> t)) throw 1;

            // logical validation
            if(!(ci==0 || ci==1)) throw 1;
            if(!(p==0 || p==1)) throw 1;
            if(!(t==0 || t==1)) throw 1;

            cout << "Valid record: " << i << " " << name << endl;
        }
        catch(...){
            if(fin.eof()) break;

            cout << "Invalid data found, skipping...\n";

            fin.clear(); // reset error
            fin.ignore(1000, '\n'); // skip bad line
        }
    }

    fin.close();

    return 0;
}