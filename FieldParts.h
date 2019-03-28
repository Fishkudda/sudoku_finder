#ifndef Field_p_h
#define Field_p_h

#include <vector>
#include <iostream>
#include <fstream>
#include <string>


using namespace std;

class CollectionOfBase;
class ThreeByThree;
class Base;
class Candidate;
class Fixed;
class LineX;
class LineY;

typedef vector<vector<Base> > matrix;
typedef vector<Base> row;
typedef vector<LineY> YLines;
typedef vector<LineX> XLines;

struct fitting{
    int idx = 0;
    vector<int>fit;
};

//Base class of all tiles
class Base{
public:
    Base(int x, int y, int value);
    
    bool validate();
    int x;
    int y;
    int value;
    bool change_able;
    bool hard;
    fitting ftt;
    Base *prev;
    int next();
    void reset();
};

Base::Base(int _x, int _y,int _value){
    x = _x;
    y = _y;
    value = _value;
    change_able = false;
    prev = nullptr;
    ftt = fitting{};
};

void Base::reset(){
    if(change_able){
        value=0;
    }
}

int Base::next(){
    
    if(change_able){
        int size_vec = (int)ftt.fit.size();
        int rand_index = rand()%size_vec;
        value = ftt.fit[rand_index];
    }
   
    return value;
};


//Candidate class value is changable, contains algo to change values

class Candidate:public Base{
public:
    Candidate(int x, int y, int value):Base(x,y,value){
        change_able = true;
    };
};

//Fixed class value is not changeable

class Fixed:public Base{
public:
    Fixed(int x, int y, int value):Base(x,y,value){
        change_able = false;
        hard = true;
    };
};
class CollectionOfBase{
public:
    void show();
    Base *line[9];
    bool validate();
    bool containsZero();
};
bool CollectionOfBase::containsZero(){
    for(int i=0;i<9;i++){
        if (line[i]->value==0){
            return true;
        }
    }
    return false;
}
void CollectionOfBase::show(){
    for (Base *ptr : line){
        printf("%d|",ptr->value);
    }
    printf("\n");
};
bool CollectionOfBase::validate(){
    try{
        int count[9]={0,0,0,0,0,0,0,0,0};
        
        for (Base *ptr : line){
            switch(ptr->value){
                case 1: count[0]++; break;
                case 2: count[1]++; break;
                case 3: count[2]++; break;
                case 4: count[3]++; break;
                case 5: count[4]++; break;
                case 6: count[5]++; break;
                case 7: count[6]++; break;
                case 8: count[7]++; break;
                case 9: count[8]++; break;
                case 0: break;
                default: break;
            }
            for(int x : count){
                if (x==2){
                    return false;
                }
            }
            
        }
        return true;
    }catch(exception e){
        return false;
    }
};
class ThreeByThree:public CollectionOfBase{
public:
    void create_tbt_by_field(matrix field);
    bool base_in_tbt(Base *base);
    Base *three_by_three[3][3];
    void show();
    void build_as_line();
};
bool ThreeByThree::base_in_tbt(Base *base){
    return false;
};
void ThreeByThree::show(){
    for (int y = 0; y < 3; y++){
        for (int x = 0; x < 3; x++){
            Base *base = three_by_three[y][x];
            printf("%d",base->value);
        }
        printf("\n");
    }
};
void ThreeByThree::build_as_line(){
    int index_c = 0;
    for (int y = 0; y < 3; y++){
        for (int x = 0; x < 3; x++){
            Base *ptr = three_by_three[y][x];
            line[index_c]=ptr;
            index_c++;
        }
    }
}
class LineX:public CollectionOfBase{};
class LineY:public CollectionOfBase{};

#endif /* FieldParts.h */
