#include<iostream>
#include<iomanip>
#include<vector>
#include<map>
#include<string>
#include<string.h>
#include<algorithm>
#include<time.h>
#include<fstream>

#include"generateTable.h"
#include"analysis.h"
#include"generatePCode.h"
//#include"resolve.h"

using namespace std;

#define CHAR 0
#define INTEGER 1
#define REAL 2
#define BOOL 3
#define FUNCTION 4
#define PROCEDURE 5

const string _boolean = "0000";
const string _char = "0001";
const string _integer = "0010";
const string _real = "0011";
// xxxxx01xx

const string _record = "01xxxx";
const string _array = "100000";
const string _freturns = "110000";

//string lex_path = "/Users/mac/Desktop/示例程序.txt";
//string lex_path = "/Users/mac/Desktop/lex2.txt";
string lex_path = "/Users/mac/Desktop/resource/Fstdout.txt";
string lge_path = "/Users/mac/Desktop/lge00.txt";
string end_path = "/Users/mac/Desktop/end_ch.txt";
string table_dir = "/Users/mac/Desktop/table.dat";
string mergeMarktable_dir = "/Users/mac/Desktop/MergedMarkTable.dat";
string updateMarktable_dir = "/Users/mac/Desktop/UpdatedMarkTable.dat";
string pcode_dir = "/Users/mac/Desktop/pcode.txt";
string pcodedat_dir = "/Users/mac/Desktop/pcode.dat";
string data_space_dir = "/Users/mac/Desktop/data_space.dat";
string mmMark_dir = "/Users/mac/Desktop/mmMark.dat";

outMMark outmark[5500];

int cur_pos = 1; // 带分析串首位置

int debug_num = 0;
int forward_num = 0;
int reduce_num = 0;

int how_many_word(string s) { // 查看一个字符串有多少单词或符号 如字符串"program id ;"返回值为3
    int pos = 0;
    int ans = 0;
    if (s.size() == 0)
        return 0;
    do {
        while (s[pos] == ' ')
            pos++;
        pos += read_a_word(s, pos).size();

        ans++;
    } while (pos != s.size());
    return ans;
}

// 语法分析
NODE* proc(string s) {
    
    ifstream fin(table_dir, ios::binary);
    unsigned table_size;
    fin.read((char*)&table_size, sizeof(unsigned));
    fin.read((char*)&table, table_size);
    fin.close();
    
    vector<int> status; // 状态栈
    vector<string> opr; // 符号栈
    vector<NODE*> nodes;
    s.append(1, '$');
    int ip_pos = 0;
    while (s[ip_pos] == ' ' || s[ip_pos] == '\n')
        ip_pos++;
    status.push_back(0);
    opr.push_back("$");
    do {
//        cout << "状态栈  ";
//        for (int i = 0; i < (int)status.size(); i++)
//            cout << status[i] << " ";
//        cout << endl;
//
//        cout << "符号栈  ";
//        for (int i = 0; i < (int)opr.size(); i++)
//            cout << opr[i] << " ";
//        cout << endl;
////
//        cout << "输入   " << s.substr(ip_pos);
//        cout << endl;
        
        int S = status.back(); // 栈顶状态
        
        while (s[ip_pos] == ' ' || s[ip_pos] == '\n' || s[ip_pos] == '\t' || s[ip_pos] == '\r') //跳过空格和回车
            ip_pos++;
        
        string ip = read_a_word(s, ip_pos); // 输入串首

        if (table[S][ch[ip]][0] == 's') { // 移进
//            cout << debug_num++ << " 移进" << forward_num++ << " " << ip << endl;
            
            
//            cout << "移进" << table[S][ch[ip]] << endl;
            opr.push_back(ip); // 输入串首压入栈
            NODE* cur_node = new NODE;
            cur_node->_name = ip;
            cur_node->pos = cur_pos++;
            
            
            nodes.push_back(cur_node);
            int ss = atoi(table[S][ch[ip]].substr(1).c_str()); // 要转移到的状态
            status.push_back(ss); // 要转移的状态压入栈
            ip_pos += ip.size(); // 输入串指针移动一位
            while (s[ip_pos] == ' ' || s[ip_pos] == '\n')
                ip_pos++;
            
            string ntword; // 用来避免..导致pos问题
            ntword = read_a_word(s, ip_pos);
            if(ntword == "." && ip == "."){
                cur_pos--;
            }
        }
        else if (table[S][ch[ip]][0] == 'r') { // 归约
            int lge_no = atoi(table[S][ch[ip]].substr(1).c_str()); // 用第几条文法归约
//            cout << debug_num++ << " 归约" << reduce_num++ << " " << lge[lge_no] << endl;
            int arrow_loc = _locate(lge[lge_no], 2); // 箭头位置
            int beta_len = how_many_word(lge[lge_no].substr(arrow_loc + 1));// 从箭头位置开始到最后有几个单词
            
            vector<NODE*> node_buf;
            for (int i = 0; i < beta_len; i++) { // 弹出被归约的单词个数
                status.pop_back();
                opr.pop_back();
                node_buf.push_back(nodes.back());
                nodes.pop_back();
            }
            int ss = status.back(); // 栈顶状态
            string a_word = read_a_word(lge[lge_no], 0); // 要归约到的单词
            opr.push_back(a_word); // 要归约到的单词压入栈
            NODE* cur_node = new NODE;
            cur_node->_name = a_word;
            for(int i = 0; i < beta_len; i++){
                cur_node->child.push_back(node_buf.back());
                node_buf.back()->father = cur_node;
                node_buf.pop_back();
            }
            nodes.push_back(cur_node);
//            cout<<lge_no<<endl;
            
            analyze(lge_no, cur_node, cur_node->child);
            int A = atoi(table[ss][ch[a_word]].c_str()); // goto到状态A
            status.push_back(A); //A压入栈
        }
        else if (table[S][ch[ip]] == "acc") { // 接受
            cout << "success" << endl;
            return nodes.front();
        }
        else { // 报错
            cout << "error" << endl;
            int err_loc = ip_pos - 1;
            if (ip_pos >= 0) {
                while (s[err_loc] == ' ')
                    err_loc--;
                while ((s[err_loc] >= 'a' && s[err_loc] <= 'z') ||
                    (s[err_loc] >= 'A' && s[err_loc] <= 'Z') ||
                    (s[err_loc] >= '0' && s[err_loc] <= '9') ||
                    s[err_loc] == '_')
                    err_loc--;
                err_loc++;

            }
            string fir_err_word = read_a_word(s, err_loc);
            err_loc += (int)fir_err_word.size();
            string sec_err_word = read_a_word(s, err_loc);

            cout << "出错位置在" << fir_err_word << " " << sec_err_word << "附近" << endl;
            exit(0);
        }
    } while (true);
}

void bfs(NODE* tree){
    queue<NODE*> que;
    que.push(tree);
    while(que.size() != 0){
        NODE* tt = que.front();
        cout<<tt->_name<<" "<<(int)tt->child.size()<<endl;
        for(int i=0;i<(int)tt->child.size();i++){
            que.push(tt->child[i]);
        }
        que.pop();
    }
}

void save_pcode(){
    ofstream fout(pcode_dir, ios::out);
    
    for(int i = 0; i < (int)pcode.size(); i++){
        fout<<i<<": ";
        for(int j = 0; j < (int)pcode[i].size(); j++){
            fout<<pcode[i][j]<<" ";
        }
        fout<<endl;
    }
    
    fout.close();
    
    ofstream fout0(pcodedat_dir, ios::binary);
    int pcode_num = (int)pcode.size();
//    unsigned pcode_size = sizeof(pcode);
    
    fout0.write((char*)&pcode_num, sizeof(int));
    
    for(int i = 0; i < pcode_num; i++){
        for(int j = 0; j < 4; j++){
            unsigned this_size = sizeof(pcode[i][j]);
            fout0.write((char*)&this_size, sizeof(unsigned));
            fout0.write((char*)&pcode[i][j], this_size);
        }
    }
//    fout0.write((char*)&pcode, pcode_size);
    fout0.close();
}

void save_data_space_dat(){
    ofstream fout0(data_space_dir, ios::binary);
    unsigned dataspace_size = sizeof(data_space);
    fout0.write((char*)&dataspace_size, sizeof(unsigned));
    fout0.write((char*)&dataspace_len, sizeof(int));
    fout0.write((char*)&data_space, sizeof(data_space));
    fout0.close();
    
}

void save_mmMark(){
    outmark[0].Belong = mMark[0].Belong;
    for(int i = 1; i <= mMark[0].Belong; i++){
        strcpy(outmark[i].value, mmMark[i].value);
        outmark[i].pos = mmMark[i].pos;
        outmark[i].depth = mmMark[i].depth;
        outmark[i].deviation = mmMark[i].deviation;
        outmark[i].Belong = mmMark[i].Belong;
        outmark[i].buffer = mmMark[i].buffer;
        memcpy(outmark[i].period, mmMark[i].period, sizeof(mmMark[i].period));
        outmark[i].period_num = mmMark[i].period_num;
        outmark[i].size = mmMark[i].size;
        outmark[i].value_or_var = mmMark[i].value_or_var;
        memcpy(outmark[i].hav, mmMark[i].hav, sizeof(mmMark[i].hav));
        outmark[i].belongPos = mmMark[i].belongPos;
        outmark[i].addr = mmMark[i].addr;
        outmark[i].para_cnt = mmMark[i].para_cnt;
        outmark[i].data_space_ptr = mmMark[i].data_space_ptr;
        outmark[i].data_space_temp_ptr = mmMark[i].data_space_temp_ptr;
        outmark[i].alloc_flag = mmMark[i].alloc_flag;
        outmark[i].array_lenth = mmMark[i].array_lenth;
        outmark[i].var_part_num = mmMark[i].var_part_num;
    }
    ofstream fout(mmMark_dir, ios::binary);
    unsigned outMark_size = sizeof(outmark);
    fout.write((char*)&outMark_size, sizeof(unsigned));
    fout.write((char*)&outmark, sizeof(outmark));
    fout.close();
}

void init_info(){
//    for(int i = 0; i < 5500; i++){
////        mmMark[i].addr = 0;
//        memset(&mmMark[i], 0, sizeof(mmMark[i]));
//        memset(&mMark[i], 0, sizeof(mMark[i]));
//        memset(&uuMark[i], 0, sizeof(uuMark[i]));
//        memset(&uMark[i], 0, sizeof(uMark[i]));
//        memset(&uMark[i], 0, sizeof(uMark[i]));
//        memset(&outmark[i], 0, sizeof(outmark[i]));
//    }
    memset(mmMark, 0, sizeof(mmMark));
    memset(mMark, 0, sizeof(mMark));
    memset(uuMark, 0, sizeof(uuMark));
    memset(uMark, 0, sizeof(uMark));
    memset(outmark, 0, sizeof(outmark));
    
}
void save_temp_ptr(){
    for(int i = 0; i < 5500; i++){
        mmMark[i].data_space_temp_ptr = mmMark[i].data_space_ptr;
    }
}

int main() {
    
    clock_t _stt = clock();
    
    /*  */
    init_info();
    ifstream fin(mergeMarktable_dir, ios::binary);
    unsigned table_size;
    fin.read((char*)&table_size, sizeof(unsigned));
    fin.read((char*)&mMark, table_size);
    fin.close();
    
    for(int i=1;i<=mMark[0].Belong;i++){
        mmMark[i].kind = mMark[i].kind;
        strcpy(mmMark[i].mark, mMark[i].mark);
        strcpy(mmMark[i].value, mMark[i].value);
        mmMark[i].pos = mMark[i].pos;
        mmMark[i].depth = mMark[i].depth;
        mmMark[i].deviation = mMark[i].deviation;
        mmMark[i].Belong = mMark[i].Belong;
        mmMark[i].size = mMark[i].size;
        memcpy(mmMark[i].hav, mMark[i].hav, sizeof(mMark[i]));
        mmMark[i].belongPos = mMark[i].belongPos;
    }
//    init_info();
    for(int i=1;i<=mMark[0].Belong;i++){
//        printf("%d %s %s %d %d %d %d\n", mmMark[i].kind, mmMark[i].mark, mmMark[i].value, mmMark[i].pos, mmMark[i].depth, mmMark[i].deviation, mmMark[i].Belong);
    }
    
    ifstream ffin(updateMarktable_dir, ios::binary);
    unsigned table_size0;
    ffin.read((char*)&table_size0, sizeof(unsigned));
    ffin.read((char*)&uMark, table_size);
    ffin.close();
    for(int i = 1; i <= uMark[0].Belong; i++){
        uuMark[i].kind = uMark[i].kind;
        strcpy(uuMark[i].mark, uMark[i].mark);
        strcpy(uuMark[i].value, uMark[i].value);
        uuMark[i].pos = uMark[i].pos;
        uuMark[i].depth = uMark[i].depth;
        uuMark[i].deviation = uMark[i].deviation;
        uuMark[i].Belong = uMark[i].Belong;
    }
    
    for(int i = 1; i <= uMark[0].Belong; i++){
//        printf("%d %s %s %d %d %d %d\n", uuMark[i].kind, uuMark[i].mark, uuMark[i].value, uuMark[i].pos, uuMark[i].depth, uuMark[i].deviation, uuMark[i].Belong);
    }
    
    /*  */
    
    /* ***************************************/
    for(int i=0;i<INUM;i++){
        for(int j=0;j<100;j++)
        table[i][j] = "";
    }
    input_lge(); //读入文法，处理单词和终结符号

//    generateTable(); //lr(1)文法，生成符号表

    //    string input_str = "program id(id,id,id);\nbegin \nend.";
    //    string input_str = "program id id, id );var id, id: integer ;begin end.";

    string input_str;
    ifstream infile;
    infile.open(lex_path);
    string str;
    while (getline(infile, str)) {
        if(str[str.length() - 1] == '\r')
            str.pop_back();
        input_str.append(str);
        input_str.append(1, ' ');
    }
    infile.close();
//    cout << "待分析串\t" << input_str << endl;


    //    string input_str = " b + b ";
    
    NODE* _root = proc(input_str);
    
    generateDataSpace();
    
//    cout<<_root->_name<<endl;
//    cout<<(int)_root->child.size()<<endl;
//    for(int i = 0; i < (int)_root->child.size(); i++)
//    cout << _root->child[i]->_name << " ";
//    cout << endl;
//    bfs(_root);
    /*integer id=record id:integer end;*/
    /* ***************************************/
    
    
//    cout << lge_cnt << endl;
    
    GENCODE generatepcode;
    generatepcode.handle_program(_root);
    save_temp_ptr();
    
    save_pcode();
    save_data_space_dat();
    save_mmMark();
    
    
    for(int i=1;i<=mMark[0].Belong;i++){
        printf("begin**********************\n");
        printf("mark:%s value:%s pos=%d depth=%d dev=%d size=%d data_ptr=%d arr_len=%d addr=%d\n", mmMark[i].mark, mmMark[i].value, mmMark[i].pos, mmMark[i].depth, mmMark[i].deviation, mmMark[i].size, mmMark[i].data_space_ptr, mmMark[i].array_lenth, mmMark[i].addr);
        printf("buffer=%d period_num=%d\n", mmMark[i].buffer, mmMark[i].period_num);
        for(int j=0;j<mmMark[i].period_num;j++){
            if(mmMark[i].period[j].kind == 1)
                printf("kind=integer start=%d end=%d\n", mmMark[i].period[j].i_start, mmMark[i].period[j].i_end);
            if(mmMark[i].period[j].kind == 2)
                printf("kind=char start=%d end=%d\n", mmMark[i].period[j].c_start, mmMark[i].period[j].c_end);
            if(mmMark[i].period[j].kind == 3)
                printf("kind=bool start=%d end=%d\n", mmMark[i].period[j].b_start, mmMark[i].period[j].b_end);
        }
        printf("end**********************\n");
    }
    
    

    
//    for(int i = 0; i < dataspace_len; i++){
//        cout<<i<<" "<<data_space[i][0]<<" ";
//        if(data_space[i][1] == 0){
//            cout<<"INTEGER";
//        }
//        if(data_space[i][1] == 1){
//            cout<<"CHAR";
//        }
//        if(data_space[i][1] == 2){
//            cout<<"REAL";
//        }
//        if(data_space[i][1] == 3){
//            cout<<"BOOLEAN";
//        }
//        cout<<endl;
//    }
    
    clock_t _end = clock();
    cout << _end - _stt << "us" << endl;
    
//    generatePCode(_root);
}
// 4468689
// 23830
