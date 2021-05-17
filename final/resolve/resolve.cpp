#include"resolve.h"

vector<string> I; // 指令寄存器
int P; // 程序计数器
int B; // 基地址寄存器
int T; // 栈指针寄存器

vector<int> calc_stack;
vector<int> runtime_stack;

int cur_level;
vector<double> que;
vector<double> que1;

void handle_OPR(vector<string> this_code){
    int opr_num = atoi(this_code[2].c_str());
    switch(opr_num){
        default:
        {
            P++;
            return;
        }
        case 0:
        {
            P = runtime_stack[B + 2];
            B = runtime_stack[B + 1];
            int num = runtime_stack[B];
            for(int i = 0; i < num; i++){
                runtime_stack.pop_back();
            }
            break;
        }
        case 1:
        {
            int ptr = calc_stack[T - 1];
            data_space[ptr][0] = -data_space[ptr][0];
            P++;
            break;
        }
        case 2:
        {
            int ptr1 = calc_stack[T - 1];
            int ptr2 = calc_stack[T - 2];
            double num1 = data_space[ptr1][0];
            double num2 = data_space[ptr2][0];
            int type1 = (int)data_space[ptr1][1];
            int type2 = (int)data_space[ptr1][1];
            data_space[dataspace_len][0] = num1 + num2;
            if(type1 == _REAL || type2 == _REAL){
                data_space[dataspace_len][1] = _REAL;
            }
            else{
                data_space[dataspace_len][1] = _INTEGER;
            }
            for(int i = 0; i < 2; i++){
                calc_stack.pop_back();
                T--;
            }
            calc_stack.push_back(dataspace_len++);
            T++;
            P++;
            break;
        }
        case 3:
        {
            int ptr1 = calc_stack[T - 1];
            int ptr2 = calc_stack[T - 2];
            double num1 = data_space[ptr1][0];
            double num2 = data_space[ptr2][0];
            int type1 = (int)data_space[ptr1][1];
            int type2 = (int)data_space[ptr1][1];
            data_space[dataspace_len][0] = num2 - num1;
            if(type1 == _REAL || type2 == _REAL){
                data_space[dataspace_len][1] = _REAL;
            }
            else{
                data_space[dataspace_len][1] = _INTEGER;
            }
            for(int i = 0; i < 2; i++){
                calc_stack.pop_back();
                T--;
            }
            calc_stack.push_back(dataspace_len++);
            T++;
            P++;
            break;
        }
        case 4:
        {
            int ptr1 = calc_stack[T - 1];
            int ptr2 = calc_stack[T - 2];
            double num1 = data_space[ptr1][0];
            double num2 = data_space[ptr2][0];
            int type1 = (int)data_space[ptr1][1];
            int type2 = (int)data_space[ptr1][1];
            data_space[dataspace_len][0] = num2 * num1;
            if(type1 == _REAL || type2 == _REAL){
                data_space[dataspace_len][1] = _REAL;
            }
            else{
                data_space[dataspace_len][1] = _INTEGER;
            }
            for(int i = 0; i < 2; i++){
                calc_stack.pop_back();
                T--;
            }
            calc_stack.push_back(dataspace_len++);
            T++;
            P++;
            break;
        }
        case 5:
        {
            int ptr1 = calc_stack[T - 1];
            int ptr2 = calc_stack[T - 2];
            double num1 = data_space[ptr1][0];
            double num2 = data_space[ptr2][0];
            data_space[dataspace_len][0] = 1.0 * num2 / num1;
            data_space[dataspace_len][1] = _REAL;
            
            for(int i = 0; i < 2; i++){
                calc_stack.pop_back();
                T--;
            }
            calc_stack.push_back(dataspace_len++);
            T++;
            P++;
            break;
        }
        case 6:
        {
            int ptr1 = calc_stack[T - 1];
            int num = (int)data_space[ptr1][0];
            if(num % 2 == 0){
                data_space[dataspace_len][0] = 0;
            }
            else{
                data_space[dataspace_len][0] = 1;
            }
            calc_stack.pop_back();
            T--;
            data_space[dataspace_len][1] = _INTEGER;
            calc_stack.push_back(dataspace_len++);
            T++;
            P++;
            break;
        }
        case 7:
        {
            P++;
            break;
        }
        case 8:
        {
            int ptr1 = calc_stack[T - 1];
            int ptr2 = calc_stack[T - 2];
            double num1 = data_space[ptr1][0];
            double num2 = data_space[ptr2][0];
            for(int i = 0; i < 2; i++){
                calc_stack.pop_back();
                T--;
            }
            if(num1 == num2){
                data_space[dataspace_len][0] = 1;
                data_space[dataspace_len][1] = _BOOLEAN;
            }
            else{
                data_space[dataspace_len][0] = 0;
                data_space[dataspace_len][1] = _BOOLEAN;
            }
            calc_stack.push_back(dataspace_len++);
            T++;
            P++;
            break;
        }
        case 9:
        {
            int ptr1 = calc_stack[T - 1];
            int ptr2 = calc_stack[T - 2];
            double num1 = data_space[ptr1][0];
            double num2 = data_space[ptr2][0];
            for(int i = 0; i < 2; i++){
                calc_stack.pop_back();
                T--;
            }
            if(num1 != num2){
                data_space[dataspace_len][0] = 1;
                data_space[dataspace_len][1] = _BOOLEAN;
            }
            else{
                data_space[dataspace_len][0] = 0;
                data_space[dataspace_len][1] = _BOOLEAN;
            }
            calc_stack.push_back(dataspace_len++);
            T++;
            P++;
            break;
        }
        case 10:
        {
            int ptr1 = calc_stack[T - 1];
            int ptr2 = calc_stack[T - 2];
            double num1 = data_space[ptr1][0];
            double num2 = data_space[ptr2][0];
            for(int i = 0; i < 2; i++){
                calc_stack.pop_back();
                T--;
            }
//            cout<<"((("<<num2<<" "<<num1<<endl;
            if(num2 < num1){
                data_space[dataspace_len][0] = 1;
                data_space[dataspace_len][1] = _BOOLEAN;
            }
            else{
                data_space[dataspace_len][0] = 0;
                data_space[dataspace_len][1] = _BOOLEAN;
            }
            calc_stack.push_back(dataspace_len++);
            T++;
            P++;
            break;
        }
        case 11:
        {
            int ptr1 = calc_stack[T - 1];
            int ptr2 = calc_stack[T - 2];
            double num1 = data_space[ptr1][0];
            double num2 = data_space[ptr2][0];
            for(int i = 0; i < 2; i++){
                calc_stack.pop_back();
                T--;
            }
            if(num2 >= num1){
                data_space[dataspace_len][0] = 1;
                data_space[dataspace_len][1] = _BOOLEAN;
            }
            else{
                data_space[dataspace_len][0] = 0;
                data_space[dataspace_len][1] = _BOOLEAN;
            }
            calc_stack.push_back(dataspace_len++);
            T++;
            P++;
            break;
        }
        case 12:
        {
            int ptr1 = calc_stack[T - 1];
            int ptr2 = calc_stack[T - 2];
            double num1 = data_space[ptr1][0];
            double num2 = data_space[ptr2][0];
            for(int i = 0; i < 2; i++){
                calc_stack.pop_back();
                T--;
            }
            if(num2 > num1){
                data_space[dataspace_len][0] = 1;
                data_space[dataspace_len][1] = _BOOLEAN;
            }
            else{
                data_space[dataspace_len][0] = 0;
                data_space[dataspace_len][1] = _BOOLEAN;
            }
            calc_stack.push_back(dataspace_len++);
            T++;
            P++;
            break;
        }
        case 13:
        {
            int ptr1 = calc_stack[T - 1];
            int ptr2 = calc_stack[T - 2];
            double num1 = data_space[ptr1][0];
            double num2 = data_space[ptr2][0];
            for(int i = 0; i < 2; i++){
                calc_stack.pop_back();
                T--;
            }
            if(num2 <= num1){
                data_space[dataspace_len][0] = 1;
                data_space[dataspace_len][1] = _BOOLEAN;
            }
            else{
                data_space[dataspace_len][0] = 0;
                data_space[dataspace_len][1] = _BOOLEAN;
            }
            calc_stack.push_back(dataspace_len++);
            T++;
            P++;
            break;
        }
        case 14:
        {
            int num_ptr = calc_stack[T-1];
            que1.push_back(num_ptr);
//            double num = data_space[num_ptr][0];
//            int type = (int)data_space[num_ptr][1];
//            if(type == _INTEGER)
//                printf("%d", (int)num);
//            else if(type == _REAL)
//                printf("%lf", num);
//            else if(type == _CHAR)
//                printf("%c", (int)num);
//            else if(type == _BOOLEAN){
//                if((int)num == 1){
//                    printf("TRUE");
//                }
//                else if((int)num == 0){
//                    printf("FALSE");
//                }
//            }
            
            P++;
            calc_stack.pop_back();
            T--;
            break;
        }
        case 15:
        {
            printf("\n");
            P++;
            break;
        }
        case 16:
        {
            string s;
            cin>>s;
            
            if((int)s.size() == 1 &&
               ( (s[0] >= 'a' && s[0] <= 'z') ||
                 (s[0] >= 'A' && s[0] <= 'Z') ||
                 (s[0] == ' ')
                ) ){
//                data_space[dataspace_len][0] = s[0];
//                data_space[dataspace_len][1] = _CHAR;
//                calc_stack.push_back(dataspace_len++);
//                T++;
                que.push_back(s[0]);
                P++;
                return ;
            }
            for(int i = 0; i < (int)s.size(); i++){
                if(s[i] == '.'){
//                    data_space[dataspace_len][0] = atof(s.c_str());
//                    data_space[dataspace_len][1] = _REAL;
//                    calc_stack.push_back(dataspace_len++);
//                    T++;
                    
                    que.push_back(atof(s.c_str()));
                    P++;
                    return ;
                }
            }
//            data_space[dataspace_len][0] = atoi(s.c_str());
//            data_space[dataspace_len][1] = _INTEGER;
//            calc_stack.push_back(dataspace_len++);
//            T++;
            que.push_back(atoi(s.c_str()));
            P++;
            
            break;
        }
        case 17:
        {
            int ptr1 = calc_stack[T - 1];
            int ptr2 = calc_stack[T - 2];
            int num1 = (int)data_space[ptr1][0];
            int num2 = (int)data_space[ptr2][0];
            for(int i = 0; i < 2; i++){
                calc_stack.pop_back();
                T--;
            }
            int res = num2 / num1;
            data_space[dataspace_len][0] = res;
            data_space[dataspace_len][1] = _INTEGER;
            
            calc_stack.push_back(dataspace_len++);
            T++;
            P++;
            break;
        }
        case 18:
        {
            int ptr1 = calc_stack[T - 1];
            int ptr2 = calc_stack[T - 2];
            int num1 = (int)data_space[ptr1][0];
            int num2 = (int)data_space[ptr2][0];
            for(int i = 0; i < 2; i++){
                calc_stack.pop_back();
                T--;
            }
            int res = num2 % num1;
            data_space[dataspace_len][0] = res;
            data_space[dataspace_len][1] = _INTEGER;
            
            calc_stack.push_back(dataspace_len++);
            T++;
            P++;
            break;
        }
        case 19:
        {
            int ptr1 = calc_stack[T - 1];
            int ptr2 = calc_stack[T - 2];
            int num1 = (int)data_space[ptr1][0];
            int num2 = (int)data_space[ptr2][0];
            for(int i = 0; i < 2; i++){
                calc_stack.pop_back();
                T--;
            }
            int res = num2 & num1;
            data_space[dataspace_len][0] = res;
            data_space[dataspace_len][1] = _INTEGER;
            
            calc_stack.push_back(dataspace_len++);
            T++;
            P++;
            break;
        }
        case 20:
        {
            int ptr1 = calc_stack[T - 1];
            data_space[ptr1][0] = not data_space[ptr1][0];
//            cout<<"???"<<data_space[ptr1][0]<<endl;
            P++;
            break;
        }
        case 21:
        {
            int ptr1 = calc_stack[T - 1];
            int ptr2 = calc_stack[T - 2];
            int num1 = (int)data_space[ptr1][0];
            int num2 = (int)data_space[ptr2][0];
            for(int i = 0; i < 2; i++){
                calc_stack.pop_back();
                T--;
            }
            int res = num2 | num1;
            data_space[dataspace_len][0] = res;
            data_space[dataspace_len][1] = _INTEGER;
            
            calc_stack.push_back(dataspace_len++);
            T++;
            P++;
            break;
        }
        case 22:
        {
            calc_stack.pop_back();
            T--;
            P++;
            break;
        }
    }
}

struct ADDR_VALUE{
    int addr;
    int value;
};

ADDR_VALUE find_value(int num1){
    ADDR_VALUE ans;
    ans.addr = num1;
    ans.value = data_space[num1][0];
    int num2 = data_space[num1][2];
    while(num2 != VALUE){
        int this_addr = (int)data_space[num1][0];
        ans.addr = this_addr;
        ans.value = data_space[this_addr][0];
        num1 = (int)ans.value;
        num2 = data_space[this_addr][2];
    }
    return ans;
}

void do_resolve(){
    P = 0;
    B = 0;
    T = 0;
    cur_level = 0;
    printf("程序开始运行---------\n");
    int cnt = 0;
    while(P != (int)pcode.size() - 1){
        I = pcode[P];
//        cout<<"processing "<<P<<endl;
        cnt++;
//        if(P == 389){
//            cout<<"??"<<data_space[7][0]<<" "<<data_space[44][0]<<endl;
//        }
//        if(P == 254){
//            cout<<"???";
//            cout<<data_space[15081][0]<<endl;
//        }
//        if(P == 230){
//            cout<<"******"<<data_space[15072][0]<<endl;
//        }
//        if(P == 264 || P == 261 || P == 267 || P == 277 || P == 280){
//            cout<<"hahaha "<<P<<" ";
//            cout<<" "<<data_space[15079][0]<<" "<<data_space[15080][0]<<endl;
//        }
//        if(P == 267){
//            cout<<"$$$$";
//            cout<<" "<<data_space[15079][0]<<" "<<data_space[15080][0]<<endl;
//        }
//        if(P == 275)
//            cout<<"&&&&&&&&"<<data_space[15079][0]<<" "<<data_space[15080][0]<<endl;
        if(I[0] == "LIT"){
            int d = atoi(I[2].c_str());
            calc_stack.push_back(d);
            T++;
            P++;
        }
        else if(I[0] == "LOD"){
            int num = atoi(I[2].c_str());
            calc_stack.push_back(mmMark[num].data_space_ptr);
            T++;
            P++;
        }
        else if(I[0] == "STO"){
            int num = atoi(I[2].c_str());
            int m = atoi(I[3].c_str());
            if(m == 0){
                double d = data_space[calc_stack[T - 1]][0];
                int ptr = mmMark[num].data_space_ptr;
                int ptr2 = mmMark[num].data_space_temp_ptr;
                data_space[ptr][0] = d;
                data_space[ptr2][0] = d;
                calc_stack.pop_back();
                T--;
                
            }
            else if(m == 1){
                int d = (int)data_space[calc_stack[T-1]][0];
                int ptr = mmMark[num].data_space_ptr;
                int ptr2 = mmMark[num].data_space_temp_ptr;
                data_space[ptr][0] = d;
                data_space[ptr2][0] = d;
//                mmMark[num].data_space_temp_ptr = calc_stack[T - 1];
                int flag = 0;
                for(int i = 1; i < mmMark[0].Belong; i++){
                    if(mmMark[i].data_space_ptr == calc_stack[T - 1]){
                        flag = i;
                        break;
                    }
                }
                mmMark[num].data_space_temp_ptr = mmMark[flag].data_space_temp_ptr;
                calc_stack.pop_back();
                T--;
            }
            P++;
        }
        else if(I[0] == "CAL"){
            int d = atoi(I[2].c_str());
            int flag = 0;
            for(int i = 1; i < mmMark[0].Belong;i++){
                if(mmMark[i].addr == d){
                    flag = i;
                    break;
                }
            }
            
            int tmp = (int)runtime_stack.size();
            for(int i = 0 ;i < mmMark[flag].size + 3; i++){
                runtime_stack.push_back(0);
            }
            runtime_stack[tmp] = mmMark[flag].size + 3;
            runtime_stack[tmp + 1] = B;
            runtime_stack[tmp + 2] = P + 1;
            P = d;
            B = tmp;
        }
        else if(I[0] == "INT"){
            
            P++;
        }
        else if(I[0] == "JMP"){
            P = atoi(I[2].c_str());
        }
        else if(I[0] == "JPC"){
            int val_ptr = calc_stack[T - 1];
            int val =
            (int)data_space[val_ptr][0];
            if(val == 0){
                P = atoi(I[2].c_str());
            }
            else{
                P++;
            }
            calc_stack.pop_back();
            T--;
        }
        else if(I[0] == "OPR"){
            handle_OPR(I);
        }
        else if(I[0] == "ARR"){
            int dimension_ptr = atoi(I[2].c_str());
            int dimension = (int)data_space[dimension_ptr][0];
            int pos1 = T - dimension;
            int pos2 = T - 2 * dimension;
            int arr_num = 0;
            for(int i = 0; i < dimension; i++){
                int num1 = data_space[calc_stack[pos1 + i]][0];
                int num2 = data_space[calc_stack[pos2 + i]][0];
                arr_num += num1;
                arr_num *= num2;
            }
//            cout<<"+++"<<arr_num<<endl;
            for(int i = 0; i < 2 * dimension; i++){
                calc_stack.pop_back();
                T--;
            }
            data_space[dataspace_len][0] = arr_num;
            data_space[dataspace_len++][1] = _INTEGER;
            
            calc_stack.push_back(dataspace_len - 1);
            T++;
            P++;
        }
        else if(I[0] == "STOA"){
            int flag = atoi(I[1].c_str());
            if(flag == 0){
                int ptr1 = calc_stack[T - 1];
                int ptr2 = calc_stack[T - 2];
                double num1 = data_space[ptr1][0];
//                int type1 = data_space[ptr1][1];
                int find_flag = 0;
                for(int i  = 1; i <= mmMark[0].Belong; i++){
                    if(mmMark[i].data_space_ptr == ptr2){
                        find_flag = i;
                    }
                }
                data_space[ptr2][0] = num1;
                int ptr3;
                if(find_flag != 0){
                    ptr3 = mmMark[find_flag].data_space_temp_ptr;
                    data_space[ptr3][0] = num1;
                }
                for(int i = 0; i < 2; i++){
                    calc_stack.pop_back();
                    T--;
                }
            }
            else if(flag == 1){
                int ptr1 = calc_stack[T - 1];
                double num1 = data_space[ptr1][0];
                int ptr2 = atoi(I[2].c_str());
                data_space[ptr2][0] = num1;
                calc_stack.pop_back();
                T--;
            }
            P++;
        }
        else if(I[0] == "LODA"){
            if(I[1] == "0"){
                int ptr = calc_stack[T - 1];
                calc_stack.pop_back();
                T--;
                double num = data_space[ptr][0];
    //            cout<<"-----"<<num<<endl;;
                calc_stack.push_back((int)num);
                T++;
            }
            else if(I[1] =="1"){
                int num = atoi(I[2].c_str());
                int ptr1 = mmMark[num].data_space_ptr;
                double val = data_space[ptr1][0];
                int type = data_space[ptr1][1];
                data_space[dataspace_len][0] = val;
//                cout<<"########"<<val<<endl;
                data_space[dataspace_len][1] = type;
                calc_stack.push_back(dataspace_len++);
                T++;
            }
            P++;
        }
        else if(I[0] == "QUE"){
            int flag = atoi(I[1].c_str());
            int ptr = atoi(I[2].c_str());
            int times = (int)data_space[ptr][0];
            if(flag == 0){
                
                for(int i = 0; i < times; i++){
                    int top_addr = calc_stack[T - 1];
                    calc_stack.pop_back();
                    T--;
                    double tail = que.back();
                    que.pop_back();
                    data_space[top_addr][0] = tail;
                }
            }
            else if(flag == 1){
                for(int i = 0; i < times; i++){
                    int num_ptr = (int)que1.back();
                    que1.pop_back();
                    double num = data_space[num_ptr][0];
                    int type = (int)data_space[num_ptr][1];
                    if(type == _INTEGER)
                        printf("%d", (int)num);
                    else if(type == _REAL)
                        printf("%lf", num);
                    else if(type == _CHAR)
                        printf("%c", (int)num);
                    else if(type == _BOOLEAN){
                        if((int)num == 1){
                            printf("TRUE");
                        }
                        else if((int)num == 0){
                            printf("FALSE");
                        }
                    }
                }
            }
            P++;
        }
    }
    printf("程序运行结束---------\n");
    
//    for(int i = 0; i < pcode.size(); i++){
//        pcode[i].swap(vector<string>(pcode[i]));
//    }
    vector<vector<string> >(pcode).swap(pcode);
    
}
