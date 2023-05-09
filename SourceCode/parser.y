%{
    #include <stdio.h>
    #include <iostream>
    #include <string>
    #include <ctype.h>
    #include "generator.h"
    int yylex();
    int yyerror(const char *s);
    extern FILE *yyin;
%}
%token _BEGIN _END _READ _WRITE _LPAREN _RPAREN _SEMICOLON _COMMA _ASSIGN_OP _PLUOP _MINUSOP _SCANEOF

%union {
    char id_struct[32];
    int int_struct;
    struct EXPR expr_struct;
    struct EXPR_LIST expr_list_struct;
    struct PRIM prim_struct;
    struct ID_LIST id_list_struct;
    char add_op_struct;
}

%start system_goal
%token <int_struct> _INTLITERAL
%token <id_struct> _ID
%type <id_list_struct> id_list
%type <prim_struct> primary
%type <expr_list_struct> expr_list
%type <expr_struct> expression
%type <add_op_struct> add_op



%%


    system_goal     : program _SCANEOF {printf("--------------end-------------\n");return 0;}
                    ;

    program         : _BEGIN statement_list _END {/*printf("\nprogram\n");*/}
                    ;

    statement_list  : statement {free_all_tmp_reg();}
                    | statement_list statement {free_all_tmp_reg();}
                    ;

    statement       : _ID _ASSIGN_OP expression _SEMICOLON {
                    // printf("\nassign statement\n");
                    // ID address, will automatically create one if such ID doesn't exist
                    int address;
                    address = find_ID($1);
                    if(address==-1){
                        address = store_ID($1);
                    }
                    int exp_type = ($3).return_type;
                    int exp_val = ($3).return_value;
                    int exp_is_negative = ($3).is_negative;
                    statement_ID_assignment(address,exp_val,exp_type,exp_is_negative);
                    
                    
                    }
                    | _READ _LPAREN id_list _RPAREN _SEMICOLON {
                    // printf("\nread statement\n");
                    for(int i = 0; i < ($3).list_length; i++){
                        // int input_val;
                        // printf("input value:");
                        // std::cin>>input_val;
                        
                        int address;
                        char ID[32]; 
                        char_copy(($3).id[i],ID,32); 
                        address = find_ID(ID);
                        if(address==-1){
                            address = store_ID(ID);
                        }
                        statement_read_ID(address);
                        // memory[address] = input_val;
                        
                    }
                    }
                    | _WRITE _LPAREN expr_list _RPAREN _SEMICOLON {
                    // 1. one by one, load expr result from memory to a same register
                    // 2. use syscall to print the outcome
                    // 3. when all over, free the register
                    // write_left_paren();
                    for(int i = 0; i < ($3).list_length; i++){
                        int address;
                        address = ($3).expressions_address[i];
                        statement_write_expression(address);
                        if(i!=($3).list_length-1){
                            write_comma();
                        }
                    }
                    // write_right_paren();


                    }
                    ;

    id_list         : _ID {
                    // id list will automatically initialize new ID in the memory with value = 0; 
                    int address;
                    address = find_ID($1);
                    if(address==-1){
                        address = store_ID($1);
                    }
                    ($$).list_length = 1;
                    char_copy(($1),($$).id[($$).list_length-1],32);
                    ;
                    }
                    | id_list _COMMA _ID {
                    int address;
                    address = find_ID($3);
                    if(address==-1){
                        address = store_ID($3);
                    }
                    ($$).list_length += 1;
                    char_copy(($3),($$).id[($$).list_length-1],32);
                    ;
                    }
                    ;

    expr_list       : expression {
                    // printf("\nexpression list detected\n");
                    // pass expression's address & val type to expr_list, and generate code to put them into mem (to prevent a long long list consumes all the registers)
                    ($$).list_length = 1;
                    int address = 666;
                    int exp_val = ($1).return_value;
                    int exp_val_type = ($1).return_type;
                    bool exp_is_negative = ($1).is_negative;
                    address = store_into_memory(exp_val,exp_val_type,exp_is_negative);
                    ($$).expressions_address[($$).list_length-1]=address;
                    

                    }
                    | expr_list _COMMA expression {
                    // printf("\nexpression list detected\n");
                    // do the same thing on latter expression, store address on the list
                    ($$).list_length += 1;
                    int address = 666;
                    int exp_val = ($3).return_value;
                    int exp_val_type = ($3).return_type;
                    bool exp_is_negative = ($3).is_negative;
                    address = store_into_memory(exp_val,exp_val_type,exp_is_negative);
                    ($$).expressions_address[($$).list_length-1]=address;


                    }
                    ;

    expression      : primary {
                    /* give primary's reg/ mem address/ int val and +- sign to expression */
                    // printf("\n + \n");
                    ($$).is_negative = ($1).is_negative;
                    ($$).return_type = ($1).return_type;
                    ($$).return_value = ($1).return_value;
                    ;
                    }
                    | expression add_op primary {
                    /* generate calculation code, store result in one reg, give reg to root expression */
                    ($$).is_negative = false; // the negative sign is removed and the register will store the negative result
                    int lhs_type = ($1).return_type;
                    int rhs_type = ($3).return_type;
                    int lhs_is_negative = ($1).is_negative;
                    int rhs_is_negative = ($3).is_negative;
                    int lhs_value = ($1).return_value;
                    int rhs_value = ($3).return_value;
                    int lhs_reg = load_into_register(lhs_value,lhs_type,lhs_is_negative);
                    int rhs_reg = load_into_register(rhs_value,rhs_type,rhs_is_negative);
                    int result_reg = do_plus_or_minus_reg_calculation(lhs_reg,rhs_reg,($2));

                    ($$).return_type = 1;
                    ($$).return_value = result_reg;

                    }
                    ;

    primary         : _LPAREN expression _RPAREN {
                    /* give exp reg to root primary */
                    ($$).is_negative = ($2).is_negative;
                    ($$).return_type = ($2).return_type;
                    ($$).return_value = ($2).return_value;
                    ;
                    } 
                    | _ID {
                    /* create ID(=0) in mem, give mem address to root primary */
                    int address;
                    address = find_ID($1);
                    if(address==-1){
                        address = store_ID($1);
                    }
                    ($$).is_negative = false;
                    ($$).return_type = 2;
                    ($$).return_value = address;
                    ;
                    }
                    | _INTLITERAL {
                    /* give int val to root primary */
                    ($$).is_negative = false;
                    ($$).return_type = 3;
                    ($$).return_value = ($1);
                    ;
                    }
                    | _PLUOP primary 
                    {/* give '+' & primary to root primary */
                    ($$).is_negative = ($2).is_negative;
                    ($$).return_type = ($2).return_type;
                    ($$).return_value = ($2).return_value;
                    }
                    | _MINUSOP primary 
                    {
                    /* give '-'' & primary to root primary */
                    ($$).is_negative = !($2).is_negative;
                    ($$).return_type = ($2).return_type;
                    ($$).return_value = ($2).return_value;
                    }
                    ;
    
    add_op          : _PLUOP {$$ = '+';/* give '+'' to add_op */}
                    | _MINUSOP {$$ = '-';/* give '-'' to add_op */}
                    ;

    



    

%%
int yyerror(const char *s){
    fprintf(stderr,"error:%s\n",s);
    return 0;
}

int yywrap(){
    return 1;
}
int main(int argc, char const *argv[])
{
    yyin = fopen(argv[1],"r");
    if(yyin){
        printf("-----generated MIPS code:-----\n");
    }
    yyparse();
    // printf("--------------end-------------\n");
}
