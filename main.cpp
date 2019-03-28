#include <iostream>
#include "Fields.h"
#include <string>
#include <time.h>
#include <math.h>

int main(int argc, const char * argv[]) {
    
    if(argv[0]&&argv[1]){
        
        string path =argv[0];
        string export_path = argv[1];
        
        clock_t t;
        t = clock();
        
        Field field = Field(path);
        field.print_field();
        t = clock() - t;
        float time_consumed = ((float)t)/CLOCKS_PER_SEC;
        printf ("Find Sudoku tooked %f seconds.\n",time_consumed);
        field.export_solution(export_path);
        
    }else{
        
        cout << "paths needed as arguments s_res_mem input.txt export.txt" << endl;
        return 1;
        
    }
    return 0;
}
