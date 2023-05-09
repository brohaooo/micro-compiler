#ifndef _generator
#define _generator



#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <ctype.h>
#include <unordered_map>

void assign_ID();

void read_ID();

void write_expr();

void calculate_exp();

//symbol_table, saved_register_table, tmp_register_table, bridge_register_table, register_check???





// primary
struct PRIM{
    bool is_negative; // if is - xxx, set this flag as true
    int return_type; // 1 for register, 2 for mem address, 3 for integer
    int return_value; // its return variable's register ID / memory address, will automatically
    
};
// expression
struct EXPR{
    // for expression, it has 2 situation
    // if is X op X type, it will do calculation and store result into one register
    // if is prim type, do nothing just pass the stuff into its root
    bool is_negative;
    int return_type; // 1 for register, 2 for mem address, 3 for integer
    int return_value; // its return variable's register ID / memory address, will automatically

};

// expression list
struct EXPR_LIST{
    // since keeping all the expression in registers will probably consumes all the register
    // I decide to store all the exp on expr list into memory
    int list_length; // 1-10
    int expressions_address[10];
};
// id list
struct ID_LIST{
    int list_length; // 1-10;
    char id[10][32]; // no more than 10 ID in one read()
};


extern std::unordered_map<std::string, int> ID_lookup_table;
extern std::unordered_map<std::string, int>::iterator iter;


// shows register's tmp part occupation, 0 means free, 1 means used
extern int tmp_reg_table[10];
// shows register's return value(from either expression or function) part occupation
extern int value_reg_table[2];

// all the tmp variable or ID variable will never be eliminated once created, thus the stack pointer will be mono-decreasing every time new variable is created
// 7ffffff->10008000
extern int stack_mem_pointer;

void move_stack_mem_pointer_by_8();

// memory simulation
extern std::unordered_map<int, int> memory;


// try to find the ID in the memory address table, return address position (7fffffff-10008000), return -1 if no exist
int find_ID(char key[32]);

// store ID in the memory address table, return address position (7fffffff-10008000), return -1 if no exist
int store_ID(char key[32]);

// store expression in the register table, return register number(8-15&24-25, $t0 - $t9), return -1 if register is full
// I set 24-25 register as operation register, meaning they will be expired immediately after operation
int store_tmp_in_registers(char key[32]);



// store ID in the memory, return memory address position(7fffffff-10008000), return -1 if memory is full
int store_tmp_in_memory(char key[32]);


// not implemented here...
bool tmp_reg_are_full();
bool value_reg_are_full();


// fuck C and C++, this function copy char array between two arrays
void char_copy(char * from,char * to,int length);


// this function will look for a free tmp reg(0-9) and sign it as 'occupied', will return its number or -1 if all reg is full
int use_a_tmp_reg();

// free an occupied tmp reg
void free_a_tmp_reg(int reg_num);

// free all tmp reg
void free_all_tmp_reg();

// get a value(reg,mem,int) into a tmp register, return the used register number
int load_into_register(int val,int val_type,bool is_negative);

// take two reg number as input, generate MIPS operation code, return result reg number
int do_plus_or_minus_reg_calculation(int lhs_reg,int rhs_reg,char op);



// store a value(reg,mem,int) into the memory, return memory address
int store_into_memory(int val,int val_type,bool is_negative);


// high level statement's MIPS code generation
void statement_ID_assignment(int ID_address,int val,int val_type,bool is_negative);//assign ID the expression's value (reg/mem/imm int) in ID's mem address
void statement_read_ID(int ID_address);//use syscall, read the input value and assign it to the ID in its mem address
void statement_write_expression(int exp_address);//use syscall, write the address's stored value to the terminal. (it only support mem type exp, because I've restricted exp list to be in mem only)

void write_comma();//use syscall, write ','
void write_left_paren();//use syscall, write '('
void write_right_paren();//use syscall, write ')'


// low level MIPS code generation
void mips_load_word(int target_reg,int pos_reg,int shift);//reg_number = MEM[reg_number+shift]
void mips_store_word(int from_reg,int pos_reg,int shift);//MEM[reg_number+shift] = reg_number
void mips_load_immediate(int reg_number,int immediate_val);//reg_number = immediate_val
void mips_add(int out_reg,int in_reg1,int in_reg2);//out_reg = in_reg1+in_reg2
void mips_subtract(int out_reg,int in_reg1,int in_reg2);//out_reg = in_reg1-in_reg2
void mips_add_immediate(int out_reg,int in_reg1,int immediate_val);//out_reg = in_reg1+immediate_val
void mips_syscall();//syscall

//MIPS registers
extern std::unordered_map<int, std::string> Registers;


#endif