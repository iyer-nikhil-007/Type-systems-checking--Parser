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
map<string,Datatype*> struct_var_to_type;
set<string> types = {"int","float","double","char","void","struct"};
map<string,Datatype*> def;
int l_no,goto_flag=1;

int main()
{
   for(auto p:types)
   {
      Datatype* temp = new Datatype;
      temp->type= p;
      temp->link = NULL;
      def[p]=temp;
   }

   ifstream fin("a.txt");
   while(!fin.eof())
   {
      string str;
      getline(fin,str);
      string cur_line = str;
      stringstream strstream(str);
      strstream >> str;
      if(str.empty())
      {
         l_no++;
         continue;
      }
      //cout<<str<<endl;

      if(str == "struct")
      {
         l_no++;
         //cout<<str<<" "<<l_no<<endl;
         strstream >> str;
         for(int i=0; i<cur_line.size();i++)
         {
            if(cur_line[i]==';')
            {
               //cout<<"AAAAAA\n";
               goto_flag = 1;
               goto var_struct_dec;
            }
         }
         //cout<<1<<endl;
         Datatype* x = new Datatype;
         x->type = "struct";
         types.insert(str);
         def[str] = x;
         while(!fin.eof())
         {
            getline(fin,str);
            l_no++;
            //cout<<str<<" "<<l_no<<endl;
            stringstream strstream(str);
            strstream >> str;
            if(str =="{")    continue;
            else if(str[0]=='}')   break;
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
                     y->link = def[y->type];
                     x->members.push_back(y);
                     //var_to_type[y->name] = y;
                  }
               }
            }
         }
         x->link = NULL;
         def[x->type] = x;
      }
      else if(str == "typedef")
      {
         l_no++;
         //cout<<2<<endl;
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
               x->link = def[new_type];
               def[x->name] = x;
               en++;
               st = en;
            }
         }
      }
      else
      {
         var_struct_dec: goto_flag = 1-goto_flag;
         //cout<<3<<endl;
         //cout<<goto_flag<<endl;
         if(goto_flag!=0)  l_no++;
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
               y->dimension = def[var_type]->dimension;
               y->type = var_type;
               y->members = def[var_type]->members;
               y->pointers = def[var_type]->pointers;
               y->line_number = l_no;

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
               if(goto_flag==0)
                  struct_var_to_type[y->name] = y;
               else var_to_type[y->name] = y;
            }
         }
         goto_flag = 0;
      }
   }

   set<set<string>> name_eq;

   for(auto p:var_to_type)
   {
      set<string> temp;
      for(auto q:var_to_type)
      {
         if(q.first != p.first)
         {
            if(q.second->type==p.second->type)
            {
               if((q.second->pointers==0 && p.second->pointers==0) &&
                  (q.second->dimension.size()==0 && p.second->dimension.size()==0))
               {
                  temp.insert(p.first);
                  temp.insert(q.first);
               }
            }
         }
      }
      if(!temp.empty())
         name_eq.insert(temp);
   }

   set<set<string>> in_name_eq;

   for(auto p:var_to_type)
   {
      set<string> temp;
      for(auto q:var_to_type)
      {
         if(q.first!=p.first && q.second->line_number == p.second->line_number)
         {
            if(q.second->pointers == p.second->pointers
               && p.second->dimension == q.second->dimension)
            {
               temp.insert(p.first);
               temp.insert(q.first);
            }
         }
      }
      if(!temp.empty()) in_name_eq.insert(temp);
   }

   for(auto p:struct_var_to_type)
   {
      set<string> temp;
      for(auto q:struct_var_to_type)
      {
         if(q.first!=p.first && q.second->line_number==p.second->line_number)
         {
            if(q.second->pointers == p.second->pointers
               && p.second->dimension == q.second->dimension)
            {
               temp.insert(p.first);
               temp.insert(q.first);
            }
         }
      }
      if(!temp.empty()) in_name_eq.insert(temp);
   }

   set<set<string>> struct_eq;

   int size = def.size();

   vector<string> cur_matrix[size],new_matrix[size];
   for(int i=0;i<size;i++)
   {
      for(int j=0; j<size; j++)
      {
         cur_matrix[i].push_back("T");
         new_matrix[i].push_back("T");
      }
   }

   map<string,Datatype*> all_types;
   set<string> type_names;
   for(auto p:def)
   {
      all_types[p.first] = p.second;
      type_names.insert(p.first);
   }

   while(true)
   {
      int i=0;
      for(auto p:all_types)
      {
         int j=0;
         for(auto q:all_types)
         {
            if(p.first!=q.first)
            {
               auto x = p.second,y = q.second;
               string type_x = x->type,type_y = y->type;
               while(x!=NULL)
               {
                  type_x = x->type;
                  x = x->link;
               }
               while(y!=NULL)
               {
                  type_y = y->type;
                  y = y->link;
               }
               if(type_x != type_y)
                  cur_matrix[i][j]="N";
               else
               {
                  x = p.second, y = q.second;
                  if(x->pointers != y->pointers)
                     cur_matrix[i][j]="N";

                  else if(x->dimension!=y->dimension)
                     cur_matrix[i][j]="N";
                  else
                  {
                     if(x->members.size()!=y->members.size())
                        cur_matrix[i][j]="N";

                     else
                     {
                        int mem_size = x->members.size();
                        for(int k=0;k < mem_size; k++)
                        {
                           Datatype* e = x->members[k];
                           Datatype* f = y->members[k];
                           string type_e = e->type;
                           string type_f = f->type;
                           int idx_e = 0, idx_f =0;

                           for(auto iter:type_names)
                              if(iter==type_e) break;
                              else idx_e++;

                           for(auto iter:type_names)
                              if(iter==type_f) break;
                              else idx_f++;

                           if(cur_matrix[idx_e][idx_f]=="N")
                              cur_matrix[i][j]="N";

                           if(e->pointers != f->pointers)
                              cur_matrix[i][j]="N";

                           if(e->dimension!=f->dimension)
                              cur_matrix[i][j]="N";
                        }
                     }
                  }
               }
            }
            j++;
         }
         i++;
      }
      int flag = 0;
      for(int i=0; i<size;i++)
         if(cur_matrix[i]!=new_matrix[i])
         {
            flag=1;
            break;
         }
      if(flag==0) break;
      for(int i=0; i<size;i++)
         new_matrix[i] = cur_matrix[i];
   }

   for(auto p:var_to_type)
   {
      set<string> temp;
      for(auto q: var_to_type)
      {
         if(p.first!=q.first)
         {
            string type1 = p.second->type;
            string type2 = q.second->type;
            int idx_e = 0, idx_f =0;

            for(auto iter:type_names)
               if(iter==type1) break;
               else idx_e++;

            for(auto iter:type_names)
               if(iter==type2) break;
               else idx_f++;

            if(cur_matrix[idx_e][idx_f]=="T")
            {
               if(p.second->pointers==q.second->pointers
                  && p.second->dimension==q.second->dimension)
               {
                  temp.insert(p.first);
                  temp.insert(q.first);
               }
            }
         }
      }
      if(!temp.empty())
         struct_eq.insert(temp);
   }

   for(auto p:name_eq)
   {
      cout<<"Variables ";
      for(auto q:p)
         cout<<q<<" ";
      cout<<"are name equivalent"<<endl;
   }

   cout<<"\n//////////////////////////////////////////////////////////////////\n\n";

   for(auto p:in_name_eq)
   {
      cout<<"Variables ";
      for(auto q:p)
         cout<<q<<" ";
      cout<<"are internal name equivalent"<<endl;
   }

   cout<<"\n//////////////////////////////////////////////////////////////////\n\n";

   for(auto p:struct_eq)
   {
      cout<<"Variables ";
      for(auto q:p)
         cout<<q<<" ";
      cout<<"are structurally equivalent"<<endl;
   }

   cout<<"\n//////////////////////////////////////////////////////////////////\n\n";

   map<string,Datatype*> all_vars;
   for(auto p:var_to_type) all_vars[p.first]=p.second;
   for(auto p:struct_var_to_type)  all_vars[p.first]=p.second;

   cout<<" ";
   for(auto p:all_vars)
      cout<<"  "<<p.first;
   cout<<endl<<endl;

   for(auto p:all_vars)
   {
      cout<<p.first<<"  ";
      for(auto q:all_vars)
      {
            string type1 = p.second->type;
            string type2 = q.second->type;
            int idx_e = 0, idx_f =0;

            for(auto iter:type_names)
               if(iter==type1) break;
               else idx_e++;

            for(auto iter:type_names)
               if(iter==type2) break;
               else idx_f++;

            cout<<cur_matrix[idx_e][idx_f]<<"  ";
      }
      cout<<endl<<endl;
   }

   return 0;
}
