#include"loadInfo.h"

vector<vector<string> > pcode;
double data_space[100000][3];
int dataspace_len;

string pcodedat_dir = "/Users/mac/Desktop/pcode.txt";
string pcode_dir = "/Users/mac/Desktop/pcode.dat";
string data_space_dir = "/Users/mac/Desktop/data_space.dat";
string mmMark_dir = "/Users/mac/Desktop/mmMark.dat";

marktable mmMark[5500];

//vector<vector<string> >ppcode;

void load_pcode(){
    ifstream fin(pcodedat_dir, ios::binary);
    string s;
    while(getline(fin, s)){
        vector<string> this_code;
        int pos = 0;
        while(s[pos] != ':')
            pos++;
        pos++;
        while(s[pos] == '\t' || s[pos] == ' ' || s[pos] == '\0')
            pos++;
        string code0;
        while(s[pos] >= 'A' && s[pos] <= 'Z'){
            code0.push_back(s[pos]);
            pos++;
        }
        this_code.push_back(code0);

        while(s[pos] == '\t' || s[pos] == ' ' || s[pos] == '\0')
            pos++;

        string code1;
        while(s[pos] >= '0' && s[pos] <= '9'){
            code1.push_back(s[pos]);
            pos++;
        }
        this_code.push_back(code1);

        while(s[pos] == '\t' || s[pos] == ' ' || s[pos] == '\0')
            pos++;

        string code2;
        while(s[pos] >= '0' && s[pos] <= '9'){
            code2.push_back(s[pos]);
            pos++;
        }
        this_code.push_back(code2);

        while((s[pos] == '\t' || s[pos] == ' ' || s[pos] == '\0') && pos != (int)s.size()){
            pos++;
        }


        if(pos == (int)s.size()){
            this_code.push_back("");
            pcode.push_back(this_code);
        }
        else{
            string code3;
            while(s[pos] >= '0' && s[pos] <= '9'){
                code3.push_back(s[pos]);
                pos++;
            }
            this_code.push_back(code3);
            pcode.push_back(this_code);
        }
    }

    fin.close();
}

//void load_pcode(){
//    ifstream fin(pcode_dir, ios::binary);
//    int pcode_num;
//    fin.read((char*)&pcode_num, sizeof(int));
//    for(int i = 0; i < pcode_num; i++){
//        vector<string> this_code;
//        for(int j = 0; j < 4; j++){
//            string s;
//            unsigned s_size;
//            fin.read((char*)&s_size, sizeof(unsigned));
//            fin.read((char*)&s, s_size);
//            this_code.push_back(s);
//        }
//        pcode.push_back(this_code);
//    }
//    fin.close();
//    for(int i = 0; i < pcode_num; i++){
//        for(int j = 0; j < 4; j++){
//            if(pcode[i][j] == "" && ppcode[i][j] != ""){
//                pcode[i][j] = ppcode[i][j];
//            }
//        }
//    }
//    vector<vector<string> >(ppcode).swap(ppcode);
//}

void load_mmMark(){
    ifstream fin1(mmMark_dir, ios::binary);
    unsigned table_size;
    fin1.read((char*)&table_size, sizeof(unsigned));
    fin1.read((char*)&mmMark, table_size);
    fin1.close();
    
}

void load_data_space(){
    ifstream fin0(data_space_dir, ios::binary);
    unsigned dataspace_size;
    fin0.read((char*)&dataspace_size, sizeof(unsigned));
    fin0.read((char*)&dataspace_len, sizeof(int));
    fin0.read((char*)&data_space, dataspace_size);
    fin0.close();
}
void load_info(){
//    load_ppcode();
    load_pcode();
    
//    for(int i = 0; i < (int)pcode.size(); i++){
//        cout<<i<<": ";
//        for(int j = 0; j < 4; j++){
//            cout<<pcode[i][j]<<" ";
//        }
//        cout<<endl;
//    }
    
    load_data_space();
    
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
    
    load_mmMark();
//    for(int i=1;i<=mmMark[0].Belong;i++){
//        printf("begin**********************\n");
//        printf("value=%s pos=%d depth=%d dev=%d size=%d data_ptr=%d temp_ptr = %d arr_len=%d, addr=%d var_or_value=%d\n", mmMark[i].value,  mmMark[i].pos, mmMark[i].depth, mmMark[i].deviation, mmMark[i].size, mmMark[i].data_space_ptr,
//               mmMark[i].data_space_temp_ptr, mmMark[i].array_lenth, mmMark[i].addr, mmMark[i].value_or_var);
//        printf("buffer=%d period_num=%d\n", mmMark[i].buffer, mmMark[i].period_num);
//        for(int j=0;j<mmMark[i].period_num;j++){
//            if(mmMark[i].period[j].kind == 1)
//                printf("kind=integer start=%d end=%d\n", mmMark[i].period[j].i_start, mmMark[i].period[j].i_end);
//            if(mmMark[i].period[j].kind == 2)
//                printf("kind=char start=%d end=%d\n", mmMark[i].period[j].c_start, mmMark[i].period[j].c_end);
//            if(mmMark[i].period[j].kind == 3)
//                printf("kind=bool start=%d end=%d\n", mmMark[i].period[j].b_start, mmMark[i].period[j].b_end);
//        }
//        printf("end**********************\n");
//    }
}
