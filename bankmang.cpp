#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <vector>
using namespace std;

class User {
private:
    string name;
    string bankacc;
    string password;
    long int phoneno;
    long int aadharno;
    int income;
    double balance;
    int creditscore;

public:
    void createacc();
    void login();
    void display();
        
}
class loan{
    protected:
    int id;
    double amt;
    double rate;
    double time;
    double emi;
    double balance;
    public:
    loan(double a){
        amt=a; 
    }
    
   virtual void input();
    void display();
    virtual double calculateEMI();
    void makePayment(double amount);
    double getBalance();
    virtual void input() = 0;
    virtual void calculateEMI() = 0;
   
};
class Admin {
public:
   virtual void approveLoan();
    virtual void rejectLoan();
};
class homeloan : public loan{
    private:
    double propertyValue;

    public:
    void input() {
        cout << "Enter loanId, amount, tenure, property value:\n";
        cin >> loanId >> amt >> time >> propertyValue;
    }

    void calculateEMI() {
        emi = amount * 0.07; // example logic
    }
    
}
class personalloan : public loan{
    
}
class carloan : public loan{
    public:
    void input() {
        cout << "Enter loanId, amount, tenure:\n";
        cin >> id >> amt >> time;
    }

    void calculateEMI() {
        emi = amount * 0.09; // different logic
    }
    
}


void bankaccount ::createaccount(){
    cout<<"Create account"<<endl;
    cout<<"Enter name";
    getline(cin,name);
   
}
void login :: user(){
    string g;
    cout<<"Enter bank account no";
    cin>>g;
}
};

int main() {
    char ch1,ch2;
    cout<<" \n 1.Enter 1 to create new bank account \n Enter 2 to login into your bank account"
    cin>>ch1;
    if(ch1==1)createacc();
    if(ch1==2)login();
    cout<<"1.Enter 1 to make a loan request \n 2.Enter 2 to make emi payment "
    cin>>ch2;
    if(ch2==1)createloan();
    if(ch2==2)payment();
    
    
    return 0;
}
