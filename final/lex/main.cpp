#include"assistv6.h"

FILE * sptr= fopen ("/Users/mac/Desktop/resource/s3.txt", "r");//????????
FILE * mptr= fopen ("/Users/mac/Desktop/resource/MarkTable.txt", "w+");//??????????????????????
// FILE * markptr= fopen ("./FMarkTable.txt", "w+");//???????
// FILE * stptr= fopen ("./Fsdout.txt", "w+");//??????





int main()
{
    printf("start\n");
    analysis();
    fclose(sptr);
    fclose(mptr);
}

void analysis()//??????????
{
    tmark=(Z_MarkT*)malloc(sizeof(Z_MarkT));
    memset(tmark->mark,0,sizeof(tmark->mark));
    memset(tmark->value,0,sizeof(tmark->value));
    state = 0;
    X_set_keyword();
    memset(NUM_buffer,0,sizeof(NUM_buffer));
    get_char();
    state = Z_check_type();
    do {
        //
        switch(state){
            case Z_NUM: //????
            Z_analy_num();
            break;

            case Z_SPECIAL://???????
            Z_special();
            break;

            case Z_WORD://?????/??????
            analysis_word();
            break;

            case Z_others://???or??????
            Z_other_ch();
            break;

            case Z_ERROR://????
            printf("????");
            break;

            default://??????
            printf("????");
            exit(0);
        }


    }while(C!=EOF);
    return ;
}

void get_char()//
{
    C=fgetc(sptr);
    charnum++;
    return;
}
int Z_store_in_buffer(char ch)//
{
    NUM_buffer[buffer_len]=ch;
    buffer_len++;
    return 0;
    return 1;
    //
}
void retract()//
{
    fseek ( sptr, -1, SEEK_CUR );
    return ;
}


int error()
{
    //????????
    return Z_ERROR;
}
int Z_check_type()//???????
{
    if(letter(C))
        return Z_WORD;
    else if (digit(C))
        return Z_NUM;
    else if (special(C))
        return Z_SPECIAL;
    else if (others(C))
        return  Z_others;

    return 0;
}

bool letter(char ch)            //??????????????
{
    if ((ch >= 'A' && ch <= 'Z') ||
        (ch >= 'a' && ch <= 'z'))
    {
        return true;
    }
    return false;
}
bool digit(char ch)            //???????????????
{
    if (ch >= '0' && ch <= '9')
    {
        return true;
    }
    return false;
}
bool special(char ch)
{
    int i;
    for (i=0;i<18;i++)
    {
        if(ch == sp_array[i])
        {
            return true;
        }
    }
    return false;
}
bool others(char ch)
{
    if(ch==' '||ch=='\n'||ch=='\t')
    return true;
    return false;
}
void change_state(char ch)
{
    if(letter(ch))
        state=Z_WORD;
    else if (digit(ch))
        state= Z_NUM;
    else if (special(ch))
        state=Z_SPECIAL;
    else if (others(ch))
        state=Z_others;
}
int Z_analy_num()
{
    int E_flag=0;
    int D_flag=0;
    int num_state=0;
    int de_flag=0;
    memset(NUM_buffer,0,sizeof(NUM_buffer));
    buffer_len=0;
    de_flag=0;
    while(true)
    {
        switch(num_state){
        case 0://????
        {
            while(digit(C))//?????????????//????
            {
                Z_store_in_buffer(C);
                get_char();
            }
            if(C=='.') num_state=1;
            else if (C=='(')
            {
                num_state=2;
            }
            else num_state=3;//???
            
        }break;

        case 1://????
        {
            Z_store_in_buffer(C);
            get_char();
            if(digit(C))
            {
                while(digit(C))//?????????????//????
            {
                Z_store_in_buffer(C);
                get_char();
            }
            if (C=='(') num_state=2;
            else num_state=3;//???
            }
            else
            {
                if (C=='.')
                {
                    de_flag=1;
                    num_state=3;
                }
                else
                num_state=4;
            }
            
        }break;

        case 2://??? (E(+|-|c)digit*)
        {
            Z_store_in_buffer(C);
            get_char();
            if(C!='E')//E
            {
                num_state=4;
                break;
            }
            Z_store_in_buffer(C);
            get_char();
            if(C!='(')//(
            {
                num_state=4;
                break;
            }
            Z_store_in_buffer(C);
            get_char();
            if(C=='-'||C=='+')//+|-
            {
                Z_store_in_buffer(C);
                get_char();
            }
            if(C!=')')//)
            {
                num_state=4;
                break;
            }
            Z_store_in_buffer(C);
            get_char();
            if(digit(C))
            {
                while(digit(C))//?????????????//????
                {
                    Z_store_in_buffer(C);
                    get_char();
                }
                if(C!=')')
                {
                    num_state=4;
                    break;
                }
                Z_store_in_buffer(C);
                get_char();
                num_state=3;
            }
            else num_state=4;
            
        }break;

        case 3://???
        {
            change_state(C);
            // printf("< num,%s>\n",NUM_buffer);
            strcpy(tmark->mark,"num");
            if(de_flag==1)
            {
                int templ=strlen(NUM_buffer);
                NUM_buffer[templ-1]=0;
            }
            strcpy(tmark->value,NUM_buffer);
            tmark->valuetype=Z_NUM;
            Z_MarkTable();
            if(de_flag==1)
            {
                // change_state(C);
                get_char();
                change_state(C);
                strcpy(tmark->mark,"Sub_D_Mark");
                strcpy(tmark->value,"..");
                tmark->valuetype=Z_SPECIAL;
                Z_MarkTable();
            }
            return 2;
        }break;

        case 4://????
        printf("a fucking error!\n");
        break;

    }

    }
    
    
    return 0;
}

int Z_special()//???????????
{
    int Z_result=0;
    char tch[2];
    memset(tch,0,sizeof(tch));
    memset(Z_lable,0,sizeof(Z_lable));
    memset(NUM_buffer,0,sizeof(NUM_buffer));
    if(is_equal>=1&&is_equal<3)
    is_equal--;
    switch(C){
        case '+':
        {
            if(is_equal>=1)
            {
                // is_equal=0;
                strcpy(Z_lable,"signal");

            }
            else
            {
                strcpy(Z_lable,"addop");
            }
                tch[0]=C;
                strcpy(NUM_buffer,tch);
                Z_result=Z_SP_OUT;
                get_char();
            
        }break;
        case '-':
        {
            if(is_equal>=1)
            {
                // is_equal=0;
                strcpy(Z_lable,"signal");
            }
            else
            {
                strcpy(Z_lable,"addop");
                
            }
                tch[0]=C;
                strcpy(NUM_buffer,tch);
                Z_result=Z_SP_OUT;
                get_char();
        }break;
        case '/':
        {
            strcpy(Z_lable,"mulop");
            tch[0]=C;
            strcpy(NUM_buffer,tch);
            Z_result=Z_SP_OUT;
            get_char();
        }break;
        case ',':
        {
            strcpy(Z_lable,"DeMark");
            tch[0]=C;
            strcpy(NUM_buffer,tch);
            Z_result=Z_SP_OUT;
            get_char();
        }break;
        case ';':
        {
            strcpy(Z_lable,"DeMark");
            tch[0]=C;
            strcpy(NUM_buffer,tch);
            Z_result=Z_SP_OUT;
            get_char();
        }break;
        case ')':
        {
            strcpy(Z_lable,"DeMark");
            tch[0]=C;
            strcpy(NUM_buffer,tch);
            Z_result=Z_SP_OUT;
            get_char();
        }break;
        case '[':
        {
            is_equal=3;
            strcpy(Z_lable,"DeMark");
            tch[0]=C;
            strcpy(NUM_buffer,tch);
            Z_result=Z_SP_OUT;
            get_char();
        }break;
        case ']':
        {
            is_equal=0;
            strcpy(Z_lable,"DeMark");
            tch[0]=C;
            strcpy(NUM_buffer,tch);
            Z_result=Z_SP_OUT;
            get_char();
        }break;
        case '{':
        {
            strcpy(Z_lable,"S_DeMark");
            tch[0]=C;
            strcpy(NUM_buffer,tch);
            Z_result=Z_SP_OUT;
            get_char();
            if_note=2;
        }break;
        case '}':
        {
            strcpy(Z_lable,"S_DeMark");
            tch[0]=C;
            strcpy(NUM_buffer,tch);
            Z_result=Z_SP_OUT;
            get_char();
            if_note=1;
        }break;
        case '=':
        {
            is_equal=2;
            if(is_const_or_type==1)
            {
                strcpy(Z_lable,"signal");
            }
            else
            strcpy(Z_lable,"relop");
            tch[0]=C;
            strcpy(NUM_buffer,tch);
            Z_result=Z_SP_OUT;
            get_char();
        }break;
        case '\'':
        {
            if(is_letter==1)
            {
                is_letter=0;
            }
            else
            {
                is_letter=1;
            }
            strcpy(Z_lable,"S_DeMark");
            tch[0]=C;
            strcpy(NUM_buffer,tch);
            Z_result=Z_SP_OUT;
            get_char();
        }break;
        //?????
        case '<':
        {
            strcpy(Z_lable,"relop");
            tch[0]=C;
            get_char();
            if(C=='='||C=='>')
            {
                    is_equal=2;
                    tch[1]=C;
                    get_char();
            }
            strcpy(NUM_buffer,tch);
            Z_result=Z_SP_OUT;

        }break;
        case '>':
        {
            strcpy(Z_lable,"relop");
            tch[0]=C;
            get_char();
            if(C=='=')
            {
                    is_equal=2;
                    tch[1]=C;
                    get_char();
            }
            strcpy(NUM_buffer,tch);
            Z_result=Z_SP_OUT;
        }break;
        case ':':
        {
            
            tch[0]=C;
            get_char();
            if(C=='=')
            {
                    is_equal=2;
                    strcpy(Z_lable,"assignop");
                    tch[1]=C;
                    get_char();
//                    printf("\n***  %s  ***\n",NUM_buffer) ;
                    strcpy(NUM_buffer,tch);
                    NUM_buffer[3]='\0';
                    Z_result=Z_SP_OUT;
            }
            else
            {
                strcpy(Z_lable,"DeMark");
                strcpy(NUM_buffer,tch);
                Z_result=Z_SP_OUT;
            }

        }break;
        case '.':
        {
            tch[0]=C;
            get_char();
            if(C=='.')
            {
                tch[1]=C;
                strcpy(NUM_buffer,tch);
                Z_result=Z_SP_OUT;
                strcpy(Z_lable,"Sub_D_mark");
                get_char();
            }
            else{
                strcpy(NUM_buffer,tch);
                Z_result=Z_SP_OUT;
                strcpy(Z_lable,"DeMark");
                if(is_end==1)
                {
                    is_end++;
                }
                else{
                    is_end=0;
                }
                
            }
            
//notes
        }break;
        case '(':
        {
            tch[0]=C;
            get_char();
            if(C=='*')
            {
                tch[1]=C;
                strcpy(NUM_buffer,tch);
                Z_result=Z_SP_OUT;
                strcpy(Z_lable,"notes_mark");
                get_char();
                if_note=2;
            }
            else{
                // is_equal=2;
                strcpy(NUM_buffer,tch);
                Z_result=Z_SP_OUT;
                strcpy(Z_lable,"DeMark");
            }
        }break;
        case '*':
        {
            tch[0]=C;
            get_char();
            if(C==')')
            {
                tch[1]=C;
                strcpy(NUM_buffer,tch);
                Z_result=Z_SP_OUT;
                strcpy(Z_lable,"notes_mark");
                get_char();
                if_note=1;
            }
            else{
                strcpy(NUM_buffer,tch);
                Z_result=Z_SP_OUT;
                strcpy(Z_lable,"mulop");
                
            }
        }break;
    



        default:
        printf("??");
        get_char();
    }

    switch (Z_result)
    {
    case Z_SP_ERROR:
        printf("special errror!\n");
        return 1;
        break;

    case Z_SP_OUT:
        // printf("< %s,%s >\n",Z_lable,NUM_buffer);
        strcpy(tmark->mark,Z_lable);
        strcpy(tmark->value,NUM_buffer);
        tmark->valuetype=Z_SPECIAL;
        Z_MarkTable();
        if(is_end==2)
        {
            state=20;
        }
        else
        change_state(C);
        return 2;
    break;
    default:
    printf("unexpected error!");
    return 3;
    }
    return 0;
}
int Z_other_ch()
{
    
    // fprintf(stptr,"%c",C);
    get_char();
    change_state(C);
    return 0;
}

int Z_MarkTable()//????????
{
    int choice;
    choice=tmark->valuetype;
    wordtype_num++;
    switch (choice)
    {
        // stptr
    case Z_NUM:
        wordtype_num++;
        // fprintf(stptr,"%s ",tmark->mark);
        // fwrite(tmark, sizeof(Z_MarkT), 1, markptr);
        // fprintf(mptr,"(%s??%s)\n",tmark->mark,tmark->value);
        // printf("(%s??%s)\n",tmark->mark,tmark->value);
        break;
    case Z_SPECIAL:
        wordtype_sp++;
        // if(tmark->mark[0]=='r'||tmark->mark[0]=='a'||tmark->mark[0]=='m'||tmark->mark[0]=='l')
        // fprintf(stptr,"%s ",tmark->mark);
        // else
        // fprintf(stptr,"%s ",tmark->value);
        // fwrite(tmark, sizeof(Z_MarkT), 1, markptr);
        // fprintf(mptr,"(%s??%s)\n",tmark->mark,tmark->value);
        // printf("(%s??%s)\n",tmark->mark,tmark->value);
        break;
    case Z_ID:
        wordtype_id++;
        // fprintf(stptr,"%s ",tmark->mark);
        // fwrite(tmark, sizeof(Z_MarkT), 1, markptr);
        // fprintf(mptr,"(%s??%s)\n",tmark->mark,tmark->value);
        // printf("(%s??%s)\n",tmark->mark,tmark->value);
        break;
    case Z_KEY:
        wordtype_key++;
        // fprintf(stptr,"%s ",tmark->value);
        
        break;

    
    
    default:
        break;
    }
    if(if_note==0)
    {
        // fwrite(tmark, sizeof(Z_MarkT), 1, markptr);
        fprintf(mptr,"(%s,%s)\n",tmark->mark,tmark->value);
    // printf("(%s,%s)\n",tmark->mark,tmark->value);
    }
    if(if_note==1)
    {
        if_note--;
    }
    memset(tmark->mark,0,sizeof(tmark->mark));
    memset(tmark->value,0,sizeof(tmark->value));

    return 0;
}


//??
int is_keyword()       //???????????????????
{
    /*for(int  i = 0; i < num_keyword; i++)
    {
        printf("\n%d",i);
        if(strcmp(word,keyword[i]) == 0)
            return i;
    }*/
    //--------------------------//???????????????
    int len = strlen(word);
    int length;
    double probability[num_keyword];     //?????
    int chara;                  //???????????
    int niu = -1;               //???????????????
    double max = 0;             //?????????
    for(int i = 0; i < num_keyword; i++)
    {
        probability[i] = 0;
        chara = 0;
        length = strlen(keyword[i]);
        if(len == length)
        {
            for(int l = 0; l < len; l++)
                if(word[l] == keyword[i][l])
                    chara++;
            probability[i] = chara / len;
            if(probability[i] >= 0.75 && probability[i] > max)//??????  0.75  ????????????????????
            {
                if(i==15||i==19||i==20||i==26||i==28)
                {
                    is_const_or_type=0;
                }
                
                if(i==9||i==26)
                {
                    is_const_or_type=1;
                }
                if(i==13)
                {
                    is_end=1;
                }

                niu = i;
                max = probability[i];
            }
        }
    }
    return niu;
    //--------------------------//

    //return -1;          //????????
}
int store_in_buffer()  //??C???????????word?????????
{
    int len = strlen(word);//???????
    if(state == X_WORD)    //?????/???????
    {
        //printf("1%d,",len);//??????
        if(len > 9 && strcmp(word,keyword[19]) != 0)    //???????8??????procedure???????
        {
            //printf("666");
            return 0;       //??????????
        }
    }
    word[len]      = C;
    word[len+1]  = '\0';
    return 1;               //?????????
}

int analysis_word()//?????????
{
    if(is_end==1)
        {
            is_end=0;
        }
    if(is_equal<3)
    is_equal=0;
    int X_state=0;
    while(true)
    {
        switch(X_state)
        {
            case 0:                     //?????
                // printf("appear>>%c\n",C);
                strcpy(word,"");        //???????????
                store_in_buffer();      //??????????????
                get_char();             //???????????
                if(letter(C) || digit(C))
                    X_state = X_WORD;    //????????/???????
                else                    //??????????????
                {
                    if(is_letter==1)
                    {
                        strcpy(tmark->mark,"letter");
                    }
                    else
                    {
                        strcpy(tmark->mark,"id");
                    }
                    
                    strcpy(tmark->value,word);
                    tmark->valuetype=Z_ID;
                    Z_MarkTable();
                    // printf("< id,%s>\n",word);
                    X_state = X_OUT;
                }
                break;
            case X_WORD:                //?????/???????
                //........................????????????????
                if(store_in_buffer() == 0)//???????
                {
                    X_state = X_ERROR_LENGTH;
                    break;
                }
                get_char();             //???????????

                if(letter(C) || digit(C))
                    X_state = X_WORD;
                else
                {
                    X_state = X_KEYWORD;
                }
                /*else if(C == ' ' || C == '\n' || C == EOF)//????????????
                {
                    X_state = X_KEYWORD;//????????/??????????
                }
                else                    //??????????????????????
                {
                    X_state = X_ERROR_CHAR;//???????????????????????/???????????..........??????????????????C?????????????????
                }*/
                break;
            case X_KEYWORD:
                //????????????????????
                int iskey;
                iskey = 0;
                iskey = is_keyword();
                if(iskey == -1)                        //????????
                {
                    if(is_letter==1)
                    {
                        strcpy(tmark->mark,"letter");
                    }
                    else
                    {
                        strcpy(tmark->mark,"id");
                    }
                    strcpy(tmark->value,word);
                    tmark->valuetype=Z_ID;
                    Z_MarkTable();
                    // printf("< id,%s>\n",word);
                    //?????????...........<id,word>
                }
                else                                    //??????
                {
                    if(iskey >= 0 && iskey <= 2)              //and/mod/div
                    {
                        strcpy(tmark->mark,"mulop");
                        strcpy(tmark->value,word);
                        tmark->valuetype=Z_SPECIAL;
                        Z_MarkTable();
                        // printf("< mulop,%s>\n",word);
                        //?????????...........<mulop,word>
                    }
                    else if(iskey == 3)                 //or
                    {
                        strcpy(tmark->mark,"addop");
                        strcpy(tmark->value,word);
                        tmark->valuetype=Z_SPECIAL;
                        Z_MarkTable();
                        // printf("< addop,%s>\n",word);
                        //?????????...........<addop,word>
                    }
                    else                                //?????????
                    {
                        strcpy(tmark->mark,"keyword");
                        strcpy(tmark->value,word);
                        tmark->valuetype=Z_KEY;
                        Z_MarkTable();
                        // printf("< %d,%s>\n",iskey,word);
                        //?????????...........<????????????>???????????iskey
                    }
                }
                if(C == ' ' || C == '\n' || C == EOF)
                    X_state = X_OUT;              //????????
                else
                {
                    state = Z_check_type();
                    return state;
                }
                break;
            case X_ERROR_CHAR:          //?????/?????...????????
                char token[10];
                strcpy(token,word);
                int length;
                length = strlen(token);
                char tmp1;
                tmp1 = C;

                //???????????????????????????????????????????????????????
                while(C != ' ' || C != '\n')
                {
                    if(letter(C) || digit(C))
                    {
                        token[length]     = C;
                        token[length+1] = '\0';
                    }
                    get_char();
                    length = strlen(token);
                }
                if(length > 8 && strcmp(word,keyword[19]) != 0)    //???????8??????procedure???????
                {
                    //????????????????????????????????????????????
                    //????............??????
                    X_state = 0;
                }
                else
                    X_state = X_KEYWORD;  //??????????????
                break;
            case X_ERROR_LENGTH:        //?????/?????...?????????
                char tmp2;
                tmp2 = C;
                get_char();             //???????????
                if(!letter(C) || !digit(C))//?????????????
                {
                    //????............??????
                    X_state = X_KEYWORD;
                    break;
                }
                else
                {
                    //???????????
                    X_state = 0;
                }
                break;
            case X_OUT:
//                get_char();
//                state = Z_check_type();
                change_state(C);
                return state;
                break;
        }
    }
}
