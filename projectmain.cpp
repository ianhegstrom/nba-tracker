

#include <stdio.h>
#include "head.h"
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <math.h>

using namespace std;

hashit::hashit(int n){
    //set the open addressing and chaining to the desired size
    arr.resize(n);
    dudes.resize(n);

    size = n;
    //counters
    openCollision = 0;
    chainCollision = 0;
    tot = 0;
    linprob = 0;

}

hashit::~hashit(){

}

//parses the input into the proper stuff
void hashit::addGuy(string word){
    string delimeter = ",", token;
    size_t pos = 0;
    int count = 0;
    node *guy = new node;

    //duplicate node
    node * dupes = new node;
    while((pos = word.find(delimeter)) != string::npos){
        token = word.substr(0, pos);
        word.erase(0, pos + delimeter.length());
        count++;
        if(count == 1){
            teams *up = new teams;
            up -> team = token;
            guy -> playedfor = up;
            teams * down = new teams;
            down-> team = token;
            dupes->playedfor = down;

        }
        else if(count == 2||count == 3||count == 5){
            guy-> playedfor -> team = guy -> playedfor -> team + "," + token;
            dupes->playedfor ->team = dupes->playedfor->team + ", " + token;
        }
        else if(count == 6){
            guy->first = token;
            guy->key = token;
        }
        else if(count == 7){
            guy->last = token;
            guy->key = guy->key + token;
        }
        else if(count == 8){
            guy->born = stoi(token);
        }
        else if(count == 9){
            guy->coun = token;
        }
        else if(count == 10){
            guy->weight = stoi(token);
        }
        else if(count == 11){
            guy->height = stoi(token);
        }

    }
    guy->thrw = word;
    //insert the new node into the open addressing
    insertin(guy, hashSum(guy->key, size));

    dupes ->first = guy->first;
    dupes ->last = guy->last;
    dupes ->born = guy->born;
    dupes ->weight = guy->weight;
    dupes ->height = guy->height;


    chainNum++;
    //insert the new node into the chaining
    hashyUnchained(dupes,size);

}

//prints out everyone for open addressing
void hashit::print(){
    for(int i = 0; i < arr.size(); i++){
        if(arr[i] == NULL){
            //cout<<"NULL"<<endl;
        }
        else{
            cout<<arr[i] -> first<<","<<arr[i] -> last<<endl;
        }
    }
}


//prints out everyone for chaining
void hashit::chainPrint()
{
    node * temp;

    for(int i = 0; i<dudes.size(); i++)
    {
        temp = dudes[i];
        while(temp != NULL)
        {

            cout <<temp->first << "," << temp->last << endl;
            temp = temp->next;
        }


    }
}


//sum function
int hashit::hashSum(string name, int maxHash)
{
    int sum = 0;
    for (int i = 0; i < name.length(); i++){
        sum = sum + (name[i]* pow(10,i%4));
    }
    sum = sum % maxHash;
    return sum;
}



//build the hash table using chaining
void hashit::hashyUnchained(node *guy, int maxHash)
{
    int index = hashSum(guy -> key,maxHash); //make the hash sum index

    if (dudes[index] == NULL) //if index is empty
    {
        dudes[index] = guy; //first guy
    }
    else //if its not the first
    {

        node * oldy = search(guy,index); //search for the name
        node * temp;
        if (oldy != NULL) //he's already there
        {
            chainNum--;
            teams *current = oldy-> playedfor;
            while(current -> next != NULL){
                current = current -> next;
            }

            current -> next = guy->playedfor;//update the date
            return;

        }
        else//put the node at the front
        {
            //its a collision
            chainCollision++;

            temp = dudes[index];
            dudes[index] = guy;
            guy->next = temp;
        }
    }
}

//search function (used in chaining)
node * hashit::search(node*guy,int index)
{
    if(dudes[index]!=NULL) //if something exists at the vector place
    {
        node * temp = dudes[index];

        while((temp!=NULL))
        {
            tot++;
            if ((temp->key == guy->key)&&(temp->height == guy->height)&&(temp->born == guy->born))
            {
                return temp;
            }
            temp = temp->next;
        }
        return NULL;
    }
    else
    {
        return NULL;
    }
}



//insert function for open addressing
void hashit::insertin(node *guy, int index){
    //if no one is at the node
    if(arr[index] == NULL){
        openNum++;
        arr[index] = guy;
    }
    else{
        //if the guy at the node is the same guy
        if(arr[index]->key == guy -> key && arr[index]->height == guy -> height && arr[index] -> born == guy -> born){
            teams *current = arr[index]->playedfor;
            while(current -> next != NULL){
                current = current -> next;
            }
            current -> next = guy -> playedfor;
        }
        else{//a collision has occured
            openCollision++;
            int i = index;
            if(index + 1 == size){
                index = 0;
            }
            else{
                index++;
            }
            while(arr[index] != NULL && i != index){//searching for a null
                linprob++;
                if(arr[index]->key == guy -> key && arr[index]->height == guy-> height && arr[index] -> born == guy -> born){//if the guy is found before a null is found
                    openCollision--;
                    teams *current = arr[index]->playedfor;
                    while(current -> next != NULL){
                        current = current -> next;
                    }
                    current -> next = guy -> playedfor;
                    break;
                }
                else if(index + 1 == size){
                    index = 0;
                }
                else{
                    index++;
                }
            }
            if(arr[index]==NULL && index < arr.size()){ //if a null node was found
                arr[index] = guy;
                openNum++;
            }
        }
    }
}

//prints for the individual player for the chaining algorithm
void hashit::findingDjango(string name)
{
    int searchCount = 0;


    int index;
    string last = name;
    int pos = name.find(' ');
    string firstName;

    name.erase(pos,name.length());
    last.erase(0,pos+1);
    name = name+last;

    index = hashSum(name, dudes.size()); //run it through the function
    node*temp = dudes[index]; //get the cahin

    while(temp != NULL)
    {
        searchCount++;
        if(temp->key == name)
        {
            cout<<temp->first<<","<<temp->last<<endl;
            cout<<"Height: "<<temp-> height<<endl;
            cout<<"Weight: "<<temp -> weight<<endl;
            cout<<"Born: "<<temp-> born <<endl;
            cout<<"Country: "<<temp-> coun<<endl;
            cout<<"Teams: " << endl;
            teams *current = temp -> playedfor;
            while(current != NULL)
            {
                cout<<current -> team << endl;
                current = current -> next;
            }
            cout <<"Number of searches: " <<searchCount << endl;

            return;
        }
        temp = temp->next;
    }
    if(temp == NULL)
    {
        cout << "player not found" << endl;;
    }


}


//find function for open addressing
void hashit::findGuy(string name){
    int linsearch = 0;
    int index;
        name.erase(remove(name.begin(),name.end(),' '),name.end());
        index = hashSum(name, size);
        if(arr[index] -> key == name){
            linsearch++;
            cout<<arr[index]->first<<","<<arr[index]->last<<endl;
            cout<<"Height: "<<arr[index] -> height<<endl;
            cout<<"Weight: "<<arr[index] -> weight<<endl;
            cout<<"Born: "<<arr[index] -> born <<endl;
            cout<<"Country: "<<arr[index] -> coun<<endl;

            cout<<"Teams: ";
            teams *current = arr[index] -> playedfor;
            while(current != NULL){
                cout<<current -> team<<endl;
                current = current -> next;
            }
            cout<<"Number of searches: "<<linsearch<<endl;
        }
        else{
            linsearch++;
            int i = index;
            index++;
            while(arr[index] -> key != name && index != i){
                linsearch++;
                if(index + 1 == size){
                    index = 0;
                }
                else{
                    index++;
                }
            }
            cout<<arr[index]->first<<","<<arr[index]->last<<endl;
            cout<<"Height: "<<arr[index] -> height<<endl;
            cout<<"Weight: "<<arr[index] -> weight<<endl;
            cout<<"Born: "<<arr[index] -> born <<endl;
            cout<<"Country: "<<arr[index] -> coun<<endl;
            cout<<"Teams: "<<endl;
            teams *current = arr[index] -> playedfor;
            while(current -> next != NULL){
                cout<<current -> team<<endl;
                current = current -> next;
            }
            cout<<current -> team<<endl;
            cout<<"Number of searches: "<<linsearch<<endl;

        }
}
