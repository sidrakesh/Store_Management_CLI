//Siddharth Rakesh
//11CS30036

#include<iostream>
#include<fstream>
#include<string>
#include<stdio.h>
#include<vector>
#include<conio.h>
#include<windows.h>
#include<cstdlib>
#define INF 999999
using namespace std;

    ofstream outfile;  //for file handling
    ifstream infile;

class Customer{
      private:
          char Name[20];
          char Address[100];
          char TelephoneNumber[11];
          char DrivingLicenseNumber[10];

      public:
          void SetName(char s[])  //to set the name
          {
               strcpy(Name,s);
          }
          void SetAddress(char s[])  //to set the address
          {
               strcpy(Address,s);
          }
          void SetTelephoneNumber(char s[])  //to set the telephone number
          {
               strcpy(TelephoneNumber,s);
          }
          void SetDrivingLicenseNumber(char s[])  //to set driving license number
          {
               strcpy(DrivingLicenseNumber,s);
          }
          void ShowName()  //to show the name
          {
               for(int i=0;Name[i]!='\0';i++)
               cout<<(char)Name[i];
          }
};

class PurchaseAccounts{      //class for monetary account of each customer
      private:
         int CNNumber;
         int PurchaseValue;

      public:
         void setPurchaseValue(int Purchase)  //sets the amount of purchase made to the account
         {
             PurchaseValue = Purchase;
         }
         int showCNNumber()  //returns the CN number
         {
             return CNNumber;
         }
         int showPurchaseValue()  //returns the purchase value
         {
             return PurchaseValue;
         }
         void setCNNumber(int CNNumb)   //sets the CN number
         {
             CNNumber = CNNumb;
         }
};

void UnregisteredUser();  //function prototypes
void RegisteredUser();
void YearEnd();

int main()
{
    Customer User;
    PurchaseAccounts Account;
    int i,position;
    int num;
    
    char UserType;
    do{
    system("CLS");
    cout<<"Welcome to the store!!\n\nPress 1 if you have registered.\n\nPress 2 if you are a new customer.\n\nPress y if the year has ended\n\nPress e to exit";
    UserType=getch();

    if(UserType=='2') UnregisteredUser();

    else if(UserType=='1') RegisteredUser();

    else if(UserType=='y') YearEnd();
    }while(UserType!='e');
}

class YearRecords  //class for maintaining yearly records
{
    private:
        int Year;
        int TotalSales;
        
    public:
        void SetYear(int year)  //sets the year of record
        {
           Year = year;
        }
        void SetTotalSales(int Sales)  //sets the amount of sales in the year
        {
           TotalSales = Sales;
        }
        void ShowRecords()  //shows the records
        {
           cout<<"\n"<<Year<<"\t"<<TotalSales;
        }

};

void YearEnd()  //function called at the end of year
{
    system("CLS");  //to clear the screen
    int year;
    cout<<"Enter year : ";
    cin>>year;
    PurchaseAccounts Account;
    Customer Winner;
    int i,k,uppmax,flag;
    int WinnersHighest[10];
    for(i=0;i<10;i++)
    WinnersHighest[i]=-1;  //initialization
    
    vector<int>GoldWinners;
    int maxi;
    infile.open("PurchaseAccountsFile.dat",ios::ate);
    int endposition = infile.tellg();
    int n = endposition/sizeof(PurchaseAccounts);  //number of customers
    int position,val;
    int max,j;
    infile.close();
    uppmax=INF;
    
    for(j=0;j<10;j++)
    {
    max=-1;
         
         for(i=0;i<n;i++)
         {
            flag=0;
            for(k=0;k<10;k++)
            if(WinnersHighest[k]==i)
            {
            flag=1;
            break;
            }
            if(flag==1)
            continue;
            
            infile.open("PurchaseAccountsFile.dat",ios::in);
            position = i*sizeof(PurchaseAccounts);
            infile.seekg(position);
            infile.read((char*)&Account,sizeof(PurchaseAccounts));  //getting account information
            val = Account.showPurchaseValue();
            if(val>max)
            {
              max = val;
              maxi = i;
            }
            infile.close();
         }
    WinnersHighest[j] = maxi;
    }
    cout<<"\n\nThe Top ten Winners are: \n";  //the top ten winners are decided
    for(i=0;i<10;i++)
    {
        infile.open("Customers.dat",ios::in);
        position = WinnersHighest[i]*sizeof(Customer);
        infile.seekg(position);
        infile.read((char*)&Winner,sizeof(Customer));
        cout<<i+1<<" - ";
        Winner.ShowName();
        cout<<"\n";
        infile.close();
    }
    
    int TotalSales = 0;
    
    for(i=0;i<n;i++)
    {
        infile.open("PurchaseAccountsFile.dat");
        position = i*sizeof(PurchaseAccounts);
        infile.seekg(position);
        infile.read((char*)&Account,sizeof(PurchaseAccounts));
        val = Account.showPurchaseValue();
        
        TotalSales += val;
        if(val>=10000)
        GoldWinners.push_back(i);  //gold winners are maintained in a vector class object
        infile.close();
    }
    
    cout<<"\nThe Gold Coin Winners are: \n";   //the gold winners are decided
    for(i=0;i<GoldWinners.size();i++)
    {
        infile.open("Customers.dat");
        position = GoldWinners[i]*sizeof(Customer);
        infile.seekg(position);
        infile.read((char*)&Winner,sizeof(Customer));
        cout<<i+1<<" - ";
        Winner.ShowName();
        cout<<"\n";
        infile.close();
    }
    YearRecords NewYear,Year;
    NewYear.SetYear(year);
    NewYear.SetTotalSales(TotalSales);
    outfile.open("AllYearRecords.dat",ios::out|ios::app);
    outfile.write((char*)&NewYear,sizeof(YearRecords));   //records updated
    outfile.close();
    
    infile.open("AllYearRecords.dat",ios::in|ios::ate);
    endposition = infile.tellg();
    n = endposition/sizeof(YearRecords);
    infile.close();
    cout<<"\nYear\tSales";
    
    for(i=0;i<n;i++)
    {
       infile.open("AllYearRecords.dat",ios::in|ios::ate);
       infile.seekg(i*sizeof(YearRecords));
       infile.read((char*)&Year,sizeof(YearRecords));
       Year.ShowRecords();
       infile.close();
    }
    
    outfile.open("PurchaseAccountsFile.dat",ios::trunc);  //earlier accounts refreshed
    outfile.close();
    outfile.open("Customers.dat",ios::trunc);
    outfile.close();
    
    cout<<"\n\nAll Earlier accounts reset.\nPress any key to continue...";
    getch();
}

void RegisteredUser()
{
    PurchaseAccounts Account,CurrAccount;
    int PurchaseMade,CNNumb,NewPurchaseValue;
    system("CLS");
    cout<<"Enter CN Number : ";
    cin>>CNNumb;
    cout<<"\nEnter value of purchase made : ";
    cin>>PurchaseMade;
    int position = (CNNumb-1)*sizeof(PurchaseAccounts);  //correct position seeked
    infile.open("PurchaseAccountsFile.dat",ios::in);
    infile.seekg(position);
    infile.read((char*)&Account,sizeof(PurchaseAccounts));
    NewPurchaseValue = Account.showPurchaseValue() + PurchaseMade;  //account balance
    Account.setPurchaseValue(NewPurchaseValue);  
    cout<<"\nCurrent Purchase Amount : "<<NewPurchaseValue<<"\nPress Any key to continue....";
    getch();
    infile.seekg(0,ios::end);
    int endposition = infile.tellg();
    int n = endposition/sizeof(PurchaseAccounts);
    infile.close();
    
    int i;
    for(i=0;i<n;i++)  //updating account balance
    {
       if(i==CNNumb-1)
       {
           outfile.open("NewFile.dat",ios::app|ios::out);
           outfile.write((char*)&Account,sizeof(PurchaseAccounts));
           outfile.close();
       }
       else
       {
           infile.open("PurchaseAccountsFile.dat",ios::in);
           infile.seekg(i*sizeof(PurchaseAccounts));
           infile.read((char*)&CurrAccount,sizeof(PurchaseAccounts));
           infile.close();
           outfile.open("NewFile.dat",ios::app|ios::out);
           outfile.write((char*)&CurrAccount,sizeof(PurchaseAccounts));
           outfile.close();
       }
    }
    
    outfile.open("PurchaseAccountsFile.dat",ios::trunc);
    outfile.close();
    for(i=0;i<n;i++)
    {
           infile.open("NewFile.dat",ios::in);
           infile.seekg(i*sizeof(PurchaseAccounts));
           infile.read((char*)&CurrAccount,sizeof(PurchaseAccounts));
           infile.close();
           outfile.open("PurchaseAccountsFile.dat",ios::app|ios::out);
           outfile.write((char*)&CurrAccount,sizeof(PurchaseAccounts));
           outfile.close();
    
    }
    remove("NewFile.dat");  //deleting temporary file
}

void UnregisteredUser()
{
    Customer NewUser;
    PurchaseAccounts Account,NewAccount;
    char s[100];
    system("CLS");
    cout<<"Welcome new customer!\n\nEnter your name : ";
    cin.getline(s,20);
    NewUser.SetName(s);
    cout<<"\nEnter your address : ";
    cin.getline(s,100);
    NewUser.SetAddress(s);
    cout<<"\nEnter your telephone number : ";
    cin.getline(s,11);
    NewUser.SetTelephoneNumber(s);
    cout<<"\nEnter your driving license number : ";
    cin.getline(s,10);
    NewUser.SetDrivingLicenseNumber(s);  //new user defined
    
    outfile.open("Customers.dat",ios::app|ios::out); // open the file
    outfile.write((char*)&NewUser,sizeof(Customer));
    outfile.close();
    
    infile.open("PurchaseAccountsFile.dat",ios::ate); // open the file
    int endposition = infile.tellg(); // find where we are
    int n = endposition/sizeof(PurchaseAccounts);
    
    if(n<=0)  //CN number generation
    {
         infile.close();
         NewAccount.setCNNumber(1);
         NewAccount.setPurchaseValue(0);
         outfile.open("PurchaseAccountsFile.dat",ios::out|ios::app);
        
         cout<<"\nYour CN number is : 1";
         outfile.write((char*)&NewAccount, sizeof(PurchaseAccounts));
         outfile.close();
    }
    else
    {
         int NewAccountNumber = n+1;
         cout<<"\nYour CN number is : "<<NewAccountNumber;
         NewAccount.setCNNumber(NewAccountNumber);
         NewAccount.setPurchaseValue(0);
         infile.close();
         outfile.open("PurchaseAccountsFile.dat",ios::out|ios::app);
         outfile.write((char*)&NewAccount,sizeof(PurchaseAccounts));
         outfile.close();
   }
   getch();
}
