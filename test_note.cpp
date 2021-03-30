#include<iostream>
#include<iomanip>
#include<vector>
#include<map>
#include<string>
#include<string.h>
#include<algorithm>
#include<time.h>
#include<fstream>
using namespace std;

string lex_path = "/Users/mac/Desktop/lex.txt";
string lge_path = "/Users/mac/Desktop/lge.txt";
string end_path = "/Users/mac/Desktop/end_ch.txt";
string table_dir = "/Users/mac/Desktop/table.dat";

string table[1000][100]; // 分析表

int n; // 文法数量
string lge[100]; // 文法

int ch_num; // 单词数量
int end_num; // 终结符数量

string word[100]; // 单词表
string end_word[100]; // 终结符表
string _word[100]; //索引对应的单词
map<string, int> ch; // 单词哈希表
map<string, int> end_ch; // 终结符哈希表
map<string, vector<string> > first_hash; // first集哈希表

string my_tostring(int k) { //整数转成对应的字符串
    char ch[25];
    int n = 0;
    while (k) {
        ch[n++] = k % 10 + '0';
        k /= 10;
    }
    char verse_ch[25];
    for (int i = 0; i < n; i++)
        verse_ch[i] = ch[n - i - 1];
    verse_ch[n] = '\0';
    string ans = verse_ch;
    return ans;
}

// 从s[pos]开始读一个词块（可以含下划线的单词或单个的符号）
string read_a_word(string s, int pos) {
    string ans; //ans存储单词

    while (s[pos] == ' ' || s[pos] == '\n') //跳过空格和回车
        pos++;
    if (pos == s.size()) { //后面没有单词了
        string voidstr = "";
        ans.append(voidstr);
        return ans;
    }
    if ((s[pos] >= 'a' && s[pos] <= 'z') ||
        (s[pos] >= 'A' && s[pos] <= 'Z') ||
        s[pos] == '_') { //大小写字母或'_'，说明是个单词
        for (int i = pos; i < (int)s.size(); i++) {
            if ((s[i] >= 'a' && s[i] <= 'z') ||
                (s[i] >= 'A' && s[i] <= 'Z') ||
                (s[i] >= '0' && s[i] <= '9') ||
                (s[i] == '_')
                )
                ans.append(1, s[i]);

            else
                break;
        }
    }
    else { //符号之类的
        ans.append(1, s[pos]);
    }

    return ans;
}

// 判断是否为终结符
bool is_end(string s) {
    map<string, int>::iterator it;
    it = end_ch.find(s);
    if (it != end_ch.end())
        return true;
    if (s == "")
        return true;
    string voidstr;
    voidstr.append(1, '\0');
    if (s == "\0")
        return true;
    return false;
}

int word_to_index(string s) { // 文法中的某一词转成索引
    map<string, int>::iterator it;
    it = ch.find(s);
    if (it != ch.end())
        return it->second;
    return -1;
}
string index_to_word(int k) {// 文法中的索引转成某一词
    return _word[k];
}

int _locate(string s, int format) { // 定位'.'、';' ->的位置 ';'用来间隔lr(1)和向前看符号 定位的箭头是'>'的位置
    if (format == 0) { //找'#'位置，表示LR0的.
        for (int i = 3; i < (int)s.size(); i++) {
            if (s[i] == '#')
                return i;
        }
    }
    if (format == 1) { //找'@'的位置
        for (int i = 3; i < (int)s.size(); i++) {
            if (s[i] == '@')
                return i;
        }
    }
    if (format == 2) { //找->的位置
        for (int i = 1; i < (int)s.size() - 1; i++) {
            if (s[i] == '-' && s[i + 1] == '>')
                return i + 1;
        }
    }
    return -1; //没找到
}

void init_ch() { //初始化单词，给每个单词安排一个索引
    //ch 单词哈希表
    //_word 索引对应单词
    ch.insert(pair<string, int>("$", 0));
    ch.insert(pair<string, int>("none", 1));
    _word[0] = "$";
    _word[1] = "none";
    for (int i = 0; i < n; i++) { //枚举所有文法
        string a_word = read_a_word(lge[i], 0); //开头单词

        if (word_to_index(a_word) == -1) { //如果没有出现过，存放到单词哈希表里
            
            int len = (int)ch.size();
            ch.insert(pair<string, int>(a_word, len));
            _word[len] = a_word;
        }
        int pos = _locate(lge[i], 2) + 1; //->的下一个位置

        while (pos != lge[i].size()) { //逐个处理单词
            while (lge[i][pos] == ' ') // 去掉空格
                pos++;
            string word = read_a_word(lge[i], pos); //读入单词
            pos += word.size();
            if (word_to_index(word) != -1) //单词已经出现过
                continue;
            else { //单词未出现，保存单词
                int len = (int)ch.size();
                ch.insert(pair<string, int>(word, len));
                _word[len] = word;
            }
        }
    }
    ch_num = (int)ch.size();
    // printf(" num = %d\n", ch.size());
    // cout << _word[89] << endl;
    // cout << _word[90] << endl;
}

void init_endch() { //初始化终结符
    ifstream infile;
    infile.open(end_path); //终结符的文档，直接读入
    string str;
    end_num = 0;
    while (infile >> str) { //先读入到end_word中，共end_num个终结符
        end_word[end_num] = str;
        end_num++;
    }
    for (int i = 0; i < end_num; i++) { //与所有单词对应，找到编号，存储到end_ch中
        int _index = ch[end_word[i]];
        end_ch.insert(pair<string, int>(end_word[i], _index));
    }
}

void input_lge() {//从文档中读入文法，处理单词和终结符
    ifstream infile;
    
    infile.open(lge_path); //文法文件
    string str;
    n = 0;
    while (getline(infile, lge[n])) {// 读入所有文法，共n条
        n++;
    }
    init_ch(); //处理单词
    init_endch(); //处理终结符
}



int which_lge(string s) { // 给一个字符串返回是第几条文法 用来归约
    for (int i = 0; i < n; i++) { //循环所有文法
        if (lge[i] == s)
            return i;
    }
    return -1;
}


vector<string> _merge(vector<string>a, vector<string> b) { // 求first集用到的合并
    vector<string> ans;
    int i = 0, j = 0;
    while (i < (int)a.size() && j < (int)b.size()) {
        if (a[i] < b[j]) {
            ans.push_back(a[i++]);
        }
        else if (a[i] > b[j]) {
            ans.push_back(b[j++]);
        }
        else {
            ans.push_back(a[i]);
            i++;
            j++;
        }
    }
    if (i == a.size()) {
        while (j < (int)b.size())
            ans.push_back(b[j++]);
    }
    if (j == b.size()) {
        while (i < (int)a.size())
            ans.push_back(a[i++]);
    }
    return ans;
}
vector<string> first(string s) { // 求first集 每个元素都是string 存在vector里 不能有A->Bsomething B->Asomething同时存在
    vector<string> ans;
    //cout << "ok1" << endl;
    if (first_hash.size() != 0 && first_hash[s].size() != 0) {
        return first_hash[s];
    }
    if (is_end(s)) {
        if (s == "") {
            ans.push_back("none");
        }
        ans.push_back(s);
        first_hash.insert(pair<string, vector<string> >(s, ans));
        return ans;
    }
    // cout << "ok2" << endl;
    // cout << "n = " << n << endl;
    for (int i = 0; i < n; i++) { // 遍历所有文法
        // cout << i << "   " << lge[i] << endl;
        string a_word;
        memset(&a_word, 0, sizeof(a_word));
        a_word = read_a_word(lge[i], 0);
        if (a_word == s) {
            string back_first = read_a_word(lge[i], (int)s.size() + 2);
            if (back_first == s)
                continue;
            if (is_end(back_first)) { // 箭头后第一个是终结符
                if (back_first == "" || back_first == "\0")
                    back_first = "none";
                int flag = 1;
                for (int j = 0; j < (int)ans.size(); j++) { // 先检查是否已经存在
                    if (ans[j] == back_first) {
                        flag = 0;
                        break;
                    }
                }
                if (flag == 1)
                    ans.push_back(back_first);
            }
            else { // 箭头后第一个是非终结符 求非终结符的first集 和ans合并
                vector<string> backfirst_ans;
                backfirst_ans = first(back_first);
                sort(ans.begin(), ans.end());
                sort(backfirst_ans.begin(), backfirst_ans.end());
                ans = _merge(ans, backfirst_ans);
            }
        }
    }
    first_hash.insert(pair<string, vector<string> >(s, ans));
    return ans;
}
void expand(string expand_ch, vector<string>& I, string add) { // 扩展.后的非终结符
    for (int i = 0; i < n; i++) { //n个文法
    // cout << "???" << lge[i] << endl;
        string a_word;
        memset(&a_word, 0, sizeof(a_word));
        a_word = read_a_word(lge[i], 0); //第i个文法的首个单词
        if (a_word == expand_ch) { //对应上了，可以拓展
            
            // 加上.和向前看符号后加入状态
            string cur = lge[i];
            int arrow_loc = _locate(cur, 2); //->位置
            cur.insert(arrow_loc + 1, "#"); //加入#，相当于.
            cur.append(1, '@'); //结尾加上@,后跟向前看符号
            cur.append(add);
            // 本状态里如果已经有了要扩展出的这句话就不用这条文法扩展 继续看下一个文法
            int flag = 0;
            for (int j = 0; j < (int)I.size(); j++) {
                if (I[j] == cur)
                    flag = 1;
            }
            if (flag == 1)
                continue;
            I.push_back(cur);
            // 看扩展后.后是不是非终结符 是的话继续扩展
            int point_loc = _locate(cur, 0); //# 位置
            string next_word = read_a_word(cur, point_loc + 1);
            // cout << a_word << "   " << expand_ch << "    " << cur << endl;
            
            if (!is_end(next_word)) { //非终结符,继续拓展
                // cout << a_word << "   " << expand_ch << "    " << cur << endl;
            
                int next_pos = point_loc;
                while (cur[next_pos + 1] == ' ')
                    next_pos++;
                next_pos += next_word.size();
                while (cur[next_pos] == ' ')
                    next_pos++;
                next_pos += 1;
                //找到.后下下个单词位置
                // cout << "next_word = " << next_word << endl;
                if (cur[next_pos] == '@' && next_word != expand_ch) { //A->·T,a类型
                    // puts("!");
                    expand(next_word, I, add);
                }
                else {
                    string s1 = read_a_word(cur, next_pos);
                    
                    //                    string s1(1, cur[point_loc + 2]);
                    vector<string> _add = first(s1); //找到s1的first集合
                    if (_add.size() == 0) //无，加入空
                        expand(next_word, I, "none");
                    for (int j = 0; j < (int)_add.size(); j++) //以所有first集合作为向前看符号
                        expand(next_word, I, _add[j]);
                }
            }
        }
    }
}

// 判断核是否已存在 存在就返回已经有的状态下标
int check_i_exist(vector<string> cur, vector<vector<string> > kernel) {
    sort(cur.begin(), cur.end());
    for (int i = 0; i < (int)kernel.size(); i++) {
        sort(kernel[i].begin(), kernel[i].end());
        if (cur == kernel[i])
            return i;
    }
    return -1;
}

// 生成一个状态
void generate_an_i(vector<string> cur, vector<vector<string> >& I, vector<vector<string> >& kernel) {

    int Inum = (int)I.size(); //状态数

    vector<string> voidstrvec;
    I.push_back(voidstrvec);
    kernel.push_back(voidstrvec);


    //把核的所有语句加进本状态 并扩展
    for (int i = 0; i < (int)cur.size(); i++) {

        int point_loc = _locate(cur[i], 0); //#符号位置
        int semi_loc = _locate(cur[i], 1); //@符号位置

        I[Inum].push_back(cur[i]);
        kernel[Inum].push_back(cur[i]);

        //读.后面的第一个单词
        string expand_word = read_a_word(cur[i], point_loc + 1);

        if (!is_end(expand_word)) { // .后面是非终结符 需要扩展
            
            int next_pos = point_loc;
            while (cur[i][next_pos + 1] == ' ') //先去掉单词前面的空格
                next_pos++;
            next_pos += expand_word.size(); // 找到单词结束的位置
            while (cur[i][next_pos] == ' ')
                next_pos++;
            next_pos += 1; // 找到下一个单词的位置
            //            if(cur[i][point_loc + (int)expand_word.size() + 1] == '@'){ // .后面的后面是分号 就直接用向前看符号
            if (cur[i][next_pos] == '@') { // .后面的后面是@ 就直接用向前看符号
                // cout << expand_word << endl;
                // cout << cur[i] << endl;
                string add = read_a_word(cur[i], semi_loc + 1); //向前看符号

                expand(expand_word, I[Inum], add); //拓展
            }
            else { //.后面的后面不是分号 就用.后面字符的first集 first集中每个字符扩展一遍（即每个字符做一次向前看符号）
                string s1 = read_a_word(cur[i], next_pos);
                vector<string> add = first(s1); //add为first集合

                if (add.size() == 0) {
                    expand(expand_word, I[Inum], "none");
                }
                for (int j = 0; j < (int)add.size(); j++) {

                    expand(expand_word, I[Inum], add[j]);
                }
            }
        }
    }
    // 找到.后有哪些字符 用索引存
    int index_arr[100];
    int index_num[100];
    memset(index_arr, 0, sizeof(index_arr));
    memset(index_num, 0, sizeof(index_num));

    //    string _cur[100][200];
    vector<string> _cur[100];
    int all_reduce = 1;
    for (int i = 0; i < (int)I[Inum].size(); i++) { // 遍历这一状态里的所有语句
        int point_loc = _locate(I[Inum][i], 0);
        int semi_loc = _locate(I[Inum][i], 1);

        if (point_loc + 1 != semi_loc) { //不是归约
            all_reduce = 0;

            string next_word = read_a_word(I[Inum][i], point_loc + 1);

            int _index = word_to_index(next_word); //.下一位的字符的索引
            if (index_arr[_index] == 1) {
                //                _cur[_index][index_num[_index]] = I[Inum][i];
                _cur[_index].push_back(I[Inum][i]);
                int a = _index;
                int b = index_num[_index];
                for (int j = 0; j < (int)next_word.size(); j++) {
                    if (_cur[a][b][point_loc + 1] == ' ')
                        j--;
                    swap(_cur[a][b][point_loc], _cur[a][b][point_loc + 1]);

                    point_loc++;
                }
                index_num[_index]++;
            }
            else {
                index_arr[_index] = 1;
                //                _cur[_index][0] = I[Inum][i];
                _cur[_index].push_back(I[Inum][i]);
                for (int j = 0; j < (int)next_word.size(); j++) {
                    if (_cur[_index][0][point_loc + 1] == ' ')
                        j--;
                    swap(_cur[_index][0][point_loc], _cur[_index][0][point_loc + 1]);
                    point_loc++;
                }
                index_num[_index]++;
            }
        }
        else { // 归约

            string s1 = I[Inum][i].substr(0, point_loc);
            int lge_num = which_lge(s1);

            string ch = read_a_word(I[Inum][i], semi_loc + 1);
            string add = I[Inum][i].substr(semi_loc + 1);
            int none_flag = 0;
            int pos = semi_loc + 1;
            while (pos != I[Inum][i].size()) {
                while (I[Inum][i][pos] == ' ') {
                    pos++;
                }
                string ss = read_a_word(I[Inum][i], pos);
                pos += ss.size();
                if (ss == "none") {
                    none_flag = 1;
                    break;
                }
            }
            if (lge_num == 0) {
                table[Inum][word_to_index(ch)] = "acc";
            }
            else if (none_flag == 1 || semi_loc + 1 == I[Inum][i].size()) {
                for (int j = 0; j < ch_num; j++) {
                    if(table[Inum][j] == "")
                        table[Inum][j] = "r" + my_tostring(lge_num);
                }
                //                table[Inum][word_to_index("$")] = "r" + my_tostring(lge_num);
            }
            else
                table[Inum][word_to_index(ch)] = "r" + my_tostring(lge_num);
        }
    }
    if (all_reduce == 1) {
        return;
    }
    // 对.后面的字符添加移进操作
    for (int i = 0; i < 100; i++) {
        if (index_arr[i] == 1) {
            vector<string>next_cur;
            for (int j = 0; j < index_num[i]; j++) {
                next_cur.push_back(_cur[i][j]);
            }
            // 检查是否已经有要新建的状态
            int exist_num = check_i_exist(next_cur, kernel);
            if (exist_num == -1) { // 如果没有就新建一个状态

                string word = index_to_word(i);

                if (!is_end(word))
                    table[Inum][i] = my_tostring((int)I.size());
                else
                    table[Inum][i] = "s" + my_tostring((int)I.size());
                generate_an_i(next_cur, I, kernel);

            }
            else { // 核已经存在就不用新建一个状态了
                string word = index_to_word(i);
                if (!is_end(word))
                    table[Inum][i] = my_tostring(exist_num);
                else
                    table[Inum][i] = "s" + my_tostring(exist_num);
            }
        }
    }
}
// lr(1)文法，生成符号表
void generateTable() {
    string cur;

    cur = lge[0]; //第一句，起始句，PROGRAM->program0
    int arrow_loc = _locate(cur, 2); // -> 的位置
    cur.insert(arrow_loc + 1, "#"); //->后面插入一个'#'
    cur.append("@$"); //最后面加入@，@后面跟向前看符号$
    vector<vector<string> > I;
    vector<vector<string> > kernel; //每个块的核，由核拓展当前块
    vector<string> first_cur;
    first_cur.push_back(cur);
    generate_an_i(first_cur, I, kernel); //根据LR(1)文法，生成符号表
    // puts("ok???");
    for (int i = 0; i < (int)I.size(); i++) {
        cout << "状态" << i << endl;
        for (int j = 0; j < (int)I[i].size(); j++) {
            cout << I[i][j] << endl;
        }
    }
    ofstream fout(table_dir, ios::binary);
    unsigned table_size = sizeof(table);
    fout.write((char*)&table_size, sizeof(unsigned));
    fout.write((char*)&table, sizeof(table));
    fout.close();
}

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
void proc(string s) {
    
    ifstream fin(table_dir, ios::binary);
    unsigned table_size;
    fin.read((char*)&table_size, sizeof(unsigned));
    fin.read((char*)&table, table_size);
    
    vector<int> status; // 状态栈
    vector<string> opr; // 符号栈
    s.append(1, '$');
    int ip_pos = 0;
    while (s[ip_pos] == ' ' || s[ip_pos] == '\n')
        ip_pos++;
    status.push_back(0);
    opr.push_back("$");
    do {
        cout << "状态栈  ";
        for (int i = 0; i < (int)status.size(); i++)
            cout << status[i] << " ";
        cout << endl;

        cout << "符号栈  ";
        for (int i = 0; i < (int)opr.size(); i++)
            cout << opr[i] << " ";
        cout << endl;

        cout << "输入   " << s.substr(ip_pos);
        cout << endl;
        int S = status.back(); // 栈顶状态
        string ip = read_a_word(s, ip_pos); // 输入串首

        if (table[S][ch[ip]][0] == 's') { // 移进
            cout << "移进" << table[S][ch[ip]] << endl;
            opr.push_back(ip); // 输入串首压入栈
            int ss = atoi(table[S][ch[ip]].substr(1).c_str()); // 要转移到的状态
            status.push_back(ss); // 要转移的状态压入栈
            ip_pos += ip.size(); // 输入串指针移动一位
            while (s[ip_pos] == ' ' || s[ip_pos] == '\n')
                ip_pos++;

        }
        else if (table[S][ch[ip]][0] == 'r') { // 归约
            int lge_no = atoi(table[S][ch[ip]].substr(1).c_str()); // 用第几条文法归约
            cout << "reduce by " << lge[lge_no] << endl;
            int arrow_loc = _locate(lge[lge_no], 2); // 箭头位置
            int beta_len = how_many_word(lge[lge_no].substr(arrow_loc + 1));// 从箭头位置开始到最后有几个单词
            for (int i = 0; i < beta_len; i++) { // 弹出被归约的单词个数
                status.pop_back();
                opr.pop_back();
            }
            int ss = status.back(); // 栈顶状态
            string a_word = read_a_word(lge[lge_no], 0); // 要归约到的单词
            opr.push_back(a_word); // 要归约到的单词压入栈
            int A = atoi(table[ss][ch[a_word]].c_str()); // goto到状态A
            status.push_back(A); //A压入栈
        }
        else if (table[S][ch[ip]] == "acc") { // 接受
            cout << "success" << endl;
            return;
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
            return;
        }
    } while (true);
}

int main() {
    
    clock_t _stt = clock();
    
    /* ***************************************/
    for(int i=0;i<1000;i++){
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
        input_str.append(str);
        input_str.append(1, ' ');
    }
    cout << "待分析串\t" << input_str << endl;


    //    string input_str = " b + b ";
    proc(input_str);
    /*integer id=record id:integer end;*/
    /* ***************************************/
    
    clock_t _end = clock();
    cout << _end - _stt << "us" << endl;

}
// 4468689
// 23830
