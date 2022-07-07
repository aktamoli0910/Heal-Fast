#include<iostream>
#include<fstream>
#include<string>
#include<cwchar>
#include<stdlib.h>
using namespace std;

class med{
    public:
    string dname;
    int dprice;
    int dtime;
    string dno;
    med()
    {
        dno=" ";
    }
};


class user{
    string pin;
    string userId;
    string fname;
    string lname;
    string location;
    string code;
    public:
    void userLogin();
    void createNewUser();
};
string cno;
struct Gnode{
    struct Gnode *next;
    string cd;
    int d;
};
int countRL(){
    ifstream fin;
    fin.open("area_map.txt");
    if(!fin)
        cout<<"area_map.txt not opened";
    int l=0;            //total land marks
    string lin;         //to store each line for counting purpose
    while(fin){
        l++;
        getline(fin,lin);
    }
    l=l-1;
       //as file handling counts last line twice so we do -1
    fin.close();
    return l;
}

Gnode * createGraph(){
    int l=countRL();
    struct Gnode * G=new struct Gnode[l];
    ifstream fin;
    fin.open("area_map.txt");
    int c=0,nt=0;
    string st;
    int n;
    struct Gnode * k;
    while(c!=l){
        fin>>st;
        k = &G[c];
        G[c].cd=st;
        G[c].d=nt;
        nt++;
        G[c].next=0;
        while(fin){
            fin>>st;
            if(st!="e"){
                struct Gnode *t=new struct Gnode;
                t->next=0;
                t->cd=st;
                fin>>n;
                t->d=n;
                k->next=t;
                k=t;
            }
            else
                break;
        }
        c+=1;
    }
    //for(int j=0;j<l;j++)
      //  cout<<G[j].cd<<" ";
    return G;
}
int minDistance(int dist[], int sptSet[],int V)
{
   // Initialize min value
   int min = INT_MAX,min_index;


   for (int v = 0; v < V; v++)
     if (sptSet[v] == 0 && dist[v] <= min)
         min = dist[v], min_index = v;

   return min_index;
}
void printPharmacys(int *dist,string d[],int rnum)
{
    struct Gnode *G = createGraph();
    int l= countRL();

    struct Gnode * tmp;
    int rt;
    string rst,ct;
    float n1,n2;
   cout<<"Pharmacy\t\t\tdistance\t\tRating(for "<<cno<<")\n";
   for (int i = 0; i < rnum; i++){
        tmp=G;
        for(int j=0;j<l;j++){
        if(tmp[j].cd==d[i]){
            rt=tmp[j].d;
            break;
        }
    }
    cout<<"\t"<<d[i]<<"\t\t\t\t"<<dist[rt]<<"\t\t";
    ifstream fin;
    fin.open("feedbackAll.txt");
    while(fin>>rst){
      fin>>ct>>n1>>n2;
      if(rst==d[i] && ct==cno){
        cout<<n1/n2<<endl;
      }
    }
    cout<<endl;
    fin.close();
   }
}
int *dijkstra(string s)
{
    struct Gnode * G= createGraph();
    int l=countRL();
    int src;
    struct Gnode * tmp=G;
    for(int i=0;i<l;i++){
    if(tmp[i].cd==s){
        src=tmp[i].d;
        break;
        }
    }
    int * dist=new int[l];
    int * sptSet=new int[l];
    for(int i=0;i<l;i++){
        dist[i]=INT_MAX;
        sptSet[i]=0;
        }

    dist[src]=0;
    int countVis=0;
    string land;
    int landnum,dland;
     while(countVis<l) {
        int u = minDistance(dist, sptSet,l);
        sptSet[u] = 1;
        countVis++;
       // Update dist value of the adjacent vertices of the picked vertex.
       tmp=G+u;
       tmp=tmp->next;
       while(tmp)
       {
        dland=tmp->d;
        land=tmp->cd;
        if(land[0]=='R')
        {
          landnum=(int)land[1]-48+15;
        }
        else if(land[0]=='L')
        {
          if(land.length()==2)
        {
          landnum=(int)land[1]-48;
        }
        else
        {
            landnum=(((int)land[1]-48)*10)+(int)land[2]-48;
        }
        }
        if(sptSet[landnum]==0){
          dist[landnum]=min(dist[landnum],dland+dist[u]);
        }
        tmp=tmp->next;
     }
     }

     return dist;
}

void menu(string loc,string uid,int choicetake,string choiceno){
    string a;
    med D;
    med d1;
    int sugg=0,avg;
    if(choicetake==0){
    cout<<"\n\n\t\t\t=====THESE ARE THE CHOICES====="<<endl;
    cout<<"\n[1] Aayurvedic\n[2] Allopathic \n[3] Homeopathic\n[4] EXIT"<<endl;
    cout<<"Enter choice number:";
    cin>>a;
    if(a=="4"){
      return;
    }
  }
  else{
    a=choiceno;
    sugg=1;
    avg=choicetake;
    cout<<"\nAll these Pharmacys have your preferences\n\n";
  }
    cno="C"+a;

    ifstream fin;
    fin.open("Pharmacy.txt");
    if(!fin)
        cout<<"Not found Pharmacy"<<endl;
    string Rt,num="0";
    string Rarr[6];
    int rnum=0;
    fin>>Rt;
    while(fin){
        while(num[0]!='R' && fin){
            fin>>num;
            if(num==a){
                //cout<<Rt<<" ";
                Rarr[rnum++]=Rt;
            }
        }
        Rt=num;
        num="0";
    }
    fin.close();
    int *distpointer = dijkstra(loc);
    printPharmacys(distpointer, Rarr, rnum);

    //--------------------------------
    ifstream fiin;
    string sa,c="C"+a,rloc;
    cout<<"Enter Pharmacy of your choice:   ";
    cin>>sa;
    rloc=sa;
    sa=sa+".txt";
    string sb=sa;
    fiin.open(sa);
    if(!fiin)
        cout<<"not open";
    while(fiin){
        fiin>>sa;
        if(sa==c){
            cout<<"\t\t\t\t\tORDER NOW\n\n\n";
            while(D.dno[0]!='C'){
                fiin>>D.dno>>D.dname>>D.dprice>>D.dtime;
                if(D.dno[0]!='C'){
                if(sugg==0)
                cout<<D.dno<<" "<<D.dname<<" "<<D.dprice<<" "<<D.dtime<<endl;
                else{
                  if(D.dprice>=avg-50 && D.dprice<=avg+50){
                    cout<<D.dno<<" "<<D.dname<<" "<<D.dprice<<" "<<D.dtime<<endl;
                  }
                }
              }
            }
            break;
        }
    }
    fiin.close();
    int qty;
    char ch='Y';
    string d;
    med ordered;
    int bill=0;
    int ocount=0;
    string omed[5];
    int oqty[5];
    while((ch=='Y'||ch=='y')&& ocount<=5){
        ocount++;
        cout<<"Enter medicne number you want to order:\t"<<endl;
        cin>>d;
        cout<<"Enter quantity:\t"<<endl;
        cin>>qty;
        fiin.open(sb);
     while(fiin){
        fiin>>sa;
        if(sa==c){
            D.dno="X";
            while(D.dno[0]!='C'){
                fiin>>D.dno>>D.dname>>D.dprice>>D.dtime;
                if(D.dno[0]!='C'){
                if(D.dno==d){
                    ordered.dno=D.dno;
                    ordered.dname=D.dname;
                    omed[ocount-1]=D.dname;
                    oqty[ocount-1]=qty;
                    ordered.dprice=D.dprice*qty;
                    cout<<"\nmed Price: "<<ordered.dprice<<" ";
                    ordered.dtime=D.dtime;
                }
                }
            }
            break;
        }
    }
    fiin.close();
    bill+=ordered.dprice;

       cout<<"\n\nWants to order more?[y/n]";
       cin>>ch;
       cout<<endl;
    }

    cout<<"------------BILL-------------\n";
    cout<<"ordered medicines\t\tquantity\n";
    for(int dco=0;dco<ocount;dco++)
        cout<<omed[dco]<<"\t\t\t"<<oqty[dco];
    cout<<"\nTotal bill: "<<bill<<endl;
    //------writing order in user file for future suggestions---------------
    ifstream feedin;
    fstream feedout;
    feedin.open("userHistory.txt");
    feedout.open("tempHistory.txt",ios::out);
    if(!feedin)
      cout<<"userHistory file not opened\n";
    if(!feedout)
      cout<<"tempHistory file not opened\n";
    string uidd,cnam;
    int ccno=8;
    int cprice,ff;
    int ccount;
    int tdsum=0;
    for(int dco=0;dco<ocount;dco++)
        tdsum+=oqty[dco];
    while(feedin>>uidd){
      feedout<<"\n"<<uidd<<" ";
      for(int i=1;i<=ccno;i++){
        feedin>>cnam>>cprice>>ccount;
        //ff=((int)cnam[1])-48;
        if(uidd==uid && cno==cnam){
          cprice+=bill;
          ccount+=tdsum;
      }
        feedout<<cnam<<" "<<cprice<<" "<<ccount<<" ";
    }
    }
    feedin.close();
    feedout.close();
    remove("userHistory.txt");
    rename("tempHistory.txt","userHistory.txt");

    //-------------writing all order in one file for future rating purpose-----------------
    fstream foout;
    foout.open("ordered.txt", ios::out | ios::app);
    if(!foout){
    cout<<"ordered.txt not opened";
    }
    foout<<"\n"<<uid<<" "<<rloc<<" "<<cno<<" "<<bill<<" "<<ocount<<" ";
    for(int dco=0;dco<ocount;dco++)
        foout<<omed[dco]<<" "<<oqty[dco]<<" ";
    foout.close();
}

void feedback(string uid){
  ifstream fin;
  fstream fout;
  fin.open("ordered.txt");
  fout.open("tempord.txt",ios::out);
  if(!fin)
    cout<<"ordered.txt file not found for FEEDBACK\n";
  if(!fout){
    cout<<"tempord.txt not opened for FEEDBACK\n";
    }
  string id;
  int tbill,tmed,dqty[10];
  string rloc,choice_,dname[10];
  while (fin>>id) {
      float fd;
      fin>>rloc>>choice_>>tbill>>tmed;
      for(int i=0;i<tmed;i++){
        fin>>dname[i]>>dqty[i];
      }
      if(id==uid){
      cout<<"\n\t\t ******** Time For Some FEEDBACK ********\n\n";
      cout<<"Rate your previous orders(in terms of safety and services) out of 5: \n";
      cout<<"Pharmacy :"<<rloc;
      cout<<"\n\tYou ordered "<<tmed<<"medecines :\n";
      for(int i=0;i<tmed;i++){
        cout<<dname[i]<<"\t"<<dqty[i]<<endl;
      }
      cout<<"You Paid:  "<<tbill<<endl;
      cout<<"Enter your rating [out of 5]: ";
      cin>>fd;
      ifstream feedin;
      fstream feedout;
      feedin.open("feedbackAll.txt");
      feedout.open("tempfeedback.txt",ios::out);
      if(!feedin)
        cout<<"feedbackAll file not opened\n";
      if(!feedout)
        cout<<"tempfeedback file not opened\n";
      string rname,cname;
      int peoplect;
      float rating;
      while(feedin>>rname){
        feedin>>cname>>rating>>peoplect;
        if(rloc==rname && cname==choice_){
          peoplect+=1;
          rating+=fd;
        }
        feedout<<"\n"<<rname<<" "<<cname<<" "<<rating<<" "<<peoplect;
      }
      feedin.close();
      feedout.close();
      remove("feedbackAll.txt");
      rename("tempfeedback.txt","feedbackAll.txt");
      }
      else{
        fout<<"\n"<<id<<" "<<rloc<<" "<<choice_<<" "<<tbill<<" "<<tmed<<" ";
        for(int i=0;i<tmed;i++){
          fout<<dname[i]<<" "<<dqty[i]<<" ";
        }
      }
  }
fout.close();
fin.close();
remove("ordered.txt");
rename("tempord.txt","ordered.txt");
}

string suggestion(string userId,int & avg)
{
  //to give suggestions
  ifstream fin;
  fin.open("userHistory.txt");
  if(!fin)
    cout<<"userHistory.txt file not found for FEEDBACK\n";
  string id,c,mxc="C1";
  int cpeople,cmrp,mx=0;
  avg=0;
  char sug;
  while(fin>>id)
  {
    if(id==userId)
    {
      for(int i=0;i<8;i++){
        fin>>c>>cmrp>>cpeople;
        if(cpeople>mx){
          mx=cpeople;
          mxc=c;
          avg=(cmrp)/cpeople;
        }
      }
    }
    else{
        for(int i=0;i<8;i++){
          fin>>c>>cmrp>>cpeople;
        }
    }
  }
  fin.close();
  if(avg==0)
  {
    cout<<"YOU HAVE NOT ORDERED ANYTHING YET! NO SUGGESTIONS FOR YOU";
    cout<<"\n\n\t\tPress enter to continue";
    string tmp;
    getline(cin,tmp);
    getline(cin,tmp);
    return "x";
  }
  else
  {
    cout<<"\n\n\n\n\n\t****  SUGGESTIONS FOR YOU  ****\n\n\n"<<endl;
    cout<<"YOU WANT TO GO WITH THE SUGGESTION(y/n)"<<endl;
    cin>>sug;
    if(sug=='n')
    {
      return "x";
    }
    else if(sug=='y')
    {
      string aa;
      aa=mxc[1];
      //menu(cd,userId,1,aa);
      return aa;
  }
}
}

void user::userLogin(){
    cout<<"\t\t\t\t If new user enter id=0 and pass=0"<<endl;
    cout<<"Enter ID:";
    cin>>userId;
    cout<<"Enter pin:";
    cin>>pin;
    if(pin=="0" && userId=="0"){
        createNewUser();
        }
    else{
        ifstream fin;
        string f,l,i,p,loc,cd;
        bool userExist=false;
        fin.open("user_detail.txt");
        if(!fin)
        cout<<"file not found";
        while (fin) {
            fin>>f>>l>>i>>p>>loc>>cd;
            if(i==userId && p==pin){
            cout<<"Welcome "<<f<<" "<<l<<endl;
            userExist=true;
            break;
            }
        }
    fin.close();

    if(!userExist){
        cout<<"Invalid Pin or UserId\n1.try again\n2.exit\n :";
    int ch;
    cin>>ch;
    if(ch==1)
        userLogin();
    else
        exit(0);
    }
    else{
        feedback(userId);
        string sug;
        int avg;
        sug=suggestion(userId,avg);
        if(sug=="x")
          menu(cd,userId,0,"x");
        else
          menu(cd,userId,avg,sug);
        }
    }
}

string checklocation(string loc){
    ifstream flin;
    flin.open("landmarks.txt");
    string l,cd;int f=0;
    while(flin){
        flin>>l;
        if(l==loc){
            cout<<"correct location found"<<loc;
            f=1;
            flin>>cd;
            cout<<cd;
            break;}
        }
    flin.close();
    if(f==0){
        cout<<"You entered wrong location. Renter right location:";
        cin>>loc;
        cd=checklocation(loc);
        }
        return cd;
}
void user::createNewUser(){
    ofstream fout;
    fout.open("user_detail.txt",ios::app);
    cout<<endl;
    cout<<"Enter first name: ";
    cin>>fname;
    cout<<"Enter last name: ";
    cin>>lname;
    cout<<"Enter user Id: ";
    cin>>userId;
    cout<<"Enter pin: ";
    cin>>pin;
    cout<<"Enter location: ";
    cin>>location;
    code=checklocation(location);
    fout<<fname<<" "<<lname<<" "<<userId<<" "<<pin<<" "<<location<<" "<<code<<endl;
    fout.close();
    //----Enter a new entry in userHistory-------------------
    fout.open("userHistory.txt",ios::app);
    fout<<endl<<userId<<" C1 0 0 C2 0 0 C3 0 0 C4 0 0 C5 0 0 C6 0 0 C7 0 0 C8 0 0 ";
    fout.close();
    cout<<"\t\t\n------------Id created sucessfully!!------------";
    menu(code,userId,0,"0");
}

int main()
{
    int var=0;
    cout<<"\n\n\n\t     =============== HEAL FAST ===============\n"<<endl;
    cout<<"\n\tPress [1] to Continue\n"<<endl;
    cout<<"\n\tPress [2] to Exit\n"<<endl;
    cout<<"\nEnter choice:  ";
    cin>>var;
    if(var==1){
        user U;
        U.userLogin();
        }
    else{
        exit(0);
    }
}
