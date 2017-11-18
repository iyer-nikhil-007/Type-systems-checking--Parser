#include<bits/stdc++.h>

using namespace std;

struct Datatype{
   string type,name;
   vector<Datatype*> members;
   vector<int> dimension;
   int pointers = 0;
   int line_number;
   Datatype* link;
};

map<string,Datatype*> var_to_type;
set<string> types = {"int","float","double","char","void"};
map<string,Datatype*> def;
int l_no;

int main()
{
   for(auto p:types)
   {
      Datatype* temp = new Datatype;
      temp->type= p;
      def[p]=temp;
   }

   ifstream fin("a.txt");
   while(!fin.eof())
   {
      string str;
      getline(fin,str);
      stringstream strstream(str);
      strstream >> str;

      if(str == "struct")
      {
         l_no++;
         strstream >> str;
         Datatype* x = new Datatype;
         x->type = str;
         types.insert(str);
         def[str] = x;
         while(!fin.eof())
         {
            getline(fin,str);
            l_no++;
            stringstream strstream(str);
            strstream >> str;
            if(str =="{")  {  l_no++;  continue;   }
            else if(str[0]=='}') {  l_no++;  break;   }
            else
            {
               l_no++;
               string var_type = str;
               strstream >> str;
               int st=0 , en=0;
               for(int i=0;i<str.size();i++)
               {
                  if(str[i]==' ')
                  {
                     st++; en++;
                     continue;
                  }
                  if(str[i] == ',' || str[i] ==';')
                  {
                     en = i;
                     string s = str.substr(st,en-st);

                     Datatype* y = new Datatype;
                     y->type = def[var_type]->type;
                     y->dimension = def[var_type]->dimension;
                     y->members = def[var_type]->members;
                     y->pointers = def[var_type]->pointers;

                     en++;
                     st = en;

                     int flag = 0,s_start=0,s_end=0;
                     for( int j = 0; j<s.size(); j++)
                     {
                        if(s[j]=='[')
                        {
                           flag = j;
                           break;
                        }
                     }

                     if(flag == 0) s_end = s.size();
                     else s_end = flag;

                     for( int j = 0; j < s.size(); j++)
                        if(s[j] == '*')   y->pointers++, s_start=j+1;

                     if(flag != 0)
                     {
                        string var_name = s.substr(s_start,s_end-s_start);
                        y->name = var_name;
                        int j = flag+1;
                        s_end = j; s_start = s_end;
                        while(j < s.size())
                        {
                           if(s[j]==']')
                           {
                              string temp = s.substr(s_start,s_end-s_start);
                              int count = atoi(temp.c_str());
                              y->dimension.push_back(count);
                           }
                           if(s[j]=='[')
                              s_start = j+1;
                           j++;   s_end++;
                        }
                     }
                     else
                     {
                        string var_name = s.substr(s_start,s_end-s_start);
                        y->name = var_name;
                     }
                     x->members.push_back(y);
                     var_to_type[y->name] = y;
                  }
               }
            }
         }
         def[x->type] = x;
      }
      else if(str == "typedef")
      {
         l_no++;
         strstream >> str;
         string new_type = str;
         strstream >> str;
         int st=0,en=0;
         for(int i = 0; i<str.size(); i++)
         {
            if(str[i]==' ')
            {
               st++; en++;
               continue;
            }
            else if(str[i]==',' || str[i]==';')
            {
               en = i;
               string s = str.substr(st,en-st);
               Datatype* x = new Datatype;
               x->type = new_type;
               x->pointers = def[new_type]->pointers;
               x->name = s;
               x->dimension = def[new_type]->dimension;
               x->members = def[new_type]->members;
               def[x->name] = x;
               en++;
               st = en;
            }
         }
      }
      else
      {
         string var_type = str;
         strstream >> str;
         int st=0 , en=0;
         for(int i=0;i<str.size();i++)
         {
            if(str[i]==' ')
            {
               st++; en++;
               continue;
            }
            if(str[i] == ',' || str[i] ==';')
            {
               en = i;
               string s = str.substr(st,en-st);

               Datatype* y = new Datatype;
               y->type = def[var_type]->type;
               y->dimension = def[var_type]->dimension;
               y->members = def[var_type]->members;
               y->pointers = def[var_type]->pointers;

               en++;
               st = en;

               int flag = 0,s_start=0,s_end=0;
               for( int j = 0; j<s.size(); j++)
               {
                  if(s[j]=='[')
                  {
                     flag = j;
                     break;
                  }
               }

               if(flag == 0) s_end = s.size();
               else s_end = flag;

               for( int j = 0; j < s.size(); j++)
                  if(s[j] == '*')   y->pointers++, s_start=j+1;

               if(flag != 0)
               {
                  string var_name = s.substr(s_start,s_end-s_start);
                  y->name = var_name;
                  int j = flag+1;
                  s_end = j; s_start = s_end;
                  while(j < s.size())
                  {
                     if(s[j]==']')
                     {
                        string temp = s.substr(s_start,s_end-s_start);
                        int count = atoi(temp.c_str());
                        y->dimension.push_back(count);
                     }
                     if(s[j]=='[')
                        s_start = j+1;
                     j++;   s_end++;
                  }
               }
               else
               {
                  string var_name = s.substr(s_start,s_end-s_start);
                  y->name = var_name;
               }
               var_to_type[y->name] = y;
            }
         }
      }
   }
   /*for(auto q: var_to_type)
   {
      cout<<q.first<<endl;
      cout<<q.second->type<<endl;
      cout<<q.second->pointers<<endl;
      cout<<q.second->members.size()<<endl;
      cout<<q.second->dimension.size()<<endl;
      cout<<"*************************************\n";
   }*/
   return 0;
}
