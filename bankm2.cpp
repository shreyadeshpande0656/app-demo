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
    ifstream fin("userinfo.txt");

    if (!fin) {
        cout << "Error opening file\n";
        return;
    }
   
    string uname, ubankacc, upassword,uphoneno, uaadharno;
    
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

            if (enteredPass == upassword) {
                cout << "Login Successful!\n";

                u.name = uname;
                u.bankacc = ubankacc;
                u.password = upassword;
                u.phoneno = uphoneno;
                u.aadharno = uaadharno;
                u.income = uincome;
                u.balance = ubalance;
                u.creditscore = ucreditscore;
                cout<<"Hello "<<uname<<",  Welcome to Bharat Bank";
            }
           else{
            cout<<"Maximum attempts limit reached";
           }
            return;
        }
    }

    if (!found) {
        cout << "Account not found.\n";
    }
}

void create_acc() {
    User u;

    cout << "Enter Name: ";
    cin >> u.name;

    cout << "Enter Phone Number: ";
    cin >> u.phoneno;

    cout << "Enter Aadhar Number: ";
    cin >> u.aadharno;

    cout << "Enter Annual Income: ";
    cin >> u.income;

    cout << "Enter Initial Balance: ";
    cin >> u.balance;

    cout << "Set Password: ";
    cin >> u.password;
    ifstream fin("userinfo.txt");

    string uname, ubankacc, upassword, uphoneno, uaadharno;
    int uincome, ucreditscore;
    double ubalance;

    int lastAccNo = 99;

    while (fin >> uname >> ubankacc >> upassword >> uphoneno
               >> uaadharno >> uincome >> ubalance >> ucreditscore) {

        lastAccNo = stoi(ubankacc);
    }

    fin.close();
    u.bankacc = to_string(lastAccNo + 1);
   
    u.creditscore = 700;

    ofstream fout("userinfo.txt", ios::app);

    fout << u.name << " "
         << u.bankacc << " "
         << u.password << " "
         << u.phoneno << " "
         << u.aadharno << " "
         << u.income << " "
         << u.balance << " "
         << u.creditscore << endl;

    fout.close();

    cout << "\nAccount Created Successfully!\n";
    cout << "Your Bank Account Number is: " << u.bankacc << endl;
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
void apply_loan(User &u) {
    int type;
     double amt, time;
    cout << "Enter Loan Type (0-Home, 1-Personal, 2-Car): ";
    cin >> type;
    cout << "Enter Loan Amount: ";
    cin >> amt;
    cout << "Enter Loan Duration (Years): ";
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
    double emi = loan->calculate_emi();
    cout<<"Your monthly emi will be "<<emi;

   
   
  
}
/*
void payemi(User &u){
    try {
    double emi = loan->calculate_emi();

    if (u.balance < emi)
        throw runtime_error("Insufficient balance for EMI payment");

    u.balance -= emi;

    cout << "EMI Paid Successfully\n";
}
catch (runtime_error &e) {
    cout << "Payment Failed: " << e.what() << endl;
}
}*/
int main(){
    string x;
    User u;
    int h;
    cout<<"Enter 1 to login, 2 to create account";
    cin>>h;
    if(h==1){
        cout<<"Enter bank account number";
        cin>>x;
        login(x,u);
        apply_loan(u);
    }
    else if(h==2){
        create_acc();
    }
    return 0;
}