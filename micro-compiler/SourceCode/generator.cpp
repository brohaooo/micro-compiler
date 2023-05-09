#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <ctype.h>
#include <unordered_map>
#include <functional>
#include "generator.h"

// a quick reference to zero&v0&a0(for syscall) register's number (我定义的，方便指代)
#define ZERO 10
#define V0 11
#define A0 12

// ID(variable)-address(7fffffff-10008000)/register(0-31) table
// in this assignment, once ID is created, it will exist forever

// struct HashFunc
// {
//     std::size_t operator()( const char key[32] ) const
//     {
//         using std::size_t;
//         using std::hash;

//         return std::hash<char>()(key[0]);
           
//     }
// };

// struct EqualKey
// {
//     bool operator () (const char lhs[32], const char rhs[32]) const
//     {
//         return lhs[0] == rhs[0]
//             && lhs[1] == rhs[1]
//             && lhs[2] == rhs[2];
//     }
// };


std::unordered_map<std::string, int> ID_lookup_table;
std::unordered_map<std::string, int>::iterator iter;


// shows register's tmp part occupation
int tmp_reg_table[10] = {0,0,0,0,0,0,0,0,0,0};
// shows register's return value(from either expression or function) part occupation
int value_reg_table[2] = {0,0};

// all the tmp variable or ID variable will never be eliminated once created, thus the stack pointer will be mono-decreasing every time new variable is created
// 7ffffff->10008000
// int stack_mem_pointer = 2147483647;
int stack_mem_pointer = 268468224+200;

void move_stack_mem_pointer_by_8(){
    stack_mem_pointer-=8;
}


// memory simulation
std::unordered_map<int, int> memory;


int find_ID(char key[32]){
    // printf("!!!!! %s",key);
    std::string key_str = key;
    iter = ID_lookup_table.find(key_str);
    if(iter == ID_lookup_table.end()){
        // printf("motherfucker");
        return -1;
    }
    return iter->second;
}
int store_ID(char key[32]){
    std::string key_str = key;
    int address = stack_mem_pointer;
    ID_lookup_table.insert({key_str,address});

    //here generate some mips code for store 0 in target address
    // reg pos $2 is set to be a constant zero here
    // memory[address] = 0;
    int from_reg = use_a_tmp_reg();
    mips_load_immediate(from_reg,0);
    mips_store_word(from_reg,ZERO,address);
    free_a_tmp_reg(from_reg);
    //move the sp to lower place
    move_stack_mem_pointer_by_8();
    return address;
}


void char_copy(char * from,char * to,int length){
    for(int i = 0;i < length;i++){
        to[i] = from[i];
    }
}




int use_a_tmp_reg(){
    int pos = 0;
    while(pos < 10){
        if(tmp_reg_table[pos]==0){
            tmp_reg_table[pos] = 1;
            break;
        }
        pos++;
    }
    if(pos >= 10){
        pos = -1;
    }
    return pos;
}

void free_a_tmp_reg(int reg_num){
    tmp_reg_table[reg_num]=0;
}

void free_all_tmp_reg(){
    for(int pos = 0;pos < 10;pos++){
        tmp_reg_table[pos]=0;
    }

}



int load_into_register(int val,int val_type,bool is_negative){
    int reg_number = 0;
    int immediate_value;
    switch(val_type)
    {
        case 1  :
            // already in register, only need to revert the sign if is negative
            reg_number = val;
            if(is_negative){
                // MIPS CODE: add -reg
                //...
                mips_subtract(reg_number,ZERO,reg_number);
            }
            break;
        case 2  :
            // load from memory into a tmp reg
            
            // memory[val];
            reg_number = use_a_tmp_reg();
            // MIPS CODE: lw
            //...
            mips_load_word(reg_number,ZERO,val);
            if(reg_number==-1){
                printf("!!! ERROR, running out of tmp register");
            }
            // revert the sign if is negative
            if(is_negative){
                // MIPS CODE: add -reg
                //...
                mips_subtract(reg_number,ZERO,reg_number);
            }
            break;
        case 3  :
            // write integer into a tmp reg
            immediate_value = val;
            if(is_negative){
                immediate_value = -immediate_value;
            }
            reg_number = use_a_tmp_reg();
            // MIPS CODE: lw
            //...
            mips_load_immediate(reg_number,immediate_value);
            break;
        default :
            printf("ERROR, unrecognized expression's sub type");
    }
    return reg_number;
}


int store_into_memory(int val,int val_type,bool is_negative){
    int address = -1;
    int immediate_value;//used for store immediate int value
    int reg_number = 0;
    switch(val_type)
    {
        case 1  :
            // value is in register, need to use sw to store in stack mem, and free that old reg
            reg_number = val;
            if(is_negative){
                // MIPS CODE: sub zero reg
                mips_subtract(reg_number,ZERO,reg_number);
            }
            // allocate an address
            address = stack_mem_pointer;
            move_stack_mem_pointer_by_8();

            // MIPS CODE: sw reg address
            //...
            mips_store_word(reg_number,ZERO,address);

            free_a_tmp_reg(reg_number);
            break;
        case 2  :
            // value is already in memory, do nothing, if is negative then take it out, make it negative, store it back
            
            address = val;
            // revert the sign if is negative
            if(is_negative){
                // MIPS CODE: lw reg; add -reg
                //...
                // memory[address] = -memory[address];
                reg_number = use_a_tmp_reg();
                mips_load_word(reg_number,ZERO,address);
                mips_subtract(reg_number,ZERO,reg_number);
                
                // MIPS CODE: sw reg
                //...
                mips_store_word(reg_number,ZERO,address);
                free_a_tmp_reg(reg_number);
            }
            // do nothing

            break;
        case 3  :
            // value is immediate int, need to use sw to store in stack mem
            // allocate an address
            address = stack_mem_pointer;
            move_stack_mem_pointer_by_8();
            immediate_value = val;
            if(is_negative){
                immediate_value = -immediate_value;
            }
            // MIPS CODE: addi reg zero imm, sw reg address
            //...
            // memory[address] = immediate_value;
            reg_number = use_a_tmp_reg();
            mips_load_immediate(reg_number,immediate_value);
            mips_store_word(reg_number,ZERO, address);
            free_a_tmp_reg(reg_number);
            break;
            
        default :
            printf("ERROR, unrecognized expression's sub type");
    }
    return address;



}




int do_plus_or_minus_reg_calculation(int lhs_reg,int rhs_reg,char op){
    int result_reg_number = use_a_tmp_reg();
    if(result_reg_number==-1){
        printf("!!! ERROR, running out of tmp register");
        free_a_tmp_reg(lhs_reg);
        free_a_tmp_reg(rhs_reg);
        return -1;
    }
    if(op=='+'){
        // MIPS CODE: add
        // ...
        mips_add(result_reg_number,lhs_reg,rhs_reg);
    }
    else if(op=='-'){
        // MIPS CODE: sub
        // ...
        mips_subtract(result_reg_number,lhs_reg,rhs_reg);
    }
    else{
        printf("!!! ERROR, runrecognized expression's op type");
        free_a_tmp_reg(lhs_reg);
        free_a_tmp_reg(rhs_reg);
        return -1;
    }

    free_a_tmp_reg(lhs_reg);
    free_a_tmp_reg(rhs_reg);
    
    return result_reg_number;


}



void statement_ID_assignment(int ID_address,int val,int val_type,bool is_negative){
    int reg_number = 0;
    int immediate_value;
    int val_address = -1;
    if(val_type==1){
        // save the register's value to the ID's address
        // MIPS CODE: sw reg address
        //...
        reg_number = val;
        if(is_negative){
                // MIPS CODE: sub zero reg
                mips_subtract(reg_number,ZERO,reg_number);
            }
        mips_store_word(reg_number,ZERO,ID_address);

        free_a_tmp_reg(reg_number);

    }
    else if(val_type==2){
        // read value from mem to a register, then save the register's value to the ID's address
        // MIPS CODE: lw address reg; sw reg address
        //...
        printf("!!!!!!!!!!!!!\n");
        val_address = val;
        reg_number = use_a_tmp_reg();
        mips_load_word(reg_number,ZERO,val_address);
        
        if(is_negative){
                // MIPS CODE: sub zero reg
                mips_subtract(reg_number,ZERO,reg_number);
            }
        mips_store_word(reg_number,ZERO,ID_address);

        free_a_tmp_reg(reg_number);

    }
    else if(val_type==3){
        // save the value to the ID's address
        // generate code...
        // MIPS CODE: add reg zero int; sw reg address
        //...
        immediate_value = val;
        reg_number = use_a_tmp_reg();
        
        
        if(is_negative){
                immediate_value = -immediate_value;
            }

        mips_load_immediate(reg_number,immediate_value);
        mips_store_word(reg_number,ZERO,ID_address);

        free_a_tmp_reg(reg_number);

        // memory[address] = ($3).return_value;
    }
    else{
        printf("ERROR, unrecognized expression type");
    }


}
void statement_read_ID(int ID_address){// use syscall's read_int
    mips_load_immediate(V0,5);
    mips_syscall();
    mips_store_word(V0,ZERO,ID_address);

}
void statement_write_expression(int exp_address){// use syscall's print_int
    mips_load_immediate(V0,1);
    mips_load_word(A0,ZERO,exp_address);
    mips_syscall();
}

void write_comma(){
    mips_load_immediate(V0,11);
    mips_load_immediate(A0,44);
    mips_syscall();
}

void write_left_paren(){
    mips_load_immediate(V0,11);
    mips_load_immediate(A0,28);
    mips_syscall();

}
void write_right_paren(){
    mips_load_immediate(V0,11);
    mips_load_immediate(A0,29);
    mips_syscall();

}



// std::unordered_map<int, std::string> Registers
//     {
//     {0,"$zero"},{1,"X"},{2,"$v0"},{3,"$v1"},{4,"$a0"},
//     {5,"$a1"},{6,"$a2"},{7,"$a3"},{8,"$t0"},{9,"$t1"},
//     {10,"$t2"},{11,"$t3"},{12,"$t4"},{13,"$t5"},{14,"$t6"},
//     {15,"$t7"},{16,"$s0"},{17,"$s1"},{18,"$s2"},{19,"$s3"},
//     {20,"$s4"},{21,"$s5"},{22,"$s6"},{23,"$s7"},{24,"$t8"},
//     {25,"$t9"},{26,"X"},{27,"X"},{28,"X"},{29,"X"},
//     {30,"X"},{31,"$ra"}
//     };

// use this one instead, since I don't need that much registers right now
std::unordered_map<int, std::string> Registers
    {
    {0,"$t0"},{1,"$t1"},{2,"$t2"},{3,"$t3"},{4,"$t4"},
    {5,"$t5"},{6,"$t6"},{7,"$t7"},{8,"$t8"},{9,"$t9"},
    {10,"$zero"},{11,"$v0"},{12,"$a0"}
    };

void mips_load_word(int target_reg,int pos_reg,int shift){
    std::string target_reg_code = Registers[target_reg];
    std::string pos_reg_code = Registers[pos_reg];
    printf("lw %s,%i(%s)\n",target_reg_code.c_str(),shift,pos_reg_code.c_str());
}

void mips_store_word(int from_reg,int pos_reg,int shift){
    std::string from_reg_code = Registers[from_reg];
    std::string pos_reg_code = Registers[pos_reg];
    printf("sw %s,%i(%s)\n",from_reg_code.c_str(),shift,pos_reg_code.c_str());
}

void mips_load_immediate(int reg_number,int immediate_val){
    std::string reg_code = Registers[reg_number];
    printf("li %s,%i\n",reg_code.c_str(),immediate_val);
}

void mips_add(int out_reg,int in_reg1,int in_reg2){
    std::string out_reg_code = Registers[out_reg];
    std::string in_reg1_code = Registers[in_reg1];
    std::string in_reg2_code = Registers[in_reg2];
    printf("add %s,%s,%s\n",out_reg_code.c_str(),in_reg1_code.c_str(),in_reg2_code.c_str());
}

void mips_subtract(int out_reg,int in_reg1,int in_reg2){
    std::string out_reg_code = Registers[out_reg];
    std::string in_reg1_code = Registers[in_reg1];
    std::string in_reg2_code = Registers[in_reg2];
    printf("sub %s,%s,%s\n",out_reg_code.c_str(),in_reg1_code.c_str(),in_reg2_code.c_str());
}

void mips_add_immediate(int out_reg,int in_reg1,int immediate_val){
    std::string out_reg_code = Registers[out_reg];
    std::string in_reg1_code = Registers[in_reg1];
    printf("addiu %s,%s,%i\n",out_reg_code.c_str(),in_reg1_code.c_str(),immediate_val);
}

void mips_syscall(){
    printf("syscall\n");
}