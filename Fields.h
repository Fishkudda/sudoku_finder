#ifndef Fields_h
#define Fields_h

#include "FieldParts.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

using namespace std;

class Field{
public:
    Field(string path){
        field_is_set = false;
        load_file(path);
        _path = path;
    };
    
    void print_field();
    void reset_field();
    ThreeByThree tbt[3][3];
    XLines x_lines;
    YLines y_lines;
    matrix field;
    bool validate_field();
    bool solution();
    void load_file(string path);
    bool contains_zero();
    void reset_relations(Base *ptr);
    void export_solution(string export_path);
    
private:
    string _path;
    void create_tbt();
    bool field_is_set;
    void print_tbt_fields();
    void create_line_ptr();
    bool fill_fitting();
    void start_guessing();
};


void Field::print_tbt_fields(){
    
    for (int y = 0; y < 3; y++){
        for (int x = 0; x < 3; x++){
            tbt[y][x].show();
            printf("TBT: X:%d Y:%d\n",x,y);
        }
    }
}

void Field::start_guessing(){
    
    int rand_value_x = rand()%9;
    int rand_value_y = rand()%9;
    Base *b = &field[rand_value_y][rand_value_x];
    
    int pre = b->value;
    b->next();
   
    
    if(!validate_field()){
        b->value = pre;
    }
    
    
    if(!b->change_able){
        start_guessing();
    }
    
}

void Field::reset_relations(Base *ptr){
    int y = ptr->y;
    int x = ptr->x;
    int field_y = y/3;
    int field_x = x/3;
    
    LineY ly = y_lines[y];
    LineX lx = x_lines[x];
    for(int line_idx = 0; line_idx < 9; line_idx++){
        ly.line[line_idx]->reset();
        lx.line[line_idx]->reset();
        tbt[field_y][field_x].line[line_idx]->reset();
    }
}

bool Field::contains_zero(){
    for(CollectionOfBase cb : x_lines){
        if(cb.containsZero()){
            return true;
        }
    }
    return false;
}


void Field::load_file(string path){
    matrix result;
    
    ifstream input_stream(path);
    string line;
    
    int y = 0;
    while(getline(input_stream,line)){
        int x = 0;
        row vec;
        Base *prev_pointer = nullptr;
        for (char c:line){
            int value = (int)c;
            value = value-48;
            Base created_base = Base(x,y,value);
            if(value > 0){
                created_base = Fixed(x,y,value);
            }else{
                created_base = Candidate(x,y,value);
            }
            created_base.prev = prev_pointer;
            prev_pointer = &created_base;
            vec.push_back(created_base);
            x++;
        }
        
        result.push_back(vec);
        y++;
        
    }
    input_stream.close();
    field_is_set = true;
    field = result;
    create_tbt();
    create_line_ptr();
    solution();
};

void Field::export_solution(string export_path){
    ofstream export_file;
    export_file.open(export_path);
    for(int y = 0; y < 9;y++){
        for (int x = 0; x < 9; x++){
            Base *b = &field[y][x];
            export_file << b->value;
        }
        export_file << "\n";
    }
    
    export_file.close();
    
}

bool Field::fill_fitting(){
    
    for(int y = 0; y < 9;y++){
        for (int x = 0; x < 9; x++){
            Base *ptr = &field[y][x];
            if(ptr->change_able){
                struct fitting fit;
                
                for(int i=1;i<10;i++){
                    int prev_value = ptr->value;
                    ptr->value = i;
                    if(validate_field()){
                        fit.fit.push_back(i);
                    }else{
                        ptr->value = prev_value;
                    }
                }
                
                ptr->value = 0;
                
                if (fit.fit.size()==1){
                    ptr->value = fit.fit[0];
                    ptr->change_able = false;
                    
                }else if(fit.fit.size() == 0){
                    return false;
                }
                ptr->ftt = fit;
            }
            
        }
    }
    return true;
    
};



bool Field::solution(){
    
    fill_fitting();
    
    while(contains_zero() || !validate_field()){
        start_guessing();
        if(!fill_fitting()){
            reset_field();
        }
    }

    return true;
}

void Field::reset_field(){
    for(int y = 0; y < 9;y++){
        for (int x = 0; x < 9; x++){
            Base *b = &field[y][x];
            if(!b->hard){
                b->value = 0;
                b->change_able = true;
            }
        }
    }
}

void Field::print_field(){
    if (!field_is_set){
        printf("Field not set now! \n");
    }
    
    for(int y = 0; y < 9;y++){
        for (int x = 0; x < 9; x++){
            if(x%3==0){
                cout << "|";
            }            Base *b = &field[y][x];
            cout<< b->value;
        }
        cout << "|";
        cout << endl;
    }
    cout << endl;
};

bool Field::validate_field(){
    
    for(int y = 0; y < 3; y++){
        for(int x = 0; x < 3;x++){
            if(!tbt[y][x].validate()){
                return false;
            }
        }
    }
    
    for (int x = 0;x<9;x++){
        if(!x_lines[x].validate()){
            return false;
        }
        if(!y_lines[x].validate()){
            return false;
        }
    }
    return true;
}

void Field::create_line_ptr(){
    for (int y = 0; y < 9; y++){
        LineY ly = LineY();
        LineX lx = LineX();
        for (int x = 0; x < 9; x++){
            Base *ptrX = &field[y][x];
            Base *ptrY = &field[x][y];
            ly.line[x]=ptrY;
            lx.line[x]=ptrX;
        }
        y_lines.push_back(ly);
        x_lines.push_back(lx);
    }
}

void Field::create_tbt(){
    
    int offset_x=0;
    int offset_y=0;
    int counter = 0;
    while(counter < 81){
        
        ThreeByThree three = ThreeByThree();
        
        for (int y = 0; y < 3; y++){
            for (int x = 0; x < 3; x++){
                int pos_y = y + (offset_y*3);
                int pos_x = x + (offset_x*3);
                Base *b = &field[pos_y][pos_x];
                three.three_by_three[y][x] = b;
                counter++;
            }
        }
        
        three.build_as_line();
        tbt[offset_y][offset_x] = three;
        
        if (offset_x == 2){
            offset_y++;
            offset_x = 0;
        }else{
            offset_x++;
        }
    }
};

#endif /* Fields_h */
