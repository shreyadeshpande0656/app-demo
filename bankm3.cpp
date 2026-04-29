#include<iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <vector>
#include <cmath>
using namespace std;

class User {
public:
    string name;
    string bankacc;
    string password;
    string phoneno;
    string aadharno;
    int income;
    double balance;
    int creditscore;
    void createacc();
    void login();
    void display();
        
};
void login(string x, User &u) {
    try {
        ifstream fin("userinfo.txt");

        if (!fin) {
            throw runtime_error("Error opening file");
        }
       string uname, ubankacc, upassword, uphoneno, uaadharno;
        int uincome, ucreditscore;
        double ubalance;

        bool found = false;
        while (fin >> uname >> ubankacc >> upassword >> uphoneno
                   >> uaadharno >> uincome >> ubalance >> ucreditscore) {
            if (ubankacc == x) {
                found = true;
                string enteredPass;
                cout << "Enter password: ";
                cin >> enteredPass;
                int attempts = 0;
                while (enteredPass != upassword && attempts < 2) {
                    cout << "Wrong password! Try again: ";
                    cin >> enteredPass;
                    attempts++;
                }
                if (enteredPass != upassword) {
                    throw runtime_error("Maximum attempts reached. Login failed.");
                }
                cout << "Login Successful!\n";
                u.name = uname;
                u.bankacc = ubankacc;
                u.password = upassword;
                u.phoneno = uphoneno;
                u.aadharno = uaadharno;
                u.income = uincome;
                u.balance = ubalance;
                u.creditscore = ucreditscore;
                cout << "Hello " << uname << ", Welcome to Bharat Bank\n";

                return;
            }
        }

        if (!found) {
            throw runtime_error("Account not found");
        }
    }
    catch (runtime_error &e) {
        cout << "Login Error: " << e.what() << endl;
    }
}


class Loan {
protected:
    double amt, rate, time;

public:
    Loan(double a, double r, double t) {
        amt = a;
        rate = r;
        time = t;
    }
    virtual bool is_eligible(User &u) = 0;
    virtual double calculate_emi() = 0;
    virtual ~Loan() {}
};
class HomeLoan : public Loan {
public:
    HomeLoan(double a,double t) : Loan(a,8.5,t) {}
    bool is_eligible(User &u)  {
        return u.income > 100000 && u.creditscore > 600;
    }
    double calculate_emi()  {
        double r = rate / (12 * 100);
        double n = time * 12;
        return amt * r * pow(1+r,n) / (pow(1+r,n)-1);
    }
};
class CarLoan : public Loan {
public:
    CarLoan(double a,double t) : Loan(a,6,t) {}

    bool is_eligible(User &u)  {
        return u.income > 50000 && u.creditscore > 300;
    }

    double calculate_emi()  {
        double r = rate / (12 * 100);
        double n = time * 12;
        return amt * r * pow(1+r,n) / (pow(1+r,n)-1);
    }
};
class PersonalLoan : public Loan {
public:
    PersonalLoan(double a,double t) : Loan(a,7.5,t) {}
    bool is_eligible(User &u)  {
        return u.income > 75000 && u.creditscore > 500;
    }
    double calculate_emi()  {
        double r = rate / (12 * 100);
        double n = time * 12;

        return amt * r * pow(1+r,n) / (pow(1+r,n)-1);
    }
};
class Record{
    public:
    string loanid;
    string bankacc1;
    int flag;
    int score;
    double emi;
    int months;
    public:
    void startofmonth();
    };
void apply_loan(User &u) {
    int type;
    int time;
     double amt;
    
    cout << "Enter Loan Type (0-Home, 1-Personal, 2-Car): ";
    cin >> type;
    cout << "Enter Loan Amount: ";
    cin >> amt;
    cout << "Enter Loan Duration (months): ";
    cin >> time;
    
    Loan* loan = nullptr;
    if(type == 0)
        loan = new HomeLoan(amt,time);

    else if(type == 1)
        loan = new PersonalLoan(amt,time);

    else if(type == 2)
        loan = new CarLoan(amt,time);

    else {
        cout << "Invalid Loan Type\n";
        return;
    }
    if(!loan->is_eligible(u)) {
        cout << "You are not eligible for this loan.\n";
        delete loan;
        return;
    }
   cout << "Congratulagions!! You are eligible for Loan!\n";
   double emi=0;
    emi = loan->calculate_emi();
    cout<<"Your monthly emi will be "<<emi;
    ifstream fin("loandata.txt");
    string lastLoanId, bankacc;
    int flag, score,m;
    int maxId = 0;
    double e;

    while(fin >> lastLoanId >> bankacc >> flag >> score>>e>>m) {
        int num = stoi(lastLoanId); 
        maxId = max(maxId, num);
    }
    fin.close();
    string newLoanId = to_string(maxId + 1);

    // Append new loan record
    ofstream fout("loandata.txt", ios::app);
    fout  << "\n" 
        << newLoanId << " "
         << u.bankacc << " "
         << 0 << " "
         << u.creditscore <<" "<<emi<<" "<<time<<" "<< endl;

    fout.close();
    cout << "Loan Record Stored Successfully with Loan ID: " << newLoanId << endl;
    delete loan;
    
 
}
void updateUserScore(string bankacc, int newScore) {
    ifstream fin("userinfo.txt");

    vector<User> users;
    User u;
    while (fin >> u.name >> u.bankacc >> u.password
               >> u.phoneno >> u.aadharno
               >> u.income >> u.balance >> u.creditscore) {
        if (u.bankacc == bankacc) {
            u.creditscore = newScore;  // update score
        }
        users.push_back(u);
    }
    fin.close();
    ofstream fout("userinfo.txt");
    for (auto &user : users) {
        fout << user.name << " "
             << user.bankacc << " "
             << user.password << " "
             << user.phoneno << " "
             << user.aadharno << " "
             << user.income << " "
             << user.balance << " "
             << user.creditscore << endl;
    }
    fout.close();
}


void payemi(User &u) {
    try {
        ifstream fin("loandata.txt");
        vector<Record> records;
        Record temp;
        bool found = false;
        double emi = 0;
        while (fin >> temp.loanid >> temp.bankacc1 >> temp.flag 
                   >> temp.score >> temp.emi >> temp.months) {
            if (temp.bankacc1 == u.bankacc && temp.months > 0) {
                found = true;
                emi = temp.emi;
                if (u.balance < emi)
                    throw runtime_error("Insufficient balance for EMI payment");
        
                u.balance -= emi;
                temp.score+=20;
                 u.creditscore += 20;
                 temp.flag += 1;                  
                temp.months--;             
                cout << "EMI of " << emi << " paid Successfully\n";
   
            }
          records.push_back(temp);
        }
        fin.close();
        if (!found) {
            cout << "No active loan found for this user\n";
            return;
        }  
        ofstream fout("loandata.txt");
        for (auto &r : records) {
            fout << r.loanid << " "
                 << r.bankacc1 << " "
                 << r.flag << " "
                 << r.score << " "
                 << r.emi << " "
                 << r.months << endl;
        }
        fout.close();
         updateUserScore(u.bankacc, u.creditscore);
    }
    catch (runtime_error &e) {
        cout << "Payment Failed: " << e.what() << endl;
    }
}
void Record::startofmonth() {
    ifstream fin("loandata.txt");
    vector<Record> records;
    Record temp;
    while (fin >> temp.loanid >> temp.bankacc1 >> temp.flag >> temp.score>>temp.emi>>temp.months) {

        if (temp.flag == 1) {
            temp.flag = 0;
        }
        else {
            temp.flag--;

            if (temp.flag == -1)
                temp.score -= 10;
            else if (temp.flag == -2)
                temp.score -= 20;
            else if (temp.flag == -3)
                temp.score -= 40;
            else if (temp.flag < -3)
                temp.score -= 60;
         
        }
        temp.months-=1;

        records.push_back(temp);
        updateUserScore(temp.bankacc1,temp.score);
    }

    fin.close();

    // Rewrite records.txt
    ofstream fout("loandata.txt");

    for (auto &r : records) {
        fout << r.loanid << " "
             << r.bankacc1 << " "
             << r.flag << " "
             << r.score<<" "<<r.emi<<" "<<r.months<< endl;
    }

    fout.close();

}

int main(){
    string x;
    User u;
    int h,t;
    double emi;
    Record r;
    cout<<"Are you user or admin(1/2)";
    cin>>h;
    if(h==1){  
        cout<<"Enter bank account number";
        cin>>x;
        login(x,u);
        cout<<"Enter 1 to apply for loan, 2 to pay emi";
        cin>>t;
        if(t==1)apply_loan(u);
        else if(t==2)payemi(u);
        
    }else if(h==2){
        char w='n';
         cout<<"Enter y to execute start of month";
        cin>>w;
        if(w=='y')r.startofmonth(); 
    }
    return 0;
}